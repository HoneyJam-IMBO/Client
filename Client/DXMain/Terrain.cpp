#include "stdafx.h"
#include "Terrain.h"
#include "TerrainContainer.h"

bool CTerrain::Begin() {
	//object_id set
//	m_objectID = object_id::OBJECT_TERRAIN;

	if (m_pTerrainContainer->GetIsTool()) {
		CGameObject::Begin();
		return true;
	}
	else {
		XMStoreFloat4x4(&m_xmf4x4World, XMMatrixIdentity());
		XMStoreFloat4(&m_xmf4Quaternion, XMQuaternionIdentity());
		m_pRenderContainer = RCSELLER->GetRenderContainer("terrain");

		float fx = static_cast<float>(m_pTerrainContainer->GetSpaceContainer()->GetOneSpaceSize());
		float fy = static_cast<float>(m_pTerrainContainer->GetSpaceContainer()->GetSpaceSize());
		float fz = static_cast<float>(m_pTerrainContainer->GetSpaceContainer()->GetOneSpaceSize());
		BoundingBox::CreateFromPoints(m_OriBoundingBox, XMVectorSet(0.f, 0.f, 0.f, 0.f), XMVectorSet(fx, fy, fz, 0.f));

		return true;
	}
}
bool CTerrain::End() {

	return CGameObject::End();
}

void CTerrain::SetTerrainContainer(CTerrainContainer * pTerrainContainer){
	m_pTerrainContainer = pTerrainContainer;
}

//void CTerrain::RegistToContainer(){
//	m_bRender = true;
//}
//
//void CTerrain::RegistToContainer2(){
//	if(m_bRender){
//		CGameObject::RegistToContainer();
//	}
//	m_bRender = false;
//}

bool CTerrain::CheckPickObject(XMVECTOR xmvWorldCameraStartPos, XMVECTOR xmvRayDir, float & distance){
	BoundingBox BoundingBox;
	GetMainBoundingBox(BoundingBox);
	bool b = BoundingBox.Intersects(xmvWorldCameraStartPos, xmvRayDir, distance);
	return b;
}

CTerrain* CTerrain::CreateTerrain(CTerrainContainer * pTerrainContainer, int x, int y){
	CTerrain* pTerrain = new CTerrain;
	pTerrain->SetTerrainContainer(pTerrainContainer);
	pTerrain->Begin();
	pTerrain->SetPosition(XMVectorSet(x*pTerrainContainer->GetSpaceContainer()->GetOneSpaceSize(), 
		0.f, y*pTerrainContainer->GetSpaceContainer()->GetOneSpaceSize(), 0.f));
	pTerrainContainer->GetSpaceContainer()->AddObject(pTerrain);

	return pTerrain;
}

CTerrain::CTerrain() : CGameObject("terrain", tag::TAG_TERRAIN) {

}
CTerrain::~CTerrain() {

}