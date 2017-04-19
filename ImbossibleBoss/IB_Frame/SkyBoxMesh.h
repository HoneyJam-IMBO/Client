#pragma once
#include "Mesh.h"


class CSkyBoxMesh : public CMesh {

public:
	//----------------------------dxobject-----------------------------
	//----------------------------dxobject-----------------------------

	//---------------------------mesh----------------------------------
	virtual void RenderExcute(UINT nnInstance);
	//begin func
	virtual bool CreateVertexBuffer();
	virtual bool CreateIndexBuffer();
	//begin func
	//---------------------------mesh----------------------------------
private:
	//----------------------vertex buffers---------------------------
	ID3D11Buffer* m_pd3dPositionBuffer{ nullptr };
	ID3D11Buffer* m_pd3dUVBuffer{ nullptr };
	//----------------------vertex buffers---------------------------

public:
	CSkyBoxMesh();
	virtual ~CSkyBoxMesh();
};