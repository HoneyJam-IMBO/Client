#pragma once

#include "Scene.h"

//object
#include "FreeCamera.h"

class CSCOriTown;
class CSCOriTown :public CScene{

public:
	CSCOriTown(SCENE_ID eID, CDirectXFramework* pFrameWork);
	~CSCOriTown();
public:
	//-----------------------------------scene--------------------------
	virtual bool Begin();
	virtual bool End();

	virtual void Animate(float fTimeElapsed);

	virtual void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	virtual void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	virtual void ProcessInput(float fTimeElapsed);
	CGameObject* PickObjectPointedByCursor(int xClient, int yClient);
	

private:
	void ReadMapData();


private:
	CGameObject* m_pObject{ nullptr };

	//CPlayer* m_pPlayer{ nullptr };
};