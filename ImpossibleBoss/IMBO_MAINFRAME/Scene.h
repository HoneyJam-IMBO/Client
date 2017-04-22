#pragma once

#include "DXObject.h"
#include "GameObject.h"

class CDirectXFramework;

class CScene{

public:
	//begin end
	virtual bool Begin() { return true; };
	virtual bool Begin(string path);
	virtual bool End();
	//begin end
	//animate
	virtual void Animate(float fTimeElapsed);
	//animate
	//input
	virtual void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {};
	virtual void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {};
	virtual void ProcessInput(float fTimeElapsed) {};
	//input
	//pick
	virtual CGameObject* PickObjectPointedByCursor(int xClient, int yClient) { return nullptr; };
	//pick
	//create scene
	static CScene* CreateScene(string name, CDirectXFramework* pFramework, SCENE_ID eID);
	//create scene

	void LoadScene(string path);

	//load resource
	void CScene::LoadResource(wstring wsMeshRoot);

protected:
	string m_sName;

	//framework
	CDirectXFramework* m_pFrameWork{ nullptr };
	shared_ptr<CCamera> m_pCamera{ nullptr };
	CGameObject*	m_pPickingObject{ nullptr };

	SCENE_ID		m_eSceneID{ SC_END };
	bool			m_bIsLoading{ false };
public:
	bool		GetIsLoading() { return m_bIsLoading; };


public:
	CScene::CScene(SCENE_ID eID){ m_eSceneID = eID; }
	~CScene() {}

	//set get
	void SetFrameWork(CDirectXFramework* pFramework) { m_pFrameWork = pFramework; }
	CDirectXFramework* GetFrameWork() { return m_pFrameWork; }
	//set get
	SCENE_ID	GetSceneID() { return m_eSceneID; }
};
