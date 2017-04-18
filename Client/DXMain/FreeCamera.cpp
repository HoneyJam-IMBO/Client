#pragma once
#include "stdafx.h"
#include "FreeCamera.h"

#include "stdafx.h"
#include "Camera.h"

//DI
bool CFreeCamera::Begin() {

	m_pGBufferUnpackingBuffer = CreateBuffer(sizeof(GBUFFER_UNPACKING_DATA), 1, nullptr, D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DEFAULT, 0);
	
	return CCamera::Begin();
}
bool CFreeCamera::End() {
	
	return CCamera::End();
}

void CFreeCamera::Rotate(float x, float y, float z) {

	m_fAngleX += x;
	m_fAngleY += y;
	
	XMStoreFloat3(&m_xmf3At, XMVector4Transform(XMLoadFloat4(&XMFLOAT4(0.f, 0.f, 1.f, 0.f)), XMMatrixRotationRollPitchYaw(m_fAngleY, m_fAngleX, 0)));
	XMStoreFloat3(&m_xmf3At, XMVector3Normalize(XMLoadFloat3(&m_xmf3At)));
}

void CFreeCamera::Update(float fTimeElapsed) {

	UpdateShaderState();
}

//상수버퍼 갱신
void CFreeCamera::UpdateShaderState() {
	//정보 갱신
	XMMATRIX xmMtxProjection = (XMLoadFloat4x4(&m_xmf4x4Projection));
	UpdateViewMtx();
	XMMATRIX xmMtxView = XMLoadFloat4x4(&m_xmf4x4View);

	XMStoreFloat4x4(&m_stCameraBufferData.m_xmf4x4ViewProjection, XMMatrixTranspose(XMMatrixMultiply(xmMtxView, xmMtxProjection)));
	m_stCameraBufferData.m_CameraPos = XMFLOAT4(m_xmf3Pos.x, m_xmf3Pos.y, m_xmf3Pos.z, 1.0f);
	//정보 갱신

	//상수버퍼 업데이트
	GLOBALVALUEMGR->GetDeviceContext()->UpdateSubresource(m_pViewProjectionBuffer, 0, NULL, &m_stCameraBufferData, 0, 0);
	//--------------------------------deferred lighting--------------------------------
	XMFLOAT4 xmf4PerspectiveValues;
	
	xmf4PerspectiveValues.x = 1.0f / m_xmf4x4Projection._11;
	xmf4PerspectiveValues.y = 1.0f / m_xmf4x4Projection._22;
	xmf4PerspectiveValues.z = m_xmf4x4Projection._43;
	xmf4PerspectiveValues.w = -m_xmf4x4Projection._33;

	XMFLOAT4X4 xmf4x4ViewInverce;
	XMStoreFloat4x4(&xmf4x4ViewInverce, XMMatrixTranspose(XMMatrixInverse(nullptr, xmMtxView)));

	GBUFFER_UNPACKING_DATA data{ xmf4PerspectiveValues , xmf4x4ViewInverce };
	GLOBALVALUEMGR->GetDeviceContext()->UpdateSubresource(m_pGBufferUnpackingBuffer, 0, NULL, &data, 0, 0);
}

void CFreeCamera::SetShaderState() {
	//상수버퍼 set
	GLOBALVALUEMGR->GetDeviceContext()->VSSetConstantBuffers(VS_CB_CAMERA, 1, &m_pViewProjectionBuffer);
	GLOBALVALUEMGR->GetDeviceContext()->DSSetConstantBuffers(DS_CB_CAMERA, 1, &m_pViewProjectionBuffer);
	GLOBALVALUEMGR->GetDeviceContext()->GSSetConstantBuffers(GS_CB_CAMERA, 1, &m_pViewProjectionBuffer);
}

void CFreeCamera::ProcessInput(float fTimeElapsed) {
	DWORD dwDirection = 0;
	//UINT iMoveState;
	float cxDelta = INPUTMGR->GetcxDelta();
	float cyDelta = INPUTMGR->GetcyDelta();
	
#define SPEED 500

	XMVECTOR xmvShift = XMVectorSet(0.0f, 0.0f, 0.0f, 0.f);
	if (INPUTMGR->OnlyKeyBoardDown(VK_W))		dwDirection |= DIR_FORWARD;
	if (INPUTMGR->OnlyKeyBoardDown(VK_S))		dwDirection |= DIR_BACKWARD;
	if (INPUTMGR->OnlyKeyBoardDown(VK_A))		dwDirection |= DIR_LEFT;
	if (INPUTMGR->OnlyKeyBoardDown(VK_D))		dwDirection |= DIR_RIGHT;
	if (INPUTMGR->OnlyKeyBoardDown(VK_E))		dwDirection |= DIR_UP;
	if (INPUTMGR->OnlyKeyBoardDown(VK_Q))		dwDirection |= DIR_DOWN;

	if (dwDirection)
	{

		XMStoreFloat3(&m_xmf3Right, XMVector3Cross(XMLoadFloat3(&XMFLOAT3(0.f, 1.f, 0.f)), XMLoadFloat3(&m_xmf3At)));
		XMStoreFloat3(&m_xmf3Right, XMVector3Normalize(XMLoadFloat3(&m_xmf3Right)));

		if (dwDirection & DIR_FORWARD)		xmvShift += XMLoadFloat3(&m_xmf3At);
		if (dwDirection & DIR_BACKWARD)		xmvShift -= XMLoadFloat3(&m_xmf3At);
		if (dwDirection & DIR_RIGHT)		xmvShift += XMLoadFloat3(&m_xmf3Right);
		if (dwDirection & DIR_LEFT)			xmvShift -= XMLoadFloat3(&m_xmf3Right);
		if (dwDirection & DIR_UP)			xmvShift += XMLoadFloat3(&m_xmf3Up);
		if (dwDirection & DIR_DOWN)			xmvShift -= XMLoadFloat3(&m_xmf3Up);

		XMVector3Normalize(xmvShift);

		//pos에 shift*speed*ftimeElapsed 더해줌 = 속도만큼 이동
		XMStoreFloat3(&m_xmf3Pos, XMLoadFloat3(&m_xmf3Pos) + ((xmvShift * SPEED) * fTimeElapsed));

	}

	if (cxDelta || cyDelta)
	{
		cxDelta *= 0.01f;
		cyDelta *= 0.01f;
		/*cxDelta는 y-축의 회전을 나타내고 cyDelta는 x-축의 회전을 나타낸다. 오른쪽 마우스 버튼이 눌려진 경우 cxDelta는 z-축의 회전을 나타낸다.*/
		if (INPUTMGR->MouseRightDown())
			Rotate(cyDelta, 0.0f, -cxDelta);
		else
			Rotate(cxDelta, cyDelta, 0.0f);
	}
}

CFreeCamera::CFreeCamera() : CCamera() {
	
}
CFreeCamera::~CFreeCamera() {

}
