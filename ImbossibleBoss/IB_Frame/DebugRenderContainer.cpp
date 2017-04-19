#include "stdafx.h"
#include "DebugRenderContainer.h"


//--------------------------container---------------------------------
void CDebugRenderContainer::UpdateShaderState(shared_ptr<CCamera> pCamera) {
	if (m_vpBuffer.empty()) return;

	m_vpMesh[0]->UpdateShaderState();
	m_pShader->UpdateShaderState();
	for (auto p : m_vpTexture) {
		p->UpdateShaderState();
	}
	for (auto p : m_vpMaterial) {
		p->UpdateShaderState();
	}
	for (auto p : m_vpBuffer) {
		p->UpdateShaderState();
	}

	//if (m_pGlobalBuffer) m_pGlobalBuffer->UpdateShaderState();
	//----------------------------update instance buffer--------------------------

	
	int nInstance = 0;

	int nBuffer = 0;
	//map
	for (auto p : m_vpBuffer) {
		m_ppBufferData[nBuffer++] = p->Map();
	}
	for (auto pObject : m_lpObjects) {
		pObject->SetBufferInfo(m_ppBufferData, nInstance, pCamera);
		nInstance++;
	}

	//unmap
	for (auto p : m_vpBuffer) {
		p->Unmap();
	}
	//----------------------------update instance buffer--------------------------


}

CDebugRenderContainer::CDebugRenderContainer() : CRenderContainer() {
	
}
CDebugRenderContainer::~CDebugRenderContainer() {

}