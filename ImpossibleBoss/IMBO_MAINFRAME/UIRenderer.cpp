#include "stdafx.h"
#include "UIRenderer.h"
#include "UIObject.h"

CUIRenderer::CUIRenderer()
{
}
CUIRenderer::~CUIRenderer()
{
	if (m_pPreBlendState)m_pPreBlendState->Release();
	if (m_pAlphaBlendState)m_pAlphaBlendState->Release();
}

HRESULT CUIRenderer::Initialize()
{
	m_pRenderShader = RESOURCEMGR->GetRenderShader("UI").get();		//UI Shader


	D3D11_BLEND_DESC tBlend;
	ZeroMemory(&tBlend, sizeof(D3D11_BLEND_DESC));
	tBlend.AlphaToCoverageEnable = TRUE;
	tBlend.IndependentBlendEnable = FALSE;
	tBlend.RenderTarget[0].BlendEnable = TRUE;
	tBlend.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	tBlend.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	tBlend.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	tBlend.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	tBlend.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	tBlend.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	tBlend.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	//blend state
	GLOBALVALUEMGR->GetDevice()->CreateBlendState(&tBlend, &m_pAlphaBlendState);


	return S_OK;
}

void CUIRenderer::RenderUI()
{
	if (m_mapUI.empty()) return;
	
	GLOBALVALUEMGR->GetDeviceContext()->OMGetBlendState(&m_pPreBlendState, m_pPreBlendFactor, &m_PreSampleMask);
	GLOBALVALUEMGR->GetDeviceContext()->OMSetBlendState(m_pAlphaBlendState, nullptr, 0xffffffff);
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

	GLOBALVALUEMGR->GetDeviceContext()->OMSetBlendState(m_pPreBlendState, nullptr, 0xffffffff);
}
