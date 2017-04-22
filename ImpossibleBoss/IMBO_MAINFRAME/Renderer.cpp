
#include "stdafx.h"
#include "Renderer.h"

bool CRenderer::Begin() {
	
	D3D11_DEPTH_STENCIL_DESC descDepth;
	descDepth.DepthEnable = TRUE;
	descDepth.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	descDepth.DepthFunc = D3D11_COMPARISON_LESS;
	descDepth.StencilEnable = TRUE;
	descDepth.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	descDepth.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
	const D3D11_DEPTH_STENCILOP_DESC stencilMarkOp = { D3D11_STENCIL_OP_REPLACE, D3D11_STENCIL_OP_REPLACE, D3D11_STENCIL_OP_REPLACE, D3D11_COMPARISON_ALWAYS };
	descDepth.FrontFace = stencilMarkOp;
	descDepth.BackFace = stencilMarkOp;
	GLOBALVALUEMGR->GetDevice()->CreateDepthStencilState(&descDepth, &m_pd3dDepthStencilState);
	if (false == CreateSwapChain()) return false;


	//layer
	m_pObjectRenderer = new CObjectRenderer();
	m_pObjectRenderer->Begin();
	m_pAORenderer = new CAORenderer();
	m_pAORenderer->Begin();

	m_pLightRenderer = new CLightRenderer();
	m_pLightRenderer->Begin();

	//post processing
	m_pBloomDownScale = new CBloomDownScale();	m_pBloomDownScale->Begin();
	m_pBloom = new CBloom();					m_pBloom->Begin();
	m_p16to1Blur = new CBlur();					m_p16to1Blur->Begin();
	m_p4to1Blur = new CBlur();					m_p4to1Blur->Begin();
	m_pPostProcessingFinalPass = new CPostProcessingFinalPass();	m_pPostProcessingFinalPass->Begin();
	m_pSSLR = new CSSLR();						m_pSSLR->Begin();
	//layer

	//shadow
	m_pShadow = new CShadow();
	m_pShadow->Begin();

	//UIRenderer 
	m_pUIRederer = new CUIRenderer();
	m_pUIRederer->Initialize();
	m_pWaterRenderer = new CWaterRenderer();
	m_pWaterRenderer->Begin();
	
	// render target�� depth-stencil buffer ����/ deferred texture ����
	if (!CreateRenderTargetView()) {
		MessageBox(GLOBALVALUEMGR->GethWnd(), TEXT("RenderTarget�̳� Depth-Stencil ���� ������ �����߽��ϴ�. ���α׷��� �����մϴ�."), TEXT("���α׷� ���� ����"), MB_OK);
		return false;
	}
	return true;
}

bool CRenderer::End() {
	if (m_pdxgiSwapChain) m_pdxgiSwapChain->Release();
	if (m_pd3dRenderTargetView) m_pd3dRenderTargetView->Release();
	if (m_pd3ddsvReadOnlyDepthStencil) m_pd3ddsvReadOnlyDepthStencil->Release();
	
	ReleaseForwardRenderTargets();

	//layer
	Safe_EndDelete(m_pObjectRenderer);
	Safe_EndDelete(m_pAORenderer);
	Safe_EndDelete(m_pLightRenderer);
	Safe_EndDelete(m_pBloomDownScale);
	Safe_EndDelete(m_pBloom);
	Safe_EndDelete(m_p16to1Blur);
	Safe_EndDelete(m_p4to1Blur);
	Safe_EndDelete(m_pPostProcessingFinalPass);
	Safe_EndDelete(m_pSSLR);
	Safe_EndDelete(m_pShadow);
	Safe_EndDelete(m_pWaterRenderer);
	//Safe_Delete(m_pUIRederer);

	if (m_pUIRederer)
		delete m_pUIRederer;

	return true;
}

