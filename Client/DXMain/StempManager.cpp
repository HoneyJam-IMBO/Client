#include "stdafx.h"
#include "StempManager.h"

void CStempManager::Begin(){
	//葛电 stemp力累
	vector<wstring> vFile;
	DIRECTORYFINDER->GetFiles(vFile, L"../../Assets/Stemp", true, true, L".bmp");
	DIRECTORYFINDER->GetFiles(vFile, L"../../Assets/Stemp", true, true, L".BMP");
	
	//const char* groupName = "File";
	for (auto data : vFile) {
		CreateStemp(data);
	}
	//葛电 stemp力累

	m_pPicposRenderInfoBuffer = CBuffer::CreateConstantBuffer(1, sizeof(TERRAIN_PICPOS_RENDER_INFO), 4, BIND_PS);
	m_pPicposRenderInfo = new TERRAIN_PICPOS_RENDER_INFO;
	m_pPicposRenderInfo->Extent = 50.f / m_fSpaceSize;

	TWBARMGR->AddMinMaxBarRW("TOOL_MODE", "StempControll", "STEMP_EXTENT", &m_fExtent, 10.f, 256.f, 0.5f);
	TWBARMGR->AddMinMaxBarRW("TOOL_MODE", "StempControll", "STEMP_INDEX", &m_nCurStemp, 0.f, m_vStemp.size()-1, 1.f);
}

bool CStempManager::End(){
	//TWBARMGR->DeleteVar("TOOL_MODE", "STEMP_EXTENT");
	//TWBARMGR->DeleteVar("TOOL_MODE", "STEMP_INDEX");

	if (m_pPicposRenderInfo) delete m_pPicposRenderInfo;
	m_pPicposRenderInfo = nullptr;
	if (m_pPicposRenderInfoBuffer) m_pPicposRenderInfoBuffer->End();
	m_pPicposRenderInfoBuffer = nullptr;

	for (auto pStemp : m_vStemp) {
		delete pStemp;
	}
	m_vStemp.clear();

	return true;
}

void CStempManager::SetShaderState(){
	RENDERER->GetTerrainRenderContainer()->AddVolatileBuffer(m_pPicposRenderInfoBuffer);
	m_vStemp[m_nCurStemp]->SetShaderState();
}

void CStempManager::CleanShaderState(){
}

void CStempManager::UpdateShaderState(){
	TERRAIN_PICPOS_RENDER_INFO* pPicposRenderInfo = (TERRAIN_PICPOS_RENDER_INFO*)m_pPicposRenderInfoBuffer->Map();
	pPicposRenderInfo->PickPos.x = (m_pPicposRenderInfo->PickPos.x);
	pPicposRenderInfo->PickPos.y = 1 - (m_pPicposRenderInfo->PickPos.y);
	pPicposRenderInfo->Extent = m_pPicposRenderInfo->Extent = m_fExtent / m_fSpaceSize;
	pPicposRenderInfo->ToolMode = (UINT)GLOBALVALUEMGR->GetToolMode();
	m_pPicposRenderInfoBuffer->Unmap();
}

void CStempManager::IncreaseTerrain(Pixel24 * pHeightMapData){
	m_vStemp[m_nCurStemp]->IncreaseTerrain(pHeightMapData, m_pPicposRenderInfo->PickPos, m_pPicposRenderInfo->Extent);
}

void CStempManager::DecreaseTerrain(Pixel24* pHeightMapData){
	m_vStemp[m_nCurStemp]->DecreaseTerrain(pHeightMapData, m_pPicposRenderInfo->PickPos, m_pPicposRenderInfo->Extent);
}

void CStempManager::SetTerrain(Pixel24 * pHeightMapData){
	m_vStemp[m_nCurStemp]->SetTerrain(pHeightMapData, m_pPicposRenderInfo->PickPos, m_pPicposRenderInfo->Extent);
}

void CStempManager::SetCurStempIndex(int index){
	if (index >= m_vStemp.size()) return;
	m_nCurStemp = index;
}

void CStempManager::CreateStemp(wstring name){
	CStemp* pStemp = CStemp::CreateStemp(name, this);
	m_vStemp.push_back(pStemp);
}

CStempManager * CStempManager::CreateStempManager(float fSpaceSize, CTerrainContainer* pTerrainContainer){
	CStempManager* pStempManager = new CStempManager;
	pStempManager->SetSpaceSize(fSpaceSize);
	pStempManager->SetTerrainContainer(pTerrainContainer);
	pStempManager->Begin();
	return pStempManager;
}

CStempManager::CStempManager() : DXObject("stempmanager"){

}

CStempManager::~CStempManager(){

}
