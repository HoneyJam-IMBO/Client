#include "stdafx.h"
#include "SpaceContainer.h"


void CSpaceContainer::Begin(){

	//한 사이드에 있는 공간의 개수
	m_oneSideSpaceNum = static_cast<int>(pow(2, m_level));
	//공간 하나의 크기
	m_oneSpaceSize = m_size / m_oneSideSpaceNum;
	//공간의 개수 쿼드트리니까 제곱
	m_nSpace = static_cast<int>(pow(m_oneSideSpaceNum, 2));

	//관리되어질 leaf spaces
	m_ppSpace = new CSpace*[m_nSpace];
	
	//공간 할당.
	m_pStartSpace = new CSpace();
	m_pStartSpace->Begin(this, m_size, m_level, XMVectorSet(0.f, 0.f, 0.f, 0.f));
}

bool CSpaceContainer::End(){
	
	//POSITION pos = m_lpBlockObject.
	size_t vecSize = m_lpBlockObject.GetCount();
	for (size_t i = 0; i < vecSize; ++i)
	{
		m_lpBlockObject[i]->End();
		delete m_lpBlockObject[i];
		m_lpBlockObject[i] = nullptr;
	}
	m_lpBlockObject.RemoveAll();
	//m_lpBlockObject.FreeExtra();
	//for (auto pObject : m_lpBlockObject) {
	//	pObject->End();
	//	delete pObject;
	//	pObject = nullptr;
	//}
	//m_lpBlockObject.clear();

	//all space end
	//+ delete space pointer
	m_pStartSpace->End();
	m_pStartSpace = nullptr;

	delete m_ppSpace;
	m_ppSpace = nullptr;
	return false;
}

void CSpaceContainer::Animate(float fTimeElapsed){
	//all space animate
	m_pStartSpace->Animate(fTimeElapsed);
	
	//animate 이후에 분명히 block object가 등장한다.
	if (false == m_lpBlockObject.IsEmpty()) {//block object list가 empty가 아니라면
		size_t vecSize = m_lpBlockObject.GetCount();
		for (size_t i = 0; i < vecSize; ++i)
		{
			AddObject(m_lpBlockObject[i]);
		}

		//for (auto pObject : m_lpBlockObject) {//객체 다시 배치
		//	AddObject(pObject);
		//}
		//m_lpBlockObject.clear();
		m_lpBlockObject.RemoveAll();
		//m_lpBlockObject.FreeExtra();
	}
}

void CSpaceContainer::PrepareRenderOneSpace(shared_ptr<CCamera> pCamera, UINT renderFlag, int render_space){
	//if (render_space < 0) {
	//	PrepareRender(pCamera, renderFlag);
	//}else{
	//	for (int i = 0; i < m_nSpace; ++i) {
	//		if(render_space == i) m_ppSpace[i]->PrepareRender(renderFlag);
	//	}
	//}
}

void CSpaceContainer::PrepareRender(shared_ptr<CCamera> pCamera, UINT renderFlag){
	m_pStartSpace->PrepareRender(pCamera, renderFlag);
}

void CSpaceContainer::AddBlockObjectList(CGameObject * pObject){
	//m_lpBlockObject.emplace_back(pObject);
	m_lpBlockObject.Add(pObject);
}

void CSpaceContainer::AddObject(CGameObject * pObject){
	//현재 index 구하기
	int current_index = SearchSpace(pObject->GetPosition());
	
	if (current_index < 0) {
		current_index = 0;
		pObject->SetPosition(XMVectorSet(0.f, 0.f, 0.f, 0.f));
	}

	//해당 공간에 객체 추가
	m_ppSpace[current_index]->AddObject(pObject);
	//객체에 공간index 등록
	pObject->SetSpaceIndex(current_index);
}

void CSpaceContainer::RemoveObject(CGameObject * pObject){
	for (int i = 0; i < m_nSpace; ++i) {
		m_ppSpace[i]->RemoveObject(pObject);
	}
}

void CSpaceContainer::RemoveObject(string name){
	for (int i = 0; i < m_nSpace; ++i) {
		m_ppSpace[i]->RemoveObject(name);
	}
}

void CSpaceContainer::ClearBlockObjectList() {
	//m_lpBlockObject.clear();
	m_lpBlockObject.RemoveAll();
	//m_lpBlockObject.FreeExtra();
}

