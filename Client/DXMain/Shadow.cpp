#include "stdafx.h"
#include "Shadow.h"


bool CShadow::Begin(){
	ReleaseBuffer();
	//m_pShadowVPBuffer = CBuffer::CreateConstantBuffer(1, sizeof(XMFLOAT4X4), 0, BIND_VS | BIND_DS , NULL);
	m_pCamera = make_shared<CFreeCamera>();
	//------------------------------------------카메라 제작--------------------------------------
	m_pCamera->Begin();
	m_pCamera->GenerateProjectionMatrix(60.0f * XM_PI / 180.0f
		, float(GLOBALVALUEMGR->GetrcClient().right) / float(GLOBALVALUEMGR->GetrcClient().bottom)
		, 5.f
		,3000.0f);

	//viewprojection행렬 제작 / 광원
	XMVECTOR eye = { 500.0f, 500.0f, -500.0f, 0.0f };
	XMVECTOR at = { 500, 0.0f, 0, 0.0f };
	XMVECTOR up = { 0.0f, 1.0f, 0.0f, 0.0f };
	m_pCamera->SetLookAt(eye, at, up);

	D3D11_RASTERIZER_DESC descRasterizer;
	ZeroMemory(&descRasterizer, sizeof(D3D11_RASTERIZER_DESC));
	descRasterizer.FillMode = D3D11_FILL_SOLID;
	descRasterizer.CullMode = D3D11_CULL_NONE;
	GLOBALVALUEMGR->GetDevice()->CreateRasterizerState(&descRasterizer, &m_pd3dRSShader);

	m_pShadowBuf = RESOURCEMGR->CreateConstantBuffer("ShadowInfoBuffer", 1, sizeof(stShadowInfo), PS_CB_SHADOW, BIND_PS);

	m_pShadowInfo = new stShadowInfo;

	_TCHAR ppstrShadowMapTextureNames[MAX_SPACE_NUM][128];
	UINT nSpace = MAX_SPACE_NUM;
	for (UINT i = 0; i<nSpace; ++i) {
		WCHAR wcShadowMapPath[256];
		wsprintf(ppstrShadowMapTextureNames[i], L"../../Assets/default.jpg");
	}

	m_pShadowTexture = CTexture::CreateTexture(nSpace, ppstrShadowMapTextureNames, 9, BIND_PS);
	//maptool
	//maptool


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
	d3dTexture2DDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	d3dTexture2DDesc.Format = d3dSRVDesc.Format = d3dRTVDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	GLOBALVALUEMGR->GetDevice()->CreateTexture2D(&d3dTexture2DDesc, nullptr, &m_pd3dtxtRenderShadow);
	GLOBALVALUEMGR->GetDevice()->CreateShaderResourceView(m_pd3dtxtRenderShadow, &d3dSRVDesc, &m_pd3dsrvRenderShadow);
	GLOBALVALUEMGR->GetDevice()->CreateRenderTargetView(m_pd3dtxtRenderShadow, &d3dRTVDesc, &m_pd3drtvRenderShadow);

	m_pDebugTextureObj = new CDebugTexture;
	m_pDebugTextureObj->Begin();
	//maptool
	//maptool
	return true;
}

