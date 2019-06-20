#include "stdafx.h"
#include "..\Headers\Player_Skill_CutHead.h"
#include "Player.h"
#include "Input_Device.h"

#include "Player_Idle.h"

_USING(Client)

CPlayer_Skill_CutHead::CPlayer_Skill_CutHead(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CPlayerState(pGraphic_Device)
{
}

HRESULT CPlayer_Skill_CutHead::Initialize_State(CPlayer & Player)
{
	Player.Set_AniIndex(CPlayer::PLAYER_ANI::CutHead);
	Player.Set_ActionID(CPlayer::ACTION_ID::ACTION_ATTACK);

	return NOERROR;
}

CPlayerState * CPlayer_Skill_CutHead::Input_Keyboard(CPlayer & Player, const float & fTimeDelta, BYTE KeyID, void * pArg)
{
	if (Player.Get_Mesh_Bone()->Get_NowPlayAniIndex() == CPlayer::PLAYER_ANI::CutHead)
	{
		if (Player.Get_Mesh_Bone()->IsAnimationEnded(0.4f))
			MovePlayerPosition(Player, 10.f, fTimeDelta, pArg, 0);

		if (Player.Get_Mesh_Bone()->IsAnimationEnded(0.63f))
			MovePlayerPosition(Player, -10.f, fTimeDelta, pArg, 0);

		if (Player.Get_Mesh_Bone()->IsAnimationEnded(0.9f))
		{
			m_iAniState = 2;
			return CPlayer_Idle::Create(m_pGraphic_Device, Player, &m_iAniState);
		}
	}

	return nullptr;
}

void CPlayer_Skill_CutHead::Update_State(CPlayer & Player, const float & fTimeDelta)
{
}

void CPlayer_Skill_CutHead::MovePlayerPosition(CPlayer & Player, _float fPlayerSpeed, const float & fTimeDelta, void * pArg, _int iMoveDir)
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

CPlayer_Skill_CutHead * CPlayer_Skill_CutHead::Create(LPDIRECT3DDEVICE9 pGraphicDevice, CPlayer & Player, void * pArg)
{
	CPlayer_Skill_CutHead* pInstance = new CPlayer_Skill_CutHead(pGraphicDevice);

	if (FAILED(pInstance->Initialize_State(Player)))
		Safe_Release(pInstance);

	return pInstance;
}

void CPlayer_Skill_CutHead::Free()
{
	CPlayerState::Free();
}
