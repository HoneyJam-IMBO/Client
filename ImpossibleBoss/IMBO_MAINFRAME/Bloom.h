#pragma once
#include "DXObject.h"
#include "Camera.h"

class CBloom : public DXObject {
public:
	bool Begin();
	virtual bool End();

	virtual void SetShaderState();
	virtual void CleanShaderState();

	virtual void UpdateShaderState();

	ID3D11ShaderResourceView* Excute(shared_ptr<CCamera> pCamera);
	void ResizeBuffer();
	void ReleaseBuffer();

private:
	UINT m_nWidth{ 0 };
	UINT m_nHeight{ 0 };
	UINT m_nDownScaleGroup{ 0 };
	
	//2
	ID3D11Texture2D			 *m_pd3dtxtTempBloom{ nullptr };
	ID3D11ShaderResourceView *m_pd3dsrvTempBloom{ nullptr };
	ID3D11UnorderedAccessView*m_pd3duavTempBloom{ nullptr };

	CComputeShader* m_pBloomComputeShader{ nullptr };
public:
	CBloom();
	virtual ~CBloom();
};