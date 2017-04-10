#include "stdafx.h"
#include "AnimationInfo.h"
#include "Animater.h"

bool CAnimationInfo::Begin(shared_ptr<CAnimater> pAnimater){
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

	m_vActiveBoundingBox.clear();
	for (auto data : m_vOriActiveBoundingBox) {
		data->End();
		delete data;
	}
	m_vOriActiveBoundingBox.clear();
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
	m_vActiveBoundingBox.clear();

	int index{ 0 };
	for (auto data : m_vOriActiveBoundingBox) {
		float fMin = data->GetMin();
		float fMax = data->GetMax();
		if (fMin <= m_CurFrame && m_CurFrame <= fMax) data->SetActive(true);
		else data->SetActive(false);

		if (data->GetActive()) {
			BoundingOrientedBox originObb = data->GetOBB();
			if (false == m_pAnimationData->GetKeyFrames(data->GetMyJointIndex()).empty()) {
				originObb.Transform(originObb, m_pAnimationData->GetKeyFrames(data->GetMyJointIndex())[m_CurFrame].GetKeyFrameTransformMtx());
				m_vActiveBoundingBox.push_back(originObb);
				//originObb.Transform(originObb, data->GetWorldMtx());
				//DEBUGER->RegistOBB(m_vActiveBoundingBox[index++]->GetOBB());
			}
		}
	}
	//update animation data
	if (m_bAnimation) {
		m_CurFrame = m_CurFrame + (fTimeElapsed*m_fAnimationSpd);
	}
	if (m_CurFrame > m_pAnimationData->GetAnimationLength()) {
		m_CurFrame = 0.f;
	}
}

void CAnimationInfo::Reset(){
	m_CurFrame = 0;
}

//void CAnimationInfo::ChangeJointData(vector<string>& vJointName){
//	//���ڷ� ���� animater�� joint tree������ ������ 
//	//�ڽ��� joint�����͸� �����Ѵ� . 
//	//���ڷ� ���� joint tree������ ��� joint���� ������ �ƿ츥��.
//	
//	//1. ���ο� ���͸� �����Ѵ�.
//	vector<CFbxJointData> vTempJoints;
//	vector<CFbxJointData> ::iterator iter;
//	int CurIndex{ 0 };
//	int ChangeIndex{ 0 };
//	for (auto jointName : vJointName) {
//		CurIndex = 0;
//		//2. ���ڷ� ���� animation tree��ŭ ������ ����.
//		iter = find_if(m_vJoints.begin(), m_vJoints.end(), [&jointName, &CurIndex](CFbxJointData& my) {
//			CurIndex++;
//			return (my.GetJointName() == jointName);
//		});
//		//3. �� joint�� �̸��� ������ ������ joint�� �̸��� ���Ͽ� ������ �ְ�
//		if (m_vJoints.end() != iter) {
//			vTempJoints.push_back(*iter);
//			m_mChangeIndex.insert(pair<int, int>(CurIndex, ChangeIndex));
//		}
//		//�ƴϸ� ���ο� joint���� offsetmtx�� �ְ� frame mtx�� �ʿ���� �����ʹ� Identity�� �ִ´�.
//		else {//���� �̸��� ���� ��찡 2���� �ִ�. �����Դ� �ִµ� ��뿡�Դ� ���� joint
//			//��뿡�Դ� �ִµ� ������ ���� joint�� �װ��̴�.
//			//�̰��� ��뿡�Դ� �ִµ� ������ ���� joint��츸 �ش��Ѵ�. 
//			CFbxJointData data;
//			//data.SetMyIndex(joint.GetMyIndex());
//			//data.SetJointName(joint.GetJointName());
//			//data.SetOffsetMtx(joint.GetOffsetMtx());
//			//data.SetParentIndex(joint.GetParentIndex());
//			vTempJoints.push_back(data);
//		}
//		ChangeIndex++;
//	}
//	//4. ������ vector�� ������ ���ο� vector�� �����Ѵ�.
//	m_vJoints.clear();
//	m_vTempBoundingBox.clear();
//
//	for (auto data : vTempJoints) {
//		m_vJoints.push_back(data);
//		//bounding box �߰�
//		CBoundingBox boundingBox;
//		boundingBox.Begin(XMVectorSet(0.f, 0.f, 0.f, 0.f), XMVectorSet(2.f, 2.f, 2.f, 1.f));
//		boundingBox.SetActive(false);
//		m_vTempBoundingBox.push_back(boundingBox);
//	}
//}
CAnimationInfo* CAnimationInfo::CreateAnimationInfoFromFBXFile(shared_ptr<CAnimater>  pAnimater){
	CAnimationInfo* pAnimationInfo = new CAnimationInfo();
	pAnimationInfo->SetAnimationIndex(pAnimater->GetAnimationCnt());
	pAnimationInfo->SetAnimater(pAnimater);

	CAnimationData* pAnimationData = new CAnimationData();
	*pAnimationData = FBXIMPORTER->GetAnimationData();

	pAnimationInfo->SetAnimationData(pAnimationData);
	pAnimationInfo->Begin(pAnimater);
	return pAnimationInfo;
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
		pAnimationInfo->GetOriActiveOBB().push_back(pBoundingBox);
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