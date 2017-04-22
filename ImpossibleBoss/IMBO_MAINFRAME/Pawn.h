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

private:
	int		m_iAnimNum{ 0 };
	bool	m_bSprit{ false };

public:
	CPawn(string name, tag t = tag::TAG_DEFAULT, bool bSprit = false);
	~CPawn();
};

