#include "stdafx.h"
#include "AnimationData.h"

bool CAnimationData::Begin()
{
	return true;
}

bool CAnimationData::End(){
	/*for (auto data : m_vvKeyFrame) {
		data.clear();
	}
	m_vvKeyFrame.clear();*/

	size_t vecSize = m_vvKeyFrame.GetCount();
	for (size_t i = 0; i < vecSize; ++i)
	{
		m_vvKeyFrame[i].RemoveAll();
		//m_vvKeyFrame[i].FreeExtra();
	}
	m_vvKeyFrame.RemoveAll();
	//m_vvKeyFrame.FreeExtra();
	return true;
}
