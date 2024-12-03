#include "PlayMode.h"
#include "MouseUtil.h"
#include "Terrain.h"

void PlayMode::Start() {
	scene.AddObject(new Terrain, "object_terrain", LAYER1);
	RegisterController();
	scene.RegisterModeName("PlayMode");
}

void PlayMode::Destructor() {
}

void PlayMode::KeyboardController(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
	if (nMessageID == WM_KEYDOWN && wParam == VK_ESCAPE)
		scene.Exit();
}

void PlayMode::MouseMotionController(HWND hWnd) {
	mouse.UpdateMousePosition(hWnd);
}


void PlayMode::MouseController(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
}

void PlayMode::RegisterController() {
	scene.RegisterKeyController(KeyboardController);
	scene.RegisterMouseController(MouseController);
	scene.RegisterMouseMotionController(MouseMotionController);
	scene.RegisterDestructor(Destructor);
}