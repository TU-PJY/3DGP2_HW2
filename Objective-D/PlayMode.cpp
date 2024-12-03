#include "PlayMode.h"
#include "MouseUtil.h"
#include "Terrain.h"
#include "Water.h"
#include "Gunship.h"
#include "Building.h"
#include "Skybox.h"
#include "Tree.h"
#include "CameraController.h"
#include <random>

bool StartCapture;

void PlayMode::Start() {
	scene.AddObject(new CameraController, "object_controller", LAYER1);
	scene.AddObject(new Skybox, "object_skybox", LAYER1);
	scene.AddObject(new Terrain, "object_terrain", LAYER1);
	scene.AddObject(new Water, "object_water", LAYER1);
	scene.AddObject(new Building, "object_building", LAYER1);

	std::random_device rd{};
	std::uniform_real_distribution<float> DistX(-150.0, 150.0);
	std::uniform_real_distribution<float> DistY(-150.0, 150.0);

	// 30개의 랜덤한 위치에 빌보드 추가
	for (int i = 0; i < 30; ++i) {
		float RandX, RandZ;
		RandX = DistX(rd);
		RandZ = DistY(rd);
		scene.AddObject(new Tree(RandX, RandZ), "object_tree", LAYER1);
	}

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