#include "stdafx.h"
#include "UIRenderer.h"
#include "UIObject.h"

CUIRenderer::CUIRenderer()
{
}
CUIRenderer::~CUIRenderer()
{
}

HRESULT CUIRenderer::Initialize()
{
	m_pRenderShader = RESOURCEMGR->GetRenderShader("UI").get();		//UI Shader
	return S_OK;
}

void CUIRenderer::RenderUI()
{
	if (m_mapUI.empty()) return;

	////공통 set
	m_pRenderShader->UpdateShaderState();
	m_pRenderShader->SetShaderState();

	multimap<float, CUIObject*>::iterator iter = m_mapUI.begin();
	multimap<float, CUIObject*>::iterator iter_End = m_mapUI.end();
	for (; iter != iter_End; ++iter)
	{
		iter->second->Render();
	}

	m_pRenderShader->CleanShaderState();
	m_mapUI.clear();		// 다그리고 제거
}
