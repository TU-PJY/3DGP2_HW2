#include "PlayMode.h"
#include "MouseUtil.h"
#include "Terrain.h"
#include "Water.h"
#include "Gunship.h"
#include "CameraController.h"

bool StartCapture;

void PlayMode::Start() {
	scene.AddObject(new CameraController, "object_controller", LAYER1);
	scene.AddObject(new Terrain, "object_terrain", LAYER1);
	scene.AddObject(new Water, "object_water", LAYER1);
	scene.AddObject(new Gunship, "object_gunship", LAYER2);
	RegisterController();
	scene.RegisterModeName("PlayMode");
}

void PlayMode::Destructor() {
}

void PlayMode::KeyboardController(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
	if (nMessageID == WM_KEYDOWN && wParam == VK_ESCAPE)
		scene.Exit();

	scene.InputKey(hWnd, nMessageID, wParam, lParam, "object_controller");
	scene.InputKey(hWnd, nMessageID, wParam, lParam, "object_gunship");
}

void PlayMode::MouseMotionController(HWND hWnd) {
	if (!StartCapture) {
		mouse.StartMotionCapture(hWnd);
		StartCapture = true;
	}
	mouse.UpdateMousePosition(hWnd);

	scene.InputMouseMotion(hWnd, "object_controller");
	scene.InputMouseMotion(hWnd, "object_gunship");
}


void PlayMode::MouseController(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
	mouse.StartMotionCapture(hWnd);
}

void PlayMode::RegisterController() {
	scene.RegisterKeyController(KeyboardController);
	scene.RegisterMouseController(MouseController);
	scene.RegisterMouseMotionController(MouseMotionController);
	scene.RegisterDestructor(Destructor);
}