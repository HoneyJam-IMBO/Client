#include "stdafx.h"
#include "DirectionalLightMesh.h"



bool CDirectionalLightMesh::CreateVertexBuffer() {
	m_nVertices = 4;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;

	m_ppd3dVertexBuffers = nullptr;
	//derectional light mesh는 점이 0개임 hlsl코드에서 알아서 제작한다.
	
	float SPACE_SIZE = 10.f;//이거 필요 없음
	float fx = SPACE_SIZE, fy = SPACE_SIZE, fz = SPACE_SIZE;

	//create space mesh aabb
	//BoundingBox::CreateFromPoints(m_BoundingBox, XMVectorSet(0.f, 0.f, 0.f, 0.f), XMVectorSet(fx, fy, fz, 0.f));
	BoundingBox boundingBox;
	BoundingBox::CreateFromPoints(boundingBox, XMVectorSet(+10.f, +10.f, +10.f, 0.f), XMVectorSet(-10.f, -10.f, -10.f, 0.f));
	m_AABB.SetBoundingBoxInfo(boundingBox);
	

	return true;
}
bool CDirectionalLightMesh::CreateIndexBuffer() {

	return true;
}

CDirectionalLightMesh::CDirectionalLightMesh() : CMesh() {

}
CDirectionalLightMesh ::~CDirectionalLightMesh() {

}