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

	//m_fAngleX += x;
	//m_fAngleY += y;

	////XMStoreFloat3(&m_xmf3At, XMVector4Transform(XMLoadFloat4(&XMFLOAT4(0.f, 0.f, 1.f, 0.f)), XMMatrixRotationRollPitchYaw(m_fLinearY, m_fLinearX, 0)));
	////XMStoreFloat3(&m_xmf3At, XMVector3Normalize(XMLoadFloat3(&m_xmf3At)));

	//if (nullptr != m_pTarget)
	//{
	//	m_pTarget->SetRotation(XMMatrixRotationY(m_fLinearX));
	//}
}

void CFreeCamera::Update(float fTimeElapsed) {

	UpdateShaderState();
}

//상수버퍼 갱신
void CFreeCamera::UpdateShaderState() {
	//정보 갱신

	float fTimeDelta = TIMEMGR->GetTimeElapsed();
	CalcultateMouseMoveValue();
	FixCamera();
	if(nullptr != m_pTarget )
		CameraInputProcess(fTimeDelta);

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


	if (true == m_bFix && MODE_FIX == m_eMode)
	{
		INPUTMGR->SetCheckMouse(false);
		POINT		ptMouse = { GLOBALVALUEMGR->GetrcClient().right >> 1, GLOBALVALUEMGR->GetrcClient().bottom >> 1 };
		ClientToScreen(GLOBALVALUEMGR->GethWnd(), &ptMouse);
		SetCursorPos(ptMouse.x, ptMouse.y);
		m_ptOldMousePos = ptMouse;
	}
}

void CFreeCamera::SetShaderState() {
	//상수버퍼 set
	GLOBALVALUEMGR->GetDeviceContext()->VSSetConstantBuffers(VS_CB_CAMERA, 1, &m_pViewProjectionBuffer);
	GLOBALVALUEMGR->GetDeviceContext()->DSSetConstantBuffers(DS_CB_CAMERA, 1, &m_pViewProjectionBuffer);
	GLOBALVALUEMGR->GetDeviceContext()->GSSetConstantBuffers(GS_CB_CAMERA, 1, &m_pViewProjectionBuffer);
}

void CFreeCamera::ProcessInput(float fTimeElapsed) {
	//DWORD dwDirection = 0;
	////UINT iMoveState;
	//float fDeltaMdis;
	//float fMouseSpeed = 10.f;
	////float cyDelta = INPUTMGR->GetcyDelta();
	//
	//if (fDeltaMdis = INPUTMGR->GetcxDelta())
	//{
	//	m_fTargetAngle[ANGLE_Y] += XMConvertToRadians(fDeltaMdis * fTimeElapsed * fMouseSpeed);
	//}

	//if (fDeltaMdis = INPUTMGR->GetcyDelta())
	//{
	//	if(XMConvertToDegrees(m_fTargetAngle[ANGLE_X]) >= -45.f && XMConvertToDegrees(m_fTargetAngle[ANGLE_X]) <= 45.f)
	//		m_fTargetAngle[ANGLE_X] += XMConvertToRadians(fDeltaMdis * fTimeElapsed * fMouseSpeed);
	//}
	//if()
}

void CFreeCamera::CameraInputProcess(float fTimeDelta)
{
	DWORD dwDirection = 0;
	//UINT iMoveState;
	//float fDeltaMdis;
	float fMouseSpeed = 20.f;
	//float cyDelta = INPUTMGR->GetcyDelta();

	if (nullptr != m_pTarget)
	{
		XMStoreFloat3(&m_xmf3At, XMVectorAdd(m_pTarget->GetPosition(), XMLoadFloat3(&XMFLOAT3(0.f, 30.f, 0.f))));
	}
		

	if (0.f != m_cxDelta)
	{
		m_fTargetAngle[ANGLE_Y] += XMConvertToRadians(m_cxDelta * fTimeDelta * fMouseSpeed);
	}

	if (0.f != m_cyDelta)
	{
		if (XMConvertToDegrees(m_fTargetAngle[ANGLE_X]) >= -45.f && XMConvertToDegrees(m_fTargetAngle[ANGLE_X]) <= 45.f)
			m_fTargetAngle[ANGLE_X] -= XMConvertToRadians(m_cyDelta * fTimeDelta * fMouseSpeed);
	}


	float fCamZoomSpd = 2.f;
	float fTagetDis = m_fTgtDistance + INPUTMGR->GetcurWheelDalta();

	//Lerp
	if (m_fTargetAngle[ANGLE_X] != m_fCurrentAngle[ANGLE_X])
	{
		m_fCurrentAngle[ANGLE_X] += (m_fTargetAngle[ANGLE_X] - m_fCurrentAngle[ANGLE_X]) * fTimeDelta * 5.f;
	}

	if (XMConvertToDegrees(m_fTargetAngle[ANGLE_X]) <= -45.f)
		m_fTargetAngle[ANGLE_X] = XMConvertToRadians(-45.f);
	else if (XMConvertToDegrees(m_fTargetAngle[ANGLE_X]) >= 45.f)
		m_fTargetAngle[ANGLE_X] = XMConvertToRadians(45.f);

	if (m_fTargetAngle[ANGLE_Y] != m_fCurrentAngle[ANGLE_Y])
	{
		m_fCurrentAngle[ANGLE_Y] += (m_fTargetAngle[ANGLE_Y] - m_fCurrentAngle[ANGLE_Y]) * fTimeDelta * 5.f;
	}

	if (fTagetDis != m_fCurDistance)
	{
		m_fCurDistance += (fTagetDis - m_fCurDistance) * fTimeDelta * 5.f;
	}


	XMMATRIX matRotX, matRotY, matWorld;
	matRotX = XMMatrixRotationX(m_fCurrentAngle[ANGLE_X]);
	matRotY = XMMatrixRotationY(m_fCurrentAngle[ANGLE_Y]);
	matWorld = XMMatrixMultiply(matRotX, matRotY);

	XMVECTOR vTempEyePos;
	vTempEyePos = XMVector4Transform(XMLoadFloat3(&XMFLOAT3(0.f, 0.f, m_fCurDistance)), matWorld);

	
	//m_vEye = m_vAt + vTempEyePos;
	XMStoreFloat3(&m_xmf3Pos, XMVectorAdd(XMLoadFloat3(&m_xmf3At), vTempEyePos));

	if (nullptr != m_pTarget)
	{
		//if(false == INPUTMGR->MouseRightDown() || false == m_pTarget->GetIdleState())
		//	m_pTarget->SetRotation(XMMatrixRotationY(m_fCurrentAngle[ANGLE_Y] + XM_PI));
	}
}

void CFreeCamera::FixCamera()
{
	if(INPUTMGR->KeyDown(VK_2))
		m_bFix ^= true;
}

void CFreeCamera::CalcultateMouseMoveValue()
{
	POINT ptCursorPos;
	GetCursorPos(&ptCursorPos);
	//마우스 버튼이 눌린 채로 이전 위치에서 현재 마우스 커서의 위치까지 움직인 양을 구한다.
	if (true == m_bFix)
	{
		m_cxDelta = (float)(ptCursorPos.x - m_ptOldMousePos.x) / 3.0f;
		m_cyDelta = (float)(ptCursorPos.y - m_ptOldMousePos.y) / 3.0f;
	}
	else
	{
		m_cxDelta = 0.f;
		m_cyDelta = 0.f;
	}
}

CFreeCamera::CFreeCamera() : CCamera() {
	
}
CFreeCamera::~CFreeCamera() {

}
