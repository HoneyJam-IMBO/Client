#pragma once

#include "RenderContainer.h"

class CDebugRenderContainer : public CRenderContainer{
public:

	//---------------------------dxobject---------------------------------
	virtual void UpdateShaderState(shared_ptr<CCamera> pCamera);
	//---------------------------dxobject---------------------------------

	//--------------------------container---------------------------------

public:
	CDebugRenderContainer();
	virtual ~CDebugRenderContainer();

};

//typedef map<string, CRenderContainer*> mapRenderContainer;
//typedef pair<string, CRenderContainer*> pairRenderContainer;
