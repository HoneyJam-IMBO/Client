#include "stdafx.h"
#include "SkyBoxContainer.h"
#include "SpaceContainer.h"

void CSkyBoxContainer::Begin(shared_ptr<CCamera> pCamera) {
	//skybox
	m_pSkyBox = new CSkyBox();
	m_pSkyBox->Begin();	
	m_pSkyBox->SetCamera(pCamera);
	//skybox

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
void CSkyBoxContainer::Update(float fTimeElapsed) {
	if (m_bActive) {
		m_pSkyBox->Animate(fTimeElapsed);
		//m_pSkyBox->RegistToContainer();
		PrepareRender();
		return;
	}
	//RENDERER->SetSkyBoxContainer(nullptr);
}
CSkyBoxContainer * CSkyBoxContainer::CreateSkyBoxContainer(LPCTSTR pSkyBoxName, UINT textureIndex, CSpaceContainer * pSpaceContainer, shared_ptr<CCamera> pCamera){
	CSkyBoxContainer* pSkyBoxContainer = new CSkyBoxContainer();
	pSkyBoxContainer->SetSkyBoxName(pSkyBoxName);
	pSkyBoxContainer->CreateSkyBoxTexture(textureIndex);
	pSkyBoxContainer->SetSpaceContainer(pSpaceContainer);
	pSkyBoxContainer->Begin(pCamera);
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
	RENDERER->GetSkyBoxRenderContainer()->ClearVolatileResources();
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

