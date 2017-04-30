#pragma once
#include "DXObject.h"
#include "GameObject.h"

//dxobject
#include "Mesh.h"
#include "RenderShader.h"
#include "Texture.h"
#include "Buffer.h"
#include "Material.h"
#include "Animater.h"//animater!!
//dxobject

#include "Camera.h"

class CRenderContainer : public DXObject {
public:
	//---------------------------dxobject---------------------------------
	virtual bool Begin();
	virtual bool End();

	virtual void SetShaderState();
	virtual void CleanShaderState();

	virtual void UpdateShaderState(shared_ptr<CCamera> pCamera);
	//---------------------------dxobject---------------------------------

	//--------------------------container---------------------------------
	void ClearVolatileResources();
private:
	virtual void RenderExcute();
	void RenderExcuteWithOutObject();
public:
	void Render(shared_ptr<CCamera> pCamera);
	void RenderWithOutObject(shared_ptr<CCamera> pCamera);

	shared_ptr<CMesh> GetMesh(UINT index = 0) { return m_vpMesh[index]; }
	vector<shared_ptr<CMesh>>& GetvMesh() { return m_vpMesh; }
	//set이 아니라이제 add다.
	void ClearMesh();
	void ClearBuffer();
	void ClearAnimater();
	void AddMesh(shared_ptr<CMesh> pMesh);
	void SetMesh(shared_ptr<CMesh> pMesh);
	void SetShader(shared_ptr<CRenderShader> pShader);
	
	//void AddMesh(shared_ptr<CMesh> pMesh);
	//void AddShader(shared_ptr<CRenderShader> pShader);
	void AddTexture(shared_ptr<CTexture> pTexture);
	//휘발성 texture를 add
	void AddVolatileTexture(shared_ptr<CTexture> pTexture);
	void AddBuffer(shared_ptr<CBuffer> pBuffers);
	//휘발성 buf를 추가합
	void AddVolatileBuffer(shared_ptr<CBuffer> pBuffers);
	void AddInstanceBuffer(shared_ptr<CBuffer> pBuffers);
	void AddMaterial(shared_ptr<CMaterial> pMaterial);
	
	//animater!
	void SetAnimater(shared_ptr<CAnimater> pAnimater) { m_pAnimater = pAnimater; }
	shared_ptr<CAnimater> GetAnimater() { return m_pAnimater; }
	//animater!

	void SetObejcts(int n, CGameObject** ppObjects);
	void AddObject(CGameObject* pObject);
	//void RemoveObject(CGameObject* pObject);
	void ClearObjectList() { m_lpObjects.clear(); }//그림 다 그리고 clear
	vector<CGameObject*>& GetObjectList() { return m_lpObjects; }

	void ClearTextures() { m_vpTexture.clear(); }
	//--------------------------container---------------------------------
protected:
	//--------------------------관리 대상------------------------------
	//컨테이너의 객체들은 동적으로 변할 것이다. 그러므로 리스트로 관리하는 것이 맞다
//	int m_nInstance{ 0 };
	vector<CGameObject*> m_lpObjects;
	

	vector<shared_ptr<CMesh>>		m_vpMesh;
	shared_ptr<CRenderShader>		m_pShader;
	vector<shared_ptr<CTexture>>	m_vpTexture;
	//휘발성 texture 한번 랜더하면 사라진다. 
	vector<shared_ptr<CTexture>>	m_vpVolatileTexture;
	vector<shared_ptr<CBuffer>>		m_vpVolatileBuffer;

	vector<shared_ptr<CBuffer>>		m_vpBuffer;
	vector<shared_ptr<CMaterial>>	m_vpMaterial;
	shared_ptr<CAnimater>			m_pAnimater{ nullptr };//animater

	int m_nBuffer{ 0 };
	int m_nMaterial{ 0 };
	//--------------------------관리 대상------------------------------

	void** m_ppBufferData{ nullptr };
public:
	CRenderContainer();
	virtual ~CRenderContainer();

};

//typedef CAtlMap<string, CRenderContainer*> mapRenderContainer;
//typedef CAtlMap<tag, mapRenderContainer> mapTagRenderContainer;
typedef map<string, CRenderContainer*> mapRenderContainer;
typedef map<tag, mapRenderContainer> mapTagRenderContainer;
//typedef pair<string, CRenderContainer*> pairRenderContainer;
//typedef pair<tag, pairRenderContainer> pairTagRenderContainer;
