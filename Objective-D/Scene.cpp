#include "Scene.h"
#include "GameResource.h"
#include "CBVUtil.h"
#include "RootConstants.h"
#include "MouseUtil.h"

// �� ������Ʈ�� �ٽ� ��ƿ�̴�. ���α׷��� ��� ��ü�� ������Ʈ �� �������� ��� �� �����ӿ�ũ�� ��ģ��.

// �����ӿ�ũ�� �ʱ�ȭ �Ѵ�. ���� �� �� �� ���� ����Ǵ� �Լ���, ���� ��ü�� �߰��� �� ��带 �����Ѵ�.
void Scene::Init(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList, Function ModeFunction) {
	// ��Ʈ �ñ״�ó�� �����Ѵ�.
	DefaultRootSignature = CreateObjectRootSignature(Device);

	// ���� ���̴��� �����Ѵ�.
	LoadShader(DefaultRootSignature, Device, CmdList);

	// ���� �⺻ �Ž��� �ε��Ѵ�.
	LoadSystemMesh(Device, CmdList);

	// ���� �Ž��� �ε��Ѵ�.
	LoadMesh(Device, CmdList);

	// ���� �ؽ�ó�� �ε��Ѵ�.
	LoadTexture(Device, CmdList);

	// ���� ��� �Լ� ����
	ModeFunction();

	// ���� ��ġ ���� �Ҵ�
	for (int i = 0; i < Layers; ++i)
		DeleteLocation[i].reserve(DELETE_LOCATION_BUFFER_SIZE);
}

// ���� ���� ���� ��� �̸��� �����Ѵ�
const char* Scene::GetMode() {
	return RunningMode;
}

// ��� �Ҹ��ڸ� ����Ѵ�.
void Scene::RegisterDestructor(Function Destructor) {
	DestructorBuffer = Destructor;
}

// ��� �Ҹ��� �����͸� ���´�.
void Scene::ReleaseDestructor() {
	DestructorBuffer = nullptr;
}

// ���� �����ϴ� ��� ��ü���� ������Ʈ�ϰ� �������Ѵ�.
// ���� ������ Ȱ��ȭ�� ��ü���� ���� Ŀ�ǵ� Ȱ��ȭ �� �������� ���� �� �ϰ� �����ȴ�.
void Scene::Routine(float FT, ID3D12GraphicsCommandList* CmdList) {
	ObjectCmdList = CmdList;
	for (int i = 0; i < Layers; ++i) {
		for (auto const& Object : ObjectList[i]) {
			Object->Update(FT);
			Object->Render();

			if (Object->DeleteReserveCommand) {
				Object->DeleteCommand = true;
				AddLocation(i, CurrentReferPosition);
			}
			
			++CurrentReferPosition;
		}
		CurrentReferPosition = 0;
	}
}

// ��带 �����Ѵ�. ��� ���� �� ���� scene�� �ִ� ��ü���� ��� �����ȴ�.
void Scene::SwitchMode(Function ModeFunction) {
	ClearAll();
	if (DestructorBuffer)
		DestructorBuffer();
	ModeFunction();
}

// ��� �̸��� ����Ѵ�. �ߺ��Ǵ� ��� �̸��� ������� �ʵ��� �����Ѵ�.
void Scene::RegisterModeName(const char* ModeName) {
	RunningMode = ModeName;
}

// ��Ʈ�ѷ� ���� �Լ��̴�. �� �Լ��� ���� �ۼ��� ���� ����,
void Scene::RegisterKeyController(void (*FunctionPtr)(HWND, UINT, WPARAM, LPARAM)) {
	KeyboardControllerPtr = FunctionPtr;
}
void Scene::RegisterMouseController(void (*FunctionPtr)(HWND, UINT, WPARAM, LPARAM)) {
	MouseControllerPtr = FunctionPtr;
}
void Scene::RegisterMouseMotionController(void (*FunctionPtr)(HWND)) {
	MouseMotionControllerPtr = FunctionPtr;
}

// ��ü�� ã�� ��Ʈ�ѷ� �Լ��� �޽����� �����Ѵ�. �ټ��� �����ϴ� ��ü�� ������� �ʵ��� �����Ѵ�.
void Scene::InputMouse(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam, const char* ObjectTag) {
	auto Object = ObjectIndex.find(ObjectTag);
	if (Object != end(ObjectIndex))
		Object->second->InputMouse(hWnd, nMessageID, wParam, lParam);
}
void Scene::InputKey(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam, const char* ObjectTag) {
	auto Object = ObjectIndex.find(ObjectTag);
	if (Object != end(ObjectIndex))
		Object->second->InputKey(hWnd, nMessageID, wParam, lParam);
}
void Scene::InputMouseMotion(HWND hWnd,  const char* ObjectTag) {
	auto Object = ObjectIndex.find(ObjectTag);
	if (Object != end(ObjectIndex))
		Object->second->InputMouseMotion(hWnd, mouse.MotionPosition);
}