bool CShadow::End()
{
	if (m_pCamera) {
		m_pCamera->End();
	}
	delete m_pShadowInfo;

	if(m_pDebugTextureObj)
		delete m_pDebugTextureObj;

	return true;
}
ID3D11ShaderResourceView* txtTest = nullptr;
void CShadow::SetShaderState(){
	ID3D11ShaderResourceView* pSRV[] = { m_pd3dsrvShadow };
	if (txtTest) {
		pSRV[0] = { txtTest };
	}
	GLOBALVALUEMGR->GetDeviceContext()->PSSetShaderResources(5, 1, pSRV);

	float offset = UPDATER->GetDirectionalLight()->GetOffsetLength();
	XMVECTOR xmvDirectionalLightDir = UPDATER->GetDirectionalLight()->GetLook();
	UINT SelectSpace = m_SelectSpace;
	UINT nSpace = UPDATER->GetSpaceContainer()->GetSpaceNum();
	XMVECTOR at;
	XMFLOAT3 xmf3Pos;
	stShadowInfo* pData = (stShadowInfo*)m_pShadowBuf->Map();
	//이건 동적 그림자 카메라
	//pData->xmmtxViewProj[0] = XMMatrixTranspose(m_pCamera->GetViewMtx()*m_pCamera->GetProjectionMtx());
	for (int i = 0; i < UPDATER->GetSpaceContainer()->GetSpaceNum(); ++i) {
		XMVECTOR xmPos = UPDATER->GetSpaceContainer()->GetAllSpace()[i]->GetPosition();
		float pos_offset = (UPDATER->GetSpaceContainer()->GetOneSpaceSize() / 2);;
		xmPos += XMVectorSet(pos_offset, pos_offset, pos_offset, 0);
		XMStoreFloat3(&xmf3Pos, xmPos);
		at = XMVectorSet(xmf3Pos.x, 0, xmf3Pos.z, 0);
		XMVECTOR eye = at + xmvDirectionalLightDir*offset;
		XMVECTOR up = { 0.0f, 1.0f, 0.0f, 0.0f };
		m_pCamera->SetLookAt(eye, at, up);

		pData->xmmtxViewProj[i] = XMMatrixTranspose(m_pCamera->GetViewMtx()*m_pCamera->GetProjectionMtx());
	}
	pData->bias = m_pShadowInfo->bias * 0.00001;
	pData->bias_offset = m_pShadowInfo->bias_offset * 0.00001;
	pData->kernelhalf = m_pShadowInfo->kernelhalf;
	pData->dist_sum = m_pShadowInfo->dist_sum;

	m_pShadowBuf->Unmap();
	m_pShadowBuf->SetShaderState();
}

void CShadow::CleanShaderState(){
	ID3D11ShaderResourceView* pSRV[] = { nullptr };
	GLOBALVALUEMGR->GetDeviceContext()->PSSetShaderResources(5, 1, pSRV);
	m_pShadowBuf->CleanShaderState();

	//static shadow map clean
	if (m_pShadowTexture) m_pShadowTexture->CleanShaderState();
}

void CShadow::UpdateShaderState(){

}

