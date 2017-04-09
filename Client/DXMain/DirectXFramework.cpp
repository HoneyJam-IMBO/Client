#include "stdafx.h"
#include "DirectXFramework.h"

void CDirectXFramework::Begin(HINSTANCE hInstance, HWND hWnd){

	//singleton Init 
	GLOBALVALUEMGR->Begin(hInstance, hWnd);
	RESOURCEMGR->Begin();
	RCSELLER->Begin();
	DEBUGER->Begin();    // 27�� �������� 
	INPUTMGR->Begin();
	RENDERER->Begin();
	UPDATER->Begin();

	_tcscpy_s(m_pszBuffer, _T("Impossible Boss ("));

	//device ���� ���� �ؾ���

	//camera
	m_pCamera = make_shared<CFreeCamera>();
	//------------------------------------------ī�޶� ����--------------------------------------
	m_pCamera->Begin();
	m_pCamera->GenerateProjectionMatrix(
		// FOV Y �� : Ŭ ���� �ָ����� �� �� �ִ�.
		60.0f * XM_PI / 180.0f
		// ��Ⱦ��
		, float(GLOBALVALUEMGR->GetrcClient().right) / float(GLOBALVALUEMGR->GetrcClient().bottom)
		// �ּ� �Ÿ�
		, 0.01f
		// �ִ� �Ÿ�
		, 10000.0f);

	//��ġ ����

	//viewprojection��� ����
	XMVECTOR eye = { 0.0f, 58.f, 00.0f, 0.0f };
	XMVECTOR at = { 500.0f, 0.0f, 500.0f, 0.0f };
	XMVECTOR up = { 0.0f, 1.0f, 0.0f, 0.0f };

	m_pCamera->SetLookAt(eye, at, up);
	// RS�� ����Ʈ ����
	m_pCamera->SetViewport(0, 0, GLOBALVALUEMGR->GetrcClient().right, GLOBALVALUEMGR->GetrcClient().bottom, 0.0f, 1.0f);
	//------------------------------------------ī�޶� ����--------------------------------------

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
	//-----------------------------ī�޶� ���� set------------------------------------
	//m_pCamera->SetShaderState();
	//-----------------------------ī�޶� ���� set------------------------------------

	RENDERER->Render(m_pCamera);

}

void CDirectXFramework::Update(float fTimeElapsed) {
	RENDERER->Update(fTimeElapsed);

	//postprocessinglayer ������ set
	//m_pPostProcessingLayer->SetAdaptation(fTimeElapsed);
	ProcessInput(fTimeElapsed);

	UPDATER->PreUpdate(fTimeElapsed);
	//-----------------------------���� �� ����--------------------------------------
	m_stackScene.top()->Animate(fTimeElapsed);
	//-----------------------------���� �� ����--------------------------------------
	UPDATER->Update(fTimeElapsed);
	UPDATER->PhisicsUpdate(fTimeElapsed);
}

void CDirectXFramework::ProcessInput(float fTimeElapsed) {
	INPUTMGR->Update(fTimeElapsed);

	//-----------------------------���� �� ����--------------------------------------
	m_stackScene.top()->ProcessInput(fTimeElapsed);
	//-----------------------------���� �� ����--------------------------------------
}


LRESULT CALLBACK CDirectXFramework::OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	INPUTMGR->OnProcessingWindowMessage(hWnd, nMessageID, wParam, lParam);
	return(0);
}

//--------------------------------------scene-----------------------------------------
void CDirectXFramework::ChangeScene(CScene* pScene) {
	//���� �ƿ� �ٲ� ������ ���� pop�ϰ� 
	if (m_nScene != 0) {
		//������ Scene�� �����ϸ�

		PopScene();
		//������ scene�� ������

		PushScene(pScene);
		//���ο� scene�� ���� ����
	}
	else {
		//�ƴϸ� ó�� �ִ� Scene�̸� �׳�
		PushScene(pScene);
		//���ο� scene�� ���� ����
	}
}
void CDirectXFramework::PopScene() {
	m_stackScene.top()->End();
	delete m_stackScene.top();

	m_stackScene.pop();
	//scene�� ������

	if (m_stackScene.top())	m_stackScene.top()->Begin();
	//top�� ������ begin
}
void CDirectXFramework::PushScene(CScene* pScene) {
	pScene->Begin();
	m_stackScene.push(pScene);
	//���ο� scene�� ���� ����
	++m_nScene;
}
//--------------------------------------scene-----------------------------------------

//--------------------------------------deferred rendering func-----------------------------

CDirectXFramework::~CDirectXFramework() {

}