void CRenderer::PreRender()
{
	//CLEAR
	RESOURCEMGR->GetSampler("WRAP_LINEAR")->SetShaderState();
	//RESOURCEMGR->GetSampler("WRAP_POINT")->SetShaderState();
	//RESOURCEMGR->GetSampler("CLAMP_LINEAR")->SetShaderState();
	//RESOURCEMGR->GetSampler("CLAMP_POINT")->SetShaderState();
	//RESOURCEMGR->GetSampler("SHADOW")->SetShaderState();
	//////


	//CLEAR
	ClearDepthStencilView(m_pd3ddsvDepthStencil);
	ID3D11RenderTargetView *pd3dRTVs[1] = { m_pd3drtvColorSpecInt };
	float fClearColor[4] = { 0.f, 0.f, 0.f, 0.f };
	//if (m_pd3drtvColorSpecInt) GLOBALVALUEMGR->GetDeviceContext()->ClearRenderTargetView(m_pd3drtvColorSpecInt, fClearColor);
	//SetRenderTargetViews(1, pd3dRTVs, m_pd3ddsvDepthStencil);
	GLOBALVALUEMGR->GetDeviceContext()->OMSetDepthStencilState(m_pd3dDepthStencilState, 1);
	//m_pCamera->SetShaderState();

	SetMainRenderTargetView();
	if (nullptr != m_pUIRederer){
		m_pUIRederer->RenderUI();
	}


	DEBUGER->ClearDebuger();

	HRESULT hr = m_pdxgiSwapChain->Present(0, 0);
	if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
	{
		DEBUGER->DebugMessageBox("explosion", "d");
		return;
	}
}

