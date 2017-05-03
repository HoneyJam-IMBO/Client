#include "stdafx.h"
#include "RenderContainer.h"



//---------------------------dxobject---------------------------------
bool CRenderContainer::Begin() {

	return true;
}
bool CRenderContainer::End() {
	//m_lpObjects.clear();
	m_lpObjects.RemoveAll();
	m_vpTexture.RemoveAll();
	m_vpBuffer.RemoveAll();
	m_vpMesh.RemoveAll();

	delete[] m_ppBufferData;

	return true;
}
//---------------------------dxobject---------------------------------

//--------------------------container---------------------------------
void CRenderContainer::UpdateShaderState(shared_ptr<CCamera> pCamera) {

	//----------------------------update instance buffer--------------------------
	if (m_vpBuffer.IsEmpty()) return;
	int nInstance = 0;
	int nBuffer = 0;
	//map
	size_t vecBufferSize = m_vpBuffer.GetCount();
	for (size_t i = 0; i < vecBufferSize; ++i)
	{
		m_ppBufferData[nBuffer++] = m_vpBuffer[i]->Map();
	}
	/*for (auto p : m_vpBuffer) {
		m_ppBufferData[nBuffer++] = p->Map();
	}*/

	bool bIsDebug = INPUTMGR->GetDebugMode();
	size_t vecSize = m_lpObjects.GetCount();
	for (size_t i = 0; i < vecSize; ++i)
	{
		if (true == bIsDebug)
		{
			DEBUGER->RegistToDebugRenderContainer(m_lpObjects[i]);
			m_lpObjects[i]->RegistToDebuger();
		}
		m_lpObjects[i]->SetBufferInfo(m_ppBufferData, nInstance, pCamera);
		nInstance++;
	}
	/*for (auto pObject : m_lpObjects) {
		if (true == bIsDebug)
		{
			DEBUGER->RegistToDebugRenderContainer(pObject);
			pObject->RegistToDebuger();
		}
		pObject->SetBufferInfo(m_ppBufferData, nInstance, pCamera);
		nInstance++;
	}*/

	//unmap
	for (size_t i = 0; i < vecBufferSize; ++i)
	{
		m_vpBuffer[i]->Unmap();
	}
	/*for (auto p : m_vpBuffer) {
		p->Unmap();
	}*/
	//----------------------------update instance buffer--------------------------


	//if (m_pAnimater) m_pAnimater->Update(TIMEMGR->GetTimeElapsed());

	m_pShader->UpdateShaderState();
	size_t vecTexSize = m_vpTexture.GetCount();
	for (size_t i = 0; i < vecTexSize; ++i)
	{
		m_vpTexture[i]->UpdateShaderState();
	}
	size_t vecMatSize = m_vpMaterial.GetCount();
	for (size_t i = 0; i < vecMatSize; ++i)
	{
		m_vpMaterial[i]->UpdateShaderState();
	}
	for (size_t i = 0; i < vecBufferSize; ++i)
	{
		m_vpBuffer[i]->UpdateShaderState();
	}
	/*for (auto p : m_vpBuffer) {
		p->UpdateShaderState();
	}*/

	//if (m_pGlobalBuffer) m_pGlobalBuffer->UpdateShaderState();

}
void CRenderContainer::SetShaderState() {
	m_pShader->SetShaderState();
	size_t vecSize = m_vpTexture.GetCount();
	for (size_t i = 0; i < vecSize; ++i)
	{
		m_vpTexture[i]->SetShaderState();
	}

	vecSize = m_vpVolatileTexture.GetCount();
	for (size_t i = 0; i < vecSize; ++i)
	{
		m_vpVolatileTexture[i]->SetShaderState();
	}

	vecSize = m_vpMaterial.GetCount();
	for (size_t i = 0; i < vecSize; ++i)
	{
		m_vpMaterial[i]->SetShaderState();
	}

	vecSize = m_vpBuffer.GetCount();
	for (size_t i = 0; i < vecSize; ++i)
	{
		m_vpBuffer[i]->SetShaderState();
	}

	vecSize = m_vpVolatileBuffer.GetCount();
	for (size_t i = 0; i < vecSize; ++i)
	{
		m_vpVolatileBuffer[i]->SetShaderState();
	}
	/*for (auto p : m_vpTexture) {
		p->SetShaderState();
	}
	for (auto p : m_vpVolatileTexture) {
		p->SetShaderState();
	}
	for (auto p : m_vpMaterial) {
		p->SetShaderState();
	}
	for (auto p : m_vpBuffer) {
		p->SetShaderState();
	}
	for (auto p : m_vpVolatileBuffer) {
		p->SetShaderState();
	}*/

	if (m_pAnimater)m_pAnimater->SetShaderState();

}

