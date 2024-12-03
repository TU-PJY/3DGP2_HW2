#include "HomeMode.h"
#include "MouseUtil.h"
#include "Help.h"

void HomeMode::Start() {
	scene.AddObject(new Help, "object_help", LAYER1);

	// scene�� ��Ʈ�ѷ� �� ��� �Ҹ��� ���
	RegisterController();

	// ��� �̸��� Scene�� ���
	scene.RegisterModeName("HomeMode");
}

void HomeMode::Destructor() {
	// ���⿡ ��� ���� �� �ʿ��� �۾� �߰� (���ҽ� �޸� ���� ��)
}

void HomeMode::KeyboardController(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
	// esc ���� �� ���α׷� ����
	if (nMessageID == WM_KEYDOWN && wParam == VK_ESCAPE)
		scene.Exit();

	// ���� ��ü�� Ű���� �Է� ����
	scene.InputKey(hWnd, nMessageID, wParam, lParam, "object_help");
}

//  ���콺 ����� ������ ��ü �����ͷ� �����Ѵ�
void HomeMode::MouseMotionController(HWND hWnd) {
	// ���콺 ��ǥ�� ����Ʈ ��ǥ�� ��ȯ�Ѵ�.
	mouse.UpdateMousePosition(hWnd);
}

// ���콺 ��ư Ŭ�� �̺�Ʈ�� ������ ��ü �����ͷ� �����Ѵ�
void HomeMode::MouseController(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {

	// ���� ��ü�� ���콺 �Է� ����
	scene.InputMouse(hWnd, nMessageID, wParam, lParam, "object_help");
}

// scene�� ��Ʈ�ѷ� �� ��� �Ҹ��� ���
void HomeMode::RegisterController() {
	scene.RegisterKeyController(KeyboardController);
	scene.RegisterMouseController(MouseController);
	scene.RegisterMouseMotionController(MouseMotionController);
	scene.RegisterDestructor(Destructor);
}