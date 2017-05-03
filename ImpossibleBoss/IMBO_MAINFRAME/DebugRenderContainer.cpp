#include "stdafx.h"
#include "DebugRenderContainer.h"


//--------------------------container---------------------------------
void CDebugRenderContainer::UpdateShaderState(shared_ptr<CCamera> pCamera) {
	if (m_vpBuffer.IsEmpty()) return;

	m_vpMesh[0]->UpdateShaderState();
	m_pShader->UpdateShaderState();

	size_t vecSize = m_vpTexture.GetCount();
	for (size_t i = 0; i < vecSize; ++i)
	{
		m_vpTexture[i]->UpdateShaderState();
	}

	vecSize = m_vpMaterial.GetCount();
	for (size_t i = 0; i < vecSize; ++i)
	{
		m_vpMaterial[i]->UpdateShaderState();
	}

	vecSize = m_vpBuffer.GetCount();
	for (size_t i = 0; i < vecSize; ++i)
	{
		m_vpBuffer[i]->UpdateShaderState();
	}
	/*for (auto p : m_vpTexture) {
		p->UpdateShaderState();
	}
	for (auto p : m_vpMaterial) {
		p->UpdateShaderState();
	}
	for (auto p : m_vpBuffer) {
		p->UpdateShaderState();
	}*/

	//if (m_pGlobalBuffer) m_pGlobalBuffer->UpdateShaderState();
	//----------------------------update instance buffer--------------------------

	
	int nInstance = 0;

	int nBuffer = 0;
	//map
	vecSize = m_vpBuffer.GetCount();
	for (size_t i = 0; i < vecSize; ++i)
	{
		m_ppBufferData[nBuffer++] = m_vpBuffer[i]->Map();
	}
	//for (auto p : m_vpBuffer) {
	//	m_ppBufferData[nBuffer++] = p->Map();
	//}
	vecSize = m_lpObjects.GetCount();
	for (size_t i = 0; i < vecSize; ++i)
	{
		m_lpObjects[i]->SetBufferInfo(m_ppBufferData, nInstance, pCamera);
		nInstance++;
	}
	/*for (auto pObject : m_lpObjects) {
		pObject->SetBufferInfo(m_ppBufferData, nInstance, pCamera);
		nInstance++;
	}*/

	//unmap
	vecSize = m_vpBuffer.GetCount();
	for (size_t i = 0; i < vecSize; ++i)
	{
		m_vpBuffer[i]->Unmap();
	}
	//for (auto p : m_vpBuffer) {
	//	p->Unmap();
	//}
	//----------------------------update instance buffer--------------------------


}

CDebugRenderContainer::CDebugRenderContainer() : CRenderContainer() {
	
}
CDebugRenderContainer::~CDebugRenderContainer() {

}