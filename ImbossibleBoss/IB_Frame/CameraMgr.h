#pragma once

#include "SingleTon.h"
#include "stdafx.h"
#include "Camera.h"

enum CAMTYPE
{
	CAM_FREE,
	CAM_LIGHT,
	CAM_END
};

class CCameraMgr
	: public CSingleTonBase<CCameraMgr>
{

public:
	bool Begin();
	bool End();

private:
	map<CAMTYPE, CCamera*>		m_mapCamera;

public:
	CCamera*	GetCamera(CAMTYPE eType);
	void		SetCamera(CAMTYPE eType, CCamera* pCam);
	
private:
	CGameObject*	m_pTarget{ nullptr };
public:
	void		SetTarget(CAMTYPE eType, CGameObject* pTarget);
	bool		IsTargetCam() {};

public:
	CCameraMgr();
	virtual ~CCameraMgr();
};