void CRenderContainer::ClearVolatileResources() {
	m_vpVolatileTexture.RemoveAll();
	m_vpVolatileBuffer.RemoveAll();
}

void CRenderContainer::RenderExcute() {
	size_t vecSize = m_vpMesh.GetCount();
	for (size_t i = 0; i < vecSize; ++i)
	{
		m_vpMesh[i]->Render(m_lpObjects.GetCount());
	}
	//for (auto p : m_vpMesh) {
	//	p->Render(m_lpObjects.GetCount());
	//}
}
void CRenderContainer::RenderExcuteWithOutObject() {
	size_t vecSize = m_vpMesh.GetCount();
	for (size_t i = 0; i < vecSize; ++i)
	{
		m_vpMesh[i]->Render(1);
	}
	/*for (auto p : m_vpMesh) {
		p->Render(1);
	}*/
}
void CRenderContainer::CleanShaderState() {
	m_pShader->CleanShaderState();

	size_t vecSize = m_vpTexture.GetCount();
	for (size_t i = 0; i < vecSize; ++i){
		m_vpTexture[i]->CleanShaderState();
	}
	vecSize = m_vpVolatileTexture.GetCount();
	for (size_t i = 0; i < vecSize; ++i) {
		m_vpVolatileTexture[i]->CleanShaderState();
	}
	vecSize = m_vpMaterial.GetCount();
	for (size_t i = 0; i < vecSize; ++i) {
		m_vpMaterial[i]->CleanShaderState();
	}
	vecSize = m_vpBuffer.GetCount();
	for (size_t i = 0; i < vecSize; ++i) {
		m_vpBuffer[i]->CleanShaderState();
	}
	vecSize = m_vpVolatileBuffer.GetCount();
	for (size_t i = 0; i < vecSize; ++i) {
		m_vpVolatileBuffer[i]->CleanShaderState();
	}
	//for (auto p : m_vpTexture) {
	//	p->CleanShaderState();
	//}
	//for (auto p : m_vpVolatileTexture) {
	//	p->CleanShaderState();
	//}
	//for (auto p : m_vpMaterial) {
	//	p->CleanShaderState();
	//}
	//for (auto p : m_vpBuffer) {
	//	p->CleanShaderState();
	//}
	//for (auto p : m_vpVolatileBuffer) {
	//	p->CleanShaderState();
	//}
	if (m_pAnimater)m_pAnimater->CleanShaderState();
	//if (m_pGlobalBuffer) m_pGlobalBuffer->CleanShaderState();//global buffer
}


//--------------------------container 불변 함수---------------------------------
void CRenderContainer::Render(shared_ptr<CCamera> pCamera) {
	if (m_lpObjects.IsEmpty()) return;

	UpdateShaderState(pCamera);
	SetShaderState();
	RenderExcute();			//Render;
	CleanShaderState();
}

void CRenderContainer::RenderWithOutObject(shared_ptr<CCamera> pCamera) {
	//shader State Update/ Instancing Buffet Update
	UpdateShaderState(pCamera);
	SetShaderState();
	RenderExcuteWithOutObject();
	CleanShaderState();
}

void CRenderContainer::ClearMesh() {
	if (m_vpMesh.IsEmpty())return;
	m_vpMesh.RemoveAll();
}

void CRenderContainer::ClearBuffer() {
	if (m_vpBuffer.IsEmpty())return;
	m_vpBuffer.RemoveAll();
	m_nBuffer = 0;
}
void CRenderContainer::ClearAnimater() {
	m_pAnimater = nullptr;
	//animater를 지울일이 없음.. 사용하는거지 내가 관리할 녀석ㅇ ㅣ아니거든..
}

