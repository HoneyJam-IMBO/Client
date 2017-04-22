#include "stdafx.h"
#include "WaterRenderer.h"


CWaterRenderer::CWaterRenderer()
	: DXObject("water") 
{
}


CWaterRenderer::~CWaterRenderer()
{
}

bool CWaterRenderer::Begin()
{
	ResizeBuffer();
	m_pWaterShader	= RESOURCEMGR->GetRenderShader("Water").get();
	m_pWaterMesh	= RESOURCEMGR->GetMesh("Water").get();


	m_pWaterVSBuffer = CBuffer::CreateConstantBuffer(1, sizeof(tWvsBuffer), 0, BIND_VS | BIND_DS, NULL);
	m_pWaterPSBuffer = CBuffer::CreateConstantBuffer(1, sizeof(tWpsBuffer), 4, BIND_PS, NULL);

	//m_pReflractionVSBuffer = CBuffer::CreateConstantBuffer(1, sizeof(float), 5, BIND_VS, NULL);


	D3D11_BLEND_DESC descBlend;
	ZeroMemory(&descBlend, sizeof(D3D11_BLEND_DESC));
	descBlend.RenderTarget[0].BlendEnable = true;
	descBlend.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	descBlend.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	descBlend.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;

	descBlend.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	descBlend.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	descBlend.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	descBlend.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	//blend state
	GLOBALVALUEMGR->GetDevice()->CreateBlendState(&descBlend, &m_pAlphaBlendState);



	return true;
}

bool CWaterRenderer::End()
{
	ReleaseBuffer();

	m_pWaterVSBuffer->End();	m_pWaterVSBuffer = nullptr;
	m_pWaterPSBuffer->End();	m_pWaterPSBuffer = nullptr;
	//m_pReflractionVSBuffer->End();	m_pReflractionVSBuffer = nullptr;

	return true;
}

void CWaterRenderer::SetShaderState()
{
}

void CWaterRenderer::CleanShaderState()
{
}

void CWaterRenderer::UpdateShaderState()
{
}

ID3D11ShaderResourceView * CWaterRenderer::RenderReflectionMap(shared_ptr<CCamera> pCamera, ID3D11DepthStencilView* pDepthStencilView, CObjectRenderer* objRenderer)
{
	GLOBALVALUEMGR->GetDeviceContext()->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	
	ID3D11RenderTargetView *pd3dRTVs[1] = { m_pd3drtvReflection };
	float fClearColor[4] = { 0.f, 0.f, 0.f, 1.f };
	if (m_pd3drtvReflection) GLOBALVALUEMGR->GetDeviceContext()->ClearRenderTargetView(m_pd3drtvReflection, fClearColor);

	CalReflectionViewProj(pCamera);
	GLOBALVALUEMGR->GetDeviceContext()->OMSetRenderTargets(1, pd3dRTVs, pDepthStencilView);
	
	objRenderer->Excute(pCamera);

	return m_pd3dsrvReflection;
}

ID3D11ShaderResourceView * CWaterRenderer::RenderRefractionMap(shared_ptr<CCamera> pCamera, ID3D11DepthStencilView * pDepthStencilView, CObjectRenderer * objRenderer)
{


	GLOBALVALUEMGR->GetDeviceContext()->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	ID3D11RenderTargetView *pd3dRTVs[1] = { m_pd3drtvRefraction };
	//float fClearColor[4] = { xmf4Xolor.x, xmf4Xolor.y, xmf4Xolor.z, xmf4Xolor.w };
	float fClearColor[4] = { 0.f, 0.f, 0.f, 1.f };
	if (m_pd3drtvRefraction) GLOBALVALUEMGR->GetDeviceContext()->ClearRenderTargetView(m_pd3drtvRefraction, fClearColor);

	CalReflectionViewProj(pCamera);
	GLOBALVALUEMGR->GetDeviceContext()->OMSetRenderTargets(1, pd3dRTVs, pDepthStencilView);

	objRenderer->Excute(pCamera);

	return m_pd3dsrvReflection;
}

void CWaterRenderer::RenderWater(shared_ptr<CCamera> pCamera, ID3D11ShaderResourceView* pDepthsrv)
{
	ID3D11BlendState*	m_pPreBlendState;
	GLOBALVALUEMGR->GetDeviceContext()->OMGetBlendState(&m_pPreBlendState, 0, nullptr);

	m_pWaterShader->UpdateShaderState();
	m_pWaterShader->SetShaderState();

	GLOBALVALUEMGR->GetDeviceContext()->PSSetShaderResources(1, 1, &m_pd3dsrvReflection);
	GLOBALVALUEMGR->GetDeviceContext()->PSSetShaderResources(2, 1, &pDepthsrv);

	CalReflectionViewProj(pCamera);
//	GLOBALVALUEMGR->GetDeviceContext()->OMSetBlendState(m_pAlphaBlendState, nullptr, 0xffffffff);

	m_pWaterMesh->Render(1);
	m_pWaterShader->CleanShaderState();
	GLOBALVALUEMGR->GetDeviceContext()->OMSetBlendState(m_pPreBlendState, nullptr, 0xffffffff);
}

