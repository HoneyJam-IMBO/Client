
#include "stdafx.h"
#include "Updater.h"

bool CUpdater::Begin() {
	//--------------------------------------space-------------------------------------
	//space
	SetSpaceContainer(CSpaceContainer::CreateSpaceContainer(512, 2));
	//terrain
	SetTerrainContainer(CTerrainContainer::CreateTerrainContainer(L"Temp", 256, 256, 0.5, GetSpaceContainer(), true));
	//skybox
	SetSkyBoxContainer(CSkyBoxContainer::CreateSkyBoxContainer(L"Temp", 0, GetSpaceContainer()));
	//-------------------------------------space-------------------------------------
	return true;
}

bool CUpdater::End() {
	//terrain
	if (m_pTerrainContainer) {
		m_pTerrainContainer->End();
		delete m_pTerrainContainer;
	}
	//skybox
	if (m_pSkyBoxContainer) {
		m_pSkyBoxContainer->End();
		delete m_pSkyBoxContainer;
	}
	//space
	if (m_pSpaceContainer) {
		m_pSpaceContainer->End();
		delete m_pSpaceContainer;
	}
	return true;
}

void CUpdater::PreUpdate(float fTimeElapsed){
	m_pCamera->Update(fTimeElapsed);
}

void CUpdater::Update(float fTimeElapsed) {
	//-----------------------------------space------------------------------
	m_pSpaceContainer->Animate(fTimeElapsed);
	m_pSpaceContainer->PrepareRender(m_pCamera);
	m_pTerrainContainer->Update(m_pCamera);
	if (m_pSkyBoxContainer)m_pSkyBoxContainer->Update(m_pCamera, fTimeElapsed);
	//-----------------------------------space------------------------------
}

void CUpdater::PhisicsUpdate(float fTimeElapsed){

}

void CUpdater::ChangeSceneContainers() {
	if (m_pSpaceContainer)m_pSpaceContainer->ChangeSpaceData();
	if (m_pTerrainContainer) m_pTerrainContainer->ChangeSpaceData();
	//if(m_pSkyBoxContainer)m_pSkyBoxContainer->ChangeSpaceData();
}

void CUpdater::LoadSpaceInfo(){
	//space info
	float space_size = IMPORTER->ReadFloat();
	float space_lv = IMPORTER->ReadFloat();
	m_pSpaceContainer->SetSpaceSize(space_size);
	m_pSpaceContainer->SetSpaceLevel(space_lv);
	m_pSpaceContainer->ChangeSpaceData();

	//terrain onoff/ skybox onoff
	bool bTerrainContainer = IMPORTER->ReadBool();
	m_pTerrainContainer->SetActive(bTerrainContainer);
	m_pTerrainContainer->ChangeSpaceData();

	bool bSkyBoxContainer = IMPORTER->ReadBool();
	m_pSkyBoxContainer->SetActive(bSkyBoxContainer);

}

void CUpdater::SaveSpaceInfo(){
	//space info
	EXPORTER->WriteFloat(UPDATER->GetSpaceContainer()->GetSpaceSize()); EXPORTER->WriteSpace();
	EXPORTER->WriteFloat(UPDATER->GetSpaceContainer()->GetSpaceLevel());
	EXPORTER->WriteEnter();
	//terrain onoff/ skybox onoff
	bool bTerrainContainer = UPDATER->GetTerrainContainer() != nullptr;
	bool bSkyBoxContainer = UPDATER->GetSkyBoxContainer() != nullptr;
	EXPORTER->WriteBool(bTerrainContainer); EXPORTER->WriteSpace();
	EXPORTER->WriteBool(bSkyBoxContainer);
	EXPORTER->WriteEnter();

}

