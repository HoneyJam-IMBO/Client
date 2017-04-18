#pragma once
#include "DXObject.h"

struct stShadowInfo {
	float bias{ 0.039f };
	float bias_offset{ 0.0105f };
	//UINT pad[2];
	float kernelhalf{ 1.f };
	float dist_sum{ 0.07f };
	XMMATRIX xmmtxViewProj[MAX_SPACE_NUM];
};

class CShadow :
	public DXObject
{
public:
	bool Begin();
	virtual bool End();

	virtual void SetShaderState();
	virtual void CleanShaderState();

	virtual void UpdateShaderState();

	ID3D11ShaderResourceView* RenderShadowMap(shared_ptr<CCamera> pCamera = nullptr);

	void ResizeBuffer();
	void ReleaseBuffer();

	void SetBias(float bias) { m_pShadowInfo->bias = bias; }
	void SetBiasOffset(float bias_offset) { m_pShadowInfo->bias_offset = bias_offset; }

	void LoadShadow(wstring wsOutputPath, wstring wsSceneName);
private:
	shared_ptr<CTexture> m_pShadowTexture{ nullptr };
	ID3D11Texture2D			 *m_pd3dtxtShadow{ nullptr };
	ID3D11ShaderResourceView *m_pd3dsrvShadow{ nullptr };
	ID3D11DepthStencilView	 *m_pd3ddsvShadow{ nullptr };

	ID3D11RasterizerState* m_pd3dRSShader{ nullptr };
	//shadow buffer
	shared_ptr<CBuffer> m_pShadowBuf{ nullptr };

	stShadowInfo* m_pShadowInfo{ nullptr };
	

	//map tool
	float m_SelectSpace{ 0 };
	ID3D11Texture2D			 *m_pd3dtxtRenderShadow{ nullptr };
	ID3D11ShaderResourceView *m_pd3dsrvRenderShadow{ nullptr };
	ID3D11RenderTargetView	 *m_pd3drtvRenderShadow{ nullptr };
	CDebugTexture* m_pDebugTextureObj{ nullptr };
	//map tool
private:
	XMFLOAT4X4			m_xmmtxShadowVP;
	shared_ptr<CBuffer>	m_pShadowVPBuffer;
	shared_ptr<CCamera> m_pCamera{ nullptr };
public:
	CShadow();
	~CShadow();
};

