#include "stdafx.h"
#include "CameraMgr.h"


bool CCameraMgr::Begin()
{
	return true;
}

bool CCameraMgr::End()
{
	map<CAMTYPE, CCamera*>::iterator iter = m_mapCamera.begin();
	map<CAMTYPE, CCamera*>::iterator iter_end = m_mapCamera.end();
	for (; iter != iter_end; ++iter)
	{
		delete iter->second;
	}
	m_mapCamera.clear();
	return true;
}

CCamera * CCameraMgr::GetCamera(CAMTYPE eType)
{
	return m_mapCamera[eType];
}

void CCameraMgr::SetCamera(CAMTYPE eType, CCamera * pCam)
{
	m_mapCamera[eType] = pCam;

	int k = 0;
	 
}

void CCameraMgr::SetTarget(CAMTYPE eType, CGameObject * pTarget)
{
	if (m_mapCamera.find(eType) == m_mapCamera.end())
		return;

	m_mapCamera[eType]->SetTarget(pTarget);
}

CCameraMgr::CCameraMgr() : CSingleTonBase<CCameraMgr>("CameraMgrsingleton")
{
}
CCameraMgr::~CCameraMgr()
{
}
