#include "stdafx.h"
#include "AnimationInfo.h"
#include "Animater.h"
bool CAnimationInfo::Begin(shared_ptr<CAnimater> pAnimater){
	int nJoint{ 0 };

	nJoint = m_pAnimater->GetSkeletonData()->GetJointDatas().size();

	for (int j = 0; j < nJoint; ++j) {
		XMMATRIX FrameTransform;
		CBoundingBox boundingBox;
		boundingBox.Begin(XMVectorSet(0.f, 0.f, 0.f, 0.f), XMVectorSet(2.f, 2.f, 2.f, 1.f));
		boundingBox.SetActive(false);
		m_vTempBoundingBox.push_back(boundingBox);
	}

//	for (int MeshIndex = 0; MeshIndex < FBXIMPORTER->GetAnimationDatas().size(); ++MeshIndex) {
//		nJoint = static_cast<int>(FBXIMPORTER->GetAnimationDatas()[MeshIndex].GetJointCnt());
//
//		for (int j = 0; j < nJoint; ++j) {
//			XMMATRIX FrameTransform;
//			m_mMeshIndexJoints[MeshIndex].push_back(FBXIMPORTER->GetAnimationDatas()[MeshIndex].GetJointDatas()[j]);
//			CBoundingBox boundingBox;
//			boundingBox.Begin(XMVectorSet(0.f, 0.f, 0.f, 0.f), XMVectorSet(2.f, 2.f, 2.f, 1.f));
//			boundingBox.SetActive(false);
//			m_vTempBoundingBox.push_back(boundingBox);
//		}
//	}
	m_pAnimBuffer = CBuffer::CreateConstantBuffer(256, sizeof(XMMATRIX), 10, BIND_VS, 0);

	m_pAnimater->AddAnimationInfo(this);
	return true;
}

bool CAnimationInfo::End(){
	m_pAnimationData->End();
	delete m_pAnimationData;
	m_pAnimationData = nullptr;

	m_pAnimBuffer->End();
	m_pAnimBuffer = nullptr;

	for (auto data : m_lActiveBoundingBox) {
		data->End();
		delete data;
	}
	m_lActiveBoundingBox.clear();
	//int MeshNum = m_mMeshIndexJoints.size();
	//for (int i = 0; i<MeshNum; ++i) {
	//	m_mMeshIndexJoints[i].clear();
	//}
	//m_mMeshIndexJoints.clear();

	return true;

}

void CAnimationInfo::SetShaderState(){
	void* pData = m_pAnimBuffer->Map();
	XMMATRIX* pAnimationData = static_cast<XMMATRIX*>(pData);

	for (int j = 0; j < m_pAnimater->GetSkeletonData()->GetJointDatas().size(); ++j) {
		
		//pAnimationData[j] = XMMatrixIdentity();
		if (m_pAnimationData->GetKeyFrames(j).empty()) {
			continue;
		}
		XMMATRIX offsetMtx = m_pAnimater->GetSkeletonData()->GetJointDatas()[j].GetOffsetMtx();
		pAnimationData[j] = XMMatrixTranspose(offsetMtx * m_pAnimationData->GetKeyFrames(j)[m_CurFrame].GetKeyFrameTransformMtx());
		//		if (m_vOBB[j].GetActive()) {
		//	
		//			DEBUGER->RegistCoordinateSys(m_ppAnimationData[static_cast<int>(m_nFrame)][j]);
		//			BoundingOrientedBox originObb = m_vOBB[j].GetOBB();
		//			originObb.Transform(originObb, m_ppAnimationData[static_cast<int>(m_nFrame)][j]);
		//			DEBUGER->RegistOBB(originObb);
		//		}
	}
	m_pAnimBuffer->Unmap();
	m_pAnimBuffer->SetShaderState();
}

void CAnimationInfo::CleanShaderState(){
	m_pAnimBuffer->CleanShaderState();
}

