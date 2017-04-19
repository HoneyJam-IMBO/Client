#pragma once
#include "DXObject.h"
#include "Camera.h"
#include "RenderContainer.h"
#include "ComputeShader.h"

struct TDownscaleCB {
	UINT nWidth;//수평/2
	UINT nHeight;//수직/2
	float fHorResRcp;
	float fVerResRcp;
	XMFLOAT4 ProjParams;
	XMFLOAT4X4 ViewMtx;
	float fOffsetRadius;
	float fRadius;
	float fMaxDepth;
	UINT pad;
};

class CAORenderer : public DXObject {
public:
	bool Begin();
	virtual bool End();

	virtual void SetShaderState();
	virtual void CleanShaderState();

	virtual void UpdateShaderState();

	ID3D11ShaderResourceView* Excute(shared_ptr<CCamera> pCamera, float fOffsetRadius, float fRadius);
	void ResizeBuffer();
	void ReleaseAmbientOcculutionViews();
private:

	//--------------------------ao----------------------------
	CComputeShader* m_pCSAONormalDepthDownScale;
	ID3D11Buffer				*m_pd3dbufAOMiniNormalDepth{ nullptr };//1/4scale texture
	ID3D11ShaderResourceView	*m_pd3dsrvAOMiniNormalDepth{ nullptr };
	ID3D11UnorderedAccessView	*m_pd3duavAOMiniNormalDepth{ nullptr };

	CComputeShader* m_pCSSSAOCompute;
	ID3D11Texture2D				*m_pd3dtxtAmbientOcculution{ nullptr };//1/4scale texture
	ID3D11ShaderResourceView	*m_pd3dsrvAmbientOcculution{ nullptr };
	ID3D11UnorderedAccessView	*m_pd3duavAmbientOcculution{ nullptr };

	//CStaticBuffer				*m_pAmbientOcculutionConstantBuffer{ nullptr };
	TDownscaleCB				*m_pTDoownscaleCB{ nullptr };
	ID3D11Buffer				*m_pd3dbufAOConstantBuffer{ nullptr };
	shared_ptr<CTexture> m_pAmbientOcculutionTexture;
	//--------------------------ao----------------------------

public:
	CAORenderer();
	virtual ~CAORenderer();
};