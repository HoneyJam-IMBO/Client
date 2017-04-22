#pragma once
#include "DXObject.h"
#include "Camera.h"

struct stBlur {
	UINT m_nWidth{ 0 };
	UINT m_nHeight{ 0 };
	XMFLOAT2 pad;
};
class CBlur : public DXObject {
public:
	bool Begin();
	virtual bool End();

	virtual void SetShaderState();
	virtual void CleanShaderState();

	virtual void UpdateShaderState();

	ID3D11ShaderResourceView* Excute(ID3D11ShaderResourceView* pSRVDest);
	void ResizeBuffer(UINT nScale);
	void ReleaseBuffer();

private:
	UINT m_nWidth{ 0 };
	UINT m_nHeight{ 0 };
	UINT m_nDownScaleGroup{ 0 };

	//2
	ID3D11Texture2D			 *m_pd3dtxtTempBlur{ nullptr };
	ID3D11ShaderResourceView *m_pd3dsrvTempBlur{ nullptr };
	ID3D11UnorderedAccessView*m_pd3duavTempBlur{ nullptr };

	CComputeShader* m_pHorBlurComputeShader{ nullptr };
	shared_ptr<CBuffer> m_pResBuffer{ nullptr };

	//3
	ID3D11Texture2D			 *m_pd3dtxtBloom{ nullptr };
	ID3D11ShaderResourceView *m_pd3dsrvBloom{ nullptr };
	ID3D11UnorderedAccessView*m_pd3duavBloom{ nullptr };

	CComputeShader* m_pVerBlurComputeShader{ nullptr };
public:
	CBlur();
	virtual ~CBlur();
};