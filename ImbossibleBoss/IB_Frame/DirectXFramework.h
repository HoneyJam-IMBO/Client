#pragma once

#include "Camera.h"
#include "FreeCamera.h"
#define TITLE_MAX_LENGTH 64

class CDirectXFramework {

public:
	//사용될 개체 및 변수들 초기화.
	void Begin(HINSTANCE hInstance, HWND hWnd);
	void End();
	
public:
	void FrameAdvance();
	void Update(float fTimeElapsed);
	void Render();

	//--------------------------------------input------------------------------------
	void ProcessInput(float fTimeElapsed);
	LRESULT CALLBACK OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	shared_ptr<CCamera> GetCamera() { return m_pCamera; }
	
private:
	_TCHAR				m_pszBuffer[50];
	shared_ptr<CCamera> m_pCamera{ nullptr };


private:
	void	InitSingleton(HINSTANCE hInstance, HWND hWnd);
	void	Make_Camera();

public :	
	CDirectXFramework() = default;
	~CDirectXFramework();

};
