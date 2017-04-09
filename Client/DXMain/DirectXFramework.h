#pragma once

#include "Scene.h"
#include "FreeCamera.h"
#include "ThirdPersonCamera.h"
#include "Timer.h"

#define TITLE_MAX_LENGTH 64
//-------------------------deferred lighting------------------
#define RENDER_TARGET_NUMBER 3
//-------------------------deferred lighting------------------


class CDirectXFramework {

public:
	//사용될 개체 및 변수들 초기화.
	void Begin(HINSTANCE hInstance, HWND hWnd);
	void End();

	//-------------------------------begin----------------------------
	using FLOAT4 = float[4];
	//-------------------------------begin----------------------------
	
public:
	//그림 그리기
	void Render();

	//객체 Animate, input수용
	void Update(float fTimeElapsed);

	//흐름
	void FrameAdvance();

	//--------------------------------------input------------------------------------
	void ProcessInput(float fTimeElapsed);
	//윈도우의 메시지(키보드, 마우스 입력)를 처리하는 함수이다. 
	LRESULT CALLBACK OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	//--------------------------------------input------------------------------------

	//--------------------------------------scene-----------------------------------------
	void ChangeScene(CScene* pScene);
	void PopScene();
	void PushScene(CScene* pScene);
	CScene* GetCurScene() { return m_stackScene.top(); }
	//--------------------------------------scene-----------------------------------------

	shared_ptr<CCamera> GetCamera() { return m_pCamera; }
	
private:
	//str 
	_TCHAR									m_pszBuffer[50];
	//------------------------------------Scene-----------------------------------
	std::stack<CScene*>	m_stackScene;
	//scene 큐
	int m_nScene{ 0 };
	//scene의 개수
	shared_ptr<CCamera> m_pCamera{ nullptr };
	//------------------------------------Scene-----------------------------------

public :	
	CDirectXFramework() = default;
	~CDirectXFramework();
};
