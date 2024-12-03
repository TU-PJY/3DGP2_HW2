#include "GameResource.h"
#include <random>
// �� ������ ���ҽ��� �����Ѵ�.
// �⺻������ ���� ���ҽ��̸�, ResourceManager.h�� ���� extern ������ ��, �� ���Ͽ� �Ʒ��� ���� �����ϸ� �ȴ�.
// Scene::Init()���� ����ȴ�.


Mesh* MeshTerrain; // �ͷ��� �Ž�
Mesh* MeshGunship; // �︮���� ��ü �Ž�
Mesh* MeshRotor; // �︮���� ���� �Ž�
Mesh* MeshBuilding; // �ǹ� �Ž�

// �Ž��� ���⼭ �ε��Ѵ�.
void LoadMesh(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList) {
	ImportMesh(Device, CmdList, MeshTerrain, "Resources//Model//Terrain.bin", MESH_TYPE_BIN);
	ImportMesh(Device, CmdList, MeshGunship, "Resources//Model//Gunship.bin", MESH_TYPE_BIN);
	ImportMesh(Device, CmdList, MeshRotor, "Resources//Model//Rotor.bin", MESH_TYPE_BIN);
	ImportMesh(Device, CmdList, MeshBuilding, "Resources//Model//Building.bin", MESH_TYPE_BIN);
}
/////////////////////////////////////////////////////////////////////////////////


Texture* TextureHelp; // ���� �ؽ�ó
Texture* TextureArrow; // ȭ��ǥ �ؽ�ó
Texture* TextureEnter; // PressEnter �ؽ�ó
Texture* TextureScroll; // Scroll help �ؽ�ó

Texture* TextureTerrain; // �ͷ��� �ؽ�ó
Texture* TextureWater; // �� �ؽ�ó
Texture* TextureGunship; // �︮���� �ؽ�ó
Texture* TextureBuilding; // �ǹ� �ؽ�ó
Texture* TextureTree; // ���� �ؽ�ó
Texture* TextureSkybox; // ��ī�̹ڽ� �ؽ�ó

// �ý�ó�� ���⼭ �ε��Ѵ�.
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

// ���̴��� ���⼭ �ε��Ѵ�.
void LoadShader(ID3D12RootSignature* RootSignature, ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList) {
	// �Ϲ� ������ ���̴� ����
	ObjectShader = new Object_Shader();
	// �⺻ ���������� ����
	ObjectShader->CreateDefaultPS(Device, RootSignature);
	// ���� �˻� ������ ���������� ����
	ObjectShader->CreateNoneDepthPS(Device, RootSignature);

	// �̹��� ��¿� ���������� ����
	ImageShader = new Image_Shader();
	ImageShader->CreateNoneDepthPS(Device, RootSignature);

	// �ٿ��ڽ� ���̴� ����
	BoundboxShader = new Boundbox_Shader();
	BoundboxShader->CreateWireframePS(Device, RootSignature);

	// ���� �귯�� ��¿� ���̴� ����
	LineShader = new Line_Shader();
	LineShader->CreateNoneDepthPS(Device, RootSignature);
}
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
// ���ε� ���۸� ó���ϱ� ���� ����
std::vector<Mesh*> LoadedMeshList;
std::vector<Texture*> LoadedTextureList;

Mesh* ImagePannel;
Mesh* SkyboxMesh;
Mesh* BoundMesh;
Mesh* BoundingSphereMesh;

// �⺻ ���� �Ž� �ε�
void LoadSystemMesh(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList) {
	ImagePannel = new Mesh;
	ImagePannel->CreateImagePannelMesh(Device, CmdList);
	LoadedMeshList.emplace_back(ImagePannel);

	// ��ī�̹ڽ� ��¿� �Ž� ����
	SkyboxMesh = new Mesh;
	SkyboxMesh->CreateSkyboxMesh(Device, CmdList);
	LoadedMeshList.emplace_back(SkyboxMesh);

	// �ٿ��ڽ� ��¿� �Ž� ����
	BoundMesh = new Mesh;
	BoundMesh->CreateBoundboxMesh(Device, CmdList);
	LoadedMeshList.emplace_back(BoundMesh);

	// �ٿ�彺��� ��¿� �Ž� ����
	BoundingSphereMesh = new Mesh(Device, CmdList, "Resources//SystemResources//Models//BoundingSphereMesh.txt", MESH_TYPE_TEXT);
	LoadedMeshList.emplace_back(BoundingSphereMesh);
}

// ���ε� ���۸� �����ϰ�, ���͸� ����.
void ClearUploadBuffer() {
	for (auto const& MeshPtr : LoadedMeshList)
		MeshPtr->ReleaseUploadBuffers();

	for (auto const& TexturePtr : LoadedTextureList)
		TexturePtr->ReleaseUploadBuffers();

	LoadedMeshList.clear();
	LoadedTextureList.clear();
}