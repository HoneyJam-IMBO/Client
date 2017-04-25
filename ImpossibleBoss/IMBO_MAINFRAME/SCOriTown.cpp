#include "stdafx.h"
#include "SCOriTown.h"
#include "Pawn.h"


bool CSCOriTown::Begin() {
	//----------------------------------camera-------------------------------------
	m_pCamera = m_pFrameWork->GetCamera();
	ReadMapData();

	m_pObject = new CPawn("new_elf10", TAG_DYNAMIC_OBJECT, true);
	m_pObject->Begin();
	m_pObject->SetTerrainContainer(UPDATER->GetTerrainContainer());
	m_pObject->SetPosition(XMVectorSet(500, 0, 500, 0));
	m_pObject->SetScale(XMVectorSet(1, 1, 1, 1));
	//m_pObject->G
	
	UPDATER->GetSpaceContainer()->AddObject(m_pObject);
	m_pObject->GetAnimater()->SetCurAnimationIndex(0);
	CAMMGR->SetTarget(CAM_FREE, m_pObject);
	CAMMGR->GetCamera(CAM_FREE)->SetMode(MODE_FIX);

	//NETWORKMGR->Connect("192.168.10.101");

	return CScene::Begin();
}

bool CSCOriTown::End() {	

	//카메라는 Framework에 존재하는 것이기에 End()작업을 진행하지 않는다.
	//Safe_EndDelete(m_pObject);
	return CScene::End();
}

void CSCOriTown::Animate(float fTimeElapsed) {
	CScene::Animate(fTimeElapsed);
}

void CSCOriTown::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
	switch (nMessageID)
	{
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
		//POINT p = INPUTMGR->GetMousePoint();
		//m_pPickingObject = PickObjectPointedByCursor(p.x, p.y);
		//if (m_pPickingObject) {
		//	m_pPickingObject->PickingProc();
		//}
		break;
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
		

	case WM_MOUSEMOVE:
		break;
	default:
		break;
	}
}
void CSCOriTown::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
	
	//RCSELLER->OnProcessingKeyboardMessage(hWnd, nMessageID, wParam, lParam);
	switch (nMessageID){
	case WM_KEYUP:
		switch (wParam){
		case VK_F1:
		case VK_F2:
		case VK_F3:
			break;
		case VK_P:
		default:
			break;
		}
		break;
	default:
		break;
	}
}

void CSCOriTown::ProcessInput(float fTimeElapsed) {
	
	if (INPUTMGR->KeyDown(VK_P)) {
		INPUTMGR->SetDebugMode(static_cast<bool>((INPUTMGR->GetDebugMode() + 1) % 2));
	}
	m_pCamera->ProcessInput(fTimeElapsed);
	if (INPUTMGR->KeyDown(VK_1))
	{
		SCENEMGR->ChangeScene(SC_ORITOWN);
	}
}


CGameObject* CSCOriTown::PickObjectPointedByCursor(int xClient, int yClient){

	if (!m_pCamera) return(NULL);

	//Get screen pos -> Camera pos
	XMFLOAT4X4 xmf4x4Projection;
	XMStoreFloat4x4(&xmf4x4Projection, m_pCamera->GetProjectionMtx());
	D3D11_VIEWPORT d3dViewport = m_pCamera->GetViewport();

	//음 이건 화면을 찍은 점의 카메라 좌표계의 녀석이고
	XMFLOAT3 xmf3PickPosition;
	xmf3PickPosition.x = (((2.0f * xClient) / d3dViewport.Width) - 1) / xmf4x4Projection._11;
	xmf3PickPosition.y = -(((2.0f * yClient) / d3dViewport.Height) - 1) / xmf4x4Projection._22;
	xmf3PickPosition.z = 1.0f;

	XMVECTOR xmvPickPosition;
	xmvPickPosition = XMLoadFloat3(&xmf3PickPosition);
	XMMATRIX xmMtxViewInverse;
	xmMtxViewInverse = XMMatrixInverse(nullptr, m_pCamera->GetViewMtx());
	//picking pos에 camera inverse를 곱했으니 이건 picking pos의 world pos!
	xmvPickPosition = XMVector3TransformCoord(xmvPickPosition, xmMtxViewInverse);
	XMVECTOR xmvRayDir = xmvPickPosition - m_pCamera->GetPosition();
	
	CGameObject* pNearestObject = NULL;
	float fHitDistance = FLT_MAX;
	float fNearDistance = FLT_MAX;
	pNearestObject = UPDATER->PickObject(m_pCamera->GetPosition(), XMVector3Normalize(xmvRayDir), fHitDistance);

	fNearDistance = fHitDistance;
	
	return(pNearestObject);
}

void CSCOriTown::ReadMapData()
{
	IMPORTER->Begin("../../Assets/SceneResource/simple_scene/simple_scene.scn");
	//output path
	wstring wsOutputPath = IMPORTER->ReadWstring();
	//scene name
	wstring wsSceneName = IMPORTER->ReadWstring();
	m_sName.assign(wsSceneName.cbegin(), wsSceneName.cend());

	UPDATER->LoadSpaceInfo();
	UPDATER->LoadTerrainInfo(wsOutputPath, wsSceneName);
	UPDATER->LoadObjectsInfo();
	//effect info
	RENDERER->LoadEffectInfo(wsOutputPath, wsSceneName);

	IMPORTER->End();
}

CSCOriTown::CSCOriTown(SCENE_ID eID, CDirectXFramework* pFrameWork) : CScene(eID) {
	m_pFrameWork = pFrameWork;
}
CSCOriTown::~CSCOriTown() {

}

