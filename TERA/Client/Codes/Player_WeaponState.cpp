#include "stdafx.h"
#include "..\Headers\Player_WeaponState.h"
#include "Player.h"
#include "Input_Device.h"
#include "Weapon.h"

#include "Player_Idle.h"

_USING(Client)

CPlayer_WeaponState::CPlayer_WeaponState(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CPlayerState(pGraphic_Device)
{
}

HRESULT CPlayer_WeaponState::Initialize_State(CPlayer & Player)
{
	//Player.Set_AniIndex(CPlayer::PLAYER_STATE::OUTWEAPON);
	Player.Set_ActionID(CPlayer::ACTION_ID::ACTION_READY);

	return NOERROR;
}

CPlayerState * CPlayer_WeaponState::Input_Keyboard(CPlayer & Player, const float & fTimeDelta, BYTE KeyID, void * pArg)
{
	if (Player.Get_Mesh_Bone()->Get_NowPlayAniIndex() == CPlayer::PLAYER_ANI::OutWeapon)
	{
		if (Player.Get_Mesh_Bone()->IsAnimationEnded(0.41f))
			dynamic_cast<CWeapon*>(CObject_Manager::GetInstance()->Get_Object(SCENE_STATIC, L"Layer_Weapon", -1))->Set_BoneMatrix(2);

		if (Player.Get_Mesh_Bone()->IsAnimationEnded(0.8))
		{
			m_iAniState = 2;
			return CPlayer_Idle::Create(m_pGraphic_Device, Player, &m_iAniState);
			//return nullptr;
		}
	}
	else if(Player.Get_Mesh_Bone()->Get_NowPlayAniIndex() == CPlayer::PLAYER_ANI::InWeapon)
	{
		if (Player.Get_Mesh_Bone()->IsAnimationEnded(0.6f))
			dynamic_cast<CWeapon*>(CObject_Manager::GetInstance()->Get_Object(SCENE_STATIC, L"Layer_Weapon", -1))->Set_BoneMatrix(1);

		if (Player.Get_Mesh_Bone()->IsAnimationEnded(0.7f))
		{
			m_iAniState = 1;
			return CPlayer_Idle::Create(m_pGraphic_Device, Player, &m_iAniState);
			//return nullptr;
		}
	}

	return nullptr;
}

void CPlayer_WeaponState::Update_State(CPlayer & Player, const float & fTimeDelta)
{
}

CPlayer_WeaponState * CPlayer_WeaponState::Create(LPDIRECT3DDEVICE9 pGraphicDevice, CPlayer & Player, void* pArg)
{
	CPlayer_WeaponState* pInstance = new CPlayer_WeaponState(pGraphicDevice);

	if (*(_int*)(pArg) == 1)
		Player.Set_AniIndex(CPlayer::PLAYER_ANI::OutWeapon);
	else if ((*(_int*)(pArg) == 2))
		Player.Set_AniIndex(CPlayer::PLAYER_ANI::InWeapon);

	if (FAILED(pInstance->Initialize_State(Player)))
		Safe_Release(pInstance);

	return pInstance;
}

void CPlayer_WeaponState::Free()
{
	CPlayerState::Free();
}
