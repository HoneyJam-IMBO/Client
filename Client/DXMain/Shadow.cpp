#include "stdafx.h"
#include "Shadow.h"


CShadow::CShadow()
	: DXObject("shadow")
{
}


CShadow::~CShadow()
{
}

bool CShadow::Begin()
{
	
	ReleaseBuffer();
	//m_pShadowVPBuffer = CBuffer::CreateConstantBuffer(1, sizeof(XMFLOAT4X4), 0, BIND_VS | BIND_DS , NULL);
	m_pCamera = make_shared<CFreeCamera>();
	//------------------------------------------카메라 제작--------------------------------------
	m_pCamera->Begin();
	m_pCamera->GenerateProjectionMatrix(60.0f * XM_PI / 180.0f
									, float(GLOBALVALUEMGR->GetrcClient().right) / float(GLOBALVALUEMGR->GetrcClient().bottom)
									, 0.01f, 10000.0f);

	//viewprojection행렬 제작 / 광원
	XMVECTOR eye = { 2000.0f, 2000.0f, 2000.0f, 0.0f };
	XMVECTOR at = { 0.0f, 0.0f, 0.0f, 0.0f };
	XMVECTOR up = { 0.0f, 1.0f, 0.0f, 0.0f };
	m_pCamera->SetLookAt(eye, at, up);

	return true;
}

bool CShadow::End()
{
	if (m_pCamera) {
		m_pCamera->End();
	}
	if (m_pd3dtxtShadow) m_pd3dtxtShadow->Release();//1
	m_pd3dtxtShadow = nullptr;
	if (m_pd3dsrvShadow) m_pd3dsrvShadow->Release();//2
	m_pd3dsrvShadow = nullptr;
	if (m_pd3ddsvShadow) m_pd3ddsvShadow->Release();
	m_pd3ddsvShadow = nullptr;
	if (m_pd3dtxtDepth) m_pd3dtxtDepth->Release();//1
	m_pd3dtxtDepth = nullptr;
	if (m_pd3drtvDepth) m_pd3drtvDepth->Release();//2
	m_pd3drtvDepth = nullptr;
	if (m_pd3dsrvDepth) m_pd3dsrvDepth->Release();
	m_pd3dsrvDepth = nullptr;

	return true;
}

void CShadow::SetShaderState()
{
}

void CShadow::CleanShaderState()
{
}

void CShadow::UpdateShaderState()
{
}

