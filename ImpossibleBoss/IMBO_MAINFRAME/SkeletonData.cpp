#include "stdafx.h"
#include "SkeletonData.h"

bool CSkeletonData::Begin(){

	return true;
}

bool CSkeletonData::End(){
	/*size_t iVecSize = m_JointDatas.GetCount();
	for (size_t i = 1; i < iVecSize; ++i)
	{
		m_JointDatas.RemoveAt(i);
	}*/
	//m_JointDatas.RemoveAll();

	return true;
}
