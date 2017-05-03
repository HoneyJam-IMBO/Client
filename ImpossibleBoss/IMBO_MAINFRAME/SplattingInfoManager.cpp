#include "stdafx.h"
#include "SplattingInfoManager.h"

void CSplattingInfoManager::Begin(){
	m_pSplattingInfoBuffer = CBuffer::CreateConstantBuffer(1, sizeof(SPLATTING_INFO), 5, BIND_PS);
}

bool CSplattingInfoManager::End(){
	if (m_pDetailTextures)m_pDetailTextures->End();
	m_pDetailTextures = nullptr;
	if (m_pBlendInfoTextures)m_pBlendInfoTextures->End();
	m_pBlendInfoTextures = nullptr;
	if (m_pSplattingInfoBuffer) m_pSplattingInfoBuffer->End();
	m_pSplattingInfoBuffer = nullptr;
	
	ClearSplattingInfo();

	return true;
}

void CSplattingInfoManager::SetShaderState(){
	
	if(m_pDetailTextures)RENDERER->GetTerrainRenderContainer()->AddVolatileTexture(m_pDetailTextures);
	if(m_pBlendInfoTextures)RENDERER->GetTerrainRenderContainer()->AddVolatileTexture(m_pBlendInfoTextures);
	if (m_pSplattingInfoBuffer) RENDERER->GetTerrainRenderContainer()->AddVolatileBuffer(m_pSplattingInfoBuffer);
}

void CSplattingInfoManager::CleanShaderState(){
	
}

void CSplattingInfoManager::UpdateShaderState(){
	
}

void CSplattingInfoManager::RemoveSplattingInfoByIndex(UINT index){
	if (m_vSplattinfInfo.IsEmpty())return;

	//remove
	//vector<CSplattingInfo*>::iterator iter;
	size_t iVecSplattingInfoSize = m_vSplattinfInfo.GetCount();
	size_t i = 0;
	for (; i < iVecSplattingInfoSize; ++i)
	{
		m_vSplattinfInfo[i]->End();
		delete m_vSplattinfInfo[i];
		break;
	}
	/*for (iter = m_vSplattinfInfo.begin(); iter != m_vSplattinfInfo.end(); ++iter) {
		if ((*iter)->GetIndex() == index) {
			(*iter)->End();
			delete (*iter);
			break;
		}
	}*/
	//if (iter == m_vSplattinfInfo.end()) return;
	if (i == iVecSplattingInfoSize) return;
	//m_vSplattinfInfo.erase(iter);

	//for 전부 돌면서 index 새로 set
	for (size_t i = 0; i < m_vSplattinfInfo.GetCount(); ++i) {
		m_vSplattinfInfo[i]->SetIndex(i);
		m_vSplattinfInfo[i]->UpdateShaderState();
	}
	m_nCurIndex = 0;
}

void CSplattingInfoManager::ClearSplattingInfo(){

	size_t iVecSplattingInfoSize = m_vSplattinfInfo.GetCount();
	for (size_t i = 0; i < iVecSplattingInfoSize; ++i)
	{
		m_vSplattinfInfo[i]->CleanShaderState();
		delete m_vSplattinfInfo[i];
	}
	m_vSplattinfInfo.RemoveAll();
	//for (auto pData : m_vSplattinfInfo) {
	//	pData->CleanShaderState();
	//	delete pData;
	//}
	//m_vSplattinfInfo.clear();

	if (m_pSplattingInfoBuffer) {
		SPLATTING_INFO* pData = (SPLATTING_INFO*)m_pSplattingInfoBuffer->Map();
		pData->nSplattingInfo = m_vSplattinfInfo.GetCount();
		m_pSplattingInfoBuffer->Unmap();
	}
}

