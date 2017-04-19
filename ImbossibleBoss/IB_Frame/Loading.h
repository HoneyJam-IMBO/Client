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
	HANDLE m_hRenderThread;
	static DWORD WINAPI FuncLoadResourceThread(LPVOID arg);
	static DWORD WINAPI FuncRenderThread(LPVOID arg);

	HANDLE m_hLoadEvent;

	vector<CUIObject*>	m_vecUI;
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
	void LoadScene_HEROSEL();
	void LoadScene_ORITOWN();
	void LoadScene_BURNNINGTOWN();
	void LoadScene_REPAIRTOWN();
	void LoadScene_ALDENAD();
	void LoadScene_BOSS();


public:	// Render pipeline
	ID3D11Texture2D			 *m_pd3dtxtDepthStencil{ nullptr };
	ID3D11Texture2D			 *m_pd3dtxtColorSpecInt{ nullptr };

	ID3D11DepthStencilView	 *m_pd3ddsvDepthStencil{ nullptr };
	ID3D11RenderTargetView   *m_pd3drtvColorSpecInt{ nullptr };

	IDXGISwapChain			*m_pdxgiSwapChain{ nullptr };

public:
	void MakeRenderPipe();
};

