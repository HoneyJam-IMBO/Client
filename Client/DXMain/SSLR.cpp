#include "stdafx.h"
#include "SSLR.h"

bool g_bShowRayTraceRes = false;

bool CSSLR::Begin(){
	// Allocate the occlussion constant buffer
	m_pOcclusionCB = CBuffer::CreateConstantBuffer(1, sizeof(CB_OCCLUSSION), 0, BIND_CS);
	m_pRayTraceCB = CBuffer::CreateConstantBuffer(1, sizeof(CB_LIGHT_RAYS), 0, BIND_PS);

	m_pMakeOcclussionComputeShader = CComputeShader::CreateComputeShader(TEXT("Occlussion.cso"));
	m_pLayTraceRenderContainer = RCSELLER->GetRenderContainer("laytrace");
	m_pConmbineRenderContainer = RCSELLER->GetRenderContainer("combine");

	// Create the additive blend state
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
	GLOBALVALUEMGR->GetDevice()->CreateBlendState(&descBlend, &m_pAdditiveBlendState);
	return true;
}

bool CSSLR::End(){
	m_pOcclusionCB->End();
	m_pRayTraceCB->End();
	m_pAdditiveBlendState->Release();
	m_pMakeOcclussionComputeShader->End();
	delete m_pMakeOcclussionComputeShader;

	return true;
}

void CSSLR::SetShaderState(){

}

void CSSLR::CleanShaderState(){

}

void CSSLR::UpdateShaderState(){
}

void CSSLR::Excute(shared_ptr<CCamera>pCamera, ID3D11RenderTargetView* pLightAccumRTV, ID3D11ShaderResourceView* pMiniDepthSRV, const XMVECTOR& vSunDir, const XMFLOAT3& vSunColor,
	float fOffsetSunPos, float fMaxSunDist, float fInitDecay, float fDistDecay, float fMaxDeltaLen){
	XMFLOAT2 xmf2DotResult;
	XMStoreFloat2(&xmf2DotResult, -XMVector3Dot(pCamera->GetLook(), vSunDir));
	const float dotCamSun = xmf2DotResult.x;
	if (dotCamSun <= 0.0f)
	{
		return;
	}

	XMVECTOR vSunPos = fOffsetSunPos * vSunDir;
	XMFLOAT3 xmf3Eye;
	XMStoreFloat3(&xmf3Eye, pCamera->GetPosition());
	XMVECTOR offset = XMVectorSet(xmf3Eye.x, 0.f, xmf3Eye.z, 0.f);
	vSunPos += offset;
	XMMATRIX mView = pCamera->GetViewMtx();
	XMMATRIX mProj = pCamera->GetProjectionMtx();
	XMMATRIX mViewProjection = mView * mProj;
	XMVECTOR vSunPosSS;
	vSunPosSS = XMVector3TransformCoord(vSunPos, mViewProjection);

	// If the sun is too far out of view we just want to turn off the effect
	XMFLOAT3 xmf3SunPossSS;
	XMStoreFloat3(&xmf3SunPossSS, vSunPos);
	if (abs(long(xmf3SunPossSS.x)) >= fMaxSunDist || abs(long(xmf3SunPossSS.y)) >= fMaxSunDist){
		return;
	}
	XMFLOAT3 vSunColorAtt = vSunColor;
	//카메라 각도와 거리를 이용해 태양의 강도를 설정하는 것인데 
	//이건 내 scale과 예제 scale이 달라서 사용불가 수정필요

	//float fMaxDist = max(abs(long(xmf3SunPossSS.x)), abs(long(xmf3SunPossSS.y)));
	//if (fMaxDist >= 1.0f)
	//{
	//	vSunColorAtt.x = vSunColorAtt.x * (fMaxSunDist - fMaxDist);
	//	vSunColorAtt.y = vSunColorAtt.y * (fMaxSunDist - fMaxDist);
	//	vSunColorAtt.z = vSunColorAtt.z * (fMaxSunDist - fMaxDist);
	//}

	PrepareOcclusion(pMiniDepthSRV);
	RayTrace(pCamera, XMFLOAT2(xmf3SunPossSS.x, xmf3SunPossSS.y), vSunColorAtt, fInitDecay, fDistDecay, fMaxDeltaLen);
	if (!g_bShowRayTraceRes)
		Combine(pCamera, pLightAccumRTV);
}

