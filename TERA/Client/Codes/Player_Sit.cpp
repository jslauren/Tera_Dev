#include "stdafx.h"
#include "..\Headers\Player_Sit.h"
#include "Player.h"
#include "Input_Device.h"

#include "Player_Idle.h"

_USING(Client)

CPlayer_Sit::CPlayer_Sit(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CPlayerState(pGraphic_Device)
{
}

HRESULT CPlayer_Sit::Initialize_State(CPlayer & Player)
{
	Player.Set_ActionID(CPlayer::ACTION_ID::ACTION_READY);

	return NOERROR;
}

CPlayerState * CPlayer_Sit::Input_Keyboard(CPlayer & Player, const float & fTimeDelta, BYTE KeyID, void * pArg)
{
	if (Player.Get_Mesh_Bone()->Get_NowPlayAniIndex() == CPlayer::PLAYER_ANI::SitStart)
	{
		if (Player.Get_Mesh_Bone()->IsAnimationEnded(0.85f))
		{
			m_iAniState = 2;
			return CPlayer_Sit::Create(m_pGraphic_Device, Player, &m_iAniState);
		}
	}
	else if (Player.Get_Mesh_Bone()->Get_NowPlayAniIndex() == CPlayer::PLAYER_ANI::SitLoop)
	{
		if (CInput_Device::GetInstance()->GetDIKeyState(DIK_R) & 0x80)
		{
			if (Player.Get_Mesh_Bone()->IsAnimationEnded(0.85f))
			{
				m_iAniState = 3;
				return CPlayer_Sit::Create(m_pGraphic_Device, Player, &m_iAniState);
			}
		}
	}
	else if (Player.Get_Mesh_Bone()->Get_NowPlayAniIndex() == CPlayer::PLAYER_ANI::SitEnd)
	{
		if (Player.Get_Mesh_Bone()->IsAnimationEnded(0.85f))
		{
			m_iAniState = 1;
			return CPlayer_Idle::Create(m_pGraphic_Device, Player, &m_iAniState);
		}
	}

	return nullptr;
}

void CPlayer_Sit::Update_State(CPlayer & Player, const float & fTimeDelta)
{
}

CPlayer_Sit * CPlayer_Sit::Create(LPDIRECT3DDEVICE9 pGraphicDevice, CPlayer & Player, void * pArg)
{
	CPlayer_Sit* pInstance = new CPlayer_Sit(pGraphicDevice);

	if (*(_int*)(pArg) == 1)
		Player.Set_AniIndex(CPlayer::PLAYER_ANI::SitStart);

	else if ((*(_int*)(pArg) == 2))
		Player.Set_AniIndex(CPlayer::PLAYER_ANI::SitLoop);

	else if ((*(_int*)(pArg) == 3))
		Player.Set_AniIndex(CPlayer::PLAYER_ANI::SitEnd);

	if (FAILED(pInstance->Initialize_State(Player)))
		Safe_Release(pInstance);

	return pInstance;
}

void CPlayer_Sit::Free()
{
	CPlayerState::Free();
}
