#include "stdafx.h"
#include "..\Headers\Player_Skill_FlatBlade.h"
#include "Player.h"
#include "Input_Device.h"

#include "Player_Idle.h"
#include "Player_KnockDown.h"

_USING(Client)

Player_Skill_FlatBlade::Player_Skill_FlatBlade(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CPlayerState(pGraphic_Device)
{
}

HRESULT Player_Skill_FlatBlade::Initialize_State(CPlayer & Player)
{
	Player.Set_AniIndex(CPlayer::PLAYER_ANI::FlatBlade);
	Player.Set_ActionID(CPlayer::ACTION_ID::ACTION_ATTACK);

	return NOERROR;
}

CPlayerState * Player_Skill_FlatBlade::Input_Keyboard(CPlayer & Player, const float & fTimeDelta, BYTE KeyID, void * pArg)
{
	if (Player.Get_Mesh_Bone()->Get_NowPlayAniIndex() == CPlayer::PLAYER_ANI::FlatBlade)
	{
		if (Player.Get_Mesh_Bone()->IsAnimationEnded(0.05f))
			MovePlayerPosition(Player, 40.f, fTimeDelta, pArg, 0);

		if (Player.Get_Mesh_Bone()->IsAnimationEnded(0.43f))
			MovePlayerPosition(Player, -38.f, fTimeDelta, pArg, 0);

		if (Player.Get_Mesh_Bone()->IsAnimationEnded(0.9f))
		{
			if (Player.CollisionCheck() == true)
				return CPlayer_KnockDown::Create(m_pGraphic_Device, Player, &m_iAniState);

			else if (Player.CollisionCheck() == false)
			{
				m_iAniState = 2;
				return CPlayer_Idle::Create(m_pGraphic_Device, Player, &m_iAniState);
			}
		}
	}

	return nullptr;
}

void Player_Skill_FlatBlade::Update_State(CPlayer & Player, const float & fTimeDelta)
{
}

void Player_Skill_FlatBlade::MovePlayerPosition(CPlayer & Player, _float fPlayerSpeed, const float & fTimeDelta, void * pArg, _int iMoveDir)
{
	if (Player.Get_CollisionCheckWhether() == false)
	{
		_uint		iCellIndx = 0;
		if (true == ((CNavigation*)(pArg))->Move_OnNavigation(Player.Get_Transform()->Get_StateInfo(CTransform::STATE_POSITION), Player.Get_Transform()->Get_StateInfo(CTransform::STATE_LOOK), 30.0f * fTimeDelta, &iCellIndx))
		{
			Player.Get_TransformMove()->Move(Player.Get_Transform()->Get_StateInfo(CTransform::STATE_LOOK), fPlayerSpeed, fTimeDelta);

			/* ※※※※※※※진짜 이동하면 꼭 호출해야합니다※※※※※※.*/
			((CNavigation*)(pArg))->SetUp_CurrentIndex(iCellIndx);
		}
	}
}

Player_Skill_FlatBlade * Player_Skill_FlatBlade::Create(LPDIRECT3DDEVICE9 pGraphicDevice, CPlayer & Player, void * pArg)
{
	Player_Skill_FlatBlade* pInstance = new Player_Skill_FlatBlade(pGraphicDevice);

	if (FAILED(pInstance->Initialize_State(Player)))
		Safe_Release(pInstance);

	return pInstance;
}

void Player_Skill_FlatBlade::Free()
{
	CPlayerState::Free();
}
