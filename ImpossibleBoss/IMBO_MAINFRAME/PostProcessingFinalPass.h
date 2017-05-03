#pragma once
#include "DXObject.h"
#include "Camera.h"

struct stFinalPassCB {
	float fMiddleGrey;
	float fLumWhiteSqr;
	float fBloomScale;
	float pad;
};

class CPostProcessingFinalPass : public DXObject {
public:
	bool Begin();
	virtual bool End();

	virtual void SetShaderState();
	virtual void CleanShaderState();

	virtual void UpdateShaderState();

	void Excute(shared_ptr<CCamera> pCamera, float fMeddleGrey, float fWhite, float fBloomScale);
	void ResizeBuffer();
	void ReleaseBuffer();

	//	void SetBloomThreshold(float fBloomThreshold) { m_fBloomThreshold = fBloomThreshold; }
private:
	//rendercontainer map!
	mapRC m_mRenderContainer;
	shared_ptr<CBuffer> m_pFinalPassCB{ nullptr };
public:
	CPostProcessingFinalPass();
	virtual ~CPostProcessingFinalPass();
};