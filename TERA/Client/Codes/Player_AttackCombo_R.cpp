#include "stdafx.h"
#include "..\Headers\Player_AttackCombo_R.h"
#include "Player.h"
#include "Input_Device.h"

#include "Player_Idle.h"

_USING(Client)

CPlayer_AttackCombo_R::CPlayer_AttackCombo_R(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CPlayerState(pGraphic_Device)
{
}

HRESULT CPlayer_AttackCombo_R::Initialize_State(CPlayer & Player)
{
	return NOERROR;
}

CPlayerState * CPlayer_AttackCombo_R::Input_Keyboard(CPlayer & Player, const float & fTimeDelta, BYTE KeyID, void * pAgr)
{
	if (Player.Get_Mesh_Bone()->IsAnimationEnded(0.8f))
	{
		m_iAniState = 2;
		return CPlayer_Idle::Create(m_pGraphic_Device, Player, &m_iAniState);
	}

	return nullptr;
}

void CPlayer_AttackCombo_R::Update_State(CPlayer & Player, const float & fTimeDelta)
{
}

CPlayer_AttackCombo_R * CPlayer_AttackCombo_R::Create(LPDIRECT3DDEVICE9 pGraphicDevice, CPlayer & Player, void * pArg)
{
	CPlayer_AttackCombo_R* pInstance = new CPlayer_AttackCombo_R(pGraphicDevice);

	if (*(_int*)(pArg) == 1)
		Player.Set_AniIndex(CPlayer::PLAYER_ANI::Combo1R);

	else if (*(_int*)(pArg) == 2)
		Player.Set_AniIndex(CPlayer::PLAYER_ANI::Combo2R);

	else if (*(_int*)(pArg) == 3)
		Player.Set_AniIndex(CPlayer::PLAYER_ANI::Combo3R);

	//else if (*(_int*)(pArg) == 4)
	//	Player.Set_AniIndex(CPlayer::PLAYER_ANI::R05UNARMEDWAIT);

	if (FAILED(pInstance->Initialize_State(Player)))
		Safe_Release(pInstance);

	return pInstance;
}

void CPlayer_AttackCombo_R::Free()
{
	CPlayerState::Free();
}