void CSplattingInfoManager::CreateSplattingInfo(const WCHAR * pDetailTextureName){
	if (m_vSplattinfInfo.GetCount() >= 10) return;
	CSplattingInfo* pSplattingInfo = CSplattingInfo::CreateSplattingInfo(this, pDetailTextureName);
	//m_vSplattinfInfo.push_back(pSplattingInfo);
	m_vSplattinfInfo.Add(pSplattingInfo);

	if (m_pDetailTextures)m_pDetailTextures->End();
	m_pDetailTextures = nullptr;
	if (m_pBlendInfoTextures)m_pBlendInfoTextures->End();
	m_pBlendInfoTextures = nullptr;

	//detail/ blending info texture제작
	_TCHAR ppstrBlendInfoTextureNames[MAX_SPLATTINGINFO_NUM][128];
	_TCHAR ppstrDetailTextureNames[MAX_SPLATTINGINFO_NUM][128];
	UINT index{ 0 };

	size_t iVecSize = m_vSplattinfInfo.GetCount();
	for(size_t i = 0; i < iVecSize; ++i)
	{
	//for (auto pSplattingInfo : m_vSplattinfInfo) {
		wsprintf(ppstrBlendInfoTextureNames[index], m_vSplattinfInfo[i]/*pSplattingInfo*/->GetBlendInfoTexturePath());
		wsprintf(ppstrDetailTextureNames[index++], m_vSplattinfInfo[i]/*pSplattingInfo*/->GetDetailTexturePath());
	}
	m_pBlendInfoTextures = CTexture::CreateTexture(index, ppstrBlendInfoTextureNames, 2, BIND_PS);
	m_pDetailTextures = CTexture::CreateTexture(index, ppstrDetailTextureNames, 1, BIND_PS);;

	SPLATTING_INFO* pData = (SPLATTING_INFO*)m_pSplattingInfoBuffer->Map();
	pData->nSplattingInfo = m_vSplattinfInfo.GetCount();
	m_pSplattingInfoBuffer->Unmap();
}
void CSplattingInfoManager::CreateSplattingInfo(const WCHAR * pDetailTextureName, const WCHAR * pBlendInfoTextureName) {
	if (m_vSplattinfInfo.GetCount() >= 10) return;
	CSplattingInfo* pSplattingInfo = CSplattingInfo::CreateSplattingInfo(this, pDetailTextureName, pBlendInfoTextureName);
	m_vSplattinfInfo.Add(pSplattingInfo);
	
	if (m_pDetailTextures)m_pDetailTextures->End();
	m_pDetailTextures = nullptr;
	if (m_pBlendInfoTextures)m_pBlendInfoTextures->End();
	m_pBlendInfoTextures = nullptr;

	//detail/ blending info texture제작
	_TCHAR ppstrBlendInfoTextureNames[MAX_SPLATTINGINFO_NUM][128];
	_TCHAR ppstrDetailTextureNames[MAX_SPLATTINGINFO_NUM][128];
	UINT index{ 0 };
	size_t iVecSize = m_vSplattinfInfo.GetCount();
	for (size_t i = 0; i < iVecSize; ++i)
	{
		wsprintf(ppstrBlendInfoTextureNames[index], m_vSplattinfInfo[i]->GetBlendInfoTexturePath());
		wsprintf(ppstrDetailTextureNames[index++], m_vSplattinfInfo[i]->GetDetailTexturePath());
	}
	m_pBlendInfoTextures = CTexture::CreateTexture(index, ppstrBlendInfoTextureNames, 2, BIND_PS);
	m_pDetailTextures = CTexture::CreateTexture(index, ppstrDetailTextureNames, 1, BIND_PS);;

	SPLATTING_INFO* pData = (SPLATTING_INFO*)m_pSplattingInfoBuffer->Map();
	pData->nSplattingInfo = m_vSplattinfInfo.GetCount();
	m_pSplattingInfoBuffer->Unmap();
}

CSplattingInfoManager * CSplattingInfoManager::CreateSplattingInfoManager(CTerrainContainer * pTerrainContainer){
	CSplattingInfoManager* pSplattingInfoManager = new CSplattingInfoManager;
	pSplattingInfoManager->SetTerrainContainer(pTerrainContainer);
	pSplattingInfoManager->Begin();
	return pSplattingInfoManager;
}

CSplattingInfoManager::CSplattingInfoManager()
{
}

CSplattingInfoManager::~CSplattingInfoManager()
{
}
