#pragma once
#include "DXObject.h"
#include "Camera.h"
#include "RenderContainer.h"

class CObjectRenderer : public DXObject{
public:
	bool Begin();
	virtual bool End();

	virtual void SetShaderState();
	virtual void CleanShaderState();

	virtual void UpdateShaderState();

	void Excute(shared_ptr<CCamera> pCamera);
	void ExcuteShadowRender(shared_ptr<CCamera> pCamera);
	void RenderSkyBox();

	CRenderContainer* GetSkyBoxRenderContainer() { return m_pSkyBoxRenderContainer; }
	CRenderContainer* GetTerrainRenderContainer() { return m_pTerrainRenderContainer; }
private:
	ID3D11DepthStencilState* m_pd3dDepthStencilState{ nullptr };
	ID3D11DepthStencilState* m_pd3dTempDepthStencilState{ nullptr };
	UINT m_TempStencil{ 0 };

	//rendercontainer map!
	mapRC	m_mObjectRenderContainer;
	
	CRenderContainer* m_pTerrainRenderContainer{ nullptr };
	CRenderContainer* m_pSkyBoxRenderContainer{ nullptr };
public:
	CObjectRenderer();
	virtual ~CObjectRenderer();
};