#pragma once
#include "GameObject.h"
#include "CameraUtil.h"
#include "Scene.h"
#include "MouseUtil.h"
#include <cmath>

// �︮���� ������Ʈ
class Gunship : public GameObject {
private:
	Vector Vec;
	XMFLOAT3 Position{ 0.0, 60.0, -80.0 };

	// ���� ȸ�� ��
	float WingRotation{};

	// �︮���� ��ü ȸ�� ��
	XMFLOAT3 HeliRotation{};
	XMFLOAT3 DestRotation{};

	// �̵� ���� �� �̵� �ӵ�
	bool MoveForward{}, MoveBackward{}, MoveRight{}, MoveLeft{};
	float ForwardSpeed{};
	float StrafeSpeed{};

	// ���� �̵� ����
	bool MoveUp{}, MoveDown{};

	// ��� ������
	XMFLOAT3 Tilt{};

	// ȸ�Ǳ⵿ ����
	bool AvoidState{};

	// �浹 ó�� Ȯ�� ���� ����
	bool CheckCollisionState{};

	// ȸ�� ���� ��� ��� ����
	bool AvoidCalculated{};

	// ȸ�� ����
	int AvoidDir{};

	// oobb
	OOBB oobb;

	// �ͷ��� ��ü���� �浹ó���� ���� ��ƿ ��ü
	TerrainUtil terrainUtil;

public:
	XMFLOAT3 GetPosition() override { return Position; }
	ObjectVector GetVectorSet() override { return Vec; }

	Gunship() {
		Math::InitVector(Vec);
	}

	void InputKey(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) override {
		if (camera.CurrentMode() == CamMode::TRACK_MODE) {
			switch (nMessageID) {
			case WM_KEYDOWN:
				switch (wParam) {
				case 'W': MoveForward = true; break;
				case 'S': MoveBackward = true; break;
				case 'A': MoveLeft = true; break;
				case 'D': MoveRight = true; break;
				case 'R': MoveUp = true; break;
				case 'F': MoveDown = true; break;
				}
				break;

			case WM_KEYUP:
				switch (wParam) {
				case 'W': MoveForward = false; break;
				case 'S': MoveBackward = false; break;
				case 'A': MoveLeft = false; break;
				case 'D': MoveRight = false; break;
				case 'R': MoveUp = false; break;
				case 'F': MoveDown = false; break;
				}
			}
		}
	}

	void InputMouseMotion(HWND hWnd, POINT MotionPosition) override {
		if (GetCapture() == hWnd) {
			mouse.HideCursor();
			GetCapture();

			XMFLOAT2 Delta = mouse.GetMotionDelta(MotionPosition, 0.1);
			mouse.UpdateMotionPosition(MotionPosition);
			UpdateMotionRotation(DestRotation, Delta.x, Delta.y);
		}
	}

	//// ���� ������ �������� �ڽ��� ��ġ�� Ư�� �������� ���� �����ʿ� �ִ��� �˻��Ѵ�. �������̶�� true, �����̶�� false�� �����Ѵ�.
	//bool IsRightOfTarget(XMFLOAT3& targetPosition) {
	//	XMFLOAT3 directionToTarget = XMFLOAT3(
	//		targetPosition.x - Position.x,
	//		targetPosition.y - Position.y,
	//		targetPosition.z - Position.z
	//	);

	//	XMVECTOR rightVec = XMLoadFloat3(&Vec.Right);
	//	XMVECTOR directionVec = XMLoadFloat3(&directionToTarget);
	//	float dotProduct = XMVectorGetX(XMVector3Dot(rightVec, directionVec));

	//	if (dotProduct < 0)
	//		return true;

	//	return false;
	//}

	//// ȸ�������� ���̸� ����Ѵ�.
	//XMVECTOR CalcRayDirection(XMFLOAT3& Rotation) {
	//	float RotationX = XMConvertToRadians(Rotation.x);
	//	float RotationY = XMConvertToRadians(Rotation.y);
	//	float RotationZ = XMConvertToRadians(Rotation.z);

