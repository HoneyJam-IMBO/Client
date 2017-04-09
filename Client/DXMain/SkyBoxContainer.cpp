#include "stdafx.h"
#include "SkyBoxContainer.h"
#include "SpaceContainer.h"

bool CSkyBoxContainer::Begin() {
	//skybox
	m_pSkyBox = new CSkyBox();
	m_pSkyBox->Begin();	
	//skybox
	return true;
}

bool CSkyBoxContainer::End() {
	//global object
	if (m_pSkyBox) {
		m_pSkyBox->End();
		delete m_pSkyBox;
	}
	m_pSkyBox = nullptr;
	return true;
}
void CSkyBoxContainer::Update(shared_ptr<CCamera> pCamera, float fTimeElapsed) {
	if (m_bActive) {
		m_pSkyBox->SetCamera(pCamera);
		//skybox camera 동기화
		m_pSkyBox->Animate(fTimeElapsed);
		//sky box 등록
		m_pSkyBox->RegistToContainer();
		//registe to renderer
		//RENDERER->SetSkyBoxContainer(this);
		PrepareRender();
		return;
	}
	//RENDERER->SetSkyBoxContainer(nullptr);
}
CSkyBoxContainer * CSkyBoxContainer::CreateSkyBoxContainer(LPCTSTR pSkyBoxName, UINT textureIndex, CSpaceContainer * pSpaceContainer){
	CSkyBoxContainer* pSkyBoxContainer = new CSkyBoxContainer();
	pSkyBoxContainer->SetSkyBoxName(pSkyBoxName);
	pSkyBoxContainer->CreateSkyBoxTexture(textureIndex);
	pSkyBoxContainer->SetSpaceContainer(pSpaceContainer);
	pSkyBoxContainer->Begin();
	return pSkyBoxContainer;
}
void CSkyBoxContainer::CreateSkyBoxTexture(UINT index){
	if (m_ptxtSkyBox) m_ptxtSkyBox->End();
	m_ptxtSkyBox = nullptr;

	//skybox
	_TCHAR pstrTextureNames[128];
	_stprintf_s(pstrTextureNames, _T("../../Assets/SkyBox_%d.dds"), index);
	string name{ "" }; name.assign(m_wsSkyBoxName.begin(), m_wsSkyBoxName.end());
	m_ptxtSkyBox = CTexture::CreateTexture(pstrTextureNames, PS_SLOT_SKYBOX, BIND_PS);
}
void CSkyBoxContainer::PrepareRender() {
	RENDERER->GetSkyBoxRenderContainer()->ClearVolitileObj();
	RENDERER->GetSkyBoxRenderContainer()->AddVolatileTexture(m_ptxtSkyBox);
	//if (m_bActive) {
	//	//skybox
	//	GLOBALVALUEMGR->GetDeviceContext()->OMGetDepthStencilState(&m_pd3dTempDepthStencilState, &m_TempStencil);
	//	GLOBALVALUEMGR->GetDeviceContext()->OMSetDepthStencilState(m_pd3dDepthStencilState, 0);
	//	
	//	m_pSkyboxContainer->Render(pCamera);
	//	GLOBALVALUEMGR->GetDeviceContext()->OMSetDepthStencilState(m_pd3dTempDepthStencilState, m_TempStencil);
	//	//skybox
	//}
	//m_pSkyboxContainer->ClearObjectList();
}
CSkyBoxContainer::CSkyBoxContainer() : CObject("skyboxcontainer") {
}

CSkyBoxContainer::~CSkyBoxContainer()
{
}

