#pragma once
#include "DXObject.h"
#include "AnimationData.h"
#include "BoundingBox.h"
#include "Buffer.h"

class CAnimater;
class CAnimationInfo :public DXObject {
public:
	bool Begin(shared_ptr<CAnimater> pAnimater);
	virtual bool End();
	virtual void SetShaderState();
	virtual void CleanShaderState();

	virtual void Update(float fTimeElapsed);

	float& GetCurFrame() { return m_CurFrame; }
	int& GetFrameCnt() { return m_pAnimationData->GetAnimationLength(); }
	bool& GetbAnimation() { return m_bAnimation; }
	float& GetAnimationSpd() { return m_fAnimationSpd; }
	UINT GetAnimationIndex() { return m_AnimationIndex; }
	void SetAnimationIndex(UINT index) { m_AnimationIndex = index; }
	void SetAnimater(shared_ptr<CAnimater> pAnimater) { m_pAnimater = pAnimater; }
	void SetAnimationData(CAnimationData* pAnimationData) { m_pAnimationData = pAnimationData; }

	//utill 
	void Reset();

	//joint data 수정 함수
	//void ChangeJointData(vector<string>& vJointName);

	XMMATRIX GetCurFrameMtx(UINT JointIndex, UINT meshIndex = 0) { return m_pAnimationData->GetKeyFrames(JointIndex)[m_CurFrame].GetKeyFrameTransformMtx(); };
	CAnimationData* GetAnimationData() { return m_pAnimationData; }
	//map<UINT, vector<CFbxJointData>>& GetAnimationInfos() { return m_mMeshIndexJoints; }
	
	vector<BoundingOrientedBox> GetvActiveOBBs() { return m_vActiveBoundingBox; }
	vector<CBoundingBox>& GetTempOBB() { return m_vTempBoundingBox; }
	list<CBoundingBox*>& GetActiveOBB() { return m_lActiveBoundingBox; }

	shared_ptr<CAnimater> GetAnimater() { return m_pAnimater; }

	//create func
	static CAnimationInfo* CreateAnimationInfoFromFBXFile(shared_ptr<CAnimater>  pAnimater);
	static CAnimationInfo* CreateAnimationInfoFromGJMFile(shared_ptr<CAnimater>  pAnimater);

	void SetAnimationSpd(float spd) { m_fAnimationSpd = spd; }
private:
	vector<BoundingOrientedBox> m_vActiveBoundingBox;
	vector<CBoundingBox> m_vTempBoundingBox;
	list<CBoundingBox*> m_lActiveBoundingBox;
	float m_fAnimationSpd{ 1.0f };
	//animation 
	CAnimationData* m_pAnimationData{ nullptr };
	shared_ptr<CAnimater> m_pAnimater{ nullptr };
	UINT m_AnimationIndex{ 0 };
	float m_CurFrame{ 0 };
	bool m_bAnimation{ true };
	//buffer
	shared_ptr<CBuffer> m_pAnimBuffer{ nullptr };
public:
	CAnimationInfo();
	~CAnimationInfo();

};