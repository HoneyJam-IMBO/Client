#pragma once
#include "DXObject.h"
#include "Camera.h"
#include "RenderContainer.h"
#include "Shadow.h"

class CLightRenderer : public DXObject {
public:
	bool Begin();
	virtual bool End();

	virtual void SetShaderState();
	virtual void CleanShaderState();

	virtual void UpdateShaderState();

	//void Excute(shared_ptr<CCamera> pCamera, shared_ptr<CCamera> pLightCam = nullptr, ID3D11ShaderResourceView* pShadowSRV = nullptr);
	void Excute(shared_ptr<CCamera> pCamera, CShadow* m_pShadow = nullptr);
private:
	//rendercontainer map!
	mapRenderContainer m_mRenderContainer;

	//조명 계산을 위한 블랜드 state.
	ID3D11BlendState* m_pLightBlendState{ nullptr };
	ID3D11DepthStencilState* m_pLightDepthStencilState{ nullptr };
	ID3D11RasterizerState* m_pLightRasterizerState{ nullptr };

	//이전 상태 저장
	ID3D11BlendState* m_pPreBlendState{ nullptr };
	float* m_pPreBlendFactor{ nullptr };
	UINT m_PreSampleMask{ 0 };
	ID3D11DepthStencilState* m_pPreDepthStencilState{ nullptr };
	UINT m_PreStencilRef{ 0 };
	ID3D11RasterizerState* m_pPreRasterizerState{ nullptr };

public:
	CLightRenderer();
	virtual ~CLightRenderer();
};