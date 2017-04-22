#pragma once

#include "SingleTon.h"
#include "DirectXFramework.h"

#include "Animater.h"//animater!!

#include "DirectionalLightMesh.h"
#include "PointLightMesh.h"
#include "SpotLightMesh.h"
#include "CapsuleLightMesh.h"
#include "TestDeferredMesh.h"
#include "SkyBoxMesh.h"//skybox
#include "SpaceMesh.h"//space
#include "TerrainMesh.h"//terrain
#include "BoundingBoxMesh.h"//BoundingBox mesh
#include "DebugTextureMesh.h"//debug texture mesh
#include "CoordinateSysMesh.h"//CoordinateSystem Mesh
#include "FileBasedMesh.h"//fbx mesh
//#include "AnimationMesh.h"//animation mesh
//#include "MultiMesh.h"//multiMesh
#include "WaterMesh.h"

#include "Light.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "CapsuleLight.h"
#include "SkyBox.h"//skybox
#include "Space.h"//space
#include "Terrain.h"//terrain
#include "DebugTexture.h"//debug texture
#include "Material.h"

#include "Texture.h"
#include "Sampler.h"
#include "RenderShader.h"


class CResourceManager : public CSingleTonBase<CResourceManager> {

public:
	bool Begin();
	bool End();

public:
	//create resource 
	//texture array
	shared_ptr<CTexture>	CreateTexture(string name, UINT nTextures, _TCHAR(*ppstrFilePaths)[128], UINT Slot = 0, UINT BindFlag = BIND_PS, shared_ptr<CBuffer> pConstantBuffer = nullptr);
	shared_ptr<CTexture>	CreateTexture(string name, UINT nTextures, ID3D11Texture2D **ppd3dTextures, UINT Slot, UINT BindFlag, shared_ptr<CBuffer> pConstantBuffer);
	//texture 1개
	shared_ptr<CTexture>	CreateTexture(string name, _TCHAR(pstrFilePath)[128], UINT Slot = 0, UINT BindFlag = BIND_PS, shared_ptr<CBuffer> pConstantBuffer = nullptr);
	//완성된 srv set
	shared_ptr<CTexture>	CreateTexture(string name, ID3D11ShaderResourceView* pShaderResourceView, UINT Slot = 0, UINT BindFlag = BIND_PS, shared_ptr<CBuffer> pConstantBuffer = nullptr);
	shared_ptr<CSampler>	CreateSampler(string name, UINT Slot = 0, UINT BindFlags = BIND_PS, D3D11_TEXTURE_ADDRESS_MODE Mode = D3D11_TEXTURE_ADDRESS_WRAP, D3D11_FILTER Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_COMPARISON_FUNC ComparisionFunc = D3D11_COMPARISON_NEVER, float MinLOD = 0, float MaxLOD = 0, float BorderColor = 0);
	shared_ptr<CRenderShader> CreateRenderShader(string name, LPCTSTR ShaderName, UINT InputElementFlag, UINT BindFlag = BIND_VS | BIND_PS);
	shared_ptr<CBuffer>		CreateConstantBuffer(string name, UINT nObject, UINT BufferStride, UINT Slot = 0, UINT BindFlag = BIND_VS, UINT Offset = 0);
	shared_ptr<CBuffer>		CreateInstancingBuffer(string name, UINT nObject, UINT BufferStride, UINT Offset = 0);
	shared_ptr<CBuffer>		CreateGlobalBuffer(string name, UINT nObject, UINT BufferStride, UINT Slot = 0, UINT BindFlag = BIND_VS, UINT Offset = 0);
	shared_ptr<CMaterial>	CreateMaterial(string name, XMFLOAT4 color, float specExp, float specIntensity);
	
public:
	UINT CreateMultiMesh(string path, string name);
	UINT CreateGJMResource(string path, string name);
	UINT CreateFBXResource(string path, string name);
	void CreateTerrainMesh(float fOneSpaceSize, string name);
	void CreateAnimater(string path, string animaterName);

	//get resource func
	shared_ptr<CTexture>		GetTexture(string name) { return m_mTexture[name]; }
	shared_ptr<CSampler>		GetSampler(string name) { return m_mSampler[name]; }
	shared_ptr<CRenderShader>	GetRenderShader(string name) { return m_mRenderShader[name]; }
	vector<shared_ptr<CMesh>>	GetvMesh(string name) { return m_mvMesh[name]; }
	shared_ptr<CMesh>			GetMesh(string name, int index = 0) { return m_mvMesh[name][index]; }
	map<string, vector<shared_ptr<CMesh>>> GetAllStempMesh() { return m_mvStempMesh; }
	vector<shared_ptr<CMesh>>	GetvStempMesh(string name) { return m_mvStempMesh[name]; }
	shared_ptr<CMesh>			GetStempMesh(string name, int index) { return m_mvStempMesh[name][index]; }
	shared_ptr<CBuffer>			GetBuffer(string name) { return m_mBuffer[name]; }
	shared_ptr<CBuffer>			GetGlobalBuffer(string name) { return m_mGlobalBuffer[name]; }
	shared_ptr<CMaterial>		GetMaterial(string name) { return m_mMaterial[name]; }
	shared_ptr<CAnimater>		GetAnimater(string name) { return m_mAnimater[name]; }

private:
	//begin func
	void CreateSamplers();
	void CreateTextures();
	void CreateRenderShaders();
	void CreateMeshs();
	void CreateStempMeshs();
	void CreateBuffers();
	void CreateGlobalBuffers();
	void CreateMaterials();
	void CreateAnimaters();
	void CreateGJMResources();
	void CreateFBXResources();


public:
	void ReleaseAllResource();

	void ReleaseMesh(string name);
	void ReleaseStempMesh(string name);
	void ReleaseAnimater(string name);
private:
	//end func
	void ReleaseSamplers();
	void ReleaseTextures();
	void ReleaseRenderShaders();
	void ReleaseMeshs();
	void ReleaseStempMeshs();
	void ReleaseBuffers();
	void ReleaseGlobalBuffers();
	void ReleaseMaterials();
	void ReleaseAnimaters();

private:
	using pairTexture	= pair<string, shared_ptr<CTexture>>;
	using pairShader	= pair<string, shared_ptr<CRenderShader>>;
	using pairMesh		= pair<string, shared_ptr<CMesh>>;
	using pairBuffer	= pair<string, shared_ptr<CBuffer>>;
	using pairMaterial	= pair<string, shared_ptr<CMaterial>>;
	using pairSampler	= pair<string, shared_ptr<CSampler>>;
	using pairAnimater	= pair<string, shared_ptr<CAnimater>>;

	map<string, shared_ptr<CTexture>>		m_mTexture;
	map<string, shared_ptr<CRenderShader>>	m_mRenderShader;
	map<string, vector<shared_ptr<CMesh>>>	m_mvMesh;
	map<string, vector<shared_ptr<CMesh>>>	m_mvStempMesh;
	map<string, shared_ptr<CBuffer>>		m_mBuffer;
	map<string, shared_ptr<CBuffer>>		m_mGlobalBuffer;
	map<string, shared_ptr<CMaterial>>		m_mMaterial;
	map<string, shared_ptr<CSampler>>		m_mSampler;
	map<string, shared_ptr<CAnimater>>		m_mAnimater;

public:
	CResourceManager();
	virtual ~CResourceManager();
};