void CRenderContainer::AddMesh(shared_ptr<CMesh> pMesh) {
	if (!pMesh) return;
	m_vpMesh.Add(pMesh);
	//m_vpMesh.push_back(pMesh);
}
void CRenderContainer::SetMesh(shared_ptr<CMesh> pMesh) {
	if (!pMesh) return;
	m_vpMesh.Add(pMesh);
	//m_vpMesh.push_back(pMesh);
}
void CRenderContainer::SetShader(shared_ptr<CRenderShader> pShader) {
	if (!pShader)return;

	m_pShader = pShader;
}
void CRenderContainer::AddTexture(shared_ptr<CTexture> pTexture) {
	if (!pTexture)return;
	m_vpTexture.Add(pTexture);
	//m_vpTexture.emplace_back(pTexture);
}

void CRenderContainer::AddVolatileTexture(shared_ptr<CTexture> pTexture) {
	if (!pTexture)return;
	m_vpVolatileTexture.Add(pTexture);
	//m_vpVolatileTexture.emplace_back(pTexture);
}

//-----------------------------------------buffer-----------------------------------------
void CRenderContainer::AddBuffer(shared_ptr<CBuffer> pBuffer) {
	if (!pBuffer) return;


	//void**를 초기화 하기 위해 필요한놈
	m_nBuffer++;
	m_vpBuffer.Add(pBuffer);
	//m_vpBuffer.emplace_back(pBuffer);

	if (m_ppBufferData) delete[] m_ppBufferData;
	m_ppBufferData = new void*[m_vpBuffer.GetCount()];
}
void CRenderContainer::AddVolatileBuffer(shared_ptr<CBuffer> pBuffer) {
	if (!pBuffer) return;
	m_vpVolatileBuffer.Add(pBuffer);
	//m_vpVolatileBuffer.emplace_back(pBuffer);
}
void CRenderContainer::AddInstanceBuffer(shared_ptr<CBuffer> pBuffer) {
	if (!pBuffer) return;

	//void**를 초기화 하기 위해 필요한놈
	m_nBuffer++;
	m_vpBuffer.Add(pBuffer);
	/*m_vpBuffer.emplace_back(pBuffer);*/

	if (m_ppBufferData) delete[] m_ppBufferData;
	m_ppBufferData = new void*[m_vpBuffer.GetCount()];

	ID3D11Buffer* ppBuffers[1] = { pBuffer->GetBuffer() };
	UINT ppStrides[1] = { pBuffer->GetBufferStride() };
	UINT ppOffset[1] = { pBuffer->GetOffset() };


	size_t vecSize = m_vpMesh.GetCount();
	for (size_t i = 0; i < vecSize; ++i)
	{
		m_vpMesh[i]->AddInstancingBuffer(pBuffer.get());
	}
	//for (auto p : m_vpMesh) {
	//	//p->AssembleToVertexBuffer(1, ppBuffers, ppStrides, ppOffset);
	//	p->AddInstancingBuffer(pBuffer.get());
	//}

}
//-----------------------------------------buffer-----------------------------------------

void CRenderContainer::AddMaterial(shared_ptr<CMaterial> pMaterial) {
	if (!pMaterial)return;

	m_nMaterial++;
	m_vpMaterial.Add(pMaterial);
	//m_vpMaterial.emplace_back(pMaterial);
}

void CRenderContainer::SetObejcts(int n, CGameObject** ppObjects) {
	if (!ppObjects) return;

	for (int i = 0; i < n; ++i) {
		m_lpObjects.Add(ppObjects[i]);
	}
}

void CRenderContainer::AddObject(CGameObject* pObject) {
	if (!pObject) return;

	m_lpObjects.Add(pObject);
}
//void CRenderContainer::RemoveObject(CGameObject* pObject) {
//	if (!pObject) return;
//	//if (0 == m_lpObjects.size()) return;
//
//	// 벡터로 다 바꿈
//	/*m_lpObjects.remove_if([&pObject](CGameObject* pO) {
//		return pObject == pO;
//	});*/
//}
//--------------------------container 불변 함수---------------------------------

CRenderContainer::CRenderContainer() : DXObject("rendercontainer") {
}
CRenderContainer::~CRenderContainer() {

}