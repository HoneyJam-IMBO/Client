
#pragma once

#include "SingleTon.h"
#include "Scene.h"
#include "stdafx.h"

class CDirectXFramework;
class CSceneMgr : public CSingleTonBase<CSceneMgr> {

public:
	bool Begin(CDirectXFramework* pFrame);
	bool End();

	CScene*		GetPresentScene() { return m_pPresentScene; }
	SCENE_ID	GetSceneID() { return m_eSceneID; }

	//Loading씬에서 셋
	void		SetPresentScene(SCENE_ID eID, CScene* pScene);
	void		ChangeScene(SCENE_ID eID);

private:
	CScene*		m_pPresentScene{ nullptr };
	CScene*		m_pLoading{ nullptr };

	SCENE_ID	m_eSceneID{ SC_END };
	//임시
	CDirectXFramework* m_pFrameWork{ nullptr };



public:
	CSceneMgr();
	virtual ~CSceneMgr();
};
