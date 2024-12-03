#pragma once
#include "GameObject.h"
#include "CameraUtil.h"
#include "TransformUtil.h"
#include "TerrainUtil.h"


class Terrain : public GameObject {
private:
	OOBB oobb;
	TerrainUtil terrainUtil;

public:
	OOBB GetOOBB() override {
		return oobb;
	}

	TerrainUtil GetTerrain() override {
		return terrainUtil;
	}

	Terrain() {
		// �ͷ��� ������ �ͷ��� �浹ó�� ��ƿ�� �����Ѵ�.
		Transform::Move(TranslateMatrix, 0.0, -5.0, 0.0);
		Transform::Scale(ScaleMatrix, 60.0, 120.0, 120.0);
		terrainUtil.InputData(TranslateMatrix, RotateMatrix, ScaleMatrix, MeshTerrain);

		// oobb ������Ʈ
		oobb.Update(MeshTerrain, TranslateMatrix, RotateMatrix, ScaleMatrix);
	}

	void Render() override {
		// �ǽð� ��ȯ�� �ʿ� �����Ƿ� static ������ ����
		// ��İ� �ʱ�ȭ ���� �״�� �����Ѵ�.
		InitRenderState(RENDER_TYPE_3D_STATIC);
		Render3D(MeshTerrain, TextureTerrain);

		// oobb ���
		oobb.Render();
	}
};