// ��ü�� �߰��Ѵ�. ���ϴ� ��ü�� �±�, ���̾ ������ �� �ִ�.
// �� �Լ����� �Է��� �±״� Find()�Լ����� ���ȴ�.
void Scene::AddObject(GameObject*&& Object, const char* Tag, int InputLayer) {
	ObjectList[InputLayer].emplace_back(Object);
	ObjectIndex.insert(std::make_pair(Tag, Object));
	Object->ObjectTag = Tag;
	Object->ObjectLayer = InputLayer;
}

// �����͸� ����Ͽ� ��ü�� �����Ѵ�. ��ü�� ���� ��ũ�� ǥ���Ѵ�.
// �� �ڵ尡 ����Ǵ� ������ ��� �������� ������ �����Ѵ�.
// ���� ��ũ�� ǥ�õ� ��ü�� UpdateObjectIndex()���� ���������� �����ȴ�.
// Ŭ���� ���ο��� this �����ͷε� �ڽ��� ������ �� �ִ�.
void Scene::DeleteObject(GameObject* Object) {
	Object->DeleteReserveCommand = true;
}

// ���� �����ϴ� ��ü�� �� Ư�� ��ü�� �����͸� ��� ������ �� ����Ѵ�.
// ���� Ž���� ����Ͽ� �˻��ϹǷ� �ſ� ������.
GameObject* Scene::Find(const char* Tag) {
	auto Object = ObjectIndex.find(Tag);
	if (Object != std::end(ObjectIndex))
		return Object->second;
	else
		return nullptr;
}

// Ư�� �±׸� ���� ������Ʈ���� ������ ������ �����Ѵ�.
// �ش� �Լ��� equal range�� ��� for������ �����ϸ� �ȴ�.
std::pair<ObjectRange, ObjectRange> Scene::EqualRange(const char* Tag) {
	return ObjectIndex.equal_range(Tag);
}

// ���� ��ũ�� ǥ�õ� ��ü�� �޸𸮿��� �����Ѵ�.
void Scene::CompleteCommand() {
	if (!CommandExist)
		return;

	ProcessObjectCommand();
	ProcessSceneCommand();
	CommandExist = false;
}

// ���α׷��� �����Ѵ�.
void Scene::Exit() {
	PostQuitMessage(1);
}


// Ű����, ���콺, ���콺 �������� WinMain���κ��� �޾ƿ´�. ���� �� ���� ����.
void Scene::InputKeyMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
	if (KeyboardControllerPtr)
		KeyboardControllerPtr(hWnd, nMessageID, wParam, lParam);
}
void Scene::InputMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
	if (MouseControllerPtr)
		MouseControllerPtr(hWnd, nMessageID, wParam, lParam);
}
void Scene::InputMouseMotionMessage(HWND hWnd) {
	if (MouseMotionControllerPtr)
		MouseMotionControllerPtr(hWnd);
}

// ��Ʈ �ñ״�ó�� �����Ѵ�
ID3D12RootSignature* Scene::GetGraphicsRootSignature() {
	return(DefaultRootSignature);
}

// ��Ʈ�ñ״�ó�� �������Ѵ�
void Scene::ReleaseObjects() {
	if (DefaultRootSignature)
		DefaultRootSignature->Release();
}

// �������� �غ��Ѵ�
void Scene::PrepareRender(ID3D12GraphicsCommandList* CmdList) {
	
}

/////////////// private

// ���� ��ġ�� �����Ѵ�.
void Scene::AddLocation(int Layer, int Position) {
	DeleteLocation[Layer].emplace_back(Position);
	CommandExist = true;
}

// ���� ��ũ�� ǥ�õ� ��ü���� �����̳ʿ��� �����Ѵ�. ������ ��ü�� �����Ǵ°��� �ƴԿ� �����Ѵ�.
void Scene::ProcessObjectCommand() {
	int Offset{};

	for (int Layer = 0; Layer < Layers; ++Layer) {
		size_t Size = DeleteLocation[Layer].size();
		if (Size == 0)
			continue;

		for (int i = 0; i < Size; ++i) {
			auto Object = begin(ObjectList[Layer]) + DeleteLocation[Layer][i] - Offset;
			ObjectList[Layer].erase(Object);
			++SceneCommandCount;
			++Offset;
		}

		DeleteLocation[Layer].clear();
		Offset = 0;
	}
}

// ���� ��ũ�� ǥ�õ� ��ü���� ������ �����Ѵ�.
void Scene::ProcessSceneCommand() {
	auto Object = begin(ObjectIndex);
	while (Object != end(ObjectIndex) && SceneCommandCount != 0) {
		if (Object->second->DeleteCommand) {
			delete Object->second;
			Object->second = nullptr;
			--SceneCommandCount;
			continue;
		}
		++Object;
	}
}

// ���� �����ϴ� ��� ��ü���� �����Ѵ�.
void Scene::ClearAll() {
	for (auto const& Object : ObjectIndex)
		Object.second->DeleteReserveCommand = true;
}