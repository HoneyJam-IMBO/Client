#pragma once

#include "Scene.h"
//seller
#include "RenderContainerSeller.h"
//object
#include "FreeCamera.h"
#include "ThirdPersonCamera.h"
#include "Bunny.h"
#include "TestCube.h"
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
	CGameObject* m_pObject{ nullptr };
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
	void AddFBXAnimationInfo(string path);
	
	void CreateTerrainContainer();
	void CreateSkyBoxContainer();
	
	CTestObject* GetFBXObject() { return m_pFBXObject; }
	void LoadScene(string path);
private:
	//tool 변수
	//모든 생성 가능한 객체를 벡터로 미리 만들어 둔다.
	//이 안의 모든 객체를 버튼으로써 제작한다.
	vector<CGameObject*> m_vpObjectList;
	vector<LoadFileStruct> m_LoadFileStruct;

	//fbx object
	CTestObject* m_pFBXObject{ nullptr };
	int m_MeshCnt{ 0 };
	//tool 변수

	//player
	CPlayer* m_pPlayer{ nullptr };
	//player
public:
	CSceneMain(SCENE_ID eID, CDirectXFramework* pFrameWork);
	~CSceneMain();
};