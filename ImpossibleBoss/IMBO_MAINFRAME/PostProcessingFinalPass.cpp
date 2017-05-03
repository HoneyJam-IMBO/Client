#include "stdafx.h"
#include "PostProcessingFinalPass.h"

bool CPostProcessingFinalPass::Begin() {

	/*for (auto RenderContainer : RCSELLER->GetTagRenderContainer()[tag::TAG_POSTPROCESSING]) {
		m_mRenderContainer[RenderContainer.first] = RenderContainer.second;
	}*/
	CAtlMap<tag, mapRC>::CPair* pOutPair = RCSELLER->GetTagRenderContainer().Lookup(tag::TAG_POSTPROCESSING);
	CAtlMap<CString, CRenderContainer*>::CPair*	pInPair = NULL;
	if (pOutPair != nullptr) {
		POSITION pos = pOutPair->m_value.GetStartPosition();
		while (pos != NULL) {
			pInPair = pOutPair->m_value.GetNext(pos);
			m_mRenderContainer[pInPair->m_key] = pInPair->m_value;
		}
	}

	m_pFinalPassCB = CBuffer::CreateConstantBuffer(1, sizeof(stFinalPassCB), 0, BIND_PS);

	return true;
}

bool CPostProcessingFinalPass::End() {
	//m_mRenderContainer.clear();
	m_mRenderContainer.RemoveAll();
	if (m_pFinalPassCB) m_pFinalPassCB->End();

	return true;
}

void CPostProcessingFinalPass::SetShaderState() {

}

void CPostProcessingFinalPass::CleanShaderState() {

}

void CPostProcessingFinalPass::UpdateShaderState() {

}

void CPostProcessingFinalPass::Excute(shared_ptr<CCamera> pCamera, float fMeddleGrey, float fWhite, float fBloomScale) {
	stFinalPassCB* pData = (stFinalPassCB*)m_pFinalPassCB->Map();
	pData->fMiddleGrey = fMeddleGrey;
	pData->fLumWhiteSqr = fWhite;
	pData->fLumWhiteSqr *= pData->fMiddleGrey;//Scale by the middle grey value
	pData->fLumWhiteSqr *= pData->fLumWhiteSqr;// Squre
	pData->fBloomScale = fBloomScale;
	m_pFinalPassCB->Unmap();
	m_pFinalPassCB->SetShaderState();

	//그것을 이용하여
	//풀 스크린 드로우를 실행 한다.
	//객체없는 랜더
	CString csName = CA2CT("postprocessing");
	m_mRenderContainer[csName]->RenderWithOutObject(pCamera);

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

void CPostProcessingFinalPass::ResizeBuffer() {
	ReleaseBuffer();

}

void CPostProcessingFinalPass::ReleaseBuffer(){
}

CPostProcessingFinalPass::CPostProcessingFinalPass() : DXObject("postprocessingfinalpass") {

}

CPostProcessingFinalPass::~CPostProcessingFinalPass() {

}
