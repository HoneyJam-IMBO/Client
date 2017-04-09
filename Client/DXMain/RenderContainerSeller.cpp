#include "stdafx.h"
#include "RenderContainerSeller.h"


bool CRenderContainerSeller::Begin(){
	string object_name;
	//test
	tag t = tag::TAG_DYNAMIC_OBJECT;
	object_name = "test";
	m_mTagRenderContainer[t][object_name] = new CRenderContainer();
	m_mTagRenderContainer[t][object_name]->SetShader(RESOURCEMGR->GetRenderShader("DEFAULT"));
	m_mTagRenderContainer[t][object_name]->SetMesh(RESOURCEMGR->GetMesh("Rect5", 0));
	m_mTagRenderContainer[t][object_name]->AddMaterial(RESOURCEMGR->GetMaterial("Terrain"));
	m_mTagRenderContainer[t][object_name]->AddInstanceBuffer(RESOURCEMGR->GetBuffer("DEFAULTIB"));
	m_mTagRenderContainer[t][object_name]->Begin();
	//test

	object_name = "fbx";
	m_mTagRenderContainer[t][object_name] = new CRenderContainer();
	m_mTagRenderContainer[t][object_name]->AddMesh(RESOURCEMGR->GetMesh("Rect1", 0));
#ifdef USE_ANIM
	m_mTagRenderContainer[t][object_name]->SetShader(RESOURCEMGR->GetRenderShader("AnimationObject"));
#else
	m_mTagRenderContainer[t][object_name]->SetShader(RESOURCEMGR->GetRenderShader("Core"));
#endif
	m_mTagRenderContainer[t][object_name]->AddBuffer(RESOURCEMGR->GetBuffer("FBX"));
	m_mTagRenderContainer[t][object_name]->AddMaterial(RESOURCEMGR->GetMaterial("FBX"));
	m_mTagRenderContainer[t][object_name]->Begin();


	object_name = "plane";
	m_mTagRenderContainer[t][object_name] = new CRenderContainer();
	m_mTagRenderContainer[t][object_name]->SetMesh(RESOURCEMGR->GetMesh("Plane", 0));//mesh set과 동시에 instancing buffer 생성 및 set
	m_mTagRenderContainer[t][object_name]->SetShader(RESOURCEMGR->GetRenderShader("DEFAULT"));
	m_mTagRenderContainer[t][object_name]->AddMaterial(RESOURCEMGR->GetMaterial("Plane"));
	m_mTagRenderContainer[t][object_name]->AddInstanceBuffer(RESOURCEMGR->GetBuffer("ONEIB"));
	m_mTagRenderContainer[t][object_name]->Begin();
		
	t = tag::TAG_LIGHT;
	object_name = "directionallight";
	m_mTagRenderContainer[t][object_name] = new CRenderContainer();
	m_mTagRenderContainer[t][object_name]->SetMesh(RESOURCEMGR->GetMesh("DirectionalLight", 0));//mesh set과 동시에 instancing buffer 생성 및 se
	m_mTagRenderContainer[t][object_name]->SetShader(RESOURCEMGR->GetRenderShader("DirectionalLight"));
	m_mTagRenderContainer[t][object_name]->AddBuffer(RESOURCEMGR->GetBuffer("DirectionalLightCB"));
	m_mTagRenderContainer[t][object_name]->Begin();
	
	object_name = "pointlight";
	//point light
	m_mTagRenderContainer[t][object_name] = new CRenderContainer();
	m_mTagRenderContainer[t][object_name]->SetMesh(RESOURCEMGR->GetMesh("PointLight", 0));
	m_mTagRenderContainer[t][object_name]->SetShader(RESOURCEMGR->GetRenderShader("PointLight"));
	m_mTagRenderContainer[t][object_name]->AddBuffer(RESOURCEMGR->GetBuffer("PointLightCB1"));
	m_mTagRenderContainer[t][object_name]->AddBuffer(RESOURCEMGR->GetBuffer("PointLightCB2"));
	m_mTagRenderContainer[t][object_name]->Begin();
		
	object_name = "spotlight";
	//spot light
	m_mTagRenderContainer[t][object_name] = new CRenderContainer();
	m_mTagRenderContainer[t][object_name]->SetMesh(RESOURCEMGR->GetMesh("SpotLight", 0));
	m_mTagRenderContainer[t][object_name]->SetShader(RESOURCEMGR->GetRenderShader("SpotLight"));
	m_mTagRenderContainer[t][object_name]->AddBuffer(RESOURCEMGR->GetBuffer("SpotLightCB1"));
	m_mTagRenderContainer[t][object_name]->AddBuffer(RESOURCEMGR->GetBuffer("SpotLightCB2"));
	m_mTagRenderContainer[t][object_name]->Begin();
	
	object_name = "capsulelight";
	//point light
	m_mTagRenderContainer[t][object_name] = new CRenderContainer();
	m_mTagRenderContainer[t][object_name]->SetMesh(RESOURCEMGR->GetMesh("CapsuleLight", 0));
	m_mTagRenderContainer[t][object_name]->SetShader(RESOURCEMGR->GetRenderShader("CapsuleLight"));
	m_mTagRenderContainer[t][object_name]->AddBuffer(RESOURCEMGR->GetBuffer("CapsuleLightCB1"));
	m_mTagRenderContainer[t][object_name]->AddBuffer(RESOURCEMGR->GetBuffer("CapsuleLightCB2"));
	m_mTagRenderContainer[t][object_name]->Begin();
	
	t = tag::TAG_POSTPROCESSING;
	object_name = "postprocessing";
	//자기 shader set
	m_mTagRenderContainer[t][object_name] = new CRenderContainer();
	m_mTagRenderContainer[t][object_name]->SetShader(RESOURCEMGR->GetRenderShader("PostProcessing"));
	m_mTagRenderContainer[t][object_name]->SetMesh(RESOURCEMGR->GetMesh("DirectionalLight", 0));
	m_mTagRenderContainer[t][object_name]->Begin();
	
	t = tag::TAG_SPACE;
	object_name = "skybox";
	m_mTagRenderContainer[t][object_name] = new CRenderContainer();
	m_mTagRenderContainer[t][object_name]->SetMesh(RESOURCEMGR->GetMesh("SkyBox", 0));
	m_mTagRenderContainer[t][object_name]->SetShader(RESOURCEMGR->GetRenderShader("SkyBox"));
	m_mTagRenderContainer[t][object_name]->AddMaterial(RESOURCEMGR->GetMaterial("SkyBox"));
	m_mTagRenderContainer[t][object_name]->AddInstanceBuffer(RESOURCEMGR->GetBuffer("ONEIB"));
	m_mTagRenderContainer[t][object_name]->Begin();
	
	object_name = "space";
	m_mTagRenderContainer[t][object_name] = new CRenderContainer();
	m_mTagRenderContainer[t][object_name]->SetMesh(RESOURCEMGR->GetMesh("Space", 0));//mesh set과 동시에 instancing buffer 생성 및 set
	m_mTagRenderContainer[t][object_name]->SetShader(RESOURCEMGR->GetRenderShader("Core"));
	m_mTagRenderContainer[t][object_name]->AddMaterial(RESOURCEMGR->GetMaterial("DEFAULT"));
	m_mTagRenderContainer[t][object_name]->AddInstanceBuffer(RESOURCEMGR->GetBuffer("SpaceIB"));
	m_mTagRenderContainer[t][object_name]->Begin();
		
	t = tag::TAG_TERRAIN;
	object_name = "terrain";
	m_mTagRenderContainer[t][object_name] = new CRenderContainer();
	m_mTagRenderContainer[t][object_name]->SetShader(RESOURCEMGR->GetRenderShader("Terrain"));
	m_mTagRenderContainer[t][object_name]->AddMaterial(RESOURCEMGR->GetMaterial("Terrain"));
	m_mTagRenderContainer[t][object_name]->Begin();
	
	t = tag::TAG_SSLR;
	object_name = "laytrace";
	m_mTagRenderContainer[t][object_name] = new CRenderContainer();
	m_mTagRenderContainer[t][object_name]->SetMesh(RESOURCEMGR->GetMesh("DirectionalLight", 0));//mesh set과 동시에 instancing buffer 생성 및 se
	m_mTagRenderContainer[t][object_name]->SetShader(RESOURCEMGR->GetRenderShader("RayTrace"));
	m_mTagRenderContainer[t][object_name]->Begin();
	
	object_name = "combine";
	m_mTagRenderContainer[t][object_name] = new CRenderContainer();
	m_mTagRenderContainer[t][object_name]->SetMesh(RESOURCEMGR->GetMesh("DirectionalLight", 0));//mesh set과 동시에 instancing buffer 생성 및 se
	m_mTagRenderContainer[t][object_name]->SetShader(RESOURCEMGR->GetRenderShader("Combine"));
	m_mTagRenderContainer[t][object_name]->Begin();
	
	t = tag::TAG_DEBUG;
	object_name = "aabb";
	m_mTagRenderContainer[t][object_name] = new CDebugRenderContainer();
	m_mTagRenderContainer[t][object_name]->SetMesh(RESOURCEMGR->GetMesh("BoundingBox", 0));
	m_mTagRenderContainer[t][object_name]->SetShader(RESOURCEMGR->GetRenderShader("BoundingBox"));
	m_mTagRenderContainer[t][object_name]->AddMaterial(RESOURCEMGR->GetMaterial("BoundingBox"));
	m_mTagRenderContainer[t][object_name]->AddInstanceBuffer(RESOURCEMGR->GetBuffer("BoundingBoxIB"));
	m_mTagRenderContainer[t][object_name]->Begin();

	object_name = "coordinatesys";
	m_mTagRenderContainer[t][object_name] = new CDebugRenderContainer();
	m_mTagRenderContainer[t][object_name]->SetMesh(RESOURCEMGR->GetMesh("CoordinateSys", 0));
	m_mTagRenderContainer[t][object_name]->SetShader(RESOURCEMGR->GetRenderShader("CoordinateSys"));
	m_mTagRenderContainer[t][object_name]->AddMaterial(RESOURCEMGR->GetMaterial("AABB"));
	m_mTagRenderContainer[t][object_name]->AddInstanceBuffer(RESOURCEMGR->GetBuffer("CoordinateSysIB"));
	m_mTagRenderContainer[t][object_name]->Begin();

	object_name = "debugpointlight";
	//point light
	m_mTagRenderContainer[t][object_name] = new CDebugRenderContainer();
	m_mTagRenderContainer[t][object_name]->SetMesh(RESOURCEMGR->GetMesh("PointLight", 0));
	m_mTagRenderContainer[t][object_name]->SetShader(RESOURCEMGR->GetRenderShader("DebugPointLight"));
	m_mTagRenderContainer[t][object_name]->AddBuffer(RESOURCEMGR->GetBuffer("PointLightCB1"));
	m_mTagRenderContainer[t][object_name]->AddBuffer(RESOURCEMGR->GetBuffer("PointLightCB2"));
	m_mTagRenderContainer[t][object_name]->Begin();

	object_name = "debugcapsulelight";
	//capsule light
	m_mTagRenderContainer[t][object_name] = new CDebugRenderContainer();
	m_mTagRenderContainer[t][object_name]->SetMesh(RESOURCEMGR->GetMesh("CapsuleLight", 0));
	m_mTagRenderContainer[t][object_name]->SetShader(RESOURCEMGR->GetRenderShader("DebugCapsuleLight"));
	m_mTagRenderContainer[t][object_name]->AddBuffer(RESOURCEMGR->GetBuffer("CapsuleLightCB1"));
	m_mTagRenderContainer[t][object_name]->AddBuffer(RESOURCEMGR->GetBuffer("CapsuleLightCB2"));
	m_mTagRenderContainer[t][object_name]->Begin();

	object_name = "debugspotlight";
	//spot light
	m_mTagRenderContainer[t][object_name] = new CDebugRenderContainer();
	m_mTagRenderContainer[t][object_name]->SetMesh(RESOURCEMGR->GetMesh("SpotLight", 0));
	m_mTagRenderContainer[t][object_name]->SetShader(RESOURCEMGR->GetRenderShader("DebugSpotLight"));
	m_mTagRenderContainer[t][object_name]->AddBuffer(RESOURCEMGR->GetBuffer("SpotLightCB1"));
	m_mTagRenderContainer[t][object_name]->AddBuffer(RESOURCEMGR->GetBuffer("SpotLightCB2"));
	m_mTagRenderContainer[t][object_name]->Begin();

	object_name = "debugtexture";
	m_mTagRenderContainer[t][object_name] = new CDebugRenderContainer();
	m_mTagRenderContainer[t][object_name]->SetMesh(RESOURCEMGR->GetMesh("DebugTexture", 0));
	m_mTagRenderContainer[t][object_name]->SetShader(RESOURCEMGR->GetRenderShader("DebugTexture"));
	m_mTagRenderContainer[t][object_name]->AddInstanceBuffer(RESOURCEMGR->GetBuffer("DebugTextureIB"));
	m_mTagRenderContainer[t][object_name]->Begin();

	//자동 rendercontainer제작!
	CreateStempRenderContainer();

	return true;
}

