#pragma once
#include "Object.h"
#include "SpaceContainer.h"
#include "SkyBox.h"

class CSkyBoxContainer : public CObject {
public:
	void Begin(shared_ptr<CCamera> pCamera);
	bool End();

	void PrepareRender();
	void Update(float fTimeElapsed);

	void SetSkyBox(CSkyBox* pSkyBox) { m_pSkyBox = pSkyBox; }
	CSkyBox* GetSkyBox() { return m_pSkyBox; }
	//void SetDirectionalLight(CDirectionalLight* pDirectionalLight) { m_pDirectionalLight = pDirectionalLight; }
	//CDirectionalLight* GetDirectionalLight() { return m_pDirectionalLight; }

	static CSkyBoxContainer* CreateSkyBoxContainer(LPCTSTR pSkyBoxName, UINT textureIndex, CSpaceContainer* pSpaceContainer, shared_ptr<CCamera> pCamera);
	void SetSkyBoxName(wstring wsName) { m_wsSkyBoxName = wsName; }
	void CreateSkyBoxTexture(UINT index);
	void SetSpaceContainer(CSpaceContainer* pSpaceContainer) { m_pSpaceContainer = pSpaceContainer; }

	void SetActive(bool b) { m_bActive = b; }
	bool GetActive() { return m_bActive; }
private:
	bool m_bActive{ true };
	wstring m_wsSkyBoxName;
	
	CSpaceContainer* m_pSpaceContainer{ nullptr };
	//global object
	CSkyBox* m_pSkyBox{ nullptr };
	//CDirectionalLight* m_pDirectionalLight{ nullptr };
	//global object

	//skybox texture
	shared_ptr<CTexture> m_ptxtSkyBox{ nullptr };
public:
	CSkyBoxContainer();
	~CSkyBoxContainer();
};