#pragma once

#include "SingleTon.h"

//
#include "DirectionalLight.h"
#include "TerrainContainer.h"
#include "SkyBoxContainer.h"
#include "ObjectRenderer.h"
#include "AORenderer.h"
#include "LightRenderer.h"
#include "BloomDownScale.h"
#include "Bloom.h"
#include "Blur.h"
#include "PostProcessingFinalPass.h"
#include "SSLR.h"
#include "Shadow.h"

class CDirectXFramework;

class CRenderer : public CSingleTonBase<CRenderer> {

public:
	bool Begin();
	bool End();

	void Render(shared_ptr<CCamera> pCamera);
	void PostProcessing(shared_ptr<CCamera> pCamera);
	void Update(float fTimeElapsed);

	void SetForwardRenderTargets();
	void SetMainRenderTargetView();
	void SetRenderTargetViews(UINT nRenderTarget, ID3D11RenderTargetView** pd3dRTVs, ID3D11DepthStencilView* pd3ddsvDepthStencil);

	bool CreateSwapChain();
	bool CreateRenderTargetView();
	void ReleaseForwardRenderTargets();
	void ClearDepthStencilView(ID3D11DepthStencilView* pDepthStencilView);

	bool ResizeBuffer();
	void SetFramework(CDirectXFramework* pFramework) { m_pFramework = pFramework; }

	CRenderContainer* GetTerrainRenderContainer() { return m_pObjectRenderer->GetTerrainRenderContainer(); }
	CRenderContainer* GetSkyBoxRenderContainer() { return m_pObjectRenderer->GetSkyBoxRenderContainer(); }

	void SaveEffectInfo();
	void LoadEffectInfo();
	//void SetTerrainContainer(CTerrainContainer* pTerrainContainer) { m_pTerrainContainer = pTerrainContainer; }
	//void SetSkyBoxContainer(CSkyBoxContainer* pSkyBoxContainer) { m_pSkyBoxContainer = pSkyBoxContainer; }
	//void SetDirectionalLight(CDirectionalLight* pDirectionalLight) { m_pDirectionalLIght = pDirectionalLight; }
private:
	CDirectXFramework* m_pFramework{ nullptr };
	CRenderContainer* m_pTerrainRenderContainer{ nullptr };
	CRenderContainer* m_pSkyBoxRenderContainer{ nullptr };

	IDXGISwapChain			*	m_pdxgiSwapChain{ nullptr };
	ID3D11RenderTargetView	*	m_pd3dRenderTargetView{ nullptr };
	
	ID3D11DepthStencilState* m_pd3dDepthStencilState{ nullptr };
	ID3D11DepthStencilView		*m_pd3ddsvReadOnlyDepthStencil{ nullptr };

	//---------------------------deferred rendering----------------------
	ID3D11Texture2D			 *m_pd3dtxtDepthStencil{ nullptr };
	ID3D11Texture2D			 *m_pd3dtxtColorSpecInt{ nullptr };//0
	ID3D11Texture2D			 *m_pd3dtxtNormal{ nullptr };//1
	ID3D11Texture2D			 *m_pd3dtxtSpecPow{ nullptr };//2


	ID3D11ShaderResourceView *m_pd3dsrvDepthStencil{ nullptr };
	ID3D11ShaderResourceView *m_pd3dsrvColorSpecInt{ nullptr };//0
	ID3D11ShaderResourceView *m_pd3dsrvNormal{ nullptr };//1
	ID3D11ShaderResourceView *m_pd3dsrvSpecPow{ nullptr };//2

	ID3D11DepthStencilView	 *m_pd3ddsvDepthStencil{ nullptr };
	ID3D11RenderTargetView   *m_pd3drtvColorSpecInt{ nullptr };//0
	ID3D11RenderTargetView   *m_pd3drtvNormal{ nullptr };//1
	ID3D11RenderTargetView   *m_pd3drtvSpecPow{ nullptr };//2

	vector<shared_ptr<CTexture>> m_vObjectLayerResultTexture;
	vector<shared_ptr<CTexture>> m_vLightLayerResultTexture;

