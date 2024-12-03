#pragma once
#include "GameObject.h"

class AvoidAlert : public GameObject {
private:
	bool RenderEnable{};
	bool UpdateEnable{};
	float Num{};

public:
	AvoidAlert() {
		Transform::Scale2D(ScaleMatrix, 1.0, 1.0);
	}

	void EnableAlert() override {
		UpdateEnable = true;
	}

	void DisableAlert() override {
		UpdateEnable = false;
	}
	
	// �︮���Ͱ� ȸ�� �⵿ �� ȸ�� �⵿ �˸��� �����Ѵ�.
	void Update(float FT) override {
		if (UpdateEnable) {
			Num += FT;

			if (!RenderEnable && Num >= 0.15) {
				RenderEnable = true;
				Num = 0.0;
			}

			else if (RenderEnable && Num >= 0.15) {
				RenderEnable = false;
				Num = 0.0;
			}
		}

		else {
			RenderEnable = false;
			Num = 0.0;
		}
	}

	void Render() override {
		if (RenderEnable) {
			InitRenderState(RENDER_TYPE_2D_STATIC);
			Render2D(TextureAlert);
		}
	}
};

