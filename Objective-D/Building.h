#pragma once
#include "GameObject.h"
#include "CameraUtil.h"

class Building : public GameObject {
private:
	AABB aabb;
	AABB aabbInside;

public:
	AABB GetAABB() override {
		return aabb;
	}

	AABB GetInsideAABB() override {
		return aabbInside;
	}

	Building() {
		Transform::Scale(ScaleMatrix, 0.04, 0.04, 0.04);
		Transform::Rotate(RotateMatrix, -90.0, 0.0, 0.0);
		Transform::Move(TranslateMatrix, 0.0, 35.0, 0.0);
		// �︮���� ���� �ν��� ���� �ܺ� aabb
		aabb.Update(XMFLOAT3(0.0, 35.0, 0.0), XMFLOAT3(30.0, 38.0, 30.0));

		// �ǹ��� �ѷ��δ� ���� aabb
		aabbInside.Update(XMFLOAT3(0.0, 35.0, 0.0), XMFLOAT3(10.0, 38.0, 10.0));
	}

	void Render() override {
		InitRenderState(RENDER_TYPE_3D_STATIC);
		Render3D(MeshBuilding, TextureBuilding);
		aabb.Render();
		aabbInside.Render();
	}
};