ID3D11ShaderResourceView * CShadow::RenderShadowMap(shared_ptr<CCamera> pCamera){
	//static shadow map set
	if(m_pShadowTexture) m_pShadowTexture->SetShaderState();

	//set global value
	if (INPUTMGR->KeyDown(VK_1)) {
		//SaveShadow(L"../outputdata/shadowmap/", L"test_scene");
		//EXPORTER->MakeSRVTexture(m_pd3dsrvShadow, L"../01.dds");
	}
	if (INPUTMGR->KeyDown(VK_2)) {
		TexMetadata info;
		ScratchImage image;
		HRESULT hr = LoadFromDDSFile(L"../outputdata/shadowmap/test_sceneShadowMap0.dds", DDS_FLAGS_NONE, &info, image);
		if (FAILED(hr))DEBUGER->DebugGMessageBox(L"fail", L"dds load key2");
		CreateShaderResourceView(GLOBALVALUEMGR->GetDevice(), image.GetImages(), image.GetImageCount(), info, &txtTest);

		LoadShadow(L"../outputdata/shadowmap/", L"test_scene");
	}
	if (INPUTMGR->KeyDown(VK_3)) {
		if (txtTest)txtTest->Release();
		//delete txtTest;
		txtTest = nullptr;
	}
	if (txtTest) {
		DEBUGER->AddDepthTexture(XMFLOAT2(300, 0), XMFLOAT2(500, 200), txtTest);
		DEBUGER->AddDepthTexture(XMFLOAT2(300, 200), XMFLOAT2(500, 400), m_pd3dsrvRenderShadow);
	}

	GLOBALVALUEMGR->GetDeviceContext()->ClearDepthStencilView(m_pd3ddsvShadow, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	ID3D11RasterizerState* pPrevRSState;
	GLOBALVALUEMGR->GetDeviceContext()->RSGetState(&pPrevRSState);
	GLOBALVALUEMGR->GetDeviceContext()->RSSetState(m_pd3dRSShader);


	float offset = UPDATER->GetDirectionalLight()->GetOffsetLength();
	XMVECTOR xmvDirectionalLightDir = UPDATER->GetDirectionalLight()->GetLook();
	//XMVECTOR xmvCameraPos = pCamera->GetPosition();
	//XMVECTOR eye = xmvCameraPos + xmvDirectionalLightDir*offset;
	//XMVECTOR at = xmvCameraPos;
	UINT SelectSpace = m_SelectSpace;
	UINT nSpace = UPDATER->GetSpaceContainer()->GetSpaceNum();
	XMVECTOR at;
	XMFLOAT3 xmf3Pos;
	if (SelectSpace == 0) {
		//select space가 0이면 camera를 쫓아 다닌다.
		if(pCamera) XMStoreFloat3(&xmf3Pos, pCamera->GetPosition());
		else xmf3Pos = XMFLOAT3{ 0.f, 0.f, 0.f };

		at = XMVectorSet(xmf3Pos.x, 0, xmf3Pos.z, 0);
	}
	else if (SelectSpace <= nSpace) {
		//space num보다 작다면
		//select space index -1 이 space의 index이다 
		XMVECTOR xmPos = UPDATER->GetSpaceContainer()->GetAllSpace()[SelectSpace - 1]->GetPosition();
		float pos_offset = (UPDATER->GetSpaceContainer()->GetOneSpaceSize() / 2);;
		xmPos += XMVectorSet(pos_offset, pos_offset, pos_offset, 0);
		XMStoreFloat3(&xmf3Pos, xmPos);
		at = XMVectorSet(xmf3Pos.x, 0, xmf3Pos.z, 0);
	}
	XMVECTOR eye = at + xmvDirectionalLightDir*offset;
	XMVECTOR up = { 0.0f, 1.0f, 0.0f, 0.0f };
	m_pCamera->SetLookAt(eye, at, up);
	m_pCamera->SetViewport(0, 0, 4096, 4096, 0.0f, 1.0f);

	m_pCamera->UpdateShaderState();
	m_pCamera->SetShaderState();

	UPDATER->GetSpaceContainer()->PrepareRenderOneSpace(m_pCamera, RTAG_TERRAIN | RTAG_DYNAMIC_OBJECT | RTAG_STATIC_OBJECT, SelectSpace - 1);

	ID3D11RenderTargetView*	rtNULL = nullptr;
	GLOBALVALUEMGR->GetDeviceContext()->OMSetRenderTargets(1, &rtNULL, m_pd3ddsvShadow);
	//GLOBALVALUEMGR->GetDeviceContext()->PSSetShaderResources(m_TextureStartSlot, 1, &m_pd3dsrvShadow);
	RENDERER->GetObjectRenderer()->ExcuteShadowRender(m_pCamera);

	DEBUGER->AddDepthTexture(XMFLOAT2(500, 150), XMFLOAT2(750, 300), m_pd3dsrvShadow);
	m_pCamera->SetViewport(0, 0, GLOBALVALUEMGR->GetrcClient().right, GLOBALVALUEMGR->GetrcClient().bottom, 0.0f, 1.0f);
	GLOBALVALUEMGR->GetDeviceContext()->RSSetState(pPrevRSState);
	return m_pd3dsrvShadow;
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
}

void CShadow::ReleaseBuffer()
{
	if (m_pd3dtxtShadow) m_pd3dtxtShadow->Release();
	if (m_pd3dsrvShadow) m_pd3dsrvShadow->Release();
	if (m_pd3ddsvShadow) m_pd3ddsvShadow->Release();
}

void CShadow::LoadShadow(wstring wsOutputPath, wstring wsSceneName){
	m_pShadowInfo->bias = IMPORTER->ReadFloat();
	m_pShadowInfo->bias_offset = IMPORTER->ReadFloat();
	m_pShadowInfo->dist_sum = IMPORTER->ReadFloat();
	m_pShadowInfo->kernelhalf = IMPORTER->ReadFloat();


	_TCHAR ppstrShadowMapTextureNames[MAX_SPACE_NUM][128];
	UINT nSpace = UPDATER->GetSpaceContainer()->GetSpaceNum();
	for (UINT i = 0; i<nSpace; ++i) {
		WCHAR wcShadowMapPath[256];
		wsprintf(ppstrShadowMapTextureNames[i], L"%s%sShadowMap%d.dds", wsOutputPath.c_str(), wsSceneName.c_str(), i);
	}
	m_pShadowTexture->End();
	m_pShadowTexture = nullptr;

	m_pShadowTexture = CTexture::CreateTexture(nSpace, ppstrShadowMapTextureNames, 9, BIND_PS);
}

CShadow::CShadow()
	: DXObject("shadow"){

}

CShadow::~CShadow(){

}