void CSSLR::PrepareOcclusion(ID3D11ShaderResourceView * pMiniDepthSRV){
	CB_OCCLUSSION* pData = (CB_OCCLUSSION*)m_pOcclusionCB->Map();
	pData->nHeight = m_nHeight;
	pData->nWidth = m_nWidth;
	m_pOcclusionCB->Unmap();
	//cb set하고
	m_pOcclusionCB->SetShaderState();

	//minidepth srv넣고
	GLOBALVALUEMGR->GetDeviceContext()->CSSetShaderResources(0, 1, &pMiniDepthSRV);
	//occlussion uav를 set하고
	ID3D11UnorderedAccessView* pOcclussionUAV[] = { m_pOcclusionUAV };
	GLOBALVALUEMGR->GetDeviceContext()->CSSetUnorderedAccessViews(0, 1, pOcclussionUAV, (UINT*)&pOcclussionUAV);
	
	//cs 넣고 
	m_pMakeOcclussionComputeShader->ExcuteShaderState(m_nThreadGroup, 1, 1);

	//clear
	ID3D11ShaderResourceView* pClearSRVs[] = { nullptr };
	GLOBALVALUEMGR->GetDeviceContext()->CSSetShaderResources(0, 1, pClearSRVs);
	ID3D11UnorderedAccessView* pClearUAV[] = { nullptr };
	GLOBALVALUEMGR->GetDeviceContext()->CSSetUnorderedAccessViews(0, 1, pClearUAV, (UINT*)&pClearUAV);
	m_pOcclusionCB->CleanShaderState();

	//DEBUGER->AddTexture(XMFLOAT2(100, 250), XMFLOAT2(250, 400), m_pOcclusionSRV);
}

void CSSLR::RayTrace(shared_ptr<CCamera>pCamera, const XMFLOAT2 & vSunPosSS, const XMFLOAT3 & vSunColor, float fInitDecay, float fDistDecay, float fMaxDeltaLen){
	
	float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	GLOBALVALUEMGR->GetDeviceContext()->ClearRenderTargetView(m_pLightRaysRTV, ClearColor);

	D3D11_VIEWPORT oldvp;
	UINT num = 1;
	GLOBALVALUEMGR->GetDeviceContext()->RSGetViewports(&num, &oldvp);
	if (!g_bShowRayTraceRes){
		D3D11_VIEWPORT vp[1] = { { 0, 0, (float)m_nWidth, (float)m_nHeight, 0.0f, 1.0f } };
		GLOBALVALUEMGR->GetDeviceContext()->RSSetViewports(1, vp);

		GLOBALVALUEMGR->GetDeviceContext()->OMSetRenderTargets(1, &m_pLightRaysRTV, NULL);
	}

	// Constants
	//m_fInitDecay(0.2f), m_fDistDecay(0.8f), m_fMaxDeltaLen(0.005f)
	CB_LIGHT_RAYS* pRayTrace = (CB_LIGHT_RAYS*)m_pRayTraceCB->Map();
	pRayTrace->vSunPos = XMFLOAT2(0.5f * vSunPosSS.x + 0.5f, -0.5f * vSunPosSS.y + 0.5f);
	
	pRayTrace->fInitDecay = fInitDecay;
	pRayTrace->fDistDecay = fDistDecay;
	pRayTrace->vRayColor = vSunColor;
	pRayTrace->fMaxDeltaLen = fMaxDeltaLen;
	m_pRayTraceCB->Unmap();
	m_pRayTraceCB->SetShaderState();

	// Input
	m_pOcclussionTexture->SetShaderState();
	//render full screen
	m_pLayTraceRenderContainer->RenderWithOutObject(pCamera);

	GLOBALVALUEMGR->GetDeviceContext()->RSSetViewports(num, &oldvp);
	
	DEBUGER->AddTexture(XMFLOAT2(500, 150), XMFLOAT2(750, 300), m_pLightRaysSRV);
}

void CSSLR::Combine(shared_ptr<CCamera>pCamera, ID3D11RenderTargetView * pLightAccumRTV){
	ID3D11BlendState* pPrevBlendState;
	FLOAT prevBlendFactor[4];
	UINT prevSampleMask;
	GLOBALVALUEMGR->GetDeviceContext()->OMGetBlendState(&pPrevBlendState, prevBlendFactor, &prevSampleMask);
	GLOBALVALUEMGR->GetDeviceContext()->OMSetBlendState(m_pAdditiveBlendState, prevBlendFactor, prevSampleMask);

	// Restore the light accumulation view
	GLOBALVALUEMGR->GetDeviceContext()->OMSetRenderTargets(1, &pLightAccumRTV, NULL);

	// Constants
	m_pRayTraceCB->SetShaderState();
	
	// Input
	m_pLightRaysTexture->SetShaderState();

	m_pConmbineRenderContainer->RenderWithOutObject(pCamera);

	GLOBALVALUEMGR->GetDeviceContext()->OMSetBlendState(pPrevBlendState, prevBlendFactor, prevSampleMask);
}

