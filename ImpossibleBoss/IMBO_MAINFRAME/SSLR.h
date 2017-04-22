#pragma once
#include "DXObject.h"

struct CB_OCCLUSSION
{
	UINT nWidth;
	UINT nHeight;
	UINT pad[2];
};

struct CB_LIGHT_RAYS
{
	XMFLOAT2 vSunPos;
	float fInitDecay;
	float fDistDecay;
	XMFLOAT3 vRayColor;
	float fMaxDeltaLen;
};

class CSSLR : public DXObject{
public:
	bool Begin();
	virtual bool End();

	virtual void SetShaderState();
	virtual void CleanShaderState();

	virtual void UpdateShaderState();

	void Excute(shared_ptr<CCamera>pCamera, ID3D11RenderTargetView* pLightAccumRTV, ID3D11ShaderResourceView* pMiniDepthSRV, const XMVECTOR& vSunDir, const XMFLOAT3& vSunColor,
		float fOffsetSunPos = -200.f,
		float fMaxSunDist = 1000.f,
	float fInitDecay = 0.2f,
	float fDistDecay = 0.8f,
	float fMaxDeltaLen = 0.005f);
	void ResizeBuffer();
	void ReleaseBuffer();

//	void SetOffsetSunPos(float fOffsetSunPos) { m_fOffsetSunPos = fOffsetSunPos; }
//	void SetSunDist(float fMaxSunDist) { m_fMaxSunDist = fMaxSunDist; }
//	void SetInitDecay(float fInitDecay) { m_fInitDecay = fInitDecay; }
//	void SetDistDecay(float fDistDecay) { m_fDistDecay = fDistDecay; }
//	void SetMaxDeltaLen(float fMaxDeltaLen) { m_fMaxDeltaLen = fMaxDeltaLen; }
//
//	float& GetOffsetSunPos() { return m_fOffsetSunPos; }
//	float& GetMaxSunDist() { return m_fMaxSunDist; }
//	float& GetInitDecay() { return m_fInitDecay; }
//	float& GetDistDecay() { return m_fDistDecay; }
//	float& GetMaxDeltaLen() { return m_fMaxDeltaLen; }
private:

	// Prepare the occlusion map
	void PrepareOcclusion(ID3D11ShaderResourceView* pMiniDepthSRV);

	// Ray trace the occlusion map to generate the rays
	void RayTrace(shared_ptr<CCamera>pCamera, const XMFLOAT2& vSunPosSS, const XMFLOAT3& vSunColor,
		float fInitDecay = 0.2f,
		float fDistDecay = 0.8f,
		float fMaxDeltaLen = 0.005f);

	// Combine the rays with the scene
	void Combine(shared_ptr<CCamera>pCamera, ID3D11RenderTargetView* pLightAccumRTV);

	UINT m_nWidth{ 0 };
	UINT m_nHeight{ 0 };
	UINT m_nThreadGroup{ 0 };

	ID3D11Texture2D* m_pOcclusionTex{ nullptr };
	ID3D11UnorderedAccessView* m_pOcclusionUAV{ nullptr };
	ID3D11ShaderResourceView* m_pOcclusionSRV{ nullptr };
	CComputeShader* m_pMakeOcclussionComputeShader{ nullptr };
	shared_ptr<CTexture> m_pOcclussionTexture{ nullptr };

	CRenderContainer* m_pLayTraceRenderContainer{ nullptr };
	ID3D11Texture2D* m_pLightRaysTex{ nullptr };
	ID3D11RenderTargetView* m_pLightRaysRTV{ nullptr };
	ID3D11ShaderResourceView* m_pLightRaysSRV{ nullptr };
	shared_ptr<CTexture> m_pLightRaysTexture{ nullptr };
	
	CRenderContainer* m_pConmbineRenderContainer{ nullptr };

	// Shaders
	shared_ptr<CBuffer> m_pOcclusionCB;
	shared_ptr<CBuffer> m_pRayTraceCB;
	
	// Additive blend state to add the light rays on top of the scene lights
	ID3D11BlendState* m_pAdditiveBlendState{ nullptr };
public:
	CSSLR();
	virtual ~CSSLR();
};

