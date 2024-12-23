#pragma once
#include "GameResource.h"
#include "CBVUtil.h"
#include "TransformUtil.h"
#include "CollisionUtil.h"
#include "MathUtil.h"
#include "LineBrush.h"
#include "TerrainUtil.h"

class Shader;
typedef ID3D12GraphicsCommandList* (CommandList);

class GameObject {
public:
	// 각 객체는 각자의 행렬을 가진다.
	XMFLOAT4X4 TranslateMatrix{ Mat4::Identity() };
	XMFLOAT4X4 RotateMatrix{ Mat4::Identity() };
	XMFLOAT4X4 ScaleMatrix { Mat4::Identity() };
	XMFLOAT4X4 ImageAspectMatrix{ Mat4::Identity() };

	// 매쉬 색상
	XMFLOAT3 ObjectColor{};

	// 텍스처 투명도
	float ObjectAlpha{ 1.0f };

	// 렌더링 타입, 해당 렌더링 타입에 따라 렌더링 형식이 달라진다.
	int RenderType{ RENDER_TYPE_3D };

	int ObjectLayer{};
	const char* ObjectTag{};
	bool DeleteCommand{};
	bool DeleteReserveCommand{};

	void InitRenderState(int RenderTypeFlag = RENDER_TYPE_3D);
	void SetColor(XMFLOAT3 Color);
	void SetColorRGB(float R, float G, float B);
	void SetLightUse(int Flag);
	void SetFogUse(int Flag);
	void FlipTexture(int FlipType);
	float ASP(float Value);
	void Render3D(Mesh* MeshPtr, Texture* TexturePtr, float AlphaValue=1.0f, bool DepthTestFlag=true);
	void Render2D(Texture* TexturePtr, float AlphaValue=1.0f, bool EnableAspect=true);
	void UpdateShaderVariables();
	void UpdateMotionRotation(float& RotationX, float& RotationY, float DeltaX, float DeltaY);
	void UpdateMotionRotation(XMFLOAT3& Rotation, float DeltaX, float DeltaY);
	int PickRayInter(Mesh* MeshPtr, XMVECTOR& xmvPickPosition, XMMATRIX& xmmtxView, float* pfHitDistance);

private:
	void GenPickingRay(XMVECTOR& xmvPickPosition, XMMATRIX& xmmtxView, XMVECTOR& xmvPickRayOrigin, XMVECTOR& xmvPickRayDirection);
	void SetCamera();

	////////// virtual functions
public:
	// 아래 함수들은 모든 객체에서 커스텀 가능한 버추얼 함수들이다. 필요하다면 새로운 버추얼 함수를 작성하여 사용할 수 있다.
	virtual ~GameObject() {}
	virtual void ReleaseShaderVariables() {}
	virtual void InputKey(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {}
	virtual void InputMouse(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {}
	virtual void InputMouseMotion(HWND hWnd, POINT MotionPosition) {}
	virtual void Update(float FT) {}
	virtual void Render() {}
	virtual Mesh* GetObjectMesh() { return {}; }
	virtual XMFLOAT3 GetPosition() { return {}; }
	virtual AABB GetAABB() { return {}; }
	virtual OOBB GetOOBB() { return {}; }
	virtual Range GetRange() { return {}; }
	virtual Vector GetVectorSet() { return {}; }
	virtual TerrainUtil GetTerrain() { return {}; }

	// 사용자 정의 리턴 함수는 아래에 정의한다.
	virtual AABB GetInsideAABB() { return{}; }
	virtual void EnableAlert() {}
	virtual void DisableAlert() {}
	virtual void SetPickingState() {}
};