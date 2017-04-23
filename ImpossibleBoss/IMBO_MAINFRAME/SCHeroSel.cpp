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
	CUIObject* pUI = CImageUI::Create(XMLoadFloat2(&XMFLOAT2(WINSIZEX * 0.5f, WINSIZEY * 0.5f)), XMLoadFloat2(&XMFLOAT2(WINSIZEX * 0.5f, WINSIZEY * 0.5f)), StringToTCHAR(strName), 0.f);
	m_vecUI.push_back(pUI);

	for (int i = 0; i < 6; ++i)
	{
		strName = "CButton_" + to_string(i);
		pUI = CClickButton::Create(XMLoadFloat2(&XMFLOAT2(WINSIZEX * 0.5f - 400.f + i * 160.f, WINSIZEY * 0.8f)),
												XMLoadFloat2(&XMFLOAT2(70.f, 120.f)), StringToTCHAR(strName));
		m_vecButtonUI.push_back(pUI);

		strName = "Char_Thumb_" + to_string(i);
		pUI = CImageUI::Create(XMLoadFloat2(&XMFLOAT2(WINSIZEX * 0.5f, WINSIZEY * 0.4f)),
							XMLoadFloat2(&XMFLOAT2(200.f, 280.f)), StringToTCHAR(strName), 1.f);
		m_vecCharUI.push_back(pUI);
	}

	return CScene::Begin();
}

bool CSCHeroSel::End()
{
	RENDERER->GetUIRenderer()->ClearData();
	size_t iVecSize = m_vecUI.size();
	for (size_t i = 0; i < iVecSize; ++i)
	{
		delete m_vecUI[i];
	}
	iVecSize = m_vecButtonUI.size();
	for (size_t i = 0; i < iVecSize; ++i)
	{
		delete m_vecButtonUI[i];
	}
	return true;
}

void CSCHeroSel::Animate(float fTimeElapsed)
{
	size_t iVecSize = m_vecUI.size();
	for (size_t i = 0; i < iVecSize; ++i)
	{
		m_vecUI[i]->Update(fTimeElapsed);
	}
	iVecSize = m_vecButtonUI.size();
	for (size_t i = 0; i < iVecSize; ++i)
	{
		m_vecButtonUI[i]->Update(fTimeElapsed);
	}
	KeyInput();

	if (-1 != m_iHeroSelNum)
	{
		m_vecCharUI[m_iHeroSelNum]->Update(fTimeElapsed);
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

void CSCHeroSel::KeyInput()
{
	if (true == INPUTMGR->MouseLeftOnlyDown())
	{
		CheckCollisionButton();
	}
}

void CSCHeroSel::CheckCollisionButton()
{
	POINT ptPos = INPUTMGR->GetMousePoint();
	size_t iVecSize = m_vecButtonUI.size();
	for (size_t i = 0; i < iVecSize; ++i)
	{
		XMFLOAT2 xmButtonPos, xmButtonSize;
		XMStoreFloat2(&xmButtonPos, m_vecButtonUI[i]->GetUIPos());
		XMStoreFloat2(&xmButtonSize, m_vecButtonUI[i]->GetUISize());
		if ((ptPos.x > xmButtonPos.x - xmButtonSize.x&& ptPos.x < xmButtonPos.x + xmButtonSize.x )
			&& (ptPos.y > xmButtonPos.y - xmButtonSize.y&& ptPos.y < xmButtonPos.y + xmButtonSize.y))
		{
			m_iHeroSelNum = i;
			return;
		}
	}
}
