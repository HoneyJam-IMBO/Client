#pragma once

#include "DXObject.h"
#include "GameObject.h"

class CDirectXFramework;

class CScene : public CObject{

public:
	//begin end
	virtual bool Begin() { return true; };
	virtual bool Begin(string path);
	virtual bool End();
	//animate
	virtual void Animate(float fTimeElapsed);
	//input
	virtual void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {};
	virtual void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {};
	virtual void ProcessInput(float fTimeElapsed) {};
	//pick
	virtual CGameObject* PickObjectPointedByCursor(int xClient, int yClient) { return nullptr; };
	//create scene
	static CScene* CreateScene(string name, CDirectXFramework* pFramework);

	void LoadScene(string path);
protected:

	//framework
	CDirectXFramework* m_pFrameWork{ nullptr };
	//진짜 카메라는 framework에 있다.
	shared_ptr<CCamera> m_pCamera{ nullptr };
	//picking
	CGameObject* m_pPickingObject{ nullptr };

	SCENE_ID		m_eSceneID{ SC_END };
private:

public:
	CScene::CScene(SCENE_ID eID):CObject("Scene") { 
		m_eSceneID = eID;
	}
	~CScene() {}

	//set get
	void SetFrameWork(CDirectXFramework* pFramework) { m_pFrameWork = pFramework; }
	CDirectXFramework* GetFrameWork() { return m_pFrameWork; }
	//set get
};
