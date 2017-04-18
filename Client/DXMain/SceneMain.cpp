#include "stdafx.h"
#include "SceneMain.h"


bool CSceneMain::Begin() {
	//----------------------------------camera-------------------------------------
	m_pCamera = m_pFrameWork->GetCamera();


	LoadScene("../outputdata/mac_scene/mac_scene.scn");
	m_pObject = new CGameObject("testtesttest", TAG_DYNAMIC_OBJECT);
	m_pObject->Begin();
	m_pObject->SetPosition(XMVectorSet(500, 0, 500, 0));
	m_pObject->SetScale(XMVectorSet(10, 10, 10, 1));
	UPDATER->GetSpaceContainer()->AddObject(m_pObject);
	m_pObject->GetAnimater()->SetCurAnimationIndex(0);
	
	
	return CScene::Begin();
}

bool CSceneMain::End() {	

	//카메라는 Framework에 존재하는 것이기에 End()작업을 진행하지 않는다.
	return CScene::End();
}

void CSceneMain::Animate(float fTimeElapsed) {
	CScene::Animate(fTimeElapsed);
	
	
}


void CSceneMain::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
	switch (nMessageID)
	{
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:

		POINT p = INPUTMGR->GetMousePoint();
		m_pPickingObject = PickObjectPointedByCursor(p.x, p.y);
		if (m_pPickingObject) {
			m_pPickingObject->PickingProc();
		}
		break;
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
		

	case WM_MOUSEMOVE:
		break;
	default:
		break;
	}
}
void CSceneMain::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
	
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

void CSceneMain::ProcessInput(float fTimeElapsed) {
	
	if (INPUTMGR->KeyDown(VK_P)) {
		INPUTMGR->SetDebugMode(static_cast<bool>((INPUTMGR->GetDebugMode() + 1) % 2));
	}
	m_pCamera->ProcessInput(fTimeElapsed);
}


CGameObject* CSceneMain::PickObjectPointedByCursor(int xClient, int yClient){

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

void CSceneMain::CreateControllObject(string path){
//resource 제작	
	m_MeshCnt = RESOURCEMGR->CreateMultiMesh(path, "Test");
	//m_MeshCnt = RESOURCEMGR->CreateMultiMesh("../outputata/text.txt", "Test");
	RCSELLER->GetRenderContainer("fbx")->ClearMesh();
	char pName[20];
	for (int i = 0; i < m_MeshCnt; ++i) {
		sprintf(pName, "%s%d", "Test", i);
		RCSELLER->GetRenderContainer("fbx")->AddMesh(RESOURCEMGR->GetMesh(pName, i));
	}
	RCSELLER->GetRenderContainer("fbx")->SetAnimater(RESOURCEMGR->GetAnimater("Test"));
//resource 제작	

//객체 제작
	m_pFBXObject = new CTestObject();
	m_pFBXObject->Begin();
	//pObject->SetTerrainContainer(m_pTerrainContainer);
	m_pFBXObject->SetPosition(XMLoadFloat3(&XMFLOAT3(UPDATER->GetSpaceContainer()->GetSpaceSize() / 2.f, 0, UPDATER->GetSpaceContainer()->GetSpaceSize() / 2.f)));
	UPDATER->GetSpaceContainer()->AddObject(m_pFBXObject);
//객체 제작
	//ui pop up!
	m_pFBXObject->PickingProc();
}

void CSceneMain::CreateSceneContainers(){
	
}

void CSceneMain::CreateTerrainContainer(){
	//terrain
	UPDATER->SetTerrainContainer(CTerrainContainer::CreateTerrainContainer(L"Temp", 256, 256, 0.5, UPDATER->GetSpaceContainer(), true));
}

void CSceneMain::CreateSkyBoxContainer(){
	//skybox
//	UPDATER->SetSkyBoxContainer(CSkyBoxContainer::CreateSkyBoxContainer(L"Temp", 0, UPDATER->GetSpaceContainer()));
}

CSceneMain::CSceneMain(SCENE_ID eID, CDirectXFramework* pFrameWork) : CScene(eID) {
	m_pFrameWork = pFrameWork;
}
CSceneMain::~CSceneMain() {

}

