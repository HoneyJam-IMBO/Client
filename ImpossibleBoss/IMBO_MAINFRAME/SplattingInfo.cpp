#include "stdafx.h"
#include "SplattingInfo.h"

void CSplattingInfo::Begin(){
	
}

bool CSplattingInfo::End(){
	m_vLoadFileStruct.RemoveAll();
	if (m_pBlendInfo) delete[] m_pBlendInfo; 
	m_pBlendInfo = nullptr;
	return true;
}

void CSplattingInfo::SetShaderState(){
	
}

void CSplattingInfo::CleanShaderState(){
	
}

void CSplattingInfo::UpdateShaderState(){
	//height map data init
	//EXPORTER->MakeBitmap24(m_pBlendInfoTexturePath.c_str() , m_pBlendInfo, BLEND_WIDTH, BLEND_HEIGHT);
}

CSplattingInfo * CSplattingInfo::CreateSplattingInfo(CSplattingInfoManager* pSplattingManager, const WCHAR * pDetailTextureName){
	//CSplattingInfo* pSplattingInfo = new CSplattingInfo;

	//pSplattingInfo->SetIndex(pSplattingManager->GetSplattingInfos().size());
	//pSplattingInfo->SetSplattingManager(pSplattingManager);
	//pSplattingInfo->SetDetailTextuePath(pDetailTextureName);

	//WCHAR pathBlendInfo[256];
	//wsprintf(pathBlendInfo, L"../outputdata/SplattingBlendInfo/BlendInfo%d.bmp", pSplattingInfo->GetIndex());
	//pSplattingInfo->SetBlendInfoTextuePath(pathBlendInfo);

	////blend data init
	//Pixel24* pBlendInfo = new Pixel24[BLEND_DATA_NUM];
	//ZeroMemory(pBlendInfo, sizeof(Pixel24) * BLEND_DATA_NUM);
	////blend data init

	//EXPORTER->MakeBitmap24(pathBlendInfo, pBlendInfo, BLEND_WIDTH, BLEND_HEIGHT);
	//
	//pSplattingInfo->SetBlendInfo(pBlendInfo);
	//pSplattingInfo->Begin();
	//return pSplattingInfo;
	return nullptr;
}

CSplattingInfo * CSplattingInfo::CreateSplattingInfo(CSplattingInfoManager * pSplattingManager, const WCHAR * pDetailTextureName, const WCHAR * pBlendInfoTextureName){
	CSplattingInfo* pSplattingInfo = new CSplattingInfo;

	pSplattingInfo->SetIndex(pSplattingManager->GetSplattingInfos().GetCount());
	pSplattingInfo->SetSplattingManager(pSplattingManager);
	pSplattingInfo->SetDetailTextuePath(pDetailTextureName);
	pSplattingInfo->SetBlendInfoTextuePath(pBlendInfoTextureName);

	//blend data init
	Pixel24* pBlendInfo = IMPORTER->ReadBitmap24(pBlendInfoTextureName);
	//blend data init

	pSplattingInfo->SetBlendInfo(pBlendInfo);
	pSplattingInfo->Begin();
	return pSplattingInfo;
}

void CSplattingInfo::SetIndex(UINT index){
	m_nIndex = index; 
}

CSplattingInfo::CSplattingInfo() : DXObject("splattinginfo")
{
}

CSplattingInfo::~CSplattingInfo()
{
}
