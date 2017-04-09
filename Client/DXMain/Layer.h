#pragma once 

#include "DXObject.h"
#include "Camera.h"
#include "RenderContainer.h"

class CLayer : public DXObject {
public:
	//---------------------------dxobject---------------------------------
	bool Begin() { return true; }
	virtual bool End() { return true; };

	virtual void SetShaderState(shared_ptr<CCamera> pCamera) {};
	virtual void CleanShaderState(shared_ptr<CCamera> pCamera) {};

	virtual void UpdateShaderState(shared_ptr<CCamera> pCamera) {};
	//---------------------------dxobject---------------------------------

	//--------------------------layer---------------------------------
	virtual void RenderExcute(shared_ptr<CCamera> pCamera) {};
	void Render(shared_ptr<CCamera> pCamera);

	mapRenderContainer& GetRenderContainerMap() { return m_mRenderContainer; }
	//--------------------------layer---------------------------------


private:
	//begin func
	virtual void CreateShaderState() {};

protected:
	//rendercontainer map!
	mapRenderContainer m_mRenderContainer;

public:
	CLayer();
	virtual ~CLayer();

};
