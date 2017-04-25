#pragma once
#include "DXObject.h"

enum ANGLETYPE {
	ANGLE_X,
	ANGLE_Y,
	ANGLE_END
};

enum CAMERA_MODE {
	MODE_FIX,
	MODE_FREE,
	MODE_END
};

struct GBUFFER_UNPACKING_DATA {
	XMFLOAT4 PerspectiveValues;
	XMFLOAT4X4 ViewInv;
};

struct stCameraBuffer {
	XMFLOAT4X4	m_xmf4x4ViewProjection;
	XMFLOAT4	m_CameraPos;
};

class CPlayer;
class CCamera :public DXObject{
public:
	//----------------------------dxobject-----------------------------
	virtual bool Begin();
	virtual bool End();

public:
	virtual void SetShaderState() {	};
	virtual void CleanShaderState() { }
	void UpdateShaderState();

public:
	//flustum culling
	bool IsInFrustum(BoundingBox& boundingBox);
	//---------------------------------------카메라 조정---------------------------------
	void SetLookAt(XMVECTOR xmvPos, XMVECTOR xmvLookAt, XMVECTOR xmvUp);
	virtual void SetPosition(XMVECTOR pos);
	virtual void Rotate(float x = 0.0f, float y = 0.0f, float z = 0.0f) = 0;
	
public:
	virtual void Update(float fTimeElapsed) = 0;
	virtual void Render() {};				//debug모드시 카메라 그리기 

	virtual void ProcessInput(float fTimeElapsed) = 0;
	virtual XMMATRIX GetWorldMtx();			//world mtx 제작 후 반환 render할 때 사용 

public:
	void GenerateProjectionMatrix(float fFov, float fRatio, float fNear, float fFar);
	void SetViewport(DWORD xTopLeft, DWORD yTopLeft, DWORD nWidth, DWORD nHeight, float fMinZ, float fMaxZ);
	
	void UpdateViewMtx();				//viewmtx 갱신
	void UpdateReflectionViewMtx();		//viewReflectionmtx 갱신
	
	////player
	//void SetPlayer(CPlayer* pPlayer) { m_pPlayer = pPlayer; }
	//CPlayer* GetPlayer() { return m_pPlayer; }
	////player

public:		// Getter
	ID3D11Buffer* GetViewProjectionBuffer() { return m_pViewProjectionBuffer; }
	ID3D11Buffer* GetGBufferUnpackingBuffer() { return m_pGBufferUnpackingBuffer; }
	XMMATRIX GetViewMtx() { return XMLoadFloat4x4(&m_xmf4x4View); }
	XMMATRIX GetReflectionViewMtx() { return XMLoadFloat4x4(&m_xmf4x4ReflectionView); }
	XMMATRIX GetProjectionMtx() { return XMLoadFloat4x4(&m_xmf4x4Projection); }
	XMFLOAT4X4& GetProjectionFloat4x4() { return m_xmf4x4Projection; }
	D3D11_VIEWPORT& GetViewport() { return m_d3dViewport; }
	RECT GetClientSize() { return m_rcClient; };
	XMVECTOR GetPosition() { return XMLoadFloat3(&m_xmf3Pos); }
	XMVECTOR GetLook() { return XMLoadFloat3(&m_xmf3Look); }
	float GetFarClip() { return m_fFarClip; }
	float GetNearClip() { return m_fNearClip; }

protected:
	float m_fNearClip{ 0.f };
	float m_fFarClip{ 0.f };

protected:	//aabb flustum rcClient
	BoundingFrustum m_OriBoundingFrustum;
	BoundingFrustum m_BoundingFrustum;

	RECT m_rcClient;		//rect size

protected:
	CGameObject*	m_pTarget{ nullptr };
	CAMERA_MODE		m_eMode{ MODE_END };

	bool			m_bFix{ false };
public:
	void SetTarget(CGameObject* pTarget);
	void SetMode(CAMERA_MODE eMode) { m_eMode = eMode; }
	bool IsTarget() { return m_pTarget == nullptr ? false : true; }

protected:	//카메라 상수버퍼
	D3D11_VIEWPORT m_d3dViewport;
	
	stCameraBuffer m_stCameraBufferData;
	ID3D11Buffer* m_pViewProjectionBuffer;
	GBUFFER_UNPACKING_DATA m_GBufferUnpackingData;
	ID3D11Buffer* m_pGBufferUnpackingBuffer{ nullptr };

	XMFLOAT4X4 m_xmf4x4View;
	XMFLOAT4X4 m_xmf4x4ReflectionView;
	XMFLOAT4X4 m_xmf4x4Projection;

	XMFLOAT3 m_xmf3Right;
	XMFLOAT3 m_xmf3Up;
	XMFLOAT3 m_xmf3Look;
	XMFLOAT3 m_xmf3Pos;

	XMFLOAT3 m_xmf3At{ 0.f, 0.f, 1.f };
	XMFLOAT3 m_xmf3UpDefault{ 0.f, 1.f, 0.f };

public:
	float m_fCurrentAngle[ANGLE_END]{ 0.f };
	float m_fTargetAngle[ANGLE_END]{ 0.f };

	float m_fCurDistance{ 100.f };
	float m_fTgtDistance{ 100.f };

	float m_cxDelta{ 0.f };
	float m_cyDelta{ 0.f };
public:
	CCamera();
	virtual ~CCamera();

};