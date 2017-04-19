#pragma once

#include "Scene.h"
//seller
#include "RenderContainerSeller.h"
//object
#include "FreeCamera.h"

//player
#include "Player.h"
//light
#include "PointLight.h"
#include "SpotLight.h"
#include "CapsuleLight.h"
#include "DirectionalLight.h"

static XMFLOAT4 xmf4DirectionalLightColor;

class CSceneMain;
struct LoadFileStruct {
	CSceneMain* m_pScene;
	string Filename;
};

class CSceneMain :public CScene{

public:
	CSceneMain(SCENE_ID eID, CDirectXFramework* pFrameWork);
	~CSceneMain();
public:
	//-----------------------------------scene--------------------------
	virtual bool Begin();
	virtual bool End();

	virtual void Animate(float fTimeElapsed);

	virtual void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	virtual void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	virtual void ProcessInput(float fTimeElapsed);
	CGameObject* PickObjectPointedByCursor(int xClient, int yClient);
	//-----------------------------------scene--------------------------

	//animation tool을 위한 함수
	void CreateControllObject(string path);
	
	//Container Creater
	void CreateSceneContainers();
	void CreateTerrainContainer();
	void CreateSkyBoxContainer();


private:
	CGameObject* m_pObject{ nullptr };
	
	int m_MeshCnt{ 0 };

	CPlayer* m_pPlayer{ nullptr };

};