void CRenderer::Render(shared_ptr<CCamera> pCamera) {
	//CLEAR
	RESOURCEMGR->GetSampler("WRAP_LINEAR")->SetShaderState();
	RESOURCEMGR->GetSampler("WRAP_POINT")->SetShaderState();
	RESOURCEMGR->GetSampler("CLAMP_LINEAR")->SetShaderState();
	RESOURCEMGR->GetSampler("CLAMP_POINT")->SetShaderState();
	RESOURCEMGR->GetSampler("SHADOW")->SetShaderState();
	//////

	//shadow render
	m_pShadow->RenderShadowMap(pCamera);

	ID3D11ShaderResourceView* pRFL = m_pWaterRenderer->RenderReflectionMap(pCamera, m_pd3ddsvDepthStencil, m_pObjectRenderer);

	//clear buff
	//CLEAR
	ClearDepthStencilView(m_pd3ddsvDepthStencil);
	SetForwardRenderTargets();//gbuff�� �� rtv/ dsv set
	GLOBALVALUEMGR->GetDeviceContext()->OMSetDepthStencilState(m_pd3dDepthStencilState, 1);


	
	//object
	pCamera->SetShaderState();
	UPDATER->GetSpaceContainer()->PrepareRender(pCamera);
	UPDATER->GetSkyBoxContainer()->GetSkyBox()->RegistToContainer();
	m_pObjectRenderer->Excute(pCamera);
	
	m_pWaterRenderer->RenderWater(pCamera, m_pd3dsrvDepthStencil);

	if (INPUTMGR->GetDebugMode()) {	DEBUGER->DebugRender(pCamera);}
	
	//SSAO
	SetRenderTargetViews(1, &m_pd3drtvLight, m_pd3ddsvReadOnlyDepthStencil);
	for (auto texture : m_vObjectLayerResultTexture) {
		texture->SetShaderState();
	}	
	float SSAO_OffsetRadius = m_fSSAOOffsetRadius;//m_pFramework->GetCurScene()->GetSSAOOffsetRadius();
	float SSAO_Radius = m_fSSAORadius;//m_pFramework->GetCurScene()->GetSSAORadius();
	ID3D11ShaderResourceView* pAmbientOcclution = m_pAORenderer->Excute(pCamera, SSAO_OffsetRadius, SSAO_Radius);
	pAmbientOcclution  = m_p4to1Blur->Excute(pAmbientOcclution);
	GLOBALVALUEMGR->GetDeviceContext()->PSSetShaderResources(4, 1, &pAmbientOcclution);

	//LIGHT RENDER
	//SetMainRenderTargetView();
	m_pLightRenderer->Excute(pCamera, m_pShadow);
	for (auto texture : m_vObjectLayerResultTexture) {
		texture->CleanShaderState();
	}

	//SSLR
	if (m_bSSLROnOff) {
		if (UPDATER->GetDirectionalLight()) {
			D3D11_VIEWPORT oldvp;
			UINT num = 1;
			GLOBALVALUEMGR->GetDeviceContext()->RSGetViewports(&num, &oldvp);
			ID3D11RasterizerState* pPrevRSState;
			GLOBALVALUEMGR->GetDeviceContext()->RSGetState(&pPrevRSState);
	
			XMVECTOR xmvSunDir = UPDATER->GetDirectionalLight()->GetLook();
			XMFLOAT3 xmf3Color = UPDATER->GetDirectionalLight()->GetColor();
			float fOffsetSunPos = UPDATER->GetDirectionalLight()->GetOffsetLength();
			float fMaxSunDist = m_fSSLRMaxSunDist;// m_pFramework->GetCurScene()->GetSSLRMaxSunDist();
			float fInitDecay = m_fSSLRInitDecay;// m_pFramework->GetCurScene()->GetSSLRInitDecay();
			float fDistDecay = m_fSSLRDistDecay;// m_pFramework->GetCurScene()->GetSSLRDistDecay();
			float fMaxDeltaLen = m_fSSLRMaxDeltaLen;//m_pFramework->GetCurScene()->GetSSLRMaxDeltaLen();
	
			m_pSSLR->Excute(pCamera, m_pd3drtvLight, pAmbientOcclution, xmvSunDir, xmf3Color,
				fOffsetSunPos, fMaxSunDist, fInitDecay, fDistDecay, fMaxDeltaLen);
	
			// Restore the states
			GLOBALVALUEMGR->GetDeviceContext()->RSSetViewports(num, &oldvp);
			GLOBALVALUEMGR->GetDeviceContext()->RSSetState(pPrevRSState);
			if (pPrevRSState)pPrevRSState->Release();
		}
	}//sslr ��尡 true�̸� sslr ���� 
	
	//POST PROCESSING
	SetMainRenderTargetView();
	m_vLightLayerResultTexture[0]->SetShaderState();
	PostProcessing(pCamera);
	for (auto texture : m_vLightLayerResultTexture) {
		texture->CleanShaderState();
	}
	
	if (nullptr != m_pUIRederer){
		m_pUIRederer->RenderUI();
	}

	//DEBUG
	if (INPUTMGR->GetDebugMode()) {
		ID3D11Buffer* pGBufferUnpackingBuffer = pCamera->GetGBufferUnpackingBuffer();
		GLOBALVALUEMGR->GetDeviceContext()->PSSetConstantBuffers(PS_UNPACKING_SLOT, 1, &pGBufferUnpackingBuffer);

		DEBUGER->AddDepthTexture(XMFLOAT2(500, 0), XMFLOAT2(750, 150), m_pd3dsrvDepthStencil);
		DEBUGER->AddTexture(XMFLOAT2(100, 0), XMFLOAT2(350, 250), pRFL);
		

		//�̰� �� ���⼭ �������.
		DEBUGER->RenderTexture();
		DEBUGER->RenderText();
	}
	else {
		DEBUGER->ClearDebuger();
	}
	//DEBUGER->ClearDebuger();
	//PRESENT
	HRESULT hr = m_pdxgiSwapChain->Present(0, 0);
	if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
	{
		DEBUGER->DebugMessageBox("explosion", "d");
		return;
	}
}
void CRenderer::Update(float fTimeElapsed) {
	m_pBloomDownScale->SetAdaptation(fTimeElapsed);
}
void CRenderer::PostProcessing(shared_ptr<CCamera> pCamera) {
	float fBloomThreshold = m_fBLOOMThreshold;// m_pFramework->GetCurScene()->GetBLOOMThreshold();
	float fMiddleGrey = m_fBLOOMMiddleGrey;// m_pFramework->GetCurScene()->GetBLOOMMiddleGrey();
	float fWhite = m_fBLOOMWhite;// m_pFramework->GetCurScene()->GetBLOOMWhite();
	float fBloomScale = m_fBLOOMScale;// m_pFramework->GetCurScene()->GetBLOOMScale();

	m_pBloomDownScale->Excute(pCamera, fBloomThreshold);
	ID3D11ShaderResourceView* pBloomImage = m_pBloom->Excute(pCamera);
	pBloomImage = m_p16to1Blur->Excute(pBloomImage);

	GLOBALVALUEMGR->GetDeviceContext()->PSSetShaderResources(2, 1, &pBloomImage);

	//rtv�� Ǯ��ũ�� ��ο� 
	m_pPostProcessingFinalPass->Excute(pCamera, fMiddleGrey, fWhite, fBloomScale);

	//������ �ϱ����� ����avgLum�� ���� ����� avgLum�� ��ȯ
	m_pBloomDownScale->SwapAdaptationBuff();
}
void CRenderer::ClearDepthStencilView(ID3D11DepthStencilView* pDepthStencilView) {
	GLOBALVALUEMGR->GetDeviceContext()->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}
