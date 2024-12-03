#pragma once
#include "GameObject.h"
#include "CameraUtil.h"
#include "MouseUtil.h"

// 관전 카메라 컨트롤러
class CameraController : public GameObject {
private:
	bool MoveForward{}, MoveBackward{}, MoveRight{}, MoveLeft{};
	bool MoveUp{}, MoveDown{};

	XMFLOAT3 CamPosition{ 0.0, 80.0, -80.0 };

	// 카메라 회전
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

			case VK_F2:  // 관전 모드로 변경한다.
				camera.SwitchCameraMode(CamMode::SPECTOR_MODE);
				// 마지막으로 기억한 위치로 카메라를 이동시킨다.
				camera.Move(CamPosition);
				break;

			case VK_F1:  // 일반 모드로 변경한다.
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
		// 관전 모드에서만 동작한다.
		if (camera.CurrentMode() == CamMode::SPECTOR_MODE) {
			// 상하 회전반경 제한
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

			// 부드러운 카메라 회전
			CamRotation.x = std::lerp(CamRotation.x, DestCamRotation.x, FT * 15);
			CamRotation.y = std::lerp(CamRotation.y, DestCamRotation.y, FT * 15);

			camera.Rotate(CamRotation.x, CamRotation.y, 0.0);

			CamPosition = camera.GetPosition();
		}
	}
};