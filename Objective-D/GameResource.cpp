#include "GameResource.h"
#include <random>
// 이 파일은 리소스를 관리한다.
// 기본적으로 전역 리소스이며, ResourceManager.h에 먼저 extern 선언한 뒤, 이 파일에 아래와 같이 정의하면 된다.
// Scene::Init()에서 실행된다.


Mesh* MeshTerrain; // 터레인 매쉬
Mesh* MeshGunship; // 헬리콥터 몸체 매쉬
Mesh* MeshRotor; // 헬리콥터 날개 매쉬
Mesh* MeshBuilding; // 건물 매쉬

// 매쉬를 여기서 로드한다.
void LoadMesh(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList) {
	ImportMesh(Device, CmdList, MeshTerrain, "Resources//Model//Terrain.bin", MESH_TYPE_BIN);
	ImportMesh(Device, CmdList, MeshGunship, "Resources//Model//Gunship.bin", MESH_TYPE_BIN);
	ImportMesh(Device, CmdList, MeshRotor, "Resources//Model//Rotor.bin", MESH_TYPE_BIN);
	ImportMesh(Device, CmdList, MeshBuilding, "Resources//Model//Building.bin", MESH_TYPE_BIN);
}
/////////////////////////////////////////////////////////////////////////////////


Texture* TextureHelp; // 도움말 텍스처
Texture* TextureArrow; // 화살표 텍스처
Texture* TextureEnter; // PressEnter 텍스처
Texture* TextureScroll; // Scroll help 텍스처

Texture* TextureTerrain; // 터레인 텍스처
Texture* TextureWater; // 물 텍스처
Texture* TextureGunship; // 헬리콥터 텍스처
Texture* TextureBuilding; // 건물 텍스처
Texture* TextureTree; // 나무 텍스처
Texture* TextureSkybox; // 스카이박스 텍스처

// 택스처를 여기서 로드한다.
void LoadTexture(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList) {
	ImportTexture(Device, CmdList, TextureHelp, L"Resources//Image//help.png", TEXTURE_TYPE_WIC);
	ImportTexture(Device, CmdList, TextureArrow, L"Resources//Image//arrow.png", TEXTURE_TYPE_WIC);
	ImportTexture(Device, CmdList, TextureEnter, L"Resources//Image//press_enter.png", TEXTURE_TYPE_WIC);
	ImportTexture(Device, CmdList, TextureScroll, L"Resources//Image//scroll_help.png", TEXTURE_TYPE_WIC);
	ImportTexture(Device, CmdList, TextureTerrain, L"Resources//Image//grass.jpg", TEXTURE_TYPE_WIC);
	ImportTexture(Device, CmdList, TextureWater, L"Resources//Image//water.png", TEXTURE_TYPE_WIC);
	ImportTexture(Device, CmdList, TextureGunship, L"Resources//Image//gunship.png", TEXTURE_TYPE_WIC);
	ImportTexture(Device, CmdList, TextureBuilding, L"Resources//Image//color_building.png", TEXTURE_TYPE_WIC);
	ImportTexture(Device, CmdList, TextureTree, L"Resources//Image//tree.png", TEXTURE_TYPE_WIC);
	ImportTexture(Device, CmdList, TextureSkybox, L"Resources//Image//sky.png", TEXTURE_TYPE_WIC);
}
/////////////////////////////////////////////////////////////////////////////////


Object_Shader* ObjectShader;
Boundbox_Shader* BoundboxShader;
Image_Shader* ImageShader;
Line_Shader* LineShader;

// 쉐이더를 여기서 로드한다.
void LoadShader(ID3D12RootSignature* RootSignature, ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList) {
	// 일반 렌더링 쉐이더 생성
	ObjectShader = new Object_Shader();
	// 기본 파이프라인 생성
	ObjectShader->CreateDefaultPS(Device, RootSignature);
	// 깊이 검사 미포함 파이프라인 생성
	ObjectShader->CreateNoneDepthPS(Device, RootSignature);

	// 이미지 출력용 파이프라인 생성
	ImageShader = new Image_Shader();
	ImageShader->CreateNoneDepthPS(Device, RootSignature);

	// 바운드박스 쉐이더 생성
	BoundboxShader = new Boundbox_Shader();
	BoundboxShader->CreateWireframePS(Device, RootSignature);

	// 라인 브러쉬 출력용 쉐이더 생성
	LineShader = new Line_Shader();
	LineShader->CreateNoneDepthPS(Device, RootSignature);
}
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
// 업로드 버퍼를 처기하기 위한 벡터
std::vector<Mesh*> LoadedMeshList;
std::vector<Texture*> LoadedTextureList;

Mesh* ImagePannel;
Mesh* SkyboxMesh;
Mesh* BoundMesh;
Mesh* BoundingSphereMesh;

// 기본 전역 매쉬 로드
void LoadSystemMesh(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList) {
	ImagePannel = new Mesh;
	ImagePannel->CreateImagePannelMesh(Device, CmdList);
	LoadedMeshList.emplace_back(ImagePannel);

	// 스카이박스 출력용 매쉬 생성
	SkyboxMesh = new Mesh;
	SkyboxMesh->CreateSkyboxMesh(Device, CmdList);
	LoadedMeshList.emplace_back(SkyboxMesh);

	// 바운드박스 출력용 매쉬 생성
	BoundMesh = new Mesh;
	BoundMesh->CreateBoundboxMesh(Device, CmdList);
	LoadedMeshList.emplace_back(BoundMesh);

	// 바운드스페어 출력용 매쉬 생성
	BoundingSphereMesh = new Mesh(Device, CmdList, "Resources//SystemResources//Models//BoundingSphereMesh.txt", MESH_TYPE_TEXT);
	LoadedMeshList.emplace_back(BoundingSphereMesh);
}

// 업로드 버퍼를 삭제하고, 벡터를 비운다.
void ClearUploadBuffer() {
	for (auto const& MeshPtr : LoadedMeshList)
		MeshPtr->ReleaseUploadBuffers();

	for (auto const& TexturePtr : LoadedTextureList)
		TexturePtr->ReleaseUploadBuffers();

	LoadedMeshList.clear();
	LoadedTextureList.clear();
}