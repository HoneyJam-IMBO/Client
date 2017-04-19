#pragma once
#include "Layer.h"
#include "Texture.h"
//#include "RenderContainerSeller.h"

#include "PostProcessingShader.h"
#include "TestDeferredMesh.h"

#include "InstancingShader.h"

#include "BloomDownScale.h"
#include "Bloom.h"
#include "Blur.h"

#define BLOOM_TEMP_TEX_NUM 2
struct stFinalPassCB {
	float fMiddleGrey;
	float fLumWhiteSqr;
	float fBloomScale;
	float pad;
};

class CPostProcessingLayer : public CLayer {
public:
	//---------------------------dxobject---------------------------------
	bool Begin();
	virtual bool End();

	virtual void SetShaderState(shared_ptr<CCamera> pCamera);
	virtual void CleanShaderState(shared_ptr<CCamera> pCamera);

	virtual void UpdateShaderState(shared_ptr<CCamera> pCamera);
	//---------------------------dxobject---------------------------------

	//--------------------------container---------------------------------
	virtual void RenderExcute(shared_ptr<CCamera> pCamera);
	//--------------------------container---------------------------------
	//-----------------------------postprocessing--------------------------
	void SetFinalPassData(float fMiddleGrey, float fWhite, float fBloomScale);
	//-----------------------------postprocessing--------------------------
private:
	virtual void CreateShaderState();

private:
	//final 풀스크린 드로우를 위한 cb
	ID3D11Buffer* m_pFinalPassCB{ nullptr };
	float m_fMeddleGrey{ 0.0 };
	float m_fWhite{ 0.0 };

	//bloom
	float m_fBloomScale{ 0.0 };

	//전체적인 cs라인에 유지해야될 데이터
	UINT m_nDownScaleGroup{ 0 };
	UINT m_nWidth{ 0 };
	UINT m_nHeight{ 0 };


	//휘도 down sacle을 위한 cs
	//CDownScaleFirstPassShader* m_pCSDownScaleFirstPass{ nullptr };
	//CDownScaleSecondPassShader* m_pCSDownScaleSecondPass{ nullptr };

	//bloom
	//CCSBloom* m_pCSBloom{ nullptr };

	//blur 
	//CCSHorizontalBlur* m_pCSHorizontalBlur{ nullptr };
	//CCSVerticalBlur* m_pCSVerticalBlur{ nullptr };

public:
	CPostProcessingLayer();
	virtual ~CPostProcessingLayer();

};