#include "stdafx.h"
#include "..\Headers\Player_Idle.h"
#include "Player.h"
#include "Input_Device.h"

#include "Player_Move.h"

_USING(Client)

CPlayer_Idle::CPlayer_Idle(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CPlayerState(pGraphic_Device)
{
}

HRESULT CPlayer_Idle::Initialize_State(CPlayer & Player)
{
	Player.Set_AniIndex(CPlayer::PLAYER_STATE::LUMOSSTRAFERIGHT);
	Player.Set_ActionID(CPlayer::ACTION_ID::ACTION_IDLE);

	return NOERROR;
}

CPlayerState * CPlayer_Idle::Input_Keyboard(CPlayer & Player, const float & fTimeDelta, BYTE KeyID)
{
	if (CInput_Device::GetInstance()->GetDIKeyState(DIK_W) & 0x80)
	{
		return CPlayer_Move::Create(m_pGraphic_Device, Player);
	}
	else if (CInput_Device::GetInstance()->GetDIKeyState(DIK_S) & 0x80)
	{
		return CPlayer_Move::Create(m_pGraphic_Device, Player);
	}
	if (CInput_Device::GetInstance()->GetDIKeyState(DIK_A) & 0x80)
	{
		return CPlayer_Move::Create(m_pGraphic_Device, Player);
	}
	else if (CInput_Device::GetInstance()->GetDIKeyState(DIK_D) & 0x80)
	{
		return CPlayer_Move::Create(m_pGraphic_Device, Player);
	}

	if (CInput_Device::GetInstance()->GetDIMouseState(CInput_Device::MOUSEBUTTON::DIM_LBUTTON))
	{
		//return CPlayer_Jump::Create(m_pGraphicDevice, Player);
	}

	return nullptr;
}

void CPlayer_Idle::Update_State(CPlayer & Player, const float & fTimeDelta)
{
}

CPlayer_Idle * CPlayer_Idle::Create(LPDIRECT3DDEVICE9 pGraphicDevice, CPlayer & Player)
{
	CPlayer_Idle* pInstance = new CPlayer_Idle(pGraphicDevice);

	if (FAILED(pInstance->Initialize_State(Player)))
		Safe_Release(pInstance);

	return pInstance;
}

void CPlayer_Idle::Free()
{
	CPlayerState::Free();
}
