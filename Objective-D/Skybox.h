#pragma once
#include "GameObject.h"
#include "Scene.h"

class Skybox : public GameObject {
private:
	XMFLOAT3 Position{};

public:
	void Update(float FT) override {
		Position = camera.GetPosition();
	}

	void Render() override {
		InitRenderState(RENDER_TYPE_3D);
		FlipTexture(FLIP_TYPE_NONE);
		SetLightUse(DISABLE_LIGHT);

		// 스카이박스에서는 안개를 비활성화 한다.
		SetFogUse(DISABLE_FOG);
		Transform::Scale(ScaleMatrix, 600.0, 600.0, 600.0);
		Transform::Move(TranslateMatrix, Position.x, Position.y, Position.z);
		Render3D(SkyboxMesh, TextureSkybox, 1.0, false);
	}
};