void CUpdater::LoadTerrainInfo(wstring wsOutputPath, wstring wsSceneName){
	if (m_pTerrainContainer->GetActive()) {//terrain container가 있다면..

		//m_pTerrainContainer->SetSceneName(wsSceneName);//이름은 나중에 바꾸도록 한다.
		//base texture path
		XMFLOAT3 xmf3Scale;
		//이렇게 되어야 함
		int width = IMPORTER->ReadInt();
		UPDATER->GetTerrainContainer()->SetTerrainWidth(width);
		int length = IMPORTER->ReadInt();
		UPDATER->GetTerrainContainer()->SetTerrainLength(length);
		float fHeightScale = IMPORTER->ReadFloat();
		UPDATER->GetTerrainContainer()->SetHeightScale(fHeightScale);

		wstring wsBaseTexturePath = IMPORTER->ReadWstring();
		UPDATER->GetTerrainContainer()->SetBaseTexture(wsBaseTexturePath);
		//height map texture name
		wstring wsHeightDataName = wsOutputPath + wsSceneName + L"HeightMap.bmp";
		UPDATER->GetTerrainContainer()->SetHeightData(IMPORTER->ReadBitmap24(wsHeightDataName.c_str()));//heightmap
		UPDATER->GetTerrainContainer()->SetHeightMapTexture(CTexture::CreateTexture(wsHeightDataName.c_str(), 1, BIND_DS));

		//normal map texture name
		wstring wsNormalDataName = wsOutputPath + wsSceneName + L"NormalMap.bmp";
		UPDATER->GetTerrainContainer()->SetNormalData(IMPORTER->ReadBitmap24(wsNormalDataName.c_str()));//nomalmap
		UPDATER->GetTerrainContainer()->SetNormalMapTexture(CTexture::CreateTexture(wsNormalDataName.c_str(), PS_SLOT_NORMALMAP, BIND_PS));

		//create splatting info
		int nSplatting = IMPORTER->ReadInt();
		UPDATER->GetTerrainContainer()->GetSplattingInfoManager()->ClearSplattingInfo();
		for (int i = 0; i<nSplatting; ++i) {
			//splatting의 detail texture는 path로 받는다.
			wstring wsDetailTexturePath = IMPORTER->ReadWstring();

			//splatting의 blending info는 [scene name]이름을 토대로 가공한다.
			//[scene name] + [splatting blending info] + [index]
			WCHAR wcBlendinfoPath[256];
			wsprintf(wcBlendinfoPath, L"%s%sBlendInfo%d.bmp", wsOutputPath.c_str(), wsSceneName.c_str(), i);

			UPDATER->GetTerrainContainer()->GetSplattingInfoManager()->CreateSplattingInfo(wsDetailTexturePath.c_str(), wcBlendinfoPath);
		}
		//create splatting info
	}
}

void CUpdater::SaveTerrainInfo(wstring wsOutputPath, wstring wsSceneName){

	if (m_pTerrainContainer->GetActive()) {//terrain container가 있다면..
		//base texture path
		float fHeightScale = UPDATER->GetTerrainContainer()->GetHeightScale();
		int width = UPDATER->GetTerrainContainer()->GetTerrainWidth();
		int length = UPDATER->GetTerrainContainer()->GetTerrainLength();
		EXPORTER->WriteInt(width); EXPORTER->WriteSpace();
		EXPORTER->WriteInt(length); EXPORTER->WriteSpace();
		EXPORTER->WriteFloat(fHeightScale); EXPORTER->WriteSpace();
		EXPORTER->WriteEnter();
		EXPORTER->WriteCHAR(UPDATER->GetTerrainContainer()->GetBaseTexture()->GetsPath().c_str());
		EXPORTER->WriteEnter();
		//height map texture name
		wstring wsHeightDataName = wsOutputPath + wsSceneName + L"HeightMap.bmp";
		EXPORTER->MakeBitmap24(wsHeightDataName.c_str(), UPDATER->GetTerrainContainer()->GetHeightData(), 256, 256);
		//normal map texture name
		wstring wsNormalDataName = wsOutputPath + wsSceneName + L"NormalMap.bmp";
		EXPORTER->MakeBitmap24(wsNormalDataName.c_str(), UPDATER->GetTerrainContainer()->GetNormalData(), 256, 256);
		//이 두개는 scene name을 가지고 가공
		//[scene name] + height map 이렇게

		int nSplatting = UPDATER->GetTerrainContainer()->GetSplattingInfoManager()->GetSplattingInfos().size();
		EXPORTER->WriteInt(nSplatting);
		EXPORTER->WriteEnter();
		for (int i = 0; i<nSplatting; ++i) {
			//splatting의 detail texture는 path로 받는다.
			wstring path = UPDATER->GetTerrainContainer()->GetSplattingInfoManager()->GetSplattingInfos()[i]->GetDetailTexturePath();
			EXPORTER->WriteWstring(path);
			EXPORTER->WriteEnter();
			//splatting의 blending info는 [scene name]이름을 토대로 가공한다.
			//[scene name] + [splatting blending info] + [index]
			WCHAR splattingName[256];
			wsprintf(splattingName, L"%s%sBlendInfo%d.bmp", wsOutputPath.c_str(), wsSceneName.c_str(), i);
			EXPORTER->MakeBitmap24(splattingName, UPDATER->GetTerrainContainer()->GetSplattingInfoManager()->GetSplattingInfos()[i]->GetBlendInfo(), 256, 256);
		}
		//splatting texture name
	}

}

CUpdater::CUpdater() :CSingleTonBase<CUpdater>("updatersingleton") {

}

CUpdater::~CUpdater() {

}
