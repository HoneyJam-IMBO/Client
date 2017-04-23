#pragma once
#include "Scene.h"

class CLoading :
	public CScene
{
public:
	CLoading(SCENE_ID eID, CDirectXFramework* pFrameWork);
	~CLoading();

public:
	HANDLE m_hLoadThread;
	static DWORD WINAPI FuncLoadResourceThread(LPVOID arg);

	HANDLE m_hLoadEvent;


private:
	bool	m_bComplete{ false };
public:
	void	SetLoadComplete(bool bCom) { m_bComplete = bCom; }
	bool	GetLoadComplete() { return m_bComplete; }

public:
	virtual bool Begin();
	virtual bool End();

	virtual void Animate(float fTimeElapsed);

	virtual void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	virtual void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	virtual void ProcessInput(float fTimeElapsed);

public: //LoadFunc
	void LoadScene_LOBBY();
	void LoadScene_HEROSEL();
	void LoadScene_ORITOWN();
	void LoadScene_REPAIRTOWN();
	void LoadScene_ALDENAD();
	void LoadScene_BOSS();


};

