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
	if (INPUTMGR->KeyBoardDown(VK_SPACE_))		m_bJump = true;

	if (true == INPUTMGR->MouseRightDown())	{
		m_bIdle = true;
	}

	SetupAnimation(dwDirection);
	float fSpeed = 100.f;
	if (dwDirection){
		if (dwDirection & DIR_FORWARD)		xmvShift += GetLook();
		if (dwDirection & DIR_BACKWARD)		xmvShift -= GetLook();
		if (dwDirection & DIR_RIGHT)		xmvShift += GetRight();
		if (dwDirection & DIR_LEFT)			xmvShift -= GetRight();

		XMStoreFloat3(&m_xmf3Position, XMLoadFloat3(&m_xmf3Position) + ((XMVector3Normalize(xmvShift) * fSpeed) * fDeltaTime));
		SetPosition(XMLoadFloat3(&m_xmf3Position));

		m_bIdle = false;
	}
	else
	{
		if (false == m_bJump)
		{
			if (ELF_ANIM_JUMP_END != m_nAnimNum)
			{
				m_nAnimNum = ELF_ANIM_IDLE;
				m_pAnimater->SetCurAnimationIndex(m_nAnimNum);
			}
			else
			{
				if (true == m_pAnimater->GetCurAnimationInfo()->GetLoopDone())
				{
					m_nAnimNum = ELF_ANIM_IDLE;
					m_pAnimater->SetCurAnimationIndex(m_nAnimNum);
				}
			}			
		}			
	}
	if (true == m_bJump)
		Jumping(fDeltaTime);


	if (true == INPUTMGR->MouseRightUp() && abs(m_pCamera->m_cxDelta + m_pCamera->m_cyDelta) > 1.f)	{
		m_bIdle = false;
	}
	if (false == m_bIdle)	{
		SetRotation(XMMatrixRotationY(m_pCamera->m_fCurrentAngle[ANGLE_Y] + XM_PI));
	}
		
}

void CPawn::GetServerData()
{
}

void CPawn::SetupAnimation(DWORD dwDirection)
{
	//UINT uAnimNum = 0;

	if (false == m_bJump)
	{
		if (dwDirection & DIR_FORWARD)
			if (m_nAnimNum != ELF_ANIM_RUN_F) m_nAnimNum = ELF_ANIM_RUN_F;
		if (dwDirection & DIR_BACKWARD)
			if (m_nAnimNum != ELF_ANIM_RUN_B) m_nAnimNum = ELF_ANIM_RUN_B;
		if (dwDirection & DIR_LEFT)
			if (m_nAnimNum != ELF_ANIM_RUN_L) m_nAnimNum = ELF_ANIM_RUN_L;
		if (dwDirection & DIR_RIGHT)
			if (m_nAnimNum != ELF_ANIM_RUN_R) m_nAnimNum = ELF_ANIM_RUN_R;

		if (dwDirection & DIR_FORWARD && dwDirection & DIR_LEFT)
			if (m_nAnimNum != ELF_ANIM_RUN_FL) m_nAnimNum = ELF_ANIM_RUN_FL;
		if (dwDirection & DIR_FORWARD && dwDirection & DIR_RIGHT)
			if (m_nAnimNum != ELF_ANIM_RUN_FR) m_nAnimNum = ELF_ANIM_RUN_FR;
		if (dwDirection & DIR_BACKWARD && dwDirection & DIR_LEFT)
			if (m_nAnimNum != ELF_ANIM_RUN_BL) m_nAnimNum = ELF_ANIM_RUN_BL;
		if (dwDirection & DIR_BACKWARD && dwDirection & DIR_RIGHT)
			if (m_nAnimNum != ELF_ANIM_RUN_BR) m_nAnimNum = ELF_ANIM_RUN_BR;

		if(0 != dwDirection)
			m_pAnimater->SetCurAnimationIndex(m_nAnimNum);
	}
	else
	{
		//if (0 == dwDirection)
		{
			if (m_nAnimNum != ELF_ANIM_JUMP_START
				&& m_nAnimNum != ELF_ANIM_JUMP_LOOP
				&& m_nAnimNum != ELF_ANIM_JUMP_END)
			{
				m_nAnimNum = ELF_ANIM_JUMP_START;
				m_pAnimater->SetCurAnimationIndex(m_nAnimNum);
			}
				

			if (m_nAnimNum == ELF_ANIM_JUMP_START
				&& true == m_pAnimater->GetCurAnimationInfo()->GetLoopDone())
			{
				m_nAnimNum = ELF_ANIM_JUMP_LOOP;
				m_pAnimater->SetCurAnimationIndex(m_nAnimNum);
			}
		}
	}
}

void CPawn::Jumping(float fDeltaTime)
{
	m_fJumpTime += fDeltaTime;
	float fJumpValue = 6.f * m_fJumpTime * m_fJumpTime;
	float fJumpPower = 1.7f;
	

	m_xmf4x4World._42 += fJumpPower - fJumpValue;
	m_xmf3Position.y += fJumpPower - fJumpValue;
	
	if (m_xmf4x4World._42 < GetTerrainHeight())
	{
		m_fJumpTime = 0.f;
		m_bJump = false;

		m_xmf4x4World._42 = GetTerrainHeight();
		m_xmf3Position.y = GetTerrainHeight();

		m_nAnimNum = ELF_ANIM_JUMP_END;
		m_pAnimater->SetCurAnimationIndex(m_nAnimNum);
	}
}

CPawn::CPawn(string name, tag t, bool bSprit)
	: CGameObject(name, t)
	, m_bSprit(bSprit)
{
}

CPawn::~CPawn()
{
}
