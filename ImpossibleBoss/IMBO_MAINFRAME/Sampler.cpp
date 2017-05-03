#include "stdafx.h"
#include "Sampler.h"

bool CSampler::Begin() {

	return true;
}
bool CSampler::End() {

	if (m_pd3dSamplerState) {
		m_pd3dSamplerState->Release();
	}

	return true;
}

void CSampler::SetShaderState() {

	//if (m_BindFlag & BIND_VS) {
		GLOBALVALUEMGR->GetDeviceContext()->VSSetSamplers(m_SamplerStartSlot, 1, &m_pd3dSamplerState);
	//}
	//if (m_BindFlag & BIND_DS) {
		GLOBALVALUEMGR->GetDeviceContext()->DSSetSamplers(m_SamplerStartSlot, 1, &m_pd3dSamplerState);
	//}
	//if (m_BindFlag & BIND_HS) {
	//	GLOBALVALUEMGR->GetDeviceContext()->HSSetSamplers(m_SamplerStartSlot, 1, &m_pd3dSamplerState);
	//}
	//if (m_BindFlag & BIND_GS) {
		GLOBALVALUEMGR->GetDeviceContext()->GSSetSamplers(m_SamplerStartSlot, 1, &m_pd3dSamplerState);
	//}
	//if (m_BindFlag & BIND_PS) {
		GLOBALVALUEMGR->GetDeviceContext()->PSSetSamplers(m_SamplerStartSlot, 1, &m_pd3dSamplerState);
	//}
	//if (m_BindFlag & BIND_CS) {
		GLOBALVALUEMGR->GetDeviceContext()->CSSetSamplers(m_SamplerStartSlot, 1, &m_pd3dSamplerState);
	//}

}
void CSampler::CleanShaderState() {
	ID3D11SamplerState* pSamplerState[1] = { nullptr };

	if (m_BindFlag & BIND_VS) {
		GLOBALVALUEMGR->GetDeviceContext()->VSSetSamplers(m_SamplerStartSlot, 1, pSamplerState);
	}
	if (m_BindFlag & BIND_DS) {
		GLOBALVALUEMGR->GetDeviceContext()->DSSetSamplers(m_SamplerStartSlot, 1, pSamplerState);
	}
	if (m_BindFlag & BIND_HS) {
		GLOBALVALUEMGR->GetDeviceContext()->HSSetSamplers(m_SamplerStartSlot, 1, pSamplerState);
	}
	if (m_BindFlag & BIND_GS) {
		GLOBALVALUEMGR->GetDeviceContext()->GSSetSamplers(m_SamplerStartSlot, 1, pSamplerState);
	}
	if (m_BindFlag & BIND_PS) {
		GLOBALVALUEMGR->GetDeviceContext()->PSSetSamplers(m_SamplerStartSlot, 1, pSamplerState);
	}
	if (m_BindFlag & BIND_CS) {
		GLOBALVALUEMGR->GetDeviceContext()->CSSetSamplers(m_SamplerStartSlot, 1, pSamplerState);
	}

}


shared_ptr<CSampler> CSampler::CreateSampler(UINT Slot, UINT BindFlags, D3D11_TEXTURE_ADDRESS_MODE Mode, D3D11_FILTER Filter, D3D11_COMPARISON_FUNC ComparisionFunc, float MinLOD, float MaxLOD, float BorderColor) {
	shared_ptr<CSampler> pSampler;
	pSampler = make_shared<CSampler>();
	D3D11_SAMPLER_DESC d3dSamplerDesc;
	ZeroMemory(&d3dSamplerDesc, sizeof(D3D11_SAMPLER_DESC));
	pSampler->SetSamplerSlot(Slot);
	pSampler->SetBindFlag(BindFlags);

	//sampler
	ID3D11SamplerState* pd3dSamplerState;
	d3dSamplerDesc.AddressU = Mode;
	d3dSamplerDesc.AddressV = Mode;
	d3dSamplerDesc.AddressW = Mode;

	d3dSamplerDesc.Filter = Filter;
	d3dSamplerDesc.ComparisonFunc = ComparisionFunc;
	d3dSamplerDesc.MinLOD = MinLOD;
	d3dSamplerDesc.MaxLOD = MaxLOD;
	d3dSamplerDesc.BorderColor[0] = BorderColor;
	d3dSamplerDesc.BorderColor[1] = BorderColor;
	d3dSamplerDesc.BorderColor[2] = BorderColor;
	d3dSamplerDesc.BorderColor[3] = BorderColor;
	GLOBALVALUEMGR->GetDevice()->CreateSamplerState(&d3dSamplerDesc, &pd3dSamplerState);
	pSampler->SetpSamplerState(pd3dSamplerState);

	return pSampler;
}


CSampler::CSampler() : DXObject("sampler") { }
CSampler::~CSampler() { };

