#pragma once
#include "GameObject.h"
#include "TransformUtil.h"
#include "PlayMode.h"
#include <cmath>

//#include "PlayMode.h"

// ����
class Help : public GameObject {
private:
	// ���� ��ũ�� ��ġ
	float ScrollY = -1.0;

	// ��ũ�� ��ġ ��ǥ
	float ScrollDest = -1.0;

	// ��ũ�� �ȳ� ȭ��ǥ ��ġ
	float ArrowPosition{};
	float Num{};

	// press to enter �ȳ��� ����
	float EnterAlpha{};
	float AlphaNum{};

public:
	void InputMouse(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) override {
		switch (nMessageID) {
		case WM_MOUSEWHEEL: {
			int ScrollVar = GET_WHEEL_DELTA_WPARAM(wParam);

			// ���� ��ũ��
			if (ScrollVar > 0 && ScrollDest > -1.0)
				ScrollDest -= 0.2;

			// �ƾַ� ��ũ��
			else if (ScrollVar < 0 && ScrollDest < 1.5)
				ScrollDest += 0.2;
		}
		break;
		}
	}

	void InputKey(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) override {
		switch (nMessageID) {
		case WM_KEYDOWN:
			switch (wParam) {
			case VK_RETURN:
				// ���� ���� �� �÷��� ���� ��ȯ
				scene.SwitchMode(PlayMode::Start);
				break;
			}

		}
	}

	void Update(float FT) override {
		//  lerp�� �ε巯�� ��ũ���� �����Ѵ�
		ScrollY = std::lerp(ScrollY, ScrollDest, FT * 10);

		// ��ũ�� �ȳ� ȭ��ǥ �ִϸ��̼�
		Num += FT * 5;
		ArrowPosition = sin(Num) * 0.1;

		// press to enter �ִϸ��̼�
		AlphaNum += FT * 2;
		EnterAlpha = sin(AlphaNum);
		if (EnterAlpha < 0.0) {
			EnterAlpha = 0.0;
			AlphaNum = 0.0;
		}
	}

	void Render() override {
		// ���� ����
		// ��� �ʱ�ȭ
		// �̹��� Ÿ������ ������ �� ������ ��Ȱ��ȭ �ǰ� ���� �˻縦 �����Ѵ�.
		InitRenderState(RENDER_TYPE_2D);
		Transform::Move(TranslateMatrix, 0.0, ScrollY, 0.0);
		Transform::Scale(ScaleMatrix, 3.0, 3.0, 1.0);
		Render2D(TextureHelp);

		// ��ũ�� �ȳ� �ؽ�Ʈ
		InitRenderState(RENDER_TYPE_2D);
		Transform::Move(TranslateMatrix, 0.7 * ASPECT, 0.0, 0.0);
		Transform::Scale(ScaleMatrix, 0.5, 0.5, 1.0);
		Render2D(TextureScroll);

		// ��ũ�� �ȳ� ȭ��ǥ(�Ʒ�)
		InitRenderState(RENDER_TYPE_2D);
		Transform::Move(TranslateMatrix, 0.7 * ASPECT, -0.3 - ArrowPosition, 0.0);
		Transform::Scale(ScaleMatrix, 0.2, 0.2, 1.0);
		Render2D(TextureArrow);

		// ��ũ�� �ȳ� ȭ��ǥ(��)
		InitRenderState(RENDER_TYPE_2D);
		Transform::Move(TranslateMatrix, 0.7 * ASPECT, 0.3 + ArrowPosition, 0.0);
		Transform::Scale(ScaleMatrix, 0.2, 0.2, 1.0);
		Transform::Rotate(RotateMatrix, 180.0, 0.0, 0.0, 1.0);
		Render2D(TextureArrow);

		// press to enter
		InitRenderState(RENDER_TYPE_2D);
		Transform::Move(TranslateMatrix, 0.0, ScrollY - 1.8, 0.0);
		Render2D(TextureEnter, EnterAlpha);
	}
};