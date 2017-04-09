#pragma once
#include "DXObject.h"
class CShadow :
	public DXObject
{
public:
	bool Begin();
	virtual bool End();

	virtual void SetShaderState();
	virtual void CleanShaderState();

	virtual void UpdateShaderState();

	ID3D11ShaderResourceView* RenderShadowMap(shared_ptr<CCamera> pCamera, shared_ptr<CCamera>& pLightCam, CObjectRenderer* objRenderer);

	void ResizeBuffer();
	void ReleaseBuffer();

private:
	ID3D11Texture2D			 *m_pd3dtxtShadow{ nullptr };
	ID3D11ShaderResourceView *m_pd3dsrvShadow{ nullptr };
	ID3D11DepthStencilView	 *m_pd3ddsvShadow{ nullptr };

	ID3D11Texture2D			 *m_pd3dtxtDepth{ nullptr };//1
	ID3D11RenderTargetView   *m_pd3drtvDepth{ nullptr };//0
	ID3D11ShaderResourceView *m_pd3dsrvDepth{ nullptr };
private:
	XMFLOAT4X4			m_xmmtxShadowVP;
	shared_ptr<CBuffer>	m_pShadowVPBuffer;
	shared_ptr<CCamera> m_pCamera{ nullptr };
public:
	CShadow();
	~CShadow();
};

