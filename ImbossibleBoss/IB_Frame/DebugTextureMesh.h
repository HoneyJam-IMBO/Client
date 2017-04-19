#pragma once
#include "Mesh.h"

class CDebugTextureMesh : public CMesh {

public:
	//----------------------------dxobject-----------------------------
	
	//----------------------------dxobject-----------------------------

	//---------------------------mesh----------------------------------
	//begin func
	virtual bool CreateVertexBuffer();
	virtual bool CreateIndexBuffer();
	//begin func
	//---------------------------mesh----------------------------------
private:
	//----------------------vertex buffers---------------------------

	//----------------------vertex buffers---------------------------

public:
	CDebugTextureMesh();
	~CDebugTextureMesh();
};