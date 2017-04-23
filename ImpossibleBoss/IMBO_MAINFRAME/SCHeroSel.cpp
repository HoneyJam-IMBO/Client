#include "stdafx.h"
#include "SCHeroSel.h"
#include "ImageUI.h"
#include "ClickButton.h"

#include "LoadingBack.h"



CSCHeroSel::CSCHeroSel(SCENE_ID eID, CDirectXFramework* pFrameWork) : CScene(eID) {
	m_pFrameWork = pFrameWork;
}
CSCHeroSel::~CSCHeroSel()
{
}

bool CSCHeroSel::Begin()
{
	UPDATER->GetSkyBoxContainer()->SetActive(false);
	UPDATER->GetTerrainContainer()->SetActive(false);

	string strName = "Back";
	//CUIObject* pUI = CImageUI::Create(XMLoadFloat2(&XMFLOAT2(WINSIZEX * 0.5f, WINSIZEY * 0.5f)), XMLoadFloat2(&XMFLOAT2(WINSIZEX * 0.5f, WINSIZEY * 0.5f)), StringToTCHAR(strName), 0.f);
	//m_vecUI.push_back(pUI);
	CUIObject* pUI;

	for (int i = 0; i < 6; ++i)
	{
		strName = "CButton_" + to_string(i);
		pUI = CClickButton::Create(XMLoadFloat2(&XMFLOAT2(WINSIZEX * 0.5f - 400.f + i * 160.f, WINSIZEY * 0.8f)),
												XMLoadFloat2(&XMFLOAT2(70.f, 120.f)), StringToTCHAR(strName));
		m_vecUI.push_back(pUI);

		/*strName = "Char_Thumb_" + to_string(i);
		pUI = CImageUI::Create(XMLoadFloat2(&XMFLOAT2(WINSIZEX * 0.5f - 300.f + i * 200.f, WINSIZEY * 0.8f)),
			XMLoadFloat2(&XMFLOAT2(60.f, 60.f)), StringToTCHAR(strName));
		m_vecUI.push_back(pUI);*/
	}
	strName = "Logo";
	RESOURCEMGR->CreateTexture("Logo", _T("../../Assets/Loading/Logo.png"), PS_TEXTURE, BIND_PS);
	pUI = CLoadingBack::Create(XMLoadFloat2(&XMFLOAT2(WINSIZEX * 0.5f, WINSIZEX * 0.5f)), XMLoadFloat2(&XMFLOAT2(250.f, 250.f)), StringToTCHAR(strName));
	m_vecUI.push_back(pUI);
	
	
	return CScene::Begin();
}

bool CSCHeroSel::End()
{
	return true;
}

void CSCHeroSel::Animate(float fTimeElapsed)
{
	size_t iVecSize = m_vecUI.size();
	for (size_t i = 0; i < iVecSize; ++i)
	{
		m_vecUI[i]->Update(fTimeElapsed);
	}


	if (INPUTMGR->KeyDown(VK_1))
	{
		SCENEMGR->ChangeScene(SC_ORITOWN);
	}
}

void CSCHeroSel::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
}

void CSCHeroSel::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
}

void CSCHeroSel::ProcessInput(float fTimeElapsed)
{
}