void CAnimationInfo::Update(float fTimeElapsed){
	vector<CBoundingBox*> vDeleteBoundingBox;
	int DeleteBoxCnt{ 0 };
	//debug
	for (int tempOBBCnt = 0; tempOBBCnt < m_vTempBoundingBox.size(); ++tempOBBCnt) {
		if (m_vTempBoundingBox[tempOBBCnt].GetActive()) {
			if(false == m_pAnimationData->GetKeyFrames(tempOBBCnt).empty())
				DEBUGER->RegistCoordinateSys(m_pAnimationData->GetKeyFrames(tempOBBCnt)[m_CurFrame].GetKeyFrameTransformMtx());
		}
	}
	//debug
	m_vActiveBoundingBox.clear();

	for (auto data : m_lActiveBoundingBox) {
		float fMin = data->GetMin();
		float fMax = data->GetMax();
		if (fMin <= m_CurFrame && m_CurFrame <= fMax) data->SetActive(true);
		else data->SetActive(false);
		
		if (data->GetToolActive()) {//ui 삭제 로직 
			
			if (data->GetActive()) {
				BoundingOrientedBox originObb = data->GetOBB();
				if (false == m_pAnimationData->GetKeyFrames(data->GetMyJointIndex()).empty()) {
					originObb.Transform(originObb, m_pAnimationData->GetKeyFrames(data->GetMyJointIndex())[m_CurFrame].GetKeyFrameTransformMtx());
					m_vActiveBoundingBox.push_back(originObb);
					//originObb.Transform(originObb, data->GetWorldMtx());
					//DEBUGER->RegistOBB(originObb);
				}
			}
		}
		else {
			vDeleteBoundingBox.push_back(data);
		}
	}
	if (false == vDeleteBoundingBox.empty()) {
		for (auto data : vDeleteBoundingBox) {
			m_lActiveBoundingBox.remove(data);
		}
	}
	//update animation data
	if (m_bAnimation) {
		m_CurFrame = m_CurFrame + (fTimeElapsed*m_fAnimationSpd);
	}
	if (m_CurFrame > m_pAnimationData->GetAnimationLength()) {
		m_bLoopDone = true;
		m_CurFrame = 0.f;
	}
	else
	{
		m_bLoopDone = false;
	}
}

void CAnimationInfo::Reset(){
	m_CurFrame = 0;
}


CAnimationInfo* CAnimationInfo::CreateAnimationInfoFromFBXFile(shared_ptr<CAnimater>  pAnimater){
	return nullptr;
}

CAnimationInfo* CAnimationInfo::CreateAnimationInfoFromGJMFile(shared_ptr<CAnimater>  pAnimater){
	CAnimationInfo* pAnimationInfo = new CAnimationInfo();
	pAnimationInfo->SetAnimationIndex(pAnimater->GetAnimationCnt());
	pAnimationInfo->SetAnimater(pAnimater);

	//obb info
	int obbCnt = IMPORTER->ReadInt();
	for (int j = 0; j < obbCnt; ++j) {
		CBoundingBox* pBoundingBox = new CBoundingBox();
		float min = IMPORTER->ReadFloat();
		float max = IMPORTER->ReadFloat();
		int myJointIndex = IMPORTER->ReadInt();

		XMFLOAT3 xmf3Pos;
		xmf3Pos.x = IMPORTER->ReadFloat();
		xmf3Pos.y = IMPORTER->ReadFloat();
		xmf3Pos.z = IMPORTER->ReadFloat();

		XMFLOAT4 xmf4Scale;
		xmf4Scale.x = IMPORTER->ReadFloat();
		xmf4Scale.y = IMPORTER->ReadFloat();
		xmf4Scale.z = IMPORTER->ReadFloat();
		xmf4Scale.w = 1.0f;

		XMFLOAT4 xmf4Quaternion;
		xmf4Quaternion.x = IMPORTER->ReadFloat();
		xmf4Quaternion.y = IMPORTER->ReadFloat();
		xmf4Quaternion.z = IMPORTER->ReadFloat();
		xmf4Quaternion.w = IMPORTER->ReadFloat();
		pBoundingBox->Begin(XMLoadFloat3(&xmf3Pos), XMLoadFloat4(&xmf4Scale), XMLoadFloat4(&xmf4Quaternion));
		//pBoundingBox->SetPosition(XMLoadFloat3(&xmf3Pos));

		pBoundingBox->SetMin(min);
		pBoundingBox->SetMax(max);
		pBoundingBox->SetMyJointIndex(myJointIndex);
		pAnimationInfo->GetActiveOBB().push_back(pBoundingBox);
	}//obb for end
	float animationSpd = IMPORTER->ReadFloat();
	pAnimationInfo->SetAnimationSpd(animationSpd);

	CAnimationData* pAnimationData = new CAnimationData();
	int jointCnt = pAnimater->GetSkeletonData()->GetJointCnt();
	int frameCnt = IMPORTER->ReadInt();
	pAnimationData->SetAnimationLength(frameCnt);

	for (int j = 0; j < jointCnt; ++j) {
		int curJointFrameCnt = IMPORTER->ReadInt();
		pAnimationData->GetAllKeyFrame().resize(jointCnt);
		if (curJointFrameCnt <= 0) continue;
		for (int k = 0; k < frameCnt; ++k) {
			CKeyFrame keyFrame;
			keyFrame.SetKeyFrameTransformMtx(IMPORTER->ReadXMMatrix());
			pAnimationData->GetAllKeyFrame()[j].push_back(keyFrame);
		}//end frame for
	}//end joint for

	pAnimationInfo->SetAnimationData(pAnimationData);
	pAnimationInfo->Begin(pAnimater);
	return pAnimationInfo;
}

CAnimationInfo::CAnimationInfo() : DXObject("animationinfo"){

}

CAnimationInfo::~CAnimationInfo()
{
}
