#include "stdafx.h"
#include "SceneMain.h"

void CSceneMain::LoadScene(string path){
	CScene::LoadScene(path);
}
bool CSceneMain::Begin() {
	//m_pPlayer = new CPlayer;
	//m_pPlayer->Begin();
	//----------------------------------camera-------------------------------------
	m_pCamera = m_pFrameWork->GetCamera();
	
	//----------------------------------camera-------------------------------------

	//--------------------------------��ü ����------------------------
	int nMaxObjects = 1;
	int space_size = static_cast<int>(UPDATER->GetSpaceContainer()->GetSpaceSize());
	
	LoadScene("../outputdata/testScene.scn");

	string path = "../outputdata/testtesttest.gjm";
	string name = GetFileName(path);
	RESOURCEMGR->CreateMultiMesh(path, name);
	RCSELLER->CreateStempRenderContainer();

	m_pObject = new CGameObject("testtesttest", TAG_DYNAMIC_OBJECT);
	m_pObject ->Begin();
	m_pObject ->SetPosition(XMVectorSet(500, 0, 500, 0));
	m_pObject->SetScale(XMVectorSet(10,10, 10, 1));
	UPDATER->GetSpaceContainer()->AddObject(m_pObject);
	m_pObject->GetAnimater()->SetCurAnimationIndex(0);

	
	return CScene::Begin();
}

bool CSceneMain::End() {
	for (auto pObject : m_vpObjectList) {
		delete pObject;
	}
	m_vpObjectList.clear();
	//m_pPlayer->End();
	//seller
	//m_RenderContainerSeller->End();
	
	

	//ī�޶�� Framework�� �����ϴ� ���̱⿡ End()�۾��� �������� �ʴ´�.
	return CScene::End();
}

void CSceneMain::Animate(float fTimeElapsed) {
	CScene::Animate(fTimeElapsed);

	//--------------------------���� ��ü animate / regist-------------------------
	UPDATER->SetCamera(m_pCamera);
	//UPDATER->GetSpaceContainer()->SetSpaceSize(m_space_size);
	//UPDATER->GetSpaceContainer()->SetSpaceLevel(m_space_lv);
	//--------------------------���� ��ü animate / regist-------------------------
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
	switch (nMessageID)
	{
	case WM_KEYUP:
		
		switch (wParam)
		{
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

	if (INPUTMGR->OnlyKeyDown(VK_I)) {
		m_pObject->Move(m_pObject->GetLook(), 0.5);
	}
	if (INPUTMGR->OnlyKeyDown(VK_K)) {
		m_pObject->Move(-m_pObject->GetLook(), 0.5);
	}
	if (INPUTMGR->OnlyKeyDown(VK_L)) {
		m_pObject->Move(m_pObject->GetRight(), 0.5);
	}
	if (INPUTMGR->OnlyKeyDown(VK_J)) {
		m_pObject->Move(-m_pObject->GetRight(), 0.5);
	}
	m_pCamera->ProcessInput(fTimeElapsed);
}


CGameObject* CSceneMain::PickObjectPointedByCursor(int xClient, int yClient){

	if (!m_pCamera) return(NULL);

	//Get screen pos -> Camera pos
	XMFLOAT4X4 xmf4x4Projection;
	XMStoreFloat4x4(&xmf4x4Projection, m_pCamera->GetProjectionMtx());
	D3D11_VIEWPORT d3dViewport = m_pCamera->GetViewport();

	//�� �̰� ȭ���� ���� ���� ī�޶� ��ǥ���� �༮�̰�
	XMFLOAT3 xmf3PickPosition;
	xmf3PickPosition.x = (((2.0f * xClient) / d3dViewport.Width) - 1) / xmf4x4Projection._11;
	xmf3PickPosition.y = -(((2.0f * yClient) / d3dViewport.Height) - 1) / xmf4x4Projection._22;
	xmf3PickPosition.z = 1.0f;

	XMVECTOR xmvPickPosition;
	xmvPickPosition = XMLoadFloat3(&xmf3PickPosition);
	XMMATRIX xmMtxViewInverse;
	xmMtxViewInverse = XMMatrixInverse(nullptr, m_pCamera->GetViewMtx());
	//picking pos�� camera inverse�� �������� �̰� picking pos�� world pos!
	xmvPickPosition = XMVector3TransformCoord(xmvPickPosition, xmMtxViewInverse);
	XMVECTOR xmvRayDir = xmvPickPosition - m_pCamera->GetPosition();
	
	CGameObject* pNearestObject = NULL;
	float fHitDistance = FLT_MAX;
	float fNearDistance = FLT_MAX;
	pNearestObject = UPDATER->GetSpaceContainer()->PickObject(m_pCamera->GetPosition(), XMVector3Normalize(xmvRayDir), fHitDistance);
	fNearDistance = fHitDistance;
	
	return(pNearestObject);
}

void CSceneMain::CreateControllObject(string path){
//resource ����	
	m_MeshCnt = RESOURCEMGR->CreateMultiMesh(path, "Test");
	//m_MeshCnt = RESOURCEMGR->CreateMultiMesh("../outputata/text.txt", "Test");
	RCSELLER->GetRenderContainer("fbx")->ClearMesh();
	char pName[20];
	for (int i = 0; i < m_MeshCnt; ++i) {
		sprintf(pName, "%s%d", "Test", i);
		RCSELLER->GetRenderContainer("fbx")->AddMesh(RESOURCEMGR->GetMesh(pName, i));
	}
	RCSELLER->GetRenderContainer("fbx")->SetAnimater(RESOURCEMGR->GetAnimater("Test"));
//resource ����	


//��ü ����
	m_pFBXObject = new CTestObject();
	m_pFBXObject->Begin();
	//pObject->SetTerrainContainer(m_pTerrainContainer);
	m_pFBXObject->SetPosition(XMLoadFloat3(&XMFLOAT3(UPDATER->GetSpaceContainer()->GetSpaceSize() / 2.f, 0, UPDATER->GetSpaceContainer()->GetSpaceSize() / 2.f)));
	UPDATER->GetSpaceContainer()->AddObject(m_pFBXObject);
//��ü ����
	//ui pop up!
	m_pFBXObject->PickingProc();
}

void CSceneMain::AddFBXAnimationInfo(string path){
	if (nullptr == m_pFBXObject) {
		//wstring ws{ L"" };
		//ws.assign(path.cbegin(), path.cend());
		CreateControllObject(path);
		return;
	}

	//fbx animation info �߰�!
	string name{ "Test" };
	FBXIMPORTER->Begin(path);
	if (FBXIMPORTER->GetHasAnimation()) {
		CAnimationInfo* pAnimationInfo = CAnimationInfo::CreateAnimationInfoFromFBXFile(RESOURCEMGR->GetAnimater(name));
	}
	FBXIMPORTER->End();
	//fbx animation info �߰�!

	m_pFBXObject->PickingProc();
}
void CSceneMain::CreateTerrainContainer(){
	//terrain
	UPDATER->SetTerrainContainer(CTerrainContainer::CreateTerrainContainer(L"Temp", 256, 256, 0.5, UPDATER->GetSpaceContainer(), true));
}
void CSceneMain::CreateSkyBoxContainer(){
	//skybox
	UPDATER->SetSkyBoxContainer(CSkyBoxContainer::CreateSkyBoxContainer(L"Temp", 0, UPDATER->GetSpaceContainer()));
}

CSceneMain::CSceneMain(SCENE_ID eID, CDirectXFramework* pFrameWork) : CScene(eID) {
	m_pFrameWork = pFrameWork;
}
CSceneMain::~CSceneMain() {

}

