#include "stdafx.h"
#include "..\Headers\Player_KnockDown.h"
#include "Player.h"
#include "Input_Device.h"

#include "Player_Wake.h"

_USING(Client)

CPlayer_KnockDown::CPlayer_KnockDown(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CPlayerState(pGraphic_Device)
{
}

HRESULT CPlayer_KnockDown::Initialize_State(CPlayer & Player)
{
	Player.Set_AniIndex(CPlayer::PLAYER_ANI::Hit_Land);

	return NOERROR;
}

CPlayerState * CPlayer_KnockDown::Input_Keyboard(CPlayer & Player, const float & fTimeDelta, BYTE KeyID, void * pArg)
{
	if (Player.Get_Mesh_Bone()->Get_NowPlayAniIndex() == CPlayer::PLAYER_ANI::Hit_Land)
	{
		if (Player.Get_Mesh_Bone()->IsAnimationEnded(0.85f))
			return CPlayer_Wake::Create(m_pGraphic_Device, Player, &m_iAniState);
	}

	return nullptr;
}

void CPlayer_KnockDown::Update_State(CPlayer & Player, const float & fTimeDelta)
{
}

CPlayer_KnockDown * CPlayer_KnockDown::Create(LPDIRECT3DDEVICE9 pGraphicDevice, CPlayer & Player, void * pArg)
{
	CPlayer_KnockDown* pInstance = new CPlayer_KnockDown(pGraphicDevice);

	if (FAILED(pInstance->Initialize_State(Player)))
		Safe_Release(pInstance);

	return pInstance;
}

void CPlayer_KnockDown::Free()
{
	CPlayerState::Free();
}
