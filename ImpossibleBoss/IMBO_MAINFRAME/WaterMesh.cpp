#include "stdafx.h"
#include "WaterMesh.h"


CWaterMesh::CWaterMesh()
{
}


CWaterMesh::~CWaterMesh()
{
}

bool CWaterMesh::CreateVertexBuffer()
{
	m_nVertices = 4;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;

	m_pVertices = new XMFLOAT3[m_nVertices];
	m_pVertices[0] = XMFLOAT3(-1.0f, 0.f, -1.0f);
	m_pVertices[1] = XMFLOAT3(-1.0f, 0.f, 1.0f);
	m_pVertices[2] = XMFLOAT3(1.0f, 0.f, -1.0f);
	m_pVertices[3] = XMFLOAT3(1.0f, 0.f, 1.0f);

	XMFLOAT2 pxmf2TexCoords[4];

	//직육면체의 각 면(삼각형 2개)에 하나의 텍스쳐 이미지 전체가 맵핑되도록 텍스쳐 좌표를 설정한다.
	pxmf2TexCoords[0] = XMFLOAT2(0, 1);
	pxmf2TexCoords[1] = XMFLOAT2(0, 0);
	pxmf2TexCoords[2] = XMFLOAT2(1, 1);
	pxmf2TexCoords[3] = XMFLOAT2(1, 0);


	m_pd3dPositionBuffer = CreateBuffer(sizeof(XMFLOAT3), m_nVertices, m_pVertices, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	m_pd3dUVBuffer = CreateBuffer(sizeof(XMFLOAT2), m_nVertices, pxmf2TexCoords, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);


	XMVECTOR pxmvNormals[4];
	CalculateVertexNormal(pxmvNormals);
	XMFLOAT3 pxmf3Normals[4];
	for (int j = 0; j < 4; ++j) {
		pxmf3Normals[j] = XMFLOAT3(0, 1, 0);
	}
	m_pd3dNormalBuffer = CreateBuffer(sizeof(XMFLOAT3), m_nVertices, pxmf3Normals, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);


	ID3D11Buffer *pd3dBuffers[3] = { m_pd3dPositionBuffer, m_pd3dNormalBuffer, m_pd3dUVBuffer };
	UINT pnBufferStrides[3] = { sizeof(XMFLOAT3), sizeof(XMFLOAT3), sizeof(XMFLOAT2) };
	UINT pnBufferOffsets[3] = { 0, 0, 0 };
	AssembleToVertexBuffer(3, pd3dBuffers, pnBufferStrides, pnBufferOffsets);

	if (m_ppd3dVertexBuffers)	return true;

	return false;
}

bool CWaterMesh::CreateIndexBuffer()
{
	return true;
}
