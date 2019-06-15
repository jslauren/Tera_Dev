#include "stdafx.h"
#include "..\Headers\Player_HitRolling.h"
#include "Player.h"
#include "Input_Device.h"

#include "Player_Idle.h"

_USING(Client)

CPlayer_HitRolling::CPlayer_HitRolling(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CPlayerState(pGraphic_Device)
{
}

HRESULT CPlayer_HitRolling::Initialize_State(CPlayer & Player)
{
	Player.Set_AniIndex(CPlayer::PLAYER_ANI::Hit_Rolling);
	Player.Set_ActionID(CPlayer::ACTION_ID::ACTION_PHYSICSSKILL);

	return NOERROR;
}

CPlayerState * CPlayer_HitRolling::Input_Keyboard(CPlayer & Player, const float & fTimeDelta, BYTE KeyID, void * pArg)
{
	if (Player.Get_Mesh_Bone()->Get_NowPlayAniIndex() == CPlayer::PLAYER_ANI::Hit)
	{
		if (Player.Get_Mesh_Bone()->IsAnimationEnded(0.9f))
		{
			m_iAniState = 2;
			return CPlayer_Idle::Create(m_pGraphic_Device, Player, &m_iAniState);
		}
	}

	return nullptr;
}

void CPlayer_HitRolling::Update_State(CPlayer & Player, const float & fTimeDelta)
{
}

CPlayer_HitRolling * CPlayer_HitRolling::Create(LPDIRECT3DDEVICE9 pGraphicDevice, CPlayer & Player, void * pArg)
{
	CPlayer_HitRolling* pInstance = new CPlayer_HitRolling(pGraphicDevice);

	if (FAILED(pInstance->Initialize_State(Player)))
		Safe_Release(pInstance);

	return pInstance;
}

void CPlayer_HitRolling::Free()
{
	CPlayerState::Free();
}