int CSpaceContainer::SearchSpace(XMVECTOR xmvPos){
	XMFLOAT3 xmf3Pos;
	XMStoreFloat3(&xmf3Pos, xmvPos);
	int x = static_cast<int>(xmf3Pos.x);
	int z = static_cast<int>(xmf3Pos.z);

	//자신의 위치를 이용해 인덱스를 얻어옴
	int indexX = (int)x / m_oneSpaceSize;
	int indexZ = (int)z / m_oneSpaceSize;

	int index = indexX + (indexZ * m_oneSideSpaceNum);

	//예외 처리 범위 밖
	if (index < 0 || index >= m_nSpace) {
		return -1;
	}
	if (x < 0 || z < 0 ||
		x > m_size || z > m_size) {
		return -1;
	}

	return index;
}

void CSpaceContainer::AddSpace(UINT index, CSpace * pSpace){
	m_ppSpace[index] = pSpace;
}

void CSpaceContainer::ChangeSpaceData(){
	//1. space안의 모든 객체 임시 저장
	vector<CGameObject*> m_vTempObjects;//임시 객체 벡터
	for (int i = 0; i < m_nSpace; ++i) {//모든 space에 대해서
		POSITION ContainerPos = m_ppSpace[i]->GetmlpObject().GetStartPosition();
		CAtlMap<tag, CAtlArray<CGameObject*>*>::CPair*		pOutPair = NULL;
		while (ContainerPos != NULL)
		{
			pOutPair = m_ppSpace[i]->GetmlpObject().GetNext(ContainerPos);
			size_t iVecSize = pOutPair->m_value->GetCount();
			for (size_t i = 0; i < iVecSize; ++i)
			{
				m_vTempObjects.push_back((*pOutPair->m_value)[i]);//임시 저장
			}
		}
	}
	
	//2. space delete/ not END()
	delete m_pStartSpace;
	m_pStartSpace = nullptr;
	if (m_nSpace != 1) {
		for (int i = 0; i < m_nSpace; ++i) {
			delete m_ppSpace[i];
		}
	}
	delete m_ppSpace;
	//3. 새로운 space제작
	m_oneSideSpaceNum = static_cast<int>(pow(2, m_level));
	m_oneSpaceSize = m_size / m_oneSideSpaceNum;
	m_nSpace = static_cast<int>(pow(m_oneSideSpaceNum, 2));
	m_ppSpace = new CSpace*[m_nSpace];
	m_pStartSpace = new CSpace();
	m_pStartSpace->Begin(this, m_size, m_level, XMVectorSet(0.f, 0.f, 0.f, 0.f));

	//4. 임시객체 재배치
	for (auto pObject : m_vTempObjects) {
		AddObject(pObject);
	}
	m_vTempObjects.clear();
}

CGameObject * CSpaceContainer::PickObject(XMVECTOR xmvWorldCameraStartPos, XMVECTOR xmvRayDir, float& distanse){
	float fHitDistance = FLT_MAX;
	float fNearHitDistance = FLT_MAX;
	CGameObject* pHitObj = nullptr;
	CGameObject* pNearObj = nullptr;

	//자신의 모든 leaf space에 대해서 검사
	for (int i = 0; i < m_nSpace; ++i) {
		if (m_ppSpace[i]->GetbRender()) {//내가 그림을 그리는 space라면
			pHitObj = m_ppSpace[i]->PickObject(xmvWorldCameraStartPos, xmvRayDir, fHitDistance);
			if (fNearHitDistance > fHitDistance) {
				fNearHitDistance = fHitDistance;
				pNearObj = pHitObj;
			}
		}
	}

	return pNearObj;
}

CSpaceContainer * CSpaceContainer::CreateSpaceContainer(int size, int lv){
	CSpaceContainer* pSpaceContainer = new CSpaceContainer();
	pSpaceContainer->SetSpaceSize(size);
	pSpaceContainer->SetSpaceLevel(lv);
	pSpaceContainer->Begin();
	return pSpaceContainer;
}

