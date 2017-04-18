#include "stdafx.h"
#include "Loading.h"
#include "SceneMgr.h"
#include "SceneMain.h"

#include "LoadingBack.h"

CLoading::CLoading(SCENE_ID eID, CDirectXFramework* pFrameWork)
	:CScene(eID)
{
	m_pFrameWork = pFrameWork;
}
CLoading::~CLoading()
{
	End();
}

bool CLoading::Begin()
{
	UPDATER->GetSkyBoxContainer()->SetActive(false);
	UPDATER->GetTerrainContainer()->SetActive(false);

	RESOURCEMGR->ReleaseAllResource();
	RCSELLER->ClearStempRenderContainer();

	string strName = "Logo";
	//RESOURCEMGR->CreateTexture("DEFAULT", _T("../../Assets/default.jpg"), PS_TEXTURE, BIND_PS);
	RESOURCEMGR->CreateTexture("Logo", _T("../../Assets/Loading/Logo.png"), PS_TEXTURE, BIND_PS);
	CUIObject* pUI = CLoadingBack::Create(XMLoadFloat2(&XMFLOAT2(WINSIZEX * 0.5f, WINSIZEX * 0.5f)), XMLoadFloat2(&XMFLOAT2(50.f, 50.f)), StringToTCHAR(strName));
	m_vecUI.push_back(pUI);

	m_hLoadThread = CreateThread(NULL, 0, FuncLoadResourceThread, this, 0, NULL);
	m_hRenderThread = CreateThread(NULL, 0, FuncRenderThread, this, 0, NULL);

	m_hLoadEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	

	//GetExitCodeThread()
	return true;
}
bool CLoading::End()
{
	RENDERER->GetUIRenderer()->ClearData();
	size_t iVecSize = m_vecUI.size();
	for (size_t i = 0; i < iVecSize; ++i)
	{
		delete m_vecUI[i];
		m_vecUI[i] = nullptr;
	}
	m_vecUI.clear();

	return true;
}

void CLoading::Animate(float fTimeElapsed)
{
	WaitForSingleObject(m_hLoadEvent, INFINITE);

	size_t iVecSize = m_vecUI.size();
	for (size_t i = 0; i < iVecSize; ++i)
	{
		m_vecUI[i]->Update(fTimeElapsed);
	}
	//if (true == m_bComplete)
	{


		CScene*	pScene = nullptr;
		switch (m_eSceneID) {
		case SC_HEROSEL:		pScene = new CSceneMain(m_eSceneID, m_pFrameWork);			break;
		case SC_ORITOWN:		pScene = new CSceneMain(m_eSceneID, m_pFrameWork);			break;
		case SC_BURNNINGTOWN:	pScene = new CSceneMain(m_eSceneID, m_pFrameWork);			break;
		case SC_REPAIRTOWN:		pScene = new CSceneMain(m_eSceneID, m_pFrameWork);			break;
		case SC_ALDENAD:		pScene = new CSceneMain(m_eSceneID, m_pFrameWork);			break;
		case SC_BOSS:			pScene = new CSceneMain(m_eSceneID, m_pFrameWork);			break;
		}
		CSceneMgr::GetInstance()->SetPresentScene(m_eSceneID, pScene);

		UPDATER->GetSkyBoxContainer()->SetActive(true);
		UPDATER->GetTerrainContainer()->SetActive(true);

		End();
		delete this;
	}
}

void CLoading::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
}

void CLoading::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
}

void CLoading::ProcessInput(float fTimeElapsed)
{

}

