#pragma once
#include "DXObject.h"
#include "AnimationInfo.h"
#include "SkeletonData.h"
#include "BoundingBox.h"

class CAnimater : public DXObject {
public:
	bool Begin();
	virtual bool End();
	virtual void SetShaderState();
	virtual void CleanShaderState();

	void Update(float fTimeElapsed);

	void AddAnimationInfo(CAnimationInfo* pAnimationInfo);
	//void CreateJointTree();
	//void ChangeAllAnimationInfoJointData();
	void DeleteAnimationInfo(UINT AnimationIndex);

	void SetCurAnimationIndex(UINT AnimationIndex);
	UINT GetCurAnimationIndex() { return m_CurAnimationIndex; }

	CAnimationInfo* GetCurAnimationInfo() { return m_vpAnimationInfos[m_CurAnimationIndex]; }
	vector<BoundingOrientedBox> GetActiveOBBs() { return m_vpAnimationInfos[m_CurAnimationIndex]->GetActiveOBB(); }
	CAnimationInfo* GetAnimationInfo(UINT AnimationIndex = 0) { return m_vpAnimationInfos[AnimationIndex]; }
	CSkeletonData* GetSkeletonData() { return m_pSkeletonData; }

	size_t GetAnimationCnt() { return m_vpAnimationInfos.size(); }

	CBoundingBox* GetMainAABB() { return m_pMainBoundingBox;}
	
	//util
	void ResetAnimationInfos();

	//helper func
	UINT GetAnimaterJointCnt();


	//create func
	static shared_ptr<CAnimater> CreateAnimaterFromFBXFile(bool bHasAnimation = true);
	static shared_ptr<CAnimater> CreateAnimaterFromGJMFile(bool bHasAnimation = true);

	void SetMeshOffsetMtx(XMFLOAT4X4& xmf4x4Mtx) { m_xmf4x4MeshOffsetMtx = xmf4x4Mtx; }
	void SetMeshOffsetMtx(XMMATRIX xmMtx) { XMStoreFloat4x4(&m_xmf4x4MeshOffsetMtx, xmMtx); }
	XMMATRIX GetMeshOffsetMtx() { return XMLoadFloat4x4(&m_xmf4x4MeshOffsetMtx); }
	void SetpAnimBuffer(shared_ptr<CBuffer> pAnimBuffer) { m_pAnimBuffer = pAnimBuffer; }
	void SetpMainAABB(CBoundingBox* pMainBoundingBox) { m_pMainBoundingBox = pMainBoundingBox; }
	void SetpSkeletonData(CSkeletonData* pSkeletonData) { m_pSkeletonData = pSkeletonData; }

private:
	XMFLOAT4X4 m_xmf4x4MeshOffsetMtx;
	CBoundingBox* m_pMainBoundingBox{ nullptr };
	CSkeletonData* m_pSkeletonData{ nullptr };

	vector<CAnimationInfo*> m_vpAnimationInfos;
	UINT m_CurAnimationIndex{ 0 };

//	//joint tree Á¤º¸
//	vector<string> m_vJointName;

	//buffer
	shared_ptr<CBuffer> m_pAnimBuffer{ nullptr };

public:
	CAnimater();
	~CAnimater();

};