void CSSLR::ResizeBuffer() {
	ReleaseBuffer();

	m_nWidth = GLOBALVALUEMGR->GetrcClient().right / 2;
	m_nHeight = GLOBALVALUEMGR->GetrcClient().bottom / 2;
	m_nThreadGroup = (UINT)ceil((float)(m_nWidth * m_nHeight) / 1024);

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// Allocate the occlusion resources
	D3D11_TEXTURE2D_DESC t2dDesc = {
		m_nWidth, //UINT Width;
		m_nHeight, //UINT Height;
		1, //UINT MipLevels;
		1, //UINT ArraySize;
		DXGI_FORMAT_R8_TYPELESS, //DXGI_FORMAT Format;
		1, //DXGI_SAMPLE_DESC SampleDesc;
		0,
		D3D11_USAGE_DEFAULT,//D3D11_USAGE Usage;
		D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE,//UINT BindFlags;
		0,//UINT CPUAccessFlags;
		0//UINT MiscFlags;    
	};
	GLOBALVALUEMGR->GetDevice()->CreateTexture2D(&t2dDesc, NULL, &m_pOcclusionTex);

	D3D11_UNORDERED_ACCESS_VIEW_DESC UAVDesc;
	ZeroMemory(&UAVDesc, sizeof(UAVDesc));
	UAVDesc.Format = DXGI_FORMAT_R8_UNORM;
	UAVDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
	GLOBALVALUEMGR->GetDevice()->CreateUnorderedAccessView(m_pOcclusionTex, &UAVDesc, &m_pOcclusionUAV);

	D3D11_SHADER_RESOURCE_VIEW_DESC dsrvd =
	{
		DXGI_FORMAT_R8_UNORM,
		D3D11_SRV_DIMENSION_TEXTURE2D,
		0,
		0
	};
	dsrvd.Texture2D.MipLevels = 1;
	GLOBALVALUEMGR->GetDevice()->CreateShaderResourceView(m_pOcclusionTex, &dsrvd, &m_pOcclusionSRV);
	m_pOcclussionTexture = CTexture::CreateTexture(m_pOcclusionSRV, 0, BIND_PS);
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// Allocate the light rays resources
	t2dDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	GLOBALVALUEMGR->GetDevice()->CreateTexture2D(&t2dDesc, NULL, &m_pLightRaysTex);

	D3D11_RENDER_TARGET_VIEW_DESC rtsvd =
	{
		DXGI_FORMAT_R8_UNORM,
		D3D11_RTV_DIMENSION_TEXTURE2D
	};
	GLOBALVALUEMGR->GetDevice()->CreateRenderTargetView(m_pLightRaysTex, &rtsvd, &m_pLightRaysRTV);

	GLOBALVALUEMGR->GetDevice()->CreateShaderResourceView(m_pLightRaysTex, &dsrvd, &m_pLightRaysSRV);
	m_pLightRaysTexture = CTexture::CreateTexture(m_pLightRaysSRV, 0, BIND_PS);
}

void CSSLR::ReleaseBuffer() {
	if(m_pOcclusionTex) m_pOcclusionTex->Release();
	m_pOcclusionTex = nullptr;
	if(m_pOcclusionUAV) m_pOcclusionUAV->Release();
	m_pOcclusionUAV = nullptr;
	if(m_pOcclusionSRV)m_pOcclusionSRV->Release();
	m_pOcclusionSRV = nullptr;
	
	if(m_pLightRaysTex)m_pLightRaysTex->Release();
	m_pLightRaysTex = nullptr;
	if(m_pLightRaysRTV)m_pLightRaysRTV->Release();
	m_pLightRaysRTV = nullptr;
	if(m_pLightRaysSRV)m_pLightRaysSRV->Release();
	m_pLightRaysSRV = nullptr;
	
	//if(m_pOcclussionTexture) m_pOcclussionTexture->End();
}


CSSLR::CSSLR() : DXObject("sslr") {

}


CSSLR::~CSSLR() {

}
