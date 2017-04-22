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
	mapTagRenderContainer& GetStempRenderContainer() { return m_mStempRenderContainer; }
	CRenderContainer* GetRenderContainer(string name);
	CRenderContainer* GetRenderContainer(tag t, string name);
public:
	void	ClearStempRenderContainer();
	void	CreateStempRenderContainer();
private:
	
	//이게 진짜 rendercontainer
	mapTagRenderContainer m_mTagRenderContainer;
	mapTagRenderContainer m_mStempRenderContainer;		// 로딩마다 유동적으로 생성/삭제 되는 렌더컨테이너 

public:
	CRenderContainerSeller() : CSingleTonBase<CRenderContainerSeller>("inputmanagersingleton") {}

};
