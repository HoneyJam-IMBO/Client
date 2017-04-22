#pragma once
#include "UIObject.h"
class CLoadingBack :
	public CUIObject
{
public:
	CLoadingBack();
	~CLoadingBack();

public:
	static CLoadingBack* Create(XMVECTOR xyPos, XMVECTOR xySize, TCHAR* pTexName);

	void	SetInfo(XMVECTOR xyPos, XMVECTOR xySize, TCHAR* pTexName);

public:
	virtual HRESULT Initialize() ;
	virtual int		Update(float fTimeElapsed) ;
	virtual void	Render() ;
	virtual void	Release() ;

private:
	void	SetParameter();
};