void CWaterRenderer::ResizeBuffer()
{
	D3D11_TEXTURE2D_DESC d3dTexture2DDesc;
	::ZeroMemory(&d3dTexture2DDesc, sizeof(D3D11_TEXTURE2D_DESC));
	d3dTexture2DDesc.Width = GLOBALVALUEMGR->GetrcClient().right;
	d3dTexture2DDesc.Height = GLOBALVALUEMGR->GetrcClient().bottom;
	d3dTexture2DDesc.MipLevels = 1;
	d3dTexture2DDesc.ArraySize = 1;
	d3dTexture2DDesc.SampleDesc.Count = 1;
	d3dTexture2DDesc.SampleDesc.Quality = 0;
	d3dTexture2DDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dTexture2DDesc.CPUAccessFlags = 0;
	d3dTexture2DDesc.MiscFlags = 0;
	//d3dTexture2DDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

	D3D11_SHADER_RESOURCE_VIEW_DESC d3dSRVDesc;
	::ZeroMemory(&d3dSRVDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	d3dSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	d3dSRVDesc.Texture2D.MipLevels = 1;

	D3D11_RENDER_TARGET_VIEW_DESC d3dRTVDesc;
	::ZeroMemory(&d3dRTVDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
	d3dRTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	d3dRTVDesc.Texture2D.MipSlice = 0;
	d3dTexture2DDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	d3dTexture2DDesc.Format = d3dSRVDesc.Format = d3dRTVDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;

	// ¹Ý»ç¸Ê
	GLOBALVALUEMGR->GetDevice()->CreateTexture2D(&d3dTexture2DDesc, nullptr, &m_pd3dtxtReflection);
	GLOBALVALUEMGR->GetDevice()->CreateRenderTargetView(m_pd3dtxtReflection, &d3dRTVDesc, &m_pd3drtvReflection);
	GLOBALVALUEMGR->GetDevice()->CreateShaderResourceView(m_pd3dtxtReflection, &d3dSRVDesc, &m_pd3dsrvReflection);
	// ±¼Àý¸Ê
	GLOBALVALUEMGR->GetDevice()->CreateTexture2D(&d3dTexture2DDesc, nullptr, &m_pd3dtxtRefraction);
	GLOBALVALUEMGR->GetDevice()->CreateRenderTargetView(m_pd3dtxtRefraction, &d3dRTVDesc, &m_pd3drtvRefraction);
	GLOBALVALUEMGR->GetDevice()->CreateShaderResourceView(m_pd3dtxtRefraction, &d3dSRVDesc, &m_pd3dsrvRefraction);
}

void CWaterRenderer::ReleaseBuffer()
{
	if (m_pd3dtxtReflection) m_pd3dtxtReflection->Release();
	if (m_pd3drtvReflection) m_pd3drtvReflection->Release();
	if (m_pd3dsrvReflection) m_pd3dsrvReflection->Release();

	if (m_pd3dtxtRefraction) m_pd3dtxtRefraction->Release();
	if (m_pd3drtvRefraction) m_pd3drtvRefraction->Release();
	if (m_pd3dsrvRefraction) m_pd3dsrvRefraction->Release();
}

void CWaterRenderer::CalReflectionViewProj(shared_ptr<CCamera> pCamera)
{
	pCamera->UpdateReflectionViewMtx();
	tWvsBuffer* pData = (tWvsBuffer*)m_pWaterVSBuffer->Map();

	XMFLOAT4X4 mtxReflect;
	XMStoreFloat4x4(&mtxReflect, XMMatrixIdentity());
	XMStoreFloat4x4(&mtxReflect, XMMatrixMultiply(pCamera->GetReflectionViewMtx(), pCamera->GetProjectionMtx()));
	XMStoreFloat4x4(&pData->reflectionMatrix, XMMatrixTranspose(XMLoadFloat4x4(&mtxReflect)));

	XMMATRIX xvworld;
	xvworld = XMMatrixAffineTransformation(XMLoadFloat4(&XMFLOAT4(1000.f, 1.f, 1000.f, 1.f)),
											XMQuaternionIdentity(),
											XMQuaternionIdentity(),
											XMLoadFloat4(&XMFLOAT4(500.f, 0.f, 500.f, 1.f)));
	XMStoreFloat4x4(&pData->worldMatrix, XMMatrixTranspose(xvworld));
	XMStoreFloat4x4(&pData->viewMatrix, XMMatrixTranspose(pCamera->GetViewMtx()));
	XMStoreFloat4x4(&pData->projectionMatrix, XMMatrixTranspose(pCamera->GetProjectionMtx()));


	m_pWaterVSBuffer->Unmap();
	m_pWaterVSBuffer->SetShaderState();
}
