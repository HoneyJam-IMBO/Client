#include "stdafx.h"
#include "PostProcessingLayer.h"
//#include "RenderContainerSeller.h"

//---------------------------dxobject---------------------------------
bool CPostProcessingLayer::Begin() {
	
	object_id id = object_id::OBJECT_LIGHT_END;
	for (int i = id + 1; i < object_id::OBJECT_POSTPROCESSING_END; ++i) {
		id = (object_id)i;
		m_mRenderContainer.insert(pairRenderContainer(id, RCSELLER->GetRenderContainer(id)));
	}

	m_pFinalPassCB = CreateBuffer(sizeof(stFinalPassCB), 1, nullptr, D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);


	CreateShaderState();

	return true;
}

void CPostProcessingLayer::CreateShaderState() {
	
}
bool CPostProcessingLayer::End() {
	if (m_pFinalPassCB) m_pFinalPassCB->Release();

	return true;
}
//---------------------------dxobject---------------------------------

//--------------------------container---------------------------------
void CPostProcessingLayer::UpdateShaderState(shared_ptr<CCamera> pCamera) {

}
void CPostProcessingLayer::SetShaderState(shared_ptr<CCamera> pCamera) {
	//finalpass에 필요한 cb를 set
	GLOBALVALUEMGR->GetDeviceContext()->PSSetConstantBuffers(0, 1, &m_pFinalPassCB);
}
void CPostProcessingLayer::RenderExcute(shared_ptr<CCamera> pCamera) {
	//그것을 이용하여
	//풀 스크린 드로우를 실행 한다.
	//객체없는 랜더
	object_id id = object_id::OBJECT_LIGHT_END;
	for (int i = id + 1; i < object_id::OBJECT_POSTPROCESSING_END; ++i) {
		id = (object_id)i;
		m_mRenderContainer[id]->RenderWithOutObject(pCamera);
	}
}

void CPostProcessingLayer::CleanShaderState(shared_ptr<CCamera> pCamera) {
	ID3D11Buffer* pBuffer[4] = { nullptr,nullptr,nullptr,nullptr };
	ID3D11ShaderResourceView* pSRVs[4] = { nullptr, nullptr,nullptr,nullptr };
	ID3D11UnorderedAccessView* pUAVs[4] = { nullptr,nullptr ,nullptr ,nullptr };

	GLOBALVALUEMGR->GetDeviceContext()->VSSetConstantBuffers(0, 4, pBuffer);
	GLOBALVALUEMGR->GetDeviceContext()->PSSetConstantBuffers(0, 4, pBuffer);
	GLOBALVALUEMGR->GetDeviceContext()->CSSetConstantBuffers(0, 4, pBuffer);

	GLOBALVALUEMGR->GetDeviceContext()->VSSetShaderResources(0, 4, pSRVs);
	GLOBALVALUEMGR->GetDeviceContext()->PSSetShaderResources(0, 4, pSRVs);
	GLOBALVALUEMGR->GetDeviceContext()->CSSetShaderResources(0, 4, pSRVs);

	GLOBALVALUEMGR->GetDeviceContext()->CSSetUnorderedAccessViews(0, 4, pUAVs, (UINT*)(&pUAVs));

	
}
void CPostProcessingLayer::SetFinalPassData(float fMiddleGrey, float fWhite, float fBloomScale) {
	m_fMeddleGrey = fMiddleGrey;
	m_fWhite = fWhite;
	m_fBloomScale = fBloomScale;

	D3D11_MAPPED_SUBRESOURCE Data;
	GLOBALVALUEMGR->GetDeviceContext()->Map(m_pFinalPassCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &Data);
	stFinalPassCB* pSecondPassCB = (stFinalPassCB*)Data.pData;
	pSecondPassCB->fMiddleGrey = m_fMeddleGrey;
	pSecondPassCB->fLumWhiteSqr = m_fWhite;
	pSecondPassCB->fLumWhiteSqr *= pSecondPassCB->fMiddleGrey;//Scale by the middle grey value
	pSecondPassCB->fLumWhiteSqr *= pSecondPassCB->fLumWhiteSqr;// Squre
	pSecondPassCB->fBloomScale = m_fBloomScale;

	GLOBALVALUEMGR->GetDeviceContext()->Unmap(m_pFinalPassCB, 0);

}


CPostProcessingLayer::CPostProcessingLayer() : CLayer() {

}
CPostProcessingLayer::~CPostProcessingLayer() {

}
