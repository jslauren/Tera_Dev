#include "stdafx.h"
#include "..\Headers\Player_Wake.h"
#include "Player.h"
#include "Input_Device.h"

#include "Player_Idle.h"

_USING(Client)

CPlayer_Wake::CPlayer_Wake(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CPlayerState(pGraphic_Device)
{
}

HRESULT CPlayer_Wake::Initialize_State(CPlayer & Player)
{
	Player.Set_AniIndex(CPlayer::PLAYER_ANI::Hit_Wake);
	Player.Set_ActionID(CPlayer::ACTION_ID::ACTION_READY);

	return NOERROR;
}

CPlayerState * CPlayer_Wake::Input_Keyboard(CPlayer & Player, const float & fTimeDelta, BYTE KeyID, void * pArg)
{
	if (Player.Get_Mesh_Bone()->Get_NowPlayAniIndex() == CPlayer::PLAYER_ANI::Hit_Wake)
	{
		if (Player.Get_Mesh_Bone()->IsAnimationEnded(0.85f))
		{
			m_iAniState = 2;
			return CPlayer_Idle::Create(m_pGraphic_Device, Player, &m_iAniState);
		}
	}

	return nullptr;
}

void CPlayer_Wake::Update_State(CPlayer & Player, const float & fTimeDelta)
{
}

CPlayer_Wake * CPlayer_Wake::Create(LPDIRECT3DDEVICE9 pGraphicDevice, CPlayer & Player, void * pArg)
{
	CPlayer_Wake* pInstance = new CPlayer_Wake(pGraphicDevice);

	if (FAILED(pInstance->Initialize_State(Player)))
		Safe_Release(pInstance);

	return pInstance;
}

void CPlayer_Wake::Free()
{
	CPlayerState::Free();
}
