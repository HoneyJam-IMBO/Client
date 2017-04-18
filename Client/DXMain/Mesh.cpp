#include "stdafx.h"
#include "Mesh.h"

#define DEFAULT_AABB 10
bool CMesh::Begin() {

	//create default aabbzzz
	BoundingBox boundingBox;
	BoundingBox::CreateFromPoints(boundingBox, XMVectorSet(+DEFAULT_AABB, +DEFAULT_AABB, +DEFAULT_AABB, 0.f), XMVectorSet(-DEFAULT_AABB, -DEFAULT_AABB, -DEFAULT_AABB, 0.f));
	m_AABB.SetBoundingBoxInfo(boundingBox);
	
	
	if (false == CreateVertexBuffer()) return false;

	if (false == CreateIndexBuffer()) return false;
	
	CreateConnectingVertexBuffers();

	return true;
}
bool CMesh::End() {
	//instancing buffer는 resourceMGR가 해제한다
	m_vInstancingBuffer.clear();//그래서 clear만 해준다.
	ReleaseConnectingVertexBuffers(); 

	delete[] m_pnIndices;
	delete[] m_pVertices;

	//----------------------vertex buffer release-------------------
	//vertex buffer infos
	if (m_ppd3dVertexBuffers) {
		for (int i = 0; i < m_nVertexBuffers; ++i) {
			if(m_ppd3dVertexBuffers[i]) m_ppd3dVertexBuffers[i]->Release();
		}
		delete[] m_ppd3dVertexBuffers;
	}
	if (m_pnVertexStrides) delete[] m_pnVertexStrides;
	if (m_pnVertexOffsets) delete[] m_pnVertexOffsets;
	//----------------------vertex buffer release-------------------
	//----------------------index buffe rrelease------------------
	if (m_pd3dIndexBuffer)			m_pd3dIndexBuffer->Release();
	//----------------------index buffe rrelease------------------

	return true;
}
void CMesh::SetShaderState() {
	for (auto pTexture : m_vMeshTexture) {
		pTexture->SetShaderState();
	}
	if (m_pMeshMaterial) m_pMeshMaterial->SetShaderState();

	//메쉬
	GLOBALVALUEMGR->GetDeviceContext()->IASetPrimitiveTopology(m_d3dPrimitiveTopology);

	// 꼭지점 버퍼를 연결합니다.
	GLOBALVALUEMGR->GetDeviceContext()->IASetVertexBuffers(m_nStartSlot, m_nConnectingVertexBuffers, m_ppd3dConnectingVertexBuffers, m_pnConnectingVertexStrides, m_pnConnectingVertexOffsets);

	// 인덱스 버퍼를 연결합니다.
	GLOBALVALUEMGR->GetDeviceContext()->IASetIndexBuffer(m_pd3dIndexBuffer, m_d3dIndexFormat, m_nIndexOffset);
}

void CMesh::CleanShaderState(){
	for (auto pTexture : m_vMeshTexture) {
		pTexture->CleanShaderState();
	}
}

void CMesh::Render(UINT nInstance){
	SetShaderState();
	RenderExcute(nInstance);
	CleanShaderState();
}

void CMesh::RenderExcute(UINT nInstance) {

	if (m_pd3dIndexBuffer)
		GLOBALVALUEMGR->GetDeviceContext()->DrawIndexedInstanced(m_nIndices, nInstance, m_nStartIndexLocation, m_nBaseVertexLocation, m_nStartInstanceLocation);
	else
		GLOBALVALUEMGR->GetDeviceContext()->DrawInstanced(m_nVertices, nInstance, m_nStartVertexLocation, m_nStartInstanceLocation);

}

bool CMesh::CreateVertexBuffer(){

	return true;
}

bool CMesh::CreateIndexBuffer(){

	return true;
}

void CMesh::AssembleToVertexBuffer(int nBuffers, ID3D11Buffer **ppd3dBuffers, UINT *pnBufferStrides, UINT *pnBufferOffsets){
	ID3D11Buffer **ppd3dNewVertexBuffers = new ID3D11Buffer*[m_nVertexBuffers + nBuffers];
	UINT *pnNewVertexStrides = new UINT[m_nVertexBuffers + nBuffers];
	UINT *pnNewVertexOffsets = new UINT[m_nVertexBuffers + nBuffers];

	if (m_nVertexBuffers > 0)
	{
		for (int i = 0; i < m_nVertexBuffers; i++)
		{
			ppd3dNewVertexBuffers[i] = m_ppd3dVertexBuffers[i];
			pnNewVertexStrides[i] = m_pnVertexStrides[i];
			pnNewVertexOffsets[i] = m_pnVertexOffsets[i];
		}
		if (m_ppd3dVertexBuffers) delete[] m_ppd3dVertexBuffers;
		if (m_pnVertexStrides) delete[] m_pnVertexStrides;
		if (m_pnVertexOffsets) delete[] m_pnVertexOffsets;
	}

	for (int i = 0; i < nBuffers; i++)
	{
		ppd3dNewVertexBuffers[m_nVertexBuffers + i] = ppd3dBuffers[i];
		pnNewVertexStrides[m_nVertexBuffers + i] = pnBufferStrides[i];
		pnNewVertexOffsets[m_nVertexBuffers + i] = pnBufferOffsets[i];
	}

	m_nVertexBuffers += nBuffers;
	m_ppd3dVertexBuffers = ppd3dNewVertexBuffers;
	m_pnVertexStrides = pnNewVertexStrides;
	m_pnVertexOffsets = pnNewVertexOffsets;
}

