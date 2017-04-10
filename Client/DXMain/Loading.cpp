#include "stdafx.h"
#include "Loading.h"


CLoading::CLoading(SCENE_ID eID, CDirectXFramework* pFrameWork)
	:CScene(eID)
{
	m_pFrameWork = pFrameWork;
}
CLoading::~CLoading()
{
}

bool CLoading::Begin()
{
	return true;
}
bool CLoading::End()
{
	return true;
}

void CLoading::Animate(float fTimeElapsed)
{
}

void CLoading::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
}

void CLoading::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
}

void CLoading::ProcessInput(float fTimeElapsed)
{
}
