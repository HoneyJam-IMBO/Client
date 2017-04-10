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

	return true;
}

void CSceneMgr::ChangeScene(SCENE_ID eID)
{
	::Safe_Delete(m_pPresentScene);

	m_pLoading = new CLoading(eID, m_pFrameWork);
	m_pLoading->Begin();
}

CSceneMgr::CSceneMgr() : CSingleTonBase<CSceneMgr>("SceneMgrsingleton")
{
}

CSceneMgr::~CSceneMgr()
{
}