void CMesh::ExchangeVertexBuffer(int index, ID3D11Buffer * pd3dBuffer, UINT nBufferStride, UINT nBufferOffset){
	if (m_nVertexBuffers > 0){
		for (int i = 0; i < m_nVertexBuffers; i++){
			if (i == index) {//해당 index이면
				m_ppd3dVertexBuffers[i]->Release();//release
				//새로 넣고
				m_ppd3dVertexBuffers[i] = pd3dBuffer;
				m_pnVertexStrides[i] = nBufferStride;
				m_pnVertexOffsets[i] = nBufferOffset;
				break;
			}
		}
	}
}

void CMesh::CreateTBFromPoints(XMFLOAT3 * pPositions, XMFLOAT2 * pUVs, XMFLOAT3 & outT, XMFLOAT3 & outB){
	XMVECTOR xmvPos[3];
	XMVECTOR xmvUV[3];
	for (int i = 0; i < 3; ++i) {
		xmvPos[i] = XMLoadFloat3(&pPositions[i]);
		xmvUV[i] = XMLoadFloat2(&pUVs[i]);
	}
	XMVECTOR e0 = XMVector3Normalize(xmvPos[1] - xmvPos[0]);
	XMVECTOR e1 = XMVector3Normalize(xmvPos[2] - xmvPos[0]);

	//p0 부터 p1까지의 uv좌표 변화량 
	XMFLOAT2 UV0;
	XMStoreFloat2(&UV0, (xmvUV[1] - xmvUV[0]));
	//p0 부터 p2까지의 uv좌표 변화량
	XMFLOAT2 UV1;
	XMStoreFloat2(&UV1, (xmvUV[2] - xmvUV[0]));

	XMMATRIX mtxOffset = XMMatrixSet(
		UV0.x, UV0.y, 0, 0,
		UV1.x, UV1.y, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0
	);
	mtxOffset = XMMatrixInverse(nullptr, mtxOffset);

	//u0*v1 - v0*u1
	float inverseValue = 1 / (UV0.x * UV1.y - UV0.y * UV1.x);
	float x = inverseValue * UV1.y;//v1
	float y = inverseValue * -UV0.y;//-v0
	float z = inverseValue * -UV1.x;//-u1
	float w = inverseValue * UV0.x;//u0

	XMStoreFloat3(&outT, XMVector3Normalize(e0*x + e1*y));
	XMStoreFloat3(&outB, XMVector3Normalize(e0*z + e1*w));
}
void CMesh::ClearMeshResources(){
	m_pMeshMaterial = nullptr;
	m_vMeshTexture.clear();
}
void CMesh::AddMeshTexture(shared_ptr<CTexture> pTexture){
	m_vMeshTexture.emplace_back(pTexture);
}
void CMesh::SetMeshTexture(UINT index, shared_ptr<CTexture> pTexture){
	m_vMeshTexture[index] = pTexture;
}
void CMesh::SetMeshMaterial(shared_ptr<CMaterial> pMaterial){
	m_pMeshMaterial = pMaterial;
}
void CMesh::AddInstancingBuffer(CBuffer * pBuffer){
	m_vInstancingBuffer.push_back(pBuffer);
	CreateConnectingVertexBuffers();
}
XMVECTOR CMesh::CalculateTriAngleNormal(UINT nIndex0, UINT nIndex1, UINT nIndex2)
{
	XMVECTOR xmvNormal = XMVectorSet(0.0f, 0.0f, 0.0f, 0.f);
	XMVECTOR xmvP0 = XMLoadFloat3(&m_pVertices[nIndex0]);
	XMVECTOR xmvP1 = XMLoadFloat3(&m_pVertices[nIndex1]);
	XMVECTOR xmvP2 = XMLoadFloat3(&m_pVertices[nIndex2]);
	XMVECTOR xmvEdge1 = xmvP1 - xmvP0;
	XMVECTOR xmvEdge2 = xmvP2 - xmvP0;
	xmvNormal = XMVector3Cross(xmvEdge1, xmvEdge2);

	return XMVector3Normalize(xmvNormal);
}

void CMesh::SetTriAngleListVertexNormal(XMVECTOR *pxmvNormals)
{
	XMVECTOR xmvNormal;
	XMFLOAT3 *pxmf3Positions = NULL;
	int nPrimitives = m_nVertices / 3;
	for (int i = 0; i < nPrimitives; i++)
	{
		xmvNormal = CalculateTriAngleNormal((i * 3 + 0), (i * 3 + 1), (i * 3 + 2));
		pxmf3Positions = m_pVertices + (i * 3 + 0);
		pxmvNormals[i * 3 + 0] = xmvNormal;
		pxmf3Positions = m_pVertices + (i * 3 + 1);
		pxmvNormals[i * 3 + 1] = xmvNormal;
		pxmf3Positions = m_pVertices + (i * 3 + 2);
		pxmvNormals[i * 3 + 2] = xmvNormal;
	}
}

