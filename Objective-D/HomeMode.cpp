#include "HomeMode.h"
#include "MouseUtil.h"
#include "Help.h"

void HomeMode::Start() {
	scene.AddObject(new Help, "object_help", LAYER1);

	// scene에 컨트롤러 및 모드 소멸자 등록
	RegisterController();

	// 모드 이름을 Scene에 등록
	scene.RegisterModeName("HomeMode");
}

void HomeMode::Destructor() {
	// 여기에 모드 종료 시 필요한 작업 추가 (리소스 메모리 해제 등)
}

void HomeMode::KeyboardController(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
	// esc 누를 시 프로그램 종료
	if (nMessageID == WM_KEYDOWN && wParam == VK_ESCAPE)
		scene.Exit();

	// 도움말 객체로 키보드 입력 전송
	scene.InputKey(hWnd, nMessageID, wParam, lParam, "object_help");
}

//  마우스 모션을 지정된 객체 포인터로 전달한다
void HomeMode::MouseMotionController(HWND hWnd) {
	// 마우스 좌표를 뷰포트 좌표로 변환한다.
	mouse.UpdateMousePosition(hWnd);
}

// 마우스 버튼 클릭 이벤트를 지정된 객체 포인터로 전달한다
void HomeMode::MouseController(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {

	// 도움말 객체로 마우스 입력 전송
	scene.InputMouse(hWnd, nMessageID, wParam, lParam, "object_help");
}

// scene에 컨트롤러 및 모드 소멸자 등록
void HomeMode::RegisterController() {
	scene.RegisterKeyController(KeyboardController);
	scene.RegisterMouseController(MouseController);
	scene.RegisterMouseMotionController(MouseMotionController);
	scene.RegisterDestructor(Destructor);
}