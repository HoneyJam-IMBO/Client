#include "stdafx.h"
#include "Loading.h"
#include "SceneMgr.h"
#include "SceneMain.h"

CLoading::CLoading(SCENE_ID eID, CDirectXFramework* pFrameWork)
	:CScene(eID)
{
	m_pFrameWork = pFrameWork;
}
CLoading::~CLoading()
{
}

bool CLoading::Begin()
{
	HANDLE hThread = CreateThread(NULL, 0, FuncThread, this, 0, NULL);

	return true;
}
bool CLoading::End()
{
	return true;
}

void CLoading::Animate(float fTimeElapsed)
{
	if (true == m_bComplete)
	{
		CScene*	pScene = nullptr;
		switch (m_eSceneID){
		case SC_HEROSEL:		pScene = new CSceneMain(m_eSceneID, m_pFrameWork);			break;
		case SC_ORITOWN:		pScene = new CSceneMain(m_eSceneID, m_pFrameWork);			break;
		case SC_BURNNINGTOWN:	pScene = new CSceneMain(m_eSceneID, m_pFrameWork);			break;
		case SC_REPAIRTOWN:		pScene = new CSceneMain(m_eSceneID, m_pFrameWork);			break;
		case SC_ALDENAD:		pScene = new CSceneMain(m_eSceneID, m_pFrameWork);			break;
		case SC_BOSS:			pScene = new CSceneMain(m_eSceneID, m_pFrameWork);			break;
		}
		CSceneMgr::GetInstance()->SetPresentScene(m_eSceneID, pScene);

		End();
		delete this;
	}
}

void CLoading::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
}

void CLoading::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
}

void CLoading::ProcessInput(float fTimeElapsed)
{

}

DWORD WINAPI CLoading::FuncThread(LPVOID arg)
{
	RESOURCEMGR->ReleaseAllResource();

	// 계속 추가하는애들 함수로 ㄱㄱ
	RESOURCEMGR->CreateTexture("DEFAULT", _T("../../Assets/default.jpg"), PS_TEXTURE, BIND_PS);

	CLoading*	pMyThread = (CLoading*)arg;
	switch (pMyThread->GetSceneID())
	{
	case SC_HEROSEL:
		pMyThread->LoadScene_HEROSEL();
		break;
	case SC_ORITOWN:
		pMyThread->LoadScene_ORITOWN();
		break;
	case SC_BURNNINGTOWN:
		pMyThread->LoadScene_BURNNINGTOWN();
		break;
	case SC_REPAIRTOWN:
		pMyThread->LoadScene_REPAIRTOWN();
		break;
	case SC_ALDENAD:
		pMyThread->LoadScene_ALDENAD();
		break;
	case SC_BOSS:
		pMyThread->LoadScene_BOSS();
		break;
	}
	pMyThread->SetLoadComplete(true);
	return 0;
}
void CLoading::LoadScene_HEROSEL()
{
}

void CLoading::LoadScene_ORITOWN()
{
}

void CLoading::LoadScene_BURNNINGTOWN()
{
}

void CLoading::LoadScene_REPAIRTOWN()
{
}

void CLoading::LoadScene_ALDENAD()
{
}

void CLoading::LoadScene_BOSS()
{
}