DWORD WINAPI CLoading::FuncLoadResourceThread(LPVOID arg)
{
	// 계속 추가하는애들 함수로 ㄱㄱ
	RESOURCEMGR->CreateTexture("DEFAULT", _T("../../Assets/default.jpg"), PS_TEXTURE, BIND_PS);

	CLoading*	pMyThread = (CLoading*)arg;
	switch (pMyThread->GetSceneID())
	{
	case SC_HEROSEL:
		pMyThread->LoadScene_HEROSEL();
		break;
	case SC_ORITOWN:
		pMyThread->LoadScene_ORITOWN();
		break;
	case SC_BURNNINGTOWN:
		pMyThread->LoadScene_BURNNINGTOWN();
		break;
	case SC_REPAIRTOWN:
		pMyThread->LoadScene_REPAIRTOWN();
		break;
	case SC_ALDENAD:
		pMyThread->LoadScene_ALDENAD();
		break;
	case SC_BOSS:
		pMyThread->LoadScene_BOSS();
		break;
	}

	RCSELLER->CreateStempRenderContainer();
	pMyThread->SetLoadComplete(true);

	SetEvent(pMyThread->m_hLoadEvent);
	return 0;
}
DWORD CLoading::FuncRenderThread(LPVOID arg)
{
	CLoading*	pMyThread = (CLoading*)arg;
	pMyThread->MakeRenderPipe();
	while (true)
	{
		//GLOBALVALUEMGR->GetDeviceContext()->ClearDepthStencilView(pMyThread->m_pd3ddsvDepthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		//ID3D11RenderTargetView *pd3dRTVs[1] = { pMyThread->m_pd3drtvColorSpecInt };
		////float fClearColor[4] = { xmf4Xolor.x, xmf4Xolor.y, xmf4Xolor.z, xmf4Xolor.w };
		//float fClearColor[4] = { 0.f, 0.f, 0.7f, 1.f };
		//if (pMyThread->m_pd3drtvColorSpecInt) GLOBALVALUEMGR->GetDeviceContext()->ClearRenderTargetView(pMyThread->m_pd3drtvColorSpecInt, fClearColor);

		//GLOBALVALUEMGR->GetDeviceContext()->OMSetRenderTargets(1, pd3dRTVs, pMyThread->m_pd3ddsvDepthStencil);



		//pMyThread->m_pdxgiSwapChain->Present(0, 0);

		DWORD dwExitCode = 0;
		GetExitCodeThread(pMyThread->m_hLoadThread, &dwExitCode);
		if (dwExitCode == STILL_ACTIVE)
			continue;

		break;
	}
	return 0;
}
void CLoading::LoadScene_HEROSEL()
{
	vector<wstring> vFile;
	DIRECTORYFINDER->GetFiles(vFile, L"../../Assets/mesh", true, true, L".gjm");
	DIRECTORYFINDER->GetFiles(vFile, L"../../Assets/mesh", true, true, L".GJM");
	DIRECTORYFINDER->GetFiles(vFile, L"../outputdata", true, true, L".gjm");

	string name;
	for (auto fileName : vFile) {
		string sPath{ "" };
		sPath.assign(fileName.begin(), fileName.end());
		name = GetFileName(sPath);
		RESOURCEMGR->CreateMultiMesh(sPath, name);
	}
	//Sleep(1000);
}

void CLoading::LoadScene_ORITOWN()
{
}

void CLoading::LoadScene_BURNNINGTOWN()
{
}

void CLoading::LoadScene_REPAIRTOWN()
{
}

void CLoading::LoadScene_ALDENAD()
{
}

void CLoading::LoadScene_BOSS()
{
}

