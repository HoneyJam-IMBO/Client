#pragma once
#include "UIRenderer.h"

struct tUImatVP {
	XMFLOAT4X4	m_xmf4x4VP;
	//float		m_fAlpha;
	//float		a, b, c;
	

};

class CUIObject
{
public:
	CUIObject();
	~CUIObject();

public:
	virtual HRESULT Initialize() { return S_OK; };
	virtual int		Update(float fTimeElapsed) { return 0; };
	virtual void	Render() {};
	virtual void	Release() { };
	
protected:
	XMFLOAT2	m_f2XYPos{ 0.f, 0.f };
	XMFLOAT2	m_f2XYSize{ 1.f, 1.f };
	
	XMFLOAT4X4	m_f4x4View;
	XMFLOAT4X4	m_f4x4Proj;

	CMesh*					m_pMesh;
	shared_ptr<CBuffer>		m_pCBuffer;
	shared_ptr<CTexture>	m_pTexture;

	CUIRenderer*		m_pUIRenderCont{ nullptr };


	float		m_fRenderLayer{ 0.f };
protected:
	TCHAR		m_szTexture[64];		//텍스처 이름
	float		m_fAccTime{ 0.f };
};

