#pragma once
#include "GameObject.h"
class CPawn :
	public CGameObject
{
public:
	bool Begin();
	virtual void Animate(float fTimeElapsed);


private:
	void	KeyInput(float fDeltaTime);
	void	PushServerData();
	void	SetupAnimation(DWORD dwDirection);
	void	Jumping(float fDeltaTime);

private:
	int		m_iAnimNum{ 0 };
	bool	m_bSprit{ false };
	float	m_fJumpTime{ 0.f };

	UINT	m_nAnimNum{ 0 };
	float	m_fAngleY{ 0.f };

	int		m_iPawnNum{ -1 };
public:
	void	SetPawNum(int iPawnNum) { m_iPawnNum = iPawnNum; }

public:
	CPawn(string name, tag t = tag::TAG_DEFAULT, bool bSprit = false);
	~CPawn();
};