	//--------------------------light render target----------------
	ID3D11Texture2D			 *m_pd3dtxtLight{ nullptr };
	ID3D11ShaderResourceView *m_pd3dsrvLight{ nullptr };
	ID3D11RenderTargetView   *m_pd3drtvLight{ nullptr };
	//--------------------------light render target----------------
	//-------------------------layer-------------------------
	CObjectRenderer*			m_pObjectRenderer{ nullptr };
	CAORenderer*				m_pAORenderer{ nullptr };
	CLightRenderer*				m_pLightRenderer{ nullptr };
	CBloomDownScale *			m_pBloomDownScale{ nullptr };
	CBloom*						m_pBloom{ nullptr };
	CBlur*						m_p16to1Blur{ nullptr };
	CBlur*						m_p4to1Blur{ nullptr };
	CPostProcessingFinalPass*	m_pPostProcessingFinalPass{ nullptr };
	CSSLR*						m_pSSLR{ nullptr };
	CShadow*					m_pShadow{ nullptr };
	//-------------------------layer-------------------------

	//effects
	//ssao
	float m_fSSAORadius{ 0.f };
	float m_fSSAOOffsetRadius{ 0.f };
	//bloom
	float m_fBLOOMThreshold{ 2.f };
	float m_fBLOOMMiddleGrey{ 0.01f };
	float m_fBLOOMWhite{ 2.0f };
	float m_fBLOOMScale{ 2.0f };
	//sslr
	bool m_bSSLROnOff;
	float m_fSSLROffsetSunPos{ -200.f };
	float m_fSSLRMaxSunDist{ 1000.f };
	float m_fSSLRInitDecay{ 0.05f };
	float m_fSSLRDistDecay{ 0.05f };
	float m_fSSLRMaxDeltaLen{ 0.05f };
	//effects
public:
	CRenderer();
	virtual ~CRenderer();

//set get
	//ssao
	void SetSSAORadius(float fSSAORadius) { m_fSSAORadius = fSSAORadius; };
	float GetSSAORadius() { return m_fSSAORadius; }
	void SetSSAOOffsetRadius(float SSAOOffsetRadius) { m_fSSAOOffsetRadius = SSAOOffsetRadius; }
	float GetSSAOOffsetRadius() { return m_fSSAOOffsetRadius; }
	//bloom
	void SetBLOOMThreshold(float BLOOMThreshold) { m_fBLOOMThreshold = BLOOMThreshold; }
	float GetBLOOMThreshold() { return m_fBLOOMThreshold; }
	void SetBLOOMMiddleGrey(float BLOOMMiddleGrey) { m_fBLOOMMiddleGrey = BLOOMMiddleGrey; }
	float GetBLOOMMiddleGrey() { return m_fBLOOMMiddleGrey; }
	void SetBLOOMWhite(float BLOOMWhith) { m_fBLOOMWhite = BLOOMWhith; }
	float GetBLOOMWhite() { return m_fBLOOMWhite; }
	void SetBLOOMScale(float BLOOMScale) { m_fBLOOMScale = BLOOMScale; }
	float GetBLOOMScale() { return m_fBLOOMScale; }
	//sslr
	void SetSSLROnOff(bool SSLROnOff) { m_bSSLROnOff = SSLROnOff; }
	bool GetSSLROnOff() { return m_bSSLROnOff; }
	void SetSSLROffsetSunPos(float SSLROffsetSunPos) { m_fSSLROffsetSunPos = SSLROffsetSunPos; }
	float GetSSLROffsetSunPos() { return m_fSSLROffsetSunPos; }
	void SetSSLRMaxSunDist(float SSLRMaxSunDist) { m_fSSLRMaxSunDist = SSLRMaxSunDist; }
	float GetSSLRMaxSunDist() { return m_fSSLRMaxSunDist; }
	void SetSSLRInitDecay(float SSLRInitDecay) { m_fSSLRInitDecay = SSLRInitDecay; }
	float GetSSLRInitDecay() { return m_fSSLRInitDecay; }
	void SetSSLRDistDecay(float SSLRDistDecay) { m_fSSLRDistDecay = SSLRDistDecay; }
	float GetSSLRDistDecay() { return m_fSSLRDistDecay; }
	void SetSSLRMaxDeltaLen(float fSSLRMaxDeltaLen) { m_fSSLRMaxDeltaLen = fSSLRMaxDeltaLen; }
	float GetSSLRMaxDeltaLen() { return m_fSSLRMaxDeltaLen; }
//set get
};
