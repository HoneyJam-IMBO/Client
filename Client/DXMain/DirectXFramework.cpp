#include "stdafx.h"
#include "DirectXFramework.h"

void CDirectXFramework::Begin(HINSTANCE hInstance, HWND hWnd){

	//singleton Init 
	GLOBALVALUEMGR->Begin(hInstance, hWnd);
	RESOURCEMGR->Begin();
	RCSELLER->Begin();
	DEBUGER->Begin();    // 27일 여까지봄 
	INPUTMGR->Begin();
	RENDERER->Begin();
	UPDATER->Begin();

	_tcscpy_s(m_pszBuffer, _T("Impossible Boss ("));

	//device 생성 이후 해야함

	//camera
	m_pCamera = make_shared<CFreeCamera>();
	//------------------------------------------카메라 제작--------------------------------------
	m_pCamera->Begin();
	m_pCamera->GenerateProjectionMatrix(
		// FOV Y 값 : 클 수록 멀리까지 볼 수 있다.
		60.0f * XM_PI / 180.0f
		// 종횡비
		, float(GLOBALVALUEMGR->GetrcClient().right) / float(GLOBALVALUEMGR->GetrcClient().bottom)
		// 최소 거리
		, 0.01f
		// 최대 거리
		, 10000.0f);

	//위치 조정

	//viewprojection행렬 제작
	XMVECTOR eye = { 0.0f, 58.f, 00.0f, 0.0f };
	XMVECTOR at = { 500.0f, 0.0f, 500.0f, 0.0f };
	XMVECTOR up = { 0.0f, 1.0f, 0.0f, 0.0f };

	m_pCamera->SetLookAt(eye, at, up);
	// RS에 뷰포트 연결
	m_pCamera->SetViewport(0, 0, GLOBALVALUEMGR->GetrcClient().right, GLOBALVALUEMGR->GetrcClient().bottom, 0.0f, 1.0f);
	//------------------------------------------카메라 제작--------------------------------------

}

void CDirectXFramework::End() {
	//camera end
	if (m_pCamera) {
		m_pCamera->End();
	}
	m_pCamera = nullptr;

	//scene end
	for (int i = 0; i < m_nScene; ++i) {
		m_stackScene.top()->End();
		delete m_stackScene.top();
		m_stackScene.pop();
	}
	//singleton End
	RESOURCEMGR->End();
	RCSELLER->End();
	DEBUGER->End();
	INPUTMGR->End();
	UPDATER->End();
	GLOBALVALUEMGR->End();
	//singleton End

}

void CDirectXFramework::FrameAdvance()
{
	TIMEMGR->Tick();

	Update(TIMEMGR->GetTimeElapsed());
	//Update2D();

	Render();
	//Render2D();

	TIMEMGR->GetFrameRate(m_pszBuffer + 17, 35);
	::SetWindowText(GLOBALVALUEMGR->GethWnd(), m_pszBuffer);
}

		
void CDirectXFramework::Render() {
	//scene
	//set
	//-----------------------------카메라 버퍼 set------------------------------------
	//m_pCamera->SetShaderState();
	//-----------------------------카메라 버퍼 set------------------------------------

	RENDERER->Render(m_pCamera);

}

void CDirectXFramework::Update(float fTimeElapsed) {
	RENDERER->Update(fTimeElapsed);

	//postprocessinglayer 적응값 set
	//m_pPostProcessingLayer->SetAdaptation(fTimeElapsed);
	ProcessInput(fTimeElapsed);

	UPDATER->PreUpdate(fTimeElapsed);
	//-----------------------------현재 씬 실행--------------------------------------
	m_stackScene.top()->Animate(fTimeElapsed);
	//-----------------------------현재 씬 실행--------------------------------------
	UPDATER->Update(fTimeElapsed);
	UPDATER->PhisicsUpdate(fTimeElapsed);
}

void CDirectXFramework::ProcessInput(float fTimeElapsed) {
	INPUTMGR->Update(fTimeElapsed);

	//-----------------------------현재 씬 실행--------------------------------------
	m_stackScene.top()->ProcessInput(fTimeElapsed);
	//-----------------------------현재 씬 실행--------------------------------------
}


LRESULT CALLBACK CDirectXFramework::OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	INPUTMGR->OnProcessingWindowMessage(hWnd, nMessageID, wParam, lParam);
	return(0);
}

//--------------------------------------scene-----------------------------------------
void CDirectXFramework::ChangeScene(CScene* pScene) {
	//씬을 아예 바꿈 이전의 씬을 pop하고 
	if (m_nScene != 0) {
		//기존의 Scene이 존재하면

		PopScene();
		//기존의 scene을 빼내고

		PushScene(pScene);
		//새로운 scene을 집어 넣음
	}
	else {
		//아니면 처음 넣는 Scene이면 그냥
		PushScene(pScene);
		//새로운 scene을 집어 넣음
	}
}
void CDirectXFramework::PopScene() {
	m_stackScene.top()->End();
	delete m_stackScene.top();

	m_stackScene.pop();
	//scene을 빼내고

	if (m_stackScene.top())	m_stackScene.top()->Begin();
	//top이 있으면 begin
}
void CDirectXFramework::PushScene(CScene* pScene) {
	pScene->Begin();
	m_stackScene.push(pScene);
	//새로운 scene을 집어 넣음
	++m_nScene;
}
//--------------------------------------scene-----------------------------------------

//--------------------------------------deferred rendering func-----------------------------

CDirectXFramework::~CDirectXFramework() {

}
