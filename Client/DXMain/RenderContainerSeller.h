#pragma once
//#include "Camera.h"
#include "SingleTon.h"

#include "RenderContainer.h"
#include "DebugRenderContainer.h"

class CRenderContainerSeller : public CSingleTonBase<CRenderContainerSeller>{
public:
	bool Begin();
	bool End();

	mapTagRenderContainer& GetTagRenderContainer() { return m_mTagRenderContainer; }
	//mapRenderContainer GetTagRenderContainer(tag t) { return m_mTagRenderContainer[t]; }
	CRenderContainer* GetRenderContainer(string name);
	CRenderContainer* GetRenderContainer(tag t, string name);

private:
	void CreateStempRenderContainer();
	//이게 진짜 rendercontainer
	mapTagRenderContainer m_mTagRenderContainer;

public:
	CRenderContainerSeller() : CSingleTonBase<CRenderContainerSeller>("inputmanagersingleton") {}

};
