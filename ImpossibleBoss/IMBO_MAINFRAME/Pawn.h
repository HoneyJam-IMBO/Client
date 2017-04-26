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
	void	GetServerData();
	void	SetupAnimation(DWORD dwDirection);
	void	Jumping(float fDeltaTime);

private:
	int		m_iAnimNum{ 0 };
	bool	m_bSprit{ false };
	float	m_fJumpTime{ 0.f };

	UINT	m_nAnimNum{ 0 };

public:
	CPawn(string name, tag t = tag::TAG_DEFAULT, bool bSprit = false);
	~CPawn();
};