void CRenderer::SetForwardRenderTargets() {
	ID3D11RenderTargetView *pd3dRTVs[RENDER_TARGET_NUMBER] = { m_pd3drtvColorSpecInt, m_pd3drtvNormal, m_pd3drtvSpecPow };
	//float fClearColor[4] = { xmf4Xolor.x, xmf4Xolor.y, xmf4Xolor.z, xmf4Xolor.w };
	float fClearColor[4] = { 0.f, 0.f, 0.f, 0.f };
	if (m_pd3drtvColorSpecInt) GLOBALVALUEMGR->GetDeviceContext()->ClearRenderTargetView(m_pd3drtvColorSpecInt, fClearColor);
	if (m_pd3drtvNormal) GLOBALVALUEMGR->GetDeviceContext()->ClearRenderTargetView(m_pd3drtvNormal, fClearColor);
	if (m_pd3drtvSpecPow) GLOBALVALUEMGR->GetDeviceContext()->ClearRenderTargetView(m_pd3drtvSpecPow, fClearColor);

	SetRenderTargetViews(RENDER_TARGET_NUMBER, pd3dRTVs, m_pd3ddsvDepthStencil);
}
void CRenderer::SetMainRenderTargetView() {
	GLOBALVALUEMGR->GetDeviceContext()->OMSetRenderTargets(1, &m_pd3dRenderTargetView, nullptr);
}
void CRenderer::SetRenderTargetViews(UINT nRenderTarget, ID3D11RenderTargetView** pd3dRTVs, ID3D11DepthStencilView* pd3ddsvDepthStencil) {
	GLOBALVALUEMGR->GetDeviceContext()->OMSetRenderTargets(nRenderTarget, pd3dRTVs, pd3ddsvDepthStencil);
}


