#include "stdafx.h"
#include "Scene.h"
#include "DirectXFramework.h"

bool CScene::Begin(string path){


	return true;
}

bool CScene::End(){
	
	return true;
}

void CScene::Animate(float fTimeElapsed){

}

CScene * CScene::CreateScene(string name, CDirectXFramework* pFramework){
	/*CScene* pScene = new CScene(name);
	pScene->LoadScene(name);
	pScene->SetFrameWork(pFramework);
	return pScene;*/
	return nullptr;
}

void CScene::LoadScene(string path){
	IMPORTER->Begin(path);
	//output path
	wstring wsOutputPath = IMPORTER->ReadWstring();
	//scene name
	wstring wsSceneName = IMPORTER->ReadWstring();
	m_name.assign(wsSceneName.cbegin(), wsSceneName.cend());

	UPDATER->LoadSpaceInfo();
	UPDATER->LoadTerrainInfo(wsOutputPath, wsSceneName);

	//effect info
	RENDERER->LoadEffectInfo();

	//object info save
	//m_pSpaceContainer->WriteObjects();
	//EXPORTER->WriteEnter();

	IMPORTER->End();
}
