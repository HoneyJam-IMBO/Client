#pragma once

#include "SingleTon.h"
#include "GameObject.h"

#define THREAD_NUM 2

enum TOOL_MODE {
	//TOOL_MODE_ANIMATION,
	TOOL_MODE_TERRAIN,
	TOOL_MODE_SPLATTING,
	TOOL_MODE_OBJECTPOSITIONING,
	TOOL_MODE_FREECAMERA,
	TOOL_MODE_CHARACTER
};
class CGlobalValueManager : public CSingleTonBase<CGlobalValueManager> {

public:
	bool Begin(HINSTANCE hInst, HWND hWnd);
	bool End();

	//mode
	void SetWireFrameMode(bool b) { m_bWireFrame = b; }
	bool GetWireFrameMode() { return m_bWireFrame; }
	void SetDebugMode(bool b) { m_bDebug = b; }
	bool GetDebugMode() { return m_bDebug; }

	//wnd
	HINSTANCE GethInst() { return m_hInst; }
	HWND GethWnd() { return m_hWnd; }
	void SetrcClient(LONG right, LONG bottom) { m_rcClient.right = right; m_rcClient.bottom = bottom; }
	RECT GetrcClient() { return m_rcClient; }

	//device
	ID3D11Device* GetDevice() { return m_pd3dDevice; }
	ID3D11DeviceContext* GetDeviceContext() { return m_pd3dDeviceContext; }

	void SetSSLR(bool bSSLR) { m_bSSLR = bSSLR; }
	bool& GetSSLR() { return m_bSSLR; }

	CGameObject* GetPositioningObject() { return m_pPositioningObject; }
	void SetPositioningObject(CGameObject* pObject) { m_pPositioningObject = pObject; }

private:
	//begin func
	//wnd
	void SethInst(HINSTANCE hInst) { m_hInst = hInst; }
	void SethWnd(HWND hWnd) { m_hWnd = hWnd; }
	//begin func

	//mode
	bool m_bWireFrame{ false };
	bool m_bDebug{ false };
	bool m_bSSLR{ false };

	//wnd
	HINSTANCE m_hInst;
	HWND m_hWnd;
	RECT m_rcClient;

	//device
	ID3D11Device			*	m_pd3dDevice{ nullptr };
	ID3D11DeviceContext		*	m_pd3dDeviceContext{ nullptr };
	//ID3D11CommandList		*	m_pd3dDeviceCommandList[THREAD_NUM];

	std::mutex	m_MyLock;
public:
	std::mutex*	GetMutex() { return &m_MyLock; }

private:

	//object pisitioning °´Ã¼ 
	CGameObject* m_pPositioningObject{ nullptr };
public:
	CGlobalValueManager();
	virtual ~CGlobalValueManager();

};