ID3D11ShaderResourceView * CShadow::RenderShadowMap(shared_ptr<CCamera> pCamera, shared_ptr<CCamera>& pLightCam, CObjectRenderer* objRenderer)
{
	//ID3D11RasterizerState* pPrevRSState;
	//GLOBALVALUEMGR->GetDeviceContext()->RSGetState(&pPrevRSState);
	//GLOBALVALUEMGR->GetDeviceContext()->RSSetState(m_pShaderRS);

	GLOBALVALUEMGR->GetDeviceContext()->ClearDepthStencilView(m_pd3ddsvShadow, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	float fClearColor[4] = {1.f, 1.f, 1.f, 1.f };
	if (m_pd3drtvDepth) GLOBALVALUEMGR->GetDeviceContext()->ClearRenderTargetView(m_pd3drtvDepth, fClearColor);

	m_pCamera->UpdateShaderState();
	m_pCamera->SetShaderState();
	m_pCamera->SetViewport(0, 0, 4096, 4096, 0.0f, 1.0f);

	UPDATER->GetSpaceContainer()->PrepareRender(m_pCamera, true);
	ID3D11RenderTargetView*	rtNULL = nullptr;
	ID3D11RenderTargetView *pd3dRTVs[1] = { m_pd3drtvDepth };
	GLOBALVALUEMGR->GetDeviceContext()->OMSetRenderTargets(1, pd3dRTVs, m_pd3ddsvShadow);
	//GLOBALVALUEMGR->GetDeviceContext()->PSSetShaderResources(m_TextureStartSlot, 1, &m_pd3dsrvShadow);
	objRenderer->ExcuteShadowRender(m_pCamera);

	pLightCam = m_pCamera;
	m_pCamera->SetViewport(0, 0, GLOBALVALUEMGR->GetrcClient().right, GLOBALVALUEMGR->GetrcClient().bottom, 0.0f, 1.0f);
	//GLOBALVALUEMGR->GetDeviceContext()->RSSetState(pPrevRSState);
	return m_pd3dsrvDepth;
}

void CShadow::ResizeBuffer()
{
	ReleaseBuffer();

	D3D11_DEPTH_STENCIL_VIEW_DESC d3dDepthStencilViewDesc;
	ZeroMemory(&d3dDepthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	d3dDepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	d3dDepthStencilViewDesc.Texture2D.MipSlice = 0;
	d3dDepthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;

	D3D11_TEXTURE2D_DESC d3dTexture2DDesc;
	::ZeroMemory(&d3dTexture2DDesc, sizeof(D3D11_TEXTURE2D_DESC));
	d3dTexture2DDesc.Width = 4096;
	d3dTexture2DDesc.Height = 4096;
	d3dTexture2DDesc.MipLevels = 1;
	d3dTexture2DDesc.ArraySize = 1;
	d3dTexture2DDesc.SampleDesc.Count = 1;
	d3dTexture2DDesc.SampleDesc.Quality = 0;
	d3dTexture2DDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dTexture2DDesc.CPUAccessFlags = 0;
	d3dTexture2DDesc.MiscFlags = 0;
	d3dTexture2DDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;
	d3dTexture2DDesc.Format = DXGI_FORMAT_R32_TYPELESS;

	D3D11_SHADER_RESOURCE_VIEW_DESC d3dSRVDesc;
	::ZeroMemory(&d3dSRVDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	d3dSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	d3dSRVDesc.Texture2D.MipLevels = 1;
	d3dSRVDesc.Format = DXGI_FORMAT_R32_FLOAT;
	//d3dTexture2DDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;
	GLOBALVALUEMGR->GetDevice()->CreateTexture2D(&d3dTexture2DDesc, nullptr, &m_pd3dtxtShadow);
	GLOBALVALUEMGR->GetDevice()->CreateDepthStencilView(m_pd3dtxtShadow, &d3dDepthStencilViewDesc, &m_pd3ddsvShadow);
	GLOBALVALUEMGR->GetDevice()->CreateShaderResourceView(m_pd3dtxtShadow, &d3dSRVDesc, &m_pd3dsrvShadow);

	// RTV 생성 할꺼임!=====================================

//	D3D11_TEXTURE2D_DESC d3dTexture2DDesc;
	::ZeroMemory(&d3dTexture2DDesc, sizeof(D3D11_TEXTURE2D_DESC));
	d3dTexture2DDesc.Width = 4096;
	d3dTexture2DDesc.Height = 4096;
	d3dTexture2DDesc.MipLevels = 1;
	d3dTexture2DDesc.ArraySize = 1;
	d3dTexture2DDesc.SampleDesc.Count = 1;
	d3dTexture2DDesc.SampleDesc.Quality = 0;
	d3dTexture2DDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dTexture2DDesc.CPUAccessFlags = 0;
	d3dTexture2DDesc.MiscFlags = 0;
	//d3dTexture2DDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

	D3D11_RENDER_TARGET_VIEW_DESC d3dRTVDesc;
	::ZeroMemory(&d3dRTVDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
	d3dRTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	d3dRTVDesc.Texture2D.MipSlice = 0;
	d3dTexture2DDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	d3dTexture2DDesc.Format = d3dSRVDesc.Format = d3dRTVDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	

	GLOBALVALUEMGR->GetDevice()->CreateTexture2D(&d3dTexture2DDesc, nullptr, &m_pd3dtxtDepth);
	GLOBALVALUEMGR->GetDevice()->CreateRenderTargetView(m_pd3dtxtDepth, &d3dRTVDesc, &m_pd3drtvDepth);
	GLOBALVALUEMGR->GetDevice()->CreateShaderResourceView(m_pd3dtxtDepth, &d3dSRVDesc, &m_pd3dsrvDepth);

}


void CShadow::ReleaseBuffer()
{
	if (m_pd3dtxtShadow) m_pd3dtxtShadow->Release();
	if (m_pd3dsrvShadow) m_pd3dsrvShadow->Release();
	if (m_pd3ddsvShadow) m_pd3ddsvShadow->Release();
	if (m_pd3dtxtDepth) m_pd3dtxtDepth->Release();
	if (m_pd3dsrvDepth) m_pd3dsrvDepth->Release();
	if (m_pd3drtvDepth) m_pd3drtvDepth->Release();
}
