#pragma once
#include "MeshUtil.h"
#include "TextureUtil.h"
#include "ImageShader.h"
#include "ObjectShader.h"
#include "BoundboxShader.h"
#include "LineShader.h"

// 매쉬, 텍스처, 쉐이더 전역 뱐수를 여기에 extern 선언
extern Mesh* MeshTerrain; // 터레인 매쉬
extern Mesh* MeshGunship; // 헬리콥터 몸체 매쉬
extern Mesh* MeshRotor; // 헬리콥터 날개 매쉬
extern Mesh* MeshBuilding; // 건물 매쉬
/////////////////////////////////////////////////////////////////////////////////

extern Texture* TextureHelp; // 도움말 텍스처
extern Texture* TextureArrow; // 화살표 텍스처
extern Texture* TextureEnter; // PressEnter 텍스처
extern Texture* TextureScroll; // Scroll help 텍스처

extern Texture* TextureTerrain; // 터레인 텍스처
extern Texture* TextureWater; // 물 텍스처
extern Texture* TextureGunship; // 헬리콥터 텍스처
extern Texture* TextureBuilding; // 건물 텍스처
extern Texture* TextureTree; // 나무 텍스처
extern Texture* TextureSkybox; // 스카이박스 텍스처
extern Texture* TextureAlert; // 회피 기동 알림 텍스처
/////////////////////////////////////////////////////////////////////////////////

extern Object_Shader* ObjectShader;
extern Boundbox_Shader* BoundboxShader;
extern Image_Shader* ImageShader;
extern Line_Shader* LineShader;
/////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////// 기본 리소스
// 매쉬, 텍스처 로드 시 포인터를 저장하는 벡터
// 한꺼번에 업로드 버퍼를 삭제함
extern std::vector<Mesh*> LoadedMeshList;
extern std::vector<Texture*> LoadedTextureList;

extern Mesh* ImagePannel;
extern Mesh* SkyboxMesh;
extern Mesh* BoundMesh;
extern Mesh* BoundingSphereMesh;

void LoadShader(ID3D12RootSignature* RootSignature, ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList);
void LoadSystemMesh(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList);
void LoadMesh(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList);
void LoadTexture(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList);
void ClearUploadBuffer();

inline void ImportMesh(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList, Mesh*& MeshPtr, char* Directory, int Type) {
	MeshPtr = new Mesh(Device, CmdList, Directory, Type);
	LoadedMeshList.emplace_back(MeshPtr);
}

inline void ImportTexture(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList, Texture*& TexturePtr, wchar_t* Directory, int Type) {
	TexturePtr = new Texture(Device, CmdList, Directory, Type);
	LoadedTextureList.emplace_back(TexturePtr);
}