bool CRenderContainerSeller::End(){
	//render container delete
	for (auto pairTagRenderContainer : m_mTagRenderContainer) {
		for (auto pairRenderContainer : pairTagRenderContainer.second) {
			delete pairRenderContainer.second;
		}
		pairTagRenderContainer.second.clear();
	}
	m_mTagRenderContainer.clear();
	//render container delete

	for (auto pairTagRenderContainer : m_mStempRenderContainer) {
		for (auto pairRenderContainer : pairTagRenderContainer.second) {
			delete pairRenderContainer.second;
		}
		pairTagRenderContainer.second.clear();
	}
	m_mStempRenderContainer.clear();
	return true;
}

CRenderContainer* CRenderContainerSeller::GetRenderContainer(string name) {
	//있으면 바로 return
	for (auto pairTagRenderContainer : m_mTagRenderContainer) {
		if (pairTagRenderContainer.second.end() != pairTagRenderContainer.second.find(name))
			return pairTagRenderContainer.second[name];
	}
	for (auto pairTagRenderContainer : m_mStempRenderContainer) {
		if (pairTagRenderContainer.second.end() != pairTagRenderContainer.second.find(name))
			return pairTagRenderContainer.second[name];
	}

	return nullptr;
}

CRenderContainer * CRenderContainerSeller::GetRenderContainer(tag t, string name){

	return nullptr;
}
void CRenderContainerSeller::CreateStempRenderContainer(){

	//tag t = tag::TAG_DYNAMIC_OBJECT;
	for (auto vStempMesh : RESOURCEMGR->GetAllStempMesh()) {
		string name = vStempMesh.second[0]->GetName();
		tag t = tag::TAG_DYNAMIC_OBJECT;
		bool bAnimation = true;//vStempMesh.second[0]->GetbAnimation();
		
		m_mStempRenderContainer[t][name] = new CRenderContainer;
		m_mStempRenderContainer[t][name]->SetShader(RESOURCEMGR->GetRenderShader("AnimationObject"));
		m_mStempRenderContainer[t][name]->AddBuffer(RESOURCEMGR->GetBuffer("FBX"));
		m_mStempRenderContainer[t][name]->AddMaterial(RESOURCEMGR->GetMaterial("FBX"));
		for (auto pStempMesh : vStempMesh.second) {
			m_mStempRenderContainer[t][name]->AddMesh(pStempMesh);
		}
		if (bAnimation) {
			m_mStempRenderContainer[t][name]->SetAnimater(RESOURCEMGR->GetAnimater(name));
		}
	}
}