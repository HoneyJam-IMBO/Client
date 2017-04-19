#include "stdafx.h"
#include "Pawn.h"


bool CPawn::Begin()
{
	CGameObject::Begin();

	return false;
}

void CPawn::Animate(float fTimeElapsed)
{
	if (true == m_bSprit)
		KeyInput(fTimeElapsed);
	
	GetServerData();


	if (m_pAnimater) m_pAnimater->Update(TIMEMGR->GetTimeElapsed());
	CGameObject::Animate(fTimeElapsed);
}

void CPawn::KeyInput(float fDeltaTime)
{
	DWORD dwDirection = 0;
	XMVECTOR xmvShift = XMVectorSet(0.0f, 0.0f, 0.0f, 0.f);
	if (INPUTMGR->OnlyKeyBoardDown(VK_W))		dwDirection |= DIR_FORWARD;
	if (INPUTMGR->OnlyKeyBoardDown(VK_S))		dwDirection |= DIR_BACKWARD;
	if (INPUTMGR->OnlyKeyBoardDown(VK_A))		dwDirection |= DIR_LEFT;
	if (INPUTMGR->OnlyKeyBoardDown(VK_D))		dwDirection |= DIR_RIGHT;

	if (INPUTMGR->KeyDown(VK_Q))
	{
		if (m_pAnimater->GetAnimationCnt()  - 1> m_iAnimNum)
			++m_iAnimNum;
		else
			m_iAnimNum = 0;

		m_pAnimater->SetCurAnimationIndex(m_iAnimNum);
	}

	float fSpeed = 200.f;
	if (dwDirection)
	{
		if (dwDirection & DIR_FORWARD)		xmvShift += GetLook();
		if (dwDirection & DIR_BACKWARD)		xmvShift -= GetLook();
		if (dwDirection & DIR_RIGHT)		xmvShift += GetRight();
		if (dwDirection & DIR_LEFT)			xmvShift -= GetRight();

		XMVector3Normalize(xmvShift);

		//pos에 shift*speed*ftimeElapsed 더해줌 = 속도만큼 이동
		XMStoreFloat3(&m_xmf3Position, XMLoadFloat3(&m_xmf3Position) + ((xmvShift * fSpeed) * fDeltaTime));
		SetPosition(XMLoadFloat3(&m_xmf3Position));
	}
	//cs_packet_player_position PosPacket;
	//PosPacket.Size = sizeof(PosPacket);
	//PosPacket.Type = CS_PLAYER_POS;
	//PosPacket.PosX = m_xmf3Position.x;
	//PosPacket.PosY = m_xmf3Position.y;
	//PosPacket.PosZ = m_xmf3Position.z;
	//PosPacket.RotY = 0.f;
	//PosPacket.AnimNumber = m_iAnimNum;
	////send_wsabuf.len = sizeof(PosPacket);

	//NETWORKMGR->SendPacket(&PosPacket);
}

void CPawn::GetServerData()
{

}

CPawn::CPawn(string name, tag t, bool bSprit)
	: CGameObject(name, t)
	, m_bSprit(bSprit)
{
}

CPawn::~CPawn()
{
}
