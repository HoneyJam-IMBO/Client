#pragma once
#include "DXObject.h"


class CSampler : public DXObject {
public:
	bool Begin();
	virtual bool End();

	virtual void SetShaderState();
	virtual void CleanShaderState();

	static shared_ptr<CSampler> CreateSampler(UINT Slot = 0, UINT BindFlags = BIND_PS, D3D11_TEXTURE_ADDRESS_MODE Mode = D3D11_TEXTURE_ADDRESS_WRAP, D3D11_FILTER Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_COMPARISON_FUNC ComparisionFunc = D3D11_COMPARISON_NEVER, float MinLOD = 0, float MaxLOD = 0, float BorderColor = 0);

protected:
	void SetpSamplerState(ID3D11SamplerState* pd3dSamplerState) { m_pd3dSamplerState = pd3dSamplerState; }
	void SetSamplerSlot(UINT nSlot) { m_SamplerStartSlot = nSlot; }
	void SetBindFlag(UINT BindFlag) { m_BindFlag = BindFlag; };


	//smapler 1°³
	ID3D11SamplerState*				m_pd3dSamplerState{ nullptr };
	UINT							m_SamplerStartSlot{ 0 };
	UINT							m_BindFlag{ 0 };

public:
	CSampler();
	virtual ~CSampler();

};