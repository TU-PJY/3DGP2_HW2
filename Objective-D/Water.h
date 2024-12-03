#pragma once
#include "GameObject.h"

class Water : public GameObject {
public: 
	Water() {
		Transform::Move(TranslateMatrix, 0.0, 10.0, 0.0);
		Transform::Scale(ScaleMatrix, 600.0, 600.0, 1.0);
		Transform::Rotate(RotateMatrix, 90.0, 0.0, 0.0);
	}

	void Render() override {
		// 반투명 렌더링 한다.
		InitRenderState(RENDER_TYPE_3D_STATIC);
		SetColor(XMFLOAT3(0.0, 0.0, 1.0));
		Render3D(ImagePannel, TextureWater, 0.3);
	}
};