bool CRenderer::CreateSwapChain()
{
	// Swap Chain Description ����ü
	DXGI_SWAP_CHAIN_DESC dxgiSwapChainDesc;
#pragma region [DXGI_SWAP_CHAIN_DESC �ʱ�ȭ]
	// ����ü ����
	::ZeroMemory(&dxgiSwapChainDesc, sizeof(dxgiSwapChainDesc));
	// BufferCount : �ĸ� ������ ���� ����
	dxgiSwapChainDesc.BufferCount = 2;

	// BufferDesc : �ĸ� ������ ���÷��� ������ ����
	{
		// Width : ������ ���� ũ�⸦ �ȼ��� ����
		dxgiSwapChainDesc.BufferDesc.Width = GLOBALVALUEMGR->GetrcClient().right;
		// Height : ������ ���� ũ�⸦ �ȼ��� ����
		dxgiSwapChainDesc.BufferDesc.Height = GLOBALVALUEMGR->GetrcClient().bottom;
		// Format : �ĸ� ���� �ȼ� ����
		/// DirectX 11-1(Chap 01)-Device, p.49 ����
		dxgiSwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
		// RefreshRate : ȭ�� ���� ������ Hz ������ ����
		{
			// Denominator : �и�
			dxgiSwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
			// Numerator : ����
			dxgiSwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
		}
		// ScanlineOrdering : scan line �׸��� ��� ����(�⺻ 0)
		//	DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED        (0) : ��ĵ ���� ������ �������� ����	
		//	DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE        (1) : ���α׷��ú� ��ĵ ���� ���� ����
		//	DXGI_MODE_SCANLINE_ORDER_UPPER_FIELD_FIRST  (2) : ���� �ʵ�� �̹��� ����
		//	DXGI_MODE_SCANLINE_ORDER_LOWER_FIELD_FIRST  (3) : ���� �ʵ�� �̹��� ����
		dxgiSwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	}

	// BufferUsage : �ĸ� ���ۿ� ���� ǥ�� ��� ��İ� CPU�� ���� ��� ����
	//	DXGI_USAGE_SHADER_INPUT				: shader �� �Է����� ���
	//	DXGI_USAGE_RENDER_TARGET_OUTPUT		: render target���� ���
	//	DXGI_USAGE_BACK_BUFFER         		: back buffer �� ���
	//	DXGI_USAGE_SHARED              		: ���� ����
	//	DXGI_USAGE_READ_ONLY           		: �б� ����
	//	DXGI_USAGE_DISCARD_ON_PRESENT  		: DXGI ���� ���� ���(����ڰ� ������� ����)
	//	DXGI_USAGE_UNORDERED_ACCESS    		: ������ȭ ����
	dxgiSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	// OutputWindow : ��� �������� �ڵ��� ����(�ݵ�� ��ȿ�ؾ� ��)
	dxgiSwapChainDesc.OutputWindow = GLOBALVALUEMGR->GethWnd();

	// SampleDesc : ���� ���ø��� ǰ���� ����
	// CheckMultisampleQualityLevels �Լ��� ����Ͽ� ���� ���ø� ���� ���θ� Ȯ���� �ڿ� �� ����
	{
		// Count : �ȼ� �� ���� ����
		//	1  : ���� ���ø��� ���� ����
		//	2~ : �ش� ����ŭ�� ���� ���ø�
		dxgiSwapChainDesc.SampleDesc.Count = 1;
		// Quality : ǰ�� ����
		// 0 : ���� ���ø��� ���� ����
		dxgiSwapChainDesc.SampleDesc.Quality = 0;
	}

	// Windowed : ������ ��� �Ǵ� ��ü ȭ�� ��� ���� ~ TRUE  | â ��� 
	//												   ~ FALSE | ��ü ȭ��
	dxgiSwapChainDesc.Windowed = TRUE;

	// Flags : Swap Chain ���ۿ� ���� ���� ������ ����
	//	DXGI_SWAP_CHAIN_FLAG_NONPREROTATED		(1) : ��ü ȭ�� ��忡�� ���� ������ ������ ȭ������ �ű� �� �ڵ� ȸ������ ����
	//	DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH	(2) : ���� ���α׷��� ���÷��� ��带 ������ �� ����
	//	DXGI_SWAP_CHAIN_FLAG_GDI_COMPATIBLE		(4) : ���� ���α׷��� GDI �� ������ �� �� ����. �ĸ� ���ۿ� GetDC() ��� ����
	dxgiSwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH/*2*/;

	// SwapEffect : Swaping�� ó���ϴ� ���û����� ����(�⺻ : 0)
	//	DXGI_SWAP_EFFECT_DISCARD		(0) : ���� ������ ���
	//	DXGI_SWAP_EFFECT_SEQUENTIAL		(1) : ���� ����
	// DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL	(2) : Flip ���� ����
	dxgiSwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// Result Handle �Դϴ�. ��ġ�� ���������� ���������� �˻��մϴ�.
	HRESULT hResult = S_OK;
	IDXGIDevice *pdxgiDevice = NULL;
	if (FAILED(hResult = GLOBALVALUEMGR->GetDevice()->QueryInterface(__uuidof(IDXGIDevice), (LPVOID*)&pdxgiDevice)))
	{
		MessageBox(GLOBALVALUEMGR->GethWnd(), TEXT("DXGI Device ��ü�� ��ȯ���� ���߽��ϴ�. ���α׷��� �����մϴ�."), TEXT("���α׷� ���� ����"), MB_OK);
		return(false);
	}
	// DXGI Factory �ν��Ͻ��� DXGIFactory���� �޽��ϴ�.
	//IDXGIFactory *pdxgiFactory = NULL;
	IDXGIFactory *pdxgiFactory = NULL;

	UINT udxgiFlag = 0;
#ifdef _DEBUG
	udxgiFlag |= DXGI_CREATE_FACTORY_DEBUG;
#endif
	//if (FAILED(hResult = CreateDXGIFactory3(udxgiFlag, __uuidof(IDXGIFactory3), (LPVOID*)&pdxgiFactory)))
	if (FAILED(hResult = CreateDXGIFactory(/*udxgiFlag,*/ __uuidof(IDXGIFactory), (LPVOID*)&pdxgiFactory)))
	{
		MessageBox(GLOBALVALUEMGR->GethWnd(), TEXT("DXGIFactory������ ��ü ������ �����߽��ϴ�. ���α׷��� �����մϴ�."), TEXT("���α׷� ���� ����"), MB_OK);
		return(false);
	}
	//  SwapChain �� �����մϴ�
	if (FAILED(hResult = pdxgiFactory->CreateSwapChain(pdxgiDevice, &dxgiSwapChainDesc, &m_pdxgiSwapChain)))
	{
		MessageBox(GLOBALVALUEMGR->GethWnd(), TEXT("SwapChain �ν��Ͻ� ������ �����߽��ϴ�. ���α׷��� �����մϴ�."), TEXT("���α׷� ���� ����"), MB_OK);
		return(false);
	}

	//	// Direct2D : Direct2D �ν��Ͻ��� �����մϴ�.
	//	if (!CreateD2D1Device(pdxgiDevice))
	//	{
	//		MessageBox(m_hWnd, TEXT("Direct2D �ν��Ͻ� ������ �����߽��ϴ�. ���α׷��� �����մϴ�."), TEXT("���α׷� ���� ����"), MB_OK);
	//		return(false);
	//	}

	// �Ҵ���� COM ��ü�� ��ȯ�մϴ�.
	if (pdxgiDevice) pdxgiDevice->Release();
	if (pdxgiFactory) pdxgiFactory->Release();
	return true;
}

