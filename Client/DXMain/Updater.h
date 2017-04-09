#pragma once

#include "SingleTon.h"

//
#include "Camera.h"
#include "SpaceContainer.h"
#include "TerrainContainer.h"
#include "SkyBoxContainer.h"

class CDirectXFramework;

class CUpdater : public CSingleTonBase<CUpdater> {

public:
	bool Begin();
	bool End();

	//func
	void PreUpdate(float fTimeElapsed);
	void Update(float fTimeElapsed);
	void PhisicsUpdate(float fTimeElapsed);
	//func

	void SetTerrainContainer(CTerrainContainer* pTerrainContainer) { m_pTerrainContainer = pTerrainContainer; }
	CTerrainContainer* GetTerrainContainer() { return m_pTerrainContainer; }
	void SetSpaceContainer(CSpaceContainer* pSpaceContainer) { m_pSpaceContainer = pSpaceContainer; }
	CSpaceContainer* GetSpaceContainer() { return m_pSpaceContainer; }
	void SetSkyBoxContainer(CSkyBoxContainer* pSkyBoxContainer) { m_pSkyBoxContainer = pSkyBoxContainer; }
	CSkyBoxContainer* GetSkyBoxContainer() { return m_pSkyBoxContainer; }

	void SetCamera(shared_ptr<CCamera> pCamera) { m_pCamera = pCamera; }
	shared_ptr<CCamera> GetCamera() { return m_pCamera; }
	void ChangeSceneContainers();

	void LoadSpaceInfo();
	void SaveSpaceInfo();
	void LoadTerrainInfo(wstring wsOutputPath, wstring wsSceneName);
	void SaveTerrainInfo(wstring wsOutputPath, wstring wsSceneName);
private:

	shared_ptr<CCamera> m_pCamera{ nullptr };
	//space
	CSpaceContainer* m_pSpaceContainer{ nullptr };
	//space
	//terrain
	CTerrainContainer* m_pTerrainContainer{ nullptr };
	//terrain
	//skybox
	CSkyBoxContainer* m_pSkyBoxContainer{ nullptr };
	//skybox

public:
	CUpdater();
	virtual ~CUpdater();
};