void CLoading::MakeRenderPipe()
{
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
	d3dTexture2DDesc.BindFlags = D3D11_BIND_RENDER_TARGET ;
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

	d3dTexture2DDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	d3dDepthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	d3dSRVDesc.Format = DXGI_FORMAT_R32_FLOAT;
	d3dTexture2DDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	GLOBALVALUEMGR->GetDevice()->CreateTexture2D(&d3dTexture2DDesc, nullptr, &m_pd3dtxtDepthStencil);
	GLOBALVALUEMGR->GetDevice()->CreateDepthStencilView(m_pd3dtxtDepthStencil, &d3dDepthStencilViewDesc, &m_pd3ddsvDepthStencil);


	d3dTexture2DDesc.BindFlags = D3D11_BIND_RENDER_TARGET ;
	d3dTexture2DDesc.Format = d3dSRVDesc.Format = d3dRTVDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;

	//--------------------------------------Scene1 RTV Create-----------------------------------------//
	GLOBALVALUEMGR->GetDevice()->CreateTexture2D(&d3dTexture2DDesc, nullptr, &m_pd3dtxtColorSpecInt);
	GLOBALVALUEMGR->GetDevice()->CreateRenderTargetView(m_pd3dtxtColorSpecInt, &d3dRTVDesc, &m_pd3drtvColorSpecInt);



	// Swap Chain Description 구조체
	DXGI_SWAP_CHAIN_DESC dxgiSwapChainDesc;
	::ZeroMemory(&dxgiSwapChainDesc, sizeof(dxgiSwapChainDesc));
	dxgiSwapChainDesc.BufferCount = 2;

	{
		dxgiSwapChainDesc.BufferDesc.Width = GLOBALVALUEMGR->GetrcClient().right;
		dxgiSwapChainDesc.BufferDesc.Height = GLOBALVALUEMGR->GetrcClient().bottom;
		dxgiSwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
		{
			dxgiSwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
			dxgiSwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
		}
		dxgiSwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	}
	dxgiSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	dxgiSwapChainDesc.OutputWindow = GLOBALVALUEMGR->GethWnd();

	{
		dxgiSwapChainDesc.SampleDesc.Count = 1;
		dxgiSwapChainDesc.SampleDesc.Quality = 0;
	}

	dxgiSwapChainDesc.Windowed = TRUE;
	dxgiSwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH/*2*/;
	dxgiSwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	HRESULT hResult = S_OK;
	IDXGIDevice *pdxgiDevice = NULL;
	if (FAILED(hResult = GLOBALVALUEMGR->GetDevice()->QueryInterface(__uuidof(IDXGIDevice), (LPVOID*)&pdxgiDevice)))
	{
		MessageBox(GLOBALVALUEMGR->GethWnd(), TEXT("DXGI Device 객체를 반환받지 못했습니다. 프로그램을 종료합니다."), TEXT("프로그램 구동 실패"), MB_OK);
		return;
	}
	// DXGI Factory 인스턴스를 DXGIFactory에서 받습니다.
	//IDXGIFactory *pdxgiFactory = NULL;
	IDXGIFactory *pdxgiFactory = NULL;

	UINT udxgiFlag = 0;
#ifdef _DEBUG
	udxgiFlag |= DXGI_CREATE_FACTORY_DEBUG;
#endif
	//if (FAILED(hResult = CreateDXGIFactory3(udxgiFlag, __uuidof(IDXGIFactory3), (LPVOID*)&pdxgiFactory)))
	if (FAILED(hResult = CreateDXGIFactory(/*udxgiFlag,*/ __uuidof(IDXGIFactory), (LPVOID*)&pdxgiFactory)))
	{
		MessageBox(GLOBALVALUEMGR->GethWnd(), TEXT("DXGIFactory에서의 객체 생성이 실패했습니다. 프로그램을 종료합니다."), TEXT("프로그램 구동 실패"), MB_OK);
		return ;
	}
	//  SwapChain 을 생성합니다
	if (FAILED(hResult = pdxgiFactory->CreateSwapChain(pdxgiDevice, &dxgiSwapChainDesc, &m_pdxgiSwapChain)))
	{
		MessageBox(GLOBALVALUEMGR->GethWnd(), TEXT("SwapChain 인스턴스 생성이 실패했습니다. 프로그램을 종료합니다."), TEXT("프로그램 구동 실패"), MB_OK);
		return;
	}
	// 할당받은 COM 객체를 반환합니다.
	if (pdxgiDevice) pdxgiDevice->Release();
	if (pdxgiFactory) pdxgiFactory->Release();
	return;
}