bool CRenderer::CreateRenderTargetView() {

	HRESULT hResult = S_OK;
	ID3D11Texture2D *pd3dBackBuffer{ nullptr };
	if (FAILED(hResult = m_pdxgiSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID *)&pd3dBackBuffer))) return(false);
	if (FAILED(hResult = GLOBALVALUEMGR->GetDevice()->CreateRenderTargetView(pd3dBackBuffer, NULL, &m_pd3dRenderTargetView))) return(false);
	if (pd3dBackBuffer) pd3dBackBuffer->Release();
	{
		//----------------------------------------Resource Desc-----------------------------------------//
		D3D11_SHADER_RESOURCE_VIEW_DESC d3dSRVDesc;
		::ZeroMemory(&d3dSRVDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
		d3dSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		d3dSRVDesc.Texture2D.MipLevels = 1;
		//d3dSRVDesc.Format = DXGI_FORMAT_R32_FLOAT;
		//----------------------------------------Resource Desc-----------------------------------------//
		//----------------------------------------TextUre Desc-----------------------------------------//
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
		d3dTexture2DDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		//d3dTexture2DDesc.Format = DXGI_FORMAT_R32_TYPELESS;
		//----------------------------------------TextUre Desc-----------------------------------------//
		//----------------------------------------Render Desc-----------------------------------------//
		D3D11_RENDER_TARGET_VIEW_DESC d3dRTVDesc;
		::ZeroMemory(&d3dRTVDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
		d3dRTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		d3dRTVDesc.Texture2D.MipSlice = 0;
		//----------------------------------------TextUre Desc-----------------------------------------//

		// Create the depth stencil view 
		D3D11_DEPTH_STENCIL_VIEW_DESC d3dDepthStencilViewDesc;
		ZeroMemory(&d3dDepthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
		d3dDepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		d3dDepthStencilViewDesc.Texture2D.MipSlice = 0;

		ReleaseForwardRenderTargets();

		//--------------------------------------Scene0 DSV Create-----------------------------------------//
		d3dTexture2DDesc.Format = DXGI_FORMAT_R32_TYPELESS;
		d3dDepthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
		d3dSRVDesc.Format = DXGI_FORMAT_R32_FLOAT;
		d3dTexture2DDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;
		GLOBALVALUEMGR->GetDevice()->CreateTexture2D(&d3dTexture2DDesc, nullptr, &m_pd3dtxtDepthStencil);
		GLOBALVALUEMGR->GetDevice()->CreateDepthStencilView(m_pd3dtxtDepthStencil, &d3dDepthStencilViewDesc, &m_pd3ddsvDepthStencil);
		GLOBALVALUEMGR->GetDevice()->CreateShaderResourceView(m_pd3dtxtDepthStencil, &d3dSRVDesc, &m_pd3dsrvDepthStencil);
		//real depth stencil
		d3dDepthStencilViewDesc.Flags = D3D11_DSV_READ_ONLY_DEPTH;
		if (FAILED(hResult = GLOBALVALUEMGR->GetDevice()->CreateDepthStencilView(m_pd3dtxtDepthStencil, &d3dDepthStencilViewDesc, &m_pd3ddsvReadOnlyDepthStencil))) return(false);
		//--------------------------------------Scene0 DSV Create-----------------------------------------//
		
		d3dTexture2DDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		d3dTexture2DDesc.Format = d3dSRVDesc.Format = d3dRTVDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;

		//--------------------------------------Scene1 RTV Create-----------------------------------------//
		GLOBALVALUEMGR->GetDevice()->CreateTexture2D(&d3dTexture2DDesc, nullptr, &m_pd3dtxtColorSpecInt);
		GLOBALVALUEMGR->GetDevice()->CreateShaderResourceView(m_pd3dtxtColorSpecInt, &d3dSRVDesc, &m_pd3dsrvColorSpecInt);
		GLOBALVALUEMGR->GetDevice()->CreateRenderTargetView(m_pd3dtxtColorSpecInt, &d3dRTVDesc, &m_pd3drtvColorSpecInt);
		//--------------------------------------Scene1 RTV Create-----------------------------------------//

		//--------------------------------------Scene2 RTV Create-----------------------------------------//
		GLOBALVALUEMGR->GetDevice()->CreateTexture2D(&d3dTexture2DDesc, nullptr, &m_pd3dtxtNormal);
		GLOBALVALUEMGR->GetDevice()->CreateRenderTargetView(m_pd3dtxtNormal, &d3dRTVDesc, &m_pd3drtvNormal);
		GLOBALVALUEMGR->GetDevice()->CreateShaderResourceView(m_pd3dtxtNormal, &d3dSRVDesc, &m_pd3dsrvNormal);
		//--------------------------------------Scene2 RTV Create-----------------------------------------//

		//--------------------------------------Scene3 RTV Create-----------------------------------------//
		GLOBALVALUEMGR->GetDevice()->CreateTexture2D(&d3dTexture2DDesc, nullptr, &m_pd3dtxtSpecPow);
		GLOBALVALUEMGR->GetDevice()->CreateRenderTargetView(m_pd3dtxtSpecPow, &d3dRTVDesc, &m_pd3drtvSpecPow);
		GLOBALVALUEMGR->GetDevice()->CreateShaderResourceView(m_pd3dtxtSpecPow, &d3dSRVDesc, &m_pd3dsrvSpecPow);
		//--------------------------------------Scene3 RTV Create-----------------------------------------//

		//�ڱ� texture set -> sampler set����
		//---------------------make texture---------------------
		//texture set to light rendercontainer
		
		ID3D11ShaderResourceView *pd3dSRV = { m_pd3dsrvDepthStencil};
		UINT Slot = { 0 };
		UINT BindFlag = { BIND_PS | BIND_CS };
		shared_ptr<CTexture> pTexture = CTexture::CreateTexture(pd3dSRV, Slot, BindFlag);
		m_vObjectLayerResultTexture.push_back(pTexture);

		pd3dSRV = { m_pd3dsrvColorSpecInt };
		Slot = { 1 };
		BindFlag = { BIND_PS | BIND_CS };
		pTexture = CTexture::CreateTexture(pd3dSRV, Slot, BindFlag);
		m_vObjectLayerResultTexture.push_back(pTexture);

		pd3dSRV = { m_pd3dsrvNormal };
		Slot = { 2 };
		BindFlag = { BIND_PS | BIND_CS };
		pTexture = CTexture::CreateTexture(pd3dSRV, Slot, BindFlag);
		m_vObjectLayerResultTexture.push_back(pTexture);
		
		pd3dSRV = { m_pd3dsrvSpecPow };
		Slot = { 3 };
		BindFlag = { BIND_PS | BIND_CS };
		pTexture = CTexture::CreateTexture(pd3dSRV, Slot, BindFlag);
		m_vObjectLayerResultTexture.push_back(pTexture);
		//---------------------make texture---------------------


		//light texture����
		GLOBALVALUEMGR->GetDevice()->CreateTexture2D(&d3dTexture2DDesc, nullptr, &m_pd3dtxtLight);
		GLOBALVALUEMGR->GetDevice()->CreateRenderTargetView(m_pd3dtxtLight, &d3dRTVDesc, &m_pd3drtvLight);
		GLOBALVALUEMGR->GetDevice()->CreateShaderResourceView(m_pd3dtxtLight, &d3dSRVDesc, &m_pd3dsrvLight);

		//make texture
		UINT LightTexSlot = { 0 };
		UINT LightTexBindFlag = { BIND_PS | BIND_CS };
		pTexture = CTexture::CreateTexture(m_pd3dsrvLight, LightTexSlot, LightTexBindFlag);
		m_vLightLayerResultTexture.push_back(pTexture);
		//light texture����
	}
	m_pAORenderer->ResizeBuffer();
	m_pBloomDownScale->ResizeBuffer();
	m_pBloom->ResizeBuffer();
	m_p16to1Blur->ResizeBuffer(4);
	m_p4to1Blur->ResizeBuffer(2);
	m_pSSLR->ResizeBuffer();
	m_pShadow->ResizeBuffer();
	//m_pWaterRenderer->ResizeBuffer();

	return true;
}

void CRenderer::ReleaseForwardRenderTargets() {
	//texture end
	m_vObjectLayerResultTexture.clear();
	m_vLightLayerResultTexture.clear();

	if (m_pd3dtxtColorSpecInt) m_pd3dtxtColorSpecInt->Release();//0
	m_pd3dtxtColorSpecInt = nullptr;

	if (m_pd3dtxtNormal) m_pd3dtxtNormal->Release();//1
	m_pd3dtxtNormal = nullptr;

	if (m_pd3dtxtSpecPow) m_pd3dtxtSpecPow->Release();//2
	m_pd3dtxtSpecPow = nullptr;

	if (m_pd3dtxtDepthStencil) m_pd3dtxtDepthStencil->Release();
	m_pd3dtxtDepthStencil = nullptr;

	if (m_pd3dsrvColorSpecInt) m_pd3dsrvColorSpecInt->Release();//0
	m_pd3dsrvColorSpecInt = nullptr;

	if (m_pd3dsrvNormal) m_pd3dsrvNormal->Release();//1
	m_pd3dsrvNormal = nullptr;

	if (m_pd3dsrvSpecPow) m_pd3dsrvSpecPow->Release();//2
	m_pd3dsrvSpecPow = nullptr;

	if (m_pd3dsrvDepthStencil) m_pd3dsrvDepthStencil->Release();
	m_pd3dsrvDepthStencil = nullptr;

	if (m_pd3drtvColorSpecInt) m_pd3drtvColorSpecInt->Release();//0
	m_pd3drtvColorSpecInt = nullptr;

	if (m_pd3drtvNormal) m_pd3drtvNormal->Release();//1
	m_pd3drtvNormal = nullptr;

	if (m_pd3drtvSpecPow) m_pd3drtvSpecPow->Release();//2
	m_pd3drtvSpecPow = nullptr;

	if (m_pd3ddsvDepthStencil) m_pd3ddsvDepthStencil->Release();
	m_pd3ddsvDepthStencil = nullptr;

	if (m_pd3drtvLight) m_pd3drtvLight->Release();
	m_pd3drtvLight = nullptr;


}


bool CRenderer::ResizeBuffer() {
	if (m_pd3dRenderTargetView) m_pd3dRenderTargetView->Release();
	if (m_pd3ddsvReadOnlyDepthStencil) m_pd3ddsvReadOnlyDepthStencil->Release();

	if (FAILED(m_pdxgiSwapChain->ResizeBuffers(1, 0, 0, DXGI_FORMAT_R16G16B16A16_FLOAT, 0)))
		MessageBox(nullptr, TEXT(""), TEXT(""), MB_OK);

	//resize rtv size, deferred texture size
	CreateRenderTargetView();

	return true;
}

void CRenderer::LoadEffectInfo(wstring wsOutputPath, wstring wsSceneName){
	//ssao
	float fSSAORadius = IMPORTER->ReadFloat();
	SetSSAORadius(fSSAORadius);
	float fSSAOOffsetRadius = IMPORTER->ReadFloat();
	SetSSAOOffsetRadius(fSSAOOffsetRadius);

	//bloom
	float fBLOOMThreshold = IMPORTER->ReadFloat();
	SetBLOOMThreshold(fBLOOMThreshold);
	float fBLOOMMiddleGrey = IMPORTER->ReadFloat();
	SetBLOOMMiddleGrey(fBLOOMMiddleGrey);
	float fBLOOMWhite = IMPORTER->ReadFloat();
	SetBLOOMWhite(fBLOOMWhite);
	float fBLOOMScale = IMPORTER->ReadFloat();
	SetBLOOMScale(fBLOOMScale);
	//sslr
	bool bSSLROnOff = IMPORTER->ReadBool();
	SetSSLROnOff(bSSLROnOff);
	float fSSLRMaxSunDist = IMPORTER->ReadFloat();
	SetSSLRMaxSunDist(fSSLRMaxSunDist);
	float fSSLRInitDecay = IMPORTER->ReadFloat();
	SetSSLRInitDecay(fSSLRInitDecay);
	float fSSLRDistDecay = IMPORTER->ReadFloat();
	SetSSLRDistDecay(fSSLRDistDecay);
	float fSSLRMaxDeltaLen = IMPORTER->ReadFloat();
	SetSSLRMaxDeltaLen(fSSLRMaxDeltaLen);

	m_pShadow->LoadShadow(wsOutputPath, wsSceneName);
}

CRenderer::CRenderer() :CSingleTonBase<CRenderer>("rendereringleton") {

}

CRenderer::~CRenderer() {

}