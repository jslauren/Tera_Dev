#include "stdafx.h"
#include "..\Headers\Player_HitHigh.h"
#include "Player.h"
#include "Input_Device.h"

#include "Player_Idle.h"

_USING(Client)

CPlayer_HitHigh::CPlayer_HitHigh(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CPlayerState(pGraphic_Device)
{
}

HRESULT CPlayer_HitHigh::Initialize_State(CPlayer & Player)
{
	Player.Set_AniIndex(CPlayer::PLAYER_ANI::Hit_High);

	return NOERROR;
}

CPlayerState * CPlayer_HitHigh::Input_Keyboard(CPlayer & Player, const float & fTimeDelta, BYTE KeyID, void * pArg)
{
	if (Player.Get_Mesh_Bone()->Get_NowPlayAniIndex() == CPlayer::PLAYER_ANI::Hit_High)
	{
		if (Player.Get_Mesh_Bone()->IsAnimationEnded(0.9f))
		{
			m_iAniState = 2;
			return CPlayer_Idle::Create(m_pGraphic_Device, Player, &m_iAniState);
		}
	}

	return nullptr;
}

void CPlayer_HitHigh::Update_State(CPlayer & Player, const float & fTimeDelta)
{
}

CPlayer_HitHigh * CPlayer_HitHigh::Create(LPDIRECT3DDEVICE9 pGraphicDevice, CPlayer & Player, void * pArg)
{
	CPlayer_HitHigh* pInstance = new CPlayer_HitHigh(pGraphicDevice);

	if (FAILED(pInstance->Initialize_State(Player)))
		Safe_Release(pInstance);

	return pInstance;
}

void CPlayer_HitHigh::Free()
{
	CPlayerState::Free();
}
