#pragma once
#include "Object.h"
#include "FbxJointData.h"

//animation data is frame data!
class CAnimationData : public CObject {
public:
	bool Begin();
	virtual bool End();

	void SetJointCnt(int size) { m_vvKeyFrame.SetCount(size); }
	CAtlArray<CKeyFrame>& GetKeyFrames(int index) { return m_vvKeyFrame[index]; }
	CAtlArray<CAtlArray<CKeyFrame>>& GetAllKeyFrame() { return m_vvKeyFrame; }
	void SetAnimationLength(int length) { m_tAnimLength = static_cast<int>(length); }
	int& GetAnimationLength() { return m_tAnimLength; }
private:
	//vector<vector<CKeyFrame>> m_vvKeyFrame;

	CAtlArray<CAtlArray<CKeyFrame>> m_vvKeyFrame;
	int m_tAnimLength;

public:
	CAnimationData() : CObject("animationdata") {}
	~CAnimationData() {}
};