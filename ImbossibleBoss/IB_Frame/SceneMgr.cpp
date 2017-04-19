#include "stdafx.h"
#include "SceneMgr.h"

#include "DirectXFramework.h"
#include "Loading.h"

bool CSceneMgr::Begin(CDirectXFramework* pFrame)
{
	m_pFrameWork = pFrame;
	return true;
}

bool CSceneMgr::End()
{
	::Safe_EndDelete(m_pPresentScene);
	return true;
}

void CSceneMgr::SetPresentScene(SCENE_ID eID, CScene * pScene)
{
	m_pPresentScene = pScene;
	m_pPresentScene->Begin();
	m_eSceneID = eID;
}

void CSceneMgr::ChangeScene(SCENE_ID eID)
{
	::Safe_EndDelete(m_pPresentScene);

	m_pLoading = new CLoading(eID, m_pFrameWork);
	m_pLoading->Begin();
	m_pPresentScene = m_pLoading;
}

CSceneMgr::CSceneMgr() : CSingleTonBase<CSceneMgr>("SceneMgrsingleton")
{
}

CSceneMgr::~CSceneMgr()
{
}
