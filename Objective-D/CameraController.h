#pragma once
#include "GameObject.h"
#include "CameraUtil.h"
#include "MouseUtil.h"

// ���� ī�޶� ��Ʈ�ѷ�
class CameraController : public GameObject {
private:
	bool MoveForward{}, MoveBackward{}, MoveRight{}, MoveLeft{};
	bool MoveUp{}, MoveDown{};

	XMFLOAT3 CamPosition{ 0.0, 80.0, -80.0 };

	// ī�޶� ȸ��
	XMFLOAT3 CamRotation{};
	XMFLOAT3 DestCamRotation{};

public:
	CameraController() {
		if(camera.CurrentMode() == CamMode::SPECTOR_MODE)
			camera.Move(CamPosition);
	}

	void InputKey(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) override {
		switch (nMessageID) {
		case WM_KEYDOWN:
			switch (wParam) {
			case 'W': MoveForward = true; break;
			case 'A': MoveLeft = true; break;
			case 'S': MoveBackward = true; break;
			case 'D': MoveRight = true; break;
			case VK_SPACE: MoveUp = true; break;
			case VK_CONTROL: MoveDown = true; break;

			case VK_F2:  // ���� ���� �����Ѵ�.
				camera.SwitchCameraMode(CamMode::SPECTOR_MODE);
				// ���������� ����� ��ġ�� ī�޶� �̵���Ų��.
				camera.Move(CamPosition);
				break;

			case VK_F1:  // �Ϲ� ���� �����Ѵ�.
				camera.SwitchCameraMode(CamMode::TRACK_MODE);
				break;
			}
			break;

		case WM_KEYUP:
			switch (wParam) {
			case 'W': MoveForward = false; break;
			case 'A': MoveLeft = false; break;
			case 'S': MoveBackward = false; break;
			case 'D': MoveRight = false; break;
			case VK_SPACE: MoveUp = false; break;
			case VK_CONTROL: MoveDown = false; break;
			}
			break;
		}
	}

	void InputMouseMotion(HWND hWnd, POINT MotionPosition) override {
		if (camera.CurrentMode() == CamMode::SPECTOR_MODE) {
			if (GetCapture() == hWnd) {
				mouse.HideCursor();
				GetCapture();

				XMFLOAT2 Delta = mouse.GetMotionDelta(MotionPosition, 0.1);
				mouse.UpdateMotionPosition(MotionPosition);
				UpdateMotionRotation(DestCamRotation, Delta.x, Delta.y);
			}
		}
	}

	void Update(float FT) override {
		// ���� ��忡���� �����Ѵ�.
		if (camera.CurrentMode() == CamMode::SPECTOR_MODE) {
			// ���� ȸ���ݰ� ����
			if (DestCamRotation.x > 90.0)
				DestCamRotation.x = 90.0;
			else if (DestCamRotation.x < -90.0)
				DestCamRotation.x = -90.0;

			if (MoveForward)
				camera.MoveForward(FT * 40);
			if (MoveBackward)
				camera.MoveForward(-FT * 40);
			if (MoveRight)
				camera.MoveStrafe(FT * 40);
			if (MoveLeft)
				camera.MoveStrafe(-FT * 40);
			if (MoveUp)
				camera.MoveUp(FT * 40);
			if (MoveDown)
				camera.MoveUp(-FT * 40);

			// �ε巯�� ī�޶� ȸ��
			CamRotation.x = std::lerp(CamRotation.x, DestCamRotation.x, FT * 15);
			CamRotation.y = std::lerp(CamRotation.y, DestCamRotation.y, FT * 15);

			camera.Rotate(CamRotation.x, CamRotation.y, 0.0);

			CamPosition = camera.GetPosition();
		}
	}
};