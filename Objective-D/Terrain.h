#pragma once
#include "GameObject.h"
#include "TerrainUtil.h"


class Terrain : public GameObject {
private:
	TerrainUtil terrainUtil;

public:
	TerrainUtil GetTerrain() override {
		return terrainUtil;
	}

	Terrain() {
		// 터레인 정보를 터레인 충돌처리 유틸에 전달한다.
		Transform::Move(TranslateMatrix, 0.0, -5.0, 0.0);
		Transform::Scale(ScaleMatrix, 60.0, 120.0, 120.0);
		terrainUtil.InputData(TranslateMatrix, RotateMatrix, ScaleMatrix, MeshTerrain);
	}

	void Render() override {
		// 실시간 변환이 필요 없으므로 static 렌더로 설정
		// 행렬값 초기화 없이 그대로 유지한다.
		InitRenderState(RENDER_TYPE_3D_STATIC);
		Render3D(MeshTerrain, TextureTerrain);
	}
};