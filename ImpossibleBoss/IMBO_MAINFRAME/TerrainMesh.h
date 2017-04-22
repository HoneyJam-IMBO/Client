#pragma once
#include "Mesh.h"

class CTerrainMesh : public CMesh {

public:
	//----------------------------dxobject-----------------------------
	//end는 Cmesh에서 진행한다.
	//----------------------------dxobject-----------------------------

	//---------------------------mesh----------------------------------
	//begin func
	virtual bool CreateVertexBuffer();
	virtual bool CreateIndexBuffer();
	//begin func
	//---------------------------mesh----------------------------------

	static shared_ptr<CTerrainMesh> CreateTerrainMesh(float one_space_size);
	void SetOneSpaceSize(float size) { m_fOneSpaceSize = size; }
private:
	float m_fOneSpaceSize{ 0 };
	//----------------------vertex buffers---------------------------
	ID3D11Buffer* m_pd3dPositionBuffer{ nullptr };
	ID3D11Buffer* m_pd3dTexCoordBuffer{ nullptr };
	ID3D11Buffer* m_pd3dTBuffer{ nullptr };
	ID3D11Buffer* m_pd3dBBuffer{ nullptr };
	//----------------------vertex buffers---------------------------

public:
	CTerrainMesh();
	~CTerrainMesh();
};