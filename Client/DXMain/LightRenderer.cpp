#include "stdafx.h"
#include "LightRenderer.h"

bool CLightRenderer::Begin() {
	for (auto RenderContainer : RCSELLER->GetTagRenderContainer()[tag::TAG_LIGHT]) {
		m_mRenderContainer[RenderContainer.first] = RenderContainer.second;
	}
	//depth stencil state
	D3D11_DEPTH_STENCIL_DESC descDepthStencil;
	ZeroMemory(&descDepthStencil, sizeof(D3D11_DEPTH_STENCIL_DESC));
	descDepthStencil.DepthEnable = true;
	descDepthStencil.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	descDepthStencil.DepthFunc = D3D11_COMPARISON_GREATER_EQUAL;

	GLOBALVALUEMGR->GetDevice()->CreateDepthStencilState(&descDepthStencil, &m_pLightDepthStencilState);
	//depth stencil state

	//rasterizer state
	D3D11_RASTERIZER_DESC descRasterizer;
	ZeroMemory(&descRasterizer, sizeof(D3D11_RASTERIZER_DESC));
	descRasterizer.FillMode = D3D11_FILL_SOLID;
	//descRasterizer.FillMode = D3D11_FILL_WIREFRAME;
	descRasterizer.CullMode = D3D11_CULL_FRONT;
	GLOBALVALUEMGR->GetDevice()->CreateRasterizerState(&descRasterizer, &m_pLightRasterizerState);
	//rasterizer state

	//blend state
	D3D11_BLEND_DESC descBlend;
	ZeroMemory(&descBlend, sizeof(D3D11_BLEND_DESC));
	descBlend.RenderTarget[0].BlendEnable = true;
	descBlend.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	descBlend.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	descBlend.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;

	descBlend.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	descBlend.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	descBlend.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	descBlend.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	//blend state
	GLOBALVALUEMGR->GetDevice()->CreateBlendState(&descBlend, &m_pLightBlendState);

	return true;
}

bool CLightRenderer::End() {
	m_mRenderContainer.clear();

	return true;
}

void CLightRenderer::SetShaderState() {

}

void CLightRenderer::CleanShaderState() {
	//이전 상태로 되돌림
	GLOBALVALUEMGR->GetDeviceContext()->OMSetBlendState(m_pPreBlendState, m_pPreBlendFactor, m_PreSampleMask);
	GLOBALVALUEMGR->GetDeviceContext()->OMSetDepthStencilState(m_pPreDepthStencilState, m_PreStencilRef);
	GLOBALVALUEMGR->GetDeviceContext()->RSSetState(m_pPreRasterizerState);

	for (auto RenderContainer : m_mRenderContainer) {
		RenderContainer.second->ClearObjectList();
	}

}

void CLightRenderer::UpdateShaderState() {

}

void CLightRenderer::Excute(shared_ptr<CCamera> pCamera, shared_ptr<CCamera> pLightCam, ID3D11ShaderResourceView* pShadowSRV) {
	//--------------------------------deferred lighting--------------------------------
	ID3D11Buffer* pGBufferUnpackingBuffer = pCamera->GetGBufferUnpackingBuffer();
	GLOBALVALUEMGR->GetDeviceContext()->VSSetConstantBuffers(0, 1, &pGBufferUnpackingBuffer);
	GLOBALVALUEMGR->GetDeviceContext()->PSSetConstantBuffers(PS_UNPACKING_SLOT, 1, &pGBufferUnpackingBuffer);

	ID3D11Buffer* pViewProjectionBuffer = pCamera->GetViewProjectionBuffer();
	GLOBALVALUEMGR->GetDeviceContext()->VSSetConstantBuffers(1, 1, &pViewProjectionBuffer);
	GLOBALVALUEMGR->GetDeviceContext()->PSSetConstantBuffers(PS_CAMERA_DATA_SLOT, 1, &pViewProjectionBuffer);
	
	if (pShadowSRV)
	{
		ID3D11Buffer* pBuf[] = { pLightCam->GetViewProjectionBuffer() };
		GLOBALVALUEMGR->GetDeviceContext()->PSSetShaderResources(5, 1, &pShadowSRV);
		GLOBALVALUEMGR->GetDeviceContext()->PSSetConstantBuffers(4, 1, pBuf);
	}

	//RENDER
	//이전 상태 저장
	GLOBALVALUEMGR->GetDeviceContext()->OMGetDepthStencilState(&m_pPreDepthStencilState, &m_PreStencilRef);
	GLOBALVALUEMGR->GetDeviceContext()->RSGetState(&m_pPreRasterizerState);
	GLOBALVALUEMGR->GetDeviceContext()->OMGetBlendState(&m_pPreBlendState, m_pPreBlendFactor, &m_PreSampleMask);

	//
	m_mRenderContainer["directionallight"]->Render(pCamera);
	GLOBALVALUEMGR->GetDeviceContext()->OMSetBlendState(m_pLightBlendState, nullptr, 0xffffffff);
	GLOBALVALUEMGR->GetDeviceContext()->OMSetDepthStencilState(m_pLightDepthStencilState, 0);
	GLOBALVALUEMGR->GetDeviceContext()->RSSetState(m_pLightRasterizerState);

	m_mRenderContainer["pointlight"]->Render(pCamera);
	m_mRenderContainer["capsulelight"]->Render(pCamera);
	m_mRenderContainer["spotlight"]->Render(pCamera);

	CleanShaderState();
}

CLightRenderer::CLightRenderer() : DXObject("lightrenderer") {

}

CLightRenderer::~CLightRenderer() {

}