void CSpaceContainer::ClearAllObjects(){
	for (int i = 0; i < m_nSpace; ++i) {//모든 공간의
		POSITION ContainerPos = m_ppSpace[i]->GetmlpObject().GetStartPosition();
		CAtlMap<tag, CAtlArray<CGameObject*>*>::CPair*		pOutPair = NULL;
		while (ContainerPos != NULL)
		{
			pOutPair = m_ppSpace[i]->GetmlpObject().GetNext(ContainerPos);
			size_t iVecSize = pOutPair->m_value->GetCount();
			for (size_t i = 0; i < iVecSize; ++i)
			{
				(*pOutPair->m_value)[i]->End();
				delete (*pOutPair->m_value)[i];
			}
		}
		//for (auto v : m_ppSpace[i]->GetmlpObject()) {//모든 map의
		//	for (auto pObject : v.second) {//모든 vector
		//		pObject->End();
		//		delete pObject;
		//	}
		//	v.second.clear();
		//}
		m_ppSpace[i]->GetmlpObject().RemoveAll();
	}
	DEBUGER->ClearDebuger();
}

void CSpaceContainer::SaveObjectInfos(){
	//UINT nObject{ 0 };
	//for (int i = 0; i < m_nSpace; ++i) {//모든 공간의
	//	for (auto v : m_ppSpace[i]->GetmlpObject()) {//모든 map의
	//		for (auto pObject : v.second) {//모든 vector
	//			if (pObject->GetTag() == TAG_TERRAIN) continue;
	//			if (pObject->GetTag() == TAG_SPACE) continue;

	//			//즉 모든 객체
	//			nObject++;
	//		}
	//	}
	//}
	//EXPORTER->WriteUINT(nObject);
	//EXPORTER->WriteEnter();
	////save
	//for (int i = 0; i < m_nSpace; ++i) {//모든 공간의
	//	for (auto v : m_ppSpace[i]->GetmlpObject()) {//모든 map의
	//		for (auto pObject : v.second) {//모든 vector
	//			if (pObject->GetTag() == TAG_TERRAIN) continue;
	//			if (pObject->GetTag() == TAG_SPACE) continue;
	//			//즉 모든 객체
	//			EXPORTER->WriteEnter();
	//		}
	//	}
	//}
	//EXPORTER->WriteEnter();
}

void CSpaceContainer::LoadObjectInfos(){
	UINT nObject = IMPORTER->ReadUINT();
	for (int i = 0; i < nObject; ++i) {
		tag t = (tag)IMPORTER->ReadUINT();
		string name = IMPORTER->Readstring();
		XMFLOAT4X4 xmf4x4Wolrd = IMPORTER->ReadFloat4x4();

		switch (t) {
		case TAG_LIGHT:
			if (name == "pointlight") {
				float fRange = IMPORTER->ReadFloat();
				float r = IMPORTER->ReadFloat();
				float g = IMPORTER->ReadFloat();
				float b = IMPORTER->ReadFloat();
				XMFLOAT3 xmf3Color = XMFLOAT3(r, g, b);

				CPointLight* pLight = CPointLight::CreatePointLight(fRange, xmf3Color);
				pLight->SetWorldMtx(XMLoadFloat4x4(&xmf4x4Wolrd));
				AddObject(pLight);
			}
			else if (name == "spotlight") {
				float inner = IMPORTER->ReadFloat();
				float outer = IMPORTER->ReadFloat();

				float r = IMPORTER->ReadFloat();
				float g = IMPORTER->ReadFloat();
				float b = IMPORTER->ReadFloat();
				XMFLOAT3 xmf3Color = XMFLOAT3(r, g, b);		
				float fRange = IMPORTER->ReadFloat();

				CSpotLight* pLight = CSpotLight::CreateSpotLight(fRange, xmf3Color, outer, inner);
				pLight->SetWorldMtx(XMLoadFloat4x4(&xmf4x4Wolrd));
				AddObject(pLight);
			}
			else if (name == "capsulelight") {
				float r = IMPORTER->ReadFloat();
				float g = IMPORTER->ReadFloat();
				float b = IMPORTER->ReadFloat();
				XMFLOAT3 xmf3Color = XMFLOAT3(r, g, b);

				float fLen = IMPORTER->ReadFloat();
				float fRange = IMPORTER->ReadFloat();

				CCapsuleLight* pLight = CCapsuleLight::CreateCapsuleLight(fLen, xmf3Color, fRange);
				pLight->SetWorldMtx(XMLoadFloat4x4(&xmf4x4Wolrd));
				AddObject(pLight);
			}
			break;
		default:
			CGameObject* pObject = CGameObject::CreateObject(name, t, XMLoadFloat4x4(&xmf4x4Wolrd));
			AddObject(pObject);
		}
	}
}


CSpaceContainer::CSpaceContainer() : CObject("spacecontainer"){

}

CSpaceContainer::~CSpaceContainer(){

}