	//	XMVECTOR rotationQuat = XMQuaternionRotationRollPitchYaw(RotationX, RotationY, RotationZ);
	//	XMVECTOR defaultForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	//	XMVECTOR rayDirection = XMVector3Rotate(defaultForward, rotationQuat);

	//	return rayDirection;
	//}

	//// ���̰� ���۵Ǵ� ��ġ�� ����Ѵ�.
	//XMVECTOR CalcRayOrigin(XMFLOAT3& Position) {
	//	return XMVectorSet(Position.x, Position.y, Position.z, 1.0f);
	//}

	//// ���̰� aabb�� �浹�ϴ��� �˻��Ѵ�
	//bool CheckRayIntersectionWithBoundingBox(XMVECTOR& rayOrigin, XMVECTOR& rayDirection, AABB& other) {
	//	float distance;
	//	return other.aabb.Intersects(rayOrigin, rayDirection, distance);
	//}

	void Update(float FT) override {
		// �ǹ��� �ܺ� aabb�� �︮������ oobb�� �浹�ϸ� �︮���Ͱ� ���� aabb�� �ٶ󺸴��� �˻��Ѵ�.
		// ���� �ٶ󺻴ٸ� ȸ�Ǳ⵿�� �����Ѵ�.
		// �ǹ� �������� �����ʿ� ������ ����������, ���ʿ� ������ �������� ȸ�Ǳ⵿�Ѵ�.
		// �︮���Ͱ� �ǹ��� �ٿ��ڽ��� ����� ������ ȸ�Ǳ⵿ ������ �ٲ��� �ʴ´�.
		//if (auto building = scene.Find("building"); building && CheckCollisionState) {
		//	if (oobb.CheckCollision(building->GetAABB())) {
		//		if (!AvoidCalculated &&
		//			CheckRayIntersectionWithBoundingBox(CalcRayOrigin(Position), CalcRayDirection(HeliRotation), building->GetInsideAABB())) {
		//			if (IsRightOfTarget(XMFLOAT3(0.0, 0.0, 0.0)))
		//				AvoidDir = 1;
		//			else
		//				AvoidDir = -1;

		//			AvoidCalculated = true;
		//			AvoidState = true;
		//		}
		//	}

		//	// �ܺ� aabb�� ����� ȸ�Ǳ⵿ ���°� �����ȴ�.
		//	else {
		//		AvoidState = false;
		//		AvoidCalculated = false;
		//	}
		//}

		// ���� ȸ��
		WingRotation += FT * 2000;

		// �︮���� ȸ�� ���� ����
		if (DestRotation.x < -50.0)
			DestRotation.x = -50.0;

		if (DestRotation.x > 50.0)
			DestRotation.x = 50.0;

		// ���⿡ �ش��ϴ� Ű�� ������ �ӵ��� ���� �Ǵ� ����� ����
		// �̵� �������� ��ü�� ����δ�.
		if (!AvoidState) {
			if (MoveForward) {
				ForwardSpeed = std::lerp(ForwardSpeed, 15.0, FT);
				Tilt.x = std::lerp(Tilt.x, 15.0, FT);
			}
			if (MoveBackward) {
				ForwardSpeed = std::lerp(ForwardSpeed, -15.0, FT);
				Tilt.x = std::lerp(Tilt.x, -15.0, FT);
			}
			if (MoveRight) {
				StrafeSpeed = std::lerp(StrafeSpeed, 15.0, FT);
				Tilt.z = std::lerp(Tilt.z, -15.0, FT);
			}
			if (MoveLeft) {
				StrafeSpeed = std::lerp(StrafeSpeed, -15.0, FT);
				Tilt.z = std::lerp(Tilt.z, 15.0, FT);
			}
			if (MoveUp) {
				Position.y += 10 * FT;
			}
			if (MoveDown) {
				Position.y -= 10 * FT;
			}

			// �� Ű�� ���ÿ� �����ų� �� �� ������ ������ �ӵ��� ���ҽ�Ų��.
			if ((!MoveForward && !MoveBackward) || (MoveForward && MoveBackward)) {
				ForwardSpeed = std::lerp(ForwardSpeed, 0.0, FT);
				Tilt.x = std::lerp(Tilt.x, 0.0, FT);
			}

			if ((MoveRight && MoveLeft) || (!MoveRight && !MoveLeft)) {
				StrafeSpeed = std::lerp(StrafeSpeed, 0.0, FT);
				Tilt.z = std::lerp(Tilt.z, 0.0, FT);
			}
		}

		//// ȸ�Ǳ⵿ �� �� �� ������ �����Ѵ�.
		//if (AvoidState) {
		//	StrafeSpeed = std::lerp(StrafeSpeed, 15.0 * AvoidDir, FT * 2.0);
		//	ForwardSpeed = std::lerp(ForwardSpeed, 15.0, FT * 2.0);
		//	Tilt.z = std::lerp(Tilt.z, -15.0 * AvoidDir, FT * 2.0);
		//	Tilt.x = std::lerp(Tilt.x, 15.0, FT * 2.0);

		//	// ũ�ν��� ���������� �����Ѵ�.
		//	if (auto crosshair = scene.Find("crosshair"); crosshair)
		//		crosshair->SetUnable();
		//}

		// �̵�
		Position.x += sin(XMConvertToRadians(HeliRotation.y)) * ForwardSpeed * FT;
		Position.z += cos(XMConvertToRadians(HeliRotation.y)) * ForwardSpeed * FT;
		Position.x += cos(XMConvertToRadians(HeliRotation.y)) * StrafeSpeed * FT;
		Position.z -= sin(XMConvertToRadians(HeliRotation.y)) * StrafeSpeed * FT;


		// �̵� ���� ����, �� ������ ���� �� ����.
		if (Position.x > 600.0)
			Position.x = 600.0;
		else if (Position.x < -600.0)
			Position.x = -600.0;

		if (Position.z > 600.0)
			Position.z = 600.0;
		else if (Position.z < -600.0)
			Position.z = -600.0;

		terrainUtil.InputPosition(Position, 2.0);

		// ���� ����, �� ������ ������ �� ����.
		if (CheckCollisionState) {
			if (auto terrain = scene.Find("object_terrain"); terrain) {
				if (terrainUtil.CheckCollision(terrain->GetTerrain()))
					terrainUtil.SetHeightToTerrain(Position);
			}
		}

		// �︮���� �ε巯�� ȸ��
		HeliRotation.x = std::lerp(HeliRotation.x, DestRotation.x, FT * 2);
		HeliRotation.y = std::lerp(HeliRotation.y, DestRotation.y, FT * 2);

		// ȸ�� �� ���� ������Ʈ
		Math::UpdateVector(Vec, HeliRotation);

		// �浹ó�� ���� ���� Ȱ��ȭ
		CheckCollisionState = true;
	}

	void Render() override {
		// ��� ����
		InitRenderState(RENDER_TYPE_3D);
		Transform::Scale(ScaleMatrix, 0.8, 0.8, 0.8);
		Transform::Move(TranslateMatrix, Position.x, Position.y, Position.z);
		Transform::Rotate(TranslateMatrix, Tilt.x, HeliRotation.y, Tilt.z);
		Transform::Rotate(TranslateMatrix, HeliRotation.x, 0.0, 0.0);
		Render3D(MeshGunship, TextureGunship);

		// ���� �κ��� oobb�� ������Ʈ
		oobb.Update(MeshGunship, TranslateMatrix, RotateMatrix, ScaleMatrix);

		// ��� ����
		// ���� �������� ������ ��ȯ�� �����Ѵ�.
		Transform::Move(TranslateMatrix, 0.0, 3.0, 0.0);
		Transform::Rotate(TranslateMatrix, 0.0, WingRotation, 0.0);
		Render3D(MeshRotor, TextureGunship);

		oobb.Render();
	}
};