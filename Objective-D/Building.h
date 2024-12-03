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
		// 헬리콥터 접근 인식을 위한 외부 aabb
		aabb.Update(XMFLOAT3(0.0, 35.0, 0.0), XMFLOAT3(30.0, 38.0, 30.0));

		// 건물을 둘러싸는 내부 aabb
		aabbInside.Update(XMFLOAT3(0.0, 35.0, 0.0), XMFLOAT3(10.0, 38.0, 10.0));
	}

	void Render() override {
		InitRenderState(RENDER_TYPE_3D_STATIC);
		Render3D(MeshBuilding, TextureBuilding);
		aabb.Render();
		aabbInside.Render();
	}
};