#pragma once
#include "GameObject.h"
#include "TransformUtil.h"
#include "MathUtil.h"
#include "TerrainUtil.h"
#include "CameraUtil.h"

// ���� ������

class Tree : public GameObject {
private:
	Vector Vec{};
	XMFLOAT3 Position{};
	TerrainUtil terrainUtil;
	float Rotation{};

public:
	Tree(float X, float Z) {
		Math::InitVector(Vec);
		Position.x = X;
		Position.z = Z;

		// ���� ��ġ������ �ͷ��� ���� ���� ��� �ش� ���̷� �̵��Ѵ�.
		terrainUtil.InputPosition(Position, -1.0);
		if (auto Terrain = scene.Find("object_terrain"); Terrain) {
			terrainUtil.CheckCollision(Terrain->GetTerrain());
			terrainUtil.SetHeightToTerrain(Position);
		}

		Transform::Move(TranslateMatrix, Position.x, Position.y, Position.z);
		Transform::Scale(ScaleMatrix, 10.0, 10.0, 1.0);
	}

	void Render() override {
		InitRenderState(RENDER_TYPE_3D_STATIC);
		SetLightUse(DISABLE_LIGHT);

		// ī�޶� �ٶ󺸵��� �Ѵ�.
		Math::BillboardLookAt(RotateMatrix, Vec, Position, camera.GetPosition());
		Render3D(ImagePannel, TextureTree);
	}
};