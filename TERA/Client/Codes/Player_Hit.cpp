#include "stdafx.h"
#include "..\Headers\Player_Hit.h"
#include "Player.h"
#include "Input_Device.h"

#include "Player_Idle.h"

_USING(Client)

CPlayer_Hit::CPlayer_Hit(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CPlayerState(pGraphic_Device)
{
}

HRESULT CPlayer_Hit::Initialize_State(CPlayer & Player)
{
	Player.Set_AniIndex(CPlayer::PLAYER_ANI::Hit);

	return NOERROR;
}

CPlayerState * CPlayer_Hit::Input_Keyboard(CPlayer & Player, const float & fTimeDelta, BYTE KeyID, void * pArg)
{
	if (Player.Get_Mesh_Bone()->Get_NowPlayAniIndex() == CPlayer::PLAYER_ANI::Hit)
	{
		if (Player.Get_Mesh_Bone()->IsAnimationEnded(0.2f))
			dynamic_cast<CWeapon*>(CObject_Manager::GetInstance()->Get_Object(SCENE_STATIC, L"Layer_Weapon", -1))->Set_BoneMatrix(3);

		if (Player.Get_Mesh_Bone()->IsAnimationEnded(0.35f))
		{
			m_iAniState = 2;
			return CPlayer_Idle::Create(m_pGraphic_Device, Player, &m_iAniState);
		}
	}

	return nullptr;
}

void CPlayer_Hit::Update_State(CPlayer & Player, const float & fTimeDelta)
{
}

CPlayer_Hit * CPlayer_Hit::Create(LPDIRECT3DDEVICE9 pGraphicDevice, CPlayer & Player, void * pArg)
{
	CPlayer_Hit* pInstance = new CPlayer_Hit(pGraphicDevice);

	if (FAILED(pInstance->Initialize_State(Player)))
		Safe_Release(pInstance);

	return pInstance;
}

void CPlayer_Hit::Free()
{
	CPlayerState::Free();
}