void CMesh::SetAverageVertexNormal(XMVECTOR* pxmvNormals, int nPrimitives, int nOffset, bool bStrip)
{
	XMVECTOR xmvSumOfNormal = XMVectorSet(0.0f, 0.0f, 0.0f, 0.f);
	XMFLOAT3 *pxmf3Positions = NULL;
	UINT nIndex0, nIndex1, nIndex2;

	for (unsigned int j = 0; j < m_nVertices; j++)
	{
		xmvSumOfNormal = XMVectorSet(0.0f, 0.0f, 0.0f, 0.f);
		for (int i = 0; i < nPrimitives; i++)
		{
			nIndex0 = (bStrip) ? (((i % 2) == 0) ? (i*nOffset + 0) : (i*nOffset + 1)) : (i*nOffset + 0);
			if (m_pnIndices) nIndex0 = m_pnIndices[nIndex0];
			nIndex1 = (bStrip) ? (((i % 2) == 0) ? (i*nOffset + 1) : (i*nOffset + 0)) : (i*nOffset + 1);
			if (m_pnIndices) nIndex1 = m_pnIndices[nIndex1];
			nIndex2 = (m_pnIndices) ? m_pnIndices[i*nOffset + 2] : (i*nOffset + 2);
			if ((nIndex0 == j) || (nIndex1 == j) || (nIndex2 == j)) xmvSumOfNormal += CalculateTriAngleNormal(nIndex0, nIndex1, nIndex2);
		}
		xmvSumOfNormal = XMVector3Normalize(xmvSumOfNormal);
		pxmvNormals[j] = xmvSumOfNormal;
		pxmf3Positions = m_pVertices + j;
	}
}

void CMesh::CalculateVertexNormal(XMVECTOR *pxmvNormals)
{
	switch (m_d3dPrimitiveTopology)
	{
	case D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST:
		if (!m_pnIndices)
			SetTriAngleListVertexNormal(pxmvNormals);
		else
			SetAverageVertexNormal(pxmvNormals, (m_nIndices / 3), 3, false);
		break;
	case D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP:
		SetAverageVertexNormal(pxmvNormals, (m_pnIndices) ? (m_nIndices - 2) : (m_nVertices - 2), 1, true);
		break;
	default:
		break;
	}
}

void CMesh::CreateConnectingVertexBuffers(){
	ReleaseConnectingVertexBuffers();

	//여기서 기존에 set된 vertexbuffer랑 set된 instancing buffer랑 합친걸 만든다.
	m_nConnectingVertexBuffers = m_nVertexBuffers + m_vInstancingBuffer.size();
	if (m_nConnectingVertexBuffers <= 0) return;
	m_ppd3dConnectingVertexBuffers = new ID3D11Buffer*[m_nConnectingVertexBuffers];
	m_pnConnectingVertexStrides = new UINT[m_nConnectingVertexBuffers];
	m_pnConnectingVertexOffsets = new UINT[m_nConnectingVertexBuffers];

	UINT nBufferIndex{ 0 };
	for (int i = 0; i < m_nVertexBuffers; ++i) {
		m_ppd3dConnectingVertexBuffers[nBufferIndex] = m_ppd3dVertexBuffers[i];
		m_pnConnectingVertexStrides[nBufferIndex] = m_pnVertexStrides[i];
		m_pnConnectingVertexOffsets[nBufferIndex++] = m_pnVertexOffsets[i];
	}
	for (auto pBuffer : m_vInstancingBuffer) {
		m_ppd3dConnectingVertexBuffers[nBufferIndex] = pBuffer->GetBuffer();
		m_pnConnectingVertexStrides[nBufferIndex] = pBuffer->GetBufferStride();
		m_pnConnectingVertexOffsets[nBufferIndex++] = pBuffer->GetOffset();
	}
}

void CMesh::ReleaseConnectingVertexBuffers(){
	//connecting vertex buffer infos 
	//connecting vertex buffer의 buffer들은 release하지 않음 다른곳에서 할꺼거든
	if (m_ppd3dConnectingVertexBuffers) delete[] m_ppd3dConnectingVertexBuffers;
	m_ppd3dConnectingVertexBuffers = nullptr;
	if (m_pnConnectingVertexStrides) delete[] m_pnConnectingVertexStrides;
	m_pnConnectingVertexStrides = nullptr;
	if (m_pnConnectingVertexOffsets) delete[] m_pnConnectingVertexOffsets;
	m_pnConnectingVertexOffsets = nullptr;
}

CMesh::CMesh() : DXObject("mesh") {
	m_pMeshMaterial = RESOURCEMGR->GetMaterial("DEFAULT");
}
CMesh::~CMesh() {

}