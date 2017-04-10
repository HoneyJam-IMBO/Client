#pragma once
#include "Scene.h"

class CLoading :
	public CScene
{
public:
	CLoading(SCENE_ID eID, CDirectXFramework* pFrameWork);
	~CLoading();

public:
	virtual bool Begin();
	virtual bool End();

	virtual void Animate(float fTimeElapsed);

	virtual void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	virtual void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	virtual void ProcessInput(float fTimeElapsed);
};

