#include "stdafx.h"
#include "..\Headers\Player_Skill_Tumbling.h"
#include "Player.h"
#include "Input_Device.h"

#include "Player_Idle.h"

#define		PLAYER_SPEED 20.f

_USING(Client)

CPlayer_Skill_Tumbling::CPlayer_Skill_Tumbling(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CPlayerState(pGraphic_Device)
{
}

HRESULT CPlayer_Skill_Tumbling::Initialize_State(CPlayer & Player)
{
	Player.Set_AniIndex(CPlayer::PLAYER_ANI::Tumbling);

	return NOERROR;
}

CPlayerState * CPlayer_Skill_Tumbling::Input_Keyboard(CPlayer & Player, const float & fTimeDelta, BYTE KeyID, void * pArg)
{
	if (Player.Get_Mesh_Bone()->Get_NowPlayAniIndex() == CPlayer::PLAYER_ANI::Tumbling)
	{
		if (Player.Get_Mesh_Bone()->IsAnimationEnded(0.75f))
		{
			m_iAniState = 2;
			return CPlayer_Idle::Create(m_pGraphic_Device, Player, &m_iAniState);
		}
	}

	return nullptr;
}

void CPlayer_Skill_Tumbling::Update_State(CPlayer & Player, const float & fTimeDelta)
{
	if (Player.Get_Direction() == 0)
	{
			_uint		iCellIndx = 0;
			if (true == (Player.Get_NaviMesh()->Move_OnNavigation(Player.Get_Transform()->Get_StateInfo(CTransform::STATE_POSITION), Player.Get_Transform()->Get_StateInfo(CTransform::STATE_LOOK), PLAYER_SPEED * fTimeDelta, &iCellIndx)))
			{
				Player.Get_TransformMove()->Move(0, PLAYER_SPEED, fTimeDelta * 1.8);

				/* ※※※※※※※진짜 이동하면 꼭 호출해야합니다※※※※※※.*/
				(Player.Get_NaviMesh()->SetUp_CurrentIndex(iCellIndx));
			}
	}
	else if (Player.Get_Direction() == 1)
	{
		_uint		iCellIndx = 0;
		if (true == (Player.Get_NaviMesh()->Move_OnNavigation(Player.Get_Transform()->Get_StateInfo(CTransform::STATE_POSITION), Player.Get_Transform()->Get_StateInfo(CTransform::STATE_LOOK), PLAYER_SPEED * fTimeDelta, &iCellIndx)))
		{
			Player.Get_TransformMove()->Move(1, PLAYER_SPEED, fTimeDelta * 1.8);

			/* ※※※※※※※진짜 이동하면 꼭 호출해야합니다※※※※※※.*/
			(Player.Get_NaviMesh()->SetUp_CurrentIndex(iCellIndx));
		}
	}
	else if (Player.Get_Direction() == 2)
	{
		_uint		iCellIndx = 0;
		if (true == (Player.Get_NaviMesh()->Move_OnNavigation(Player.Get_Transform()->Get_StateInfo(CTransform::STATE_POSITION), Player.Get_Transform()->Get_StateInfo(CTransform::STATE_LOOK), PLAYER_SPEED * fTimeDelta, &iCellIndx)))
		{
			Player.Get_TransformMove()->Move(2, PLAYER_SPEED, fTimeDelta * 1.8);

			/* ※※※※※※※진짜 이동하면 꼭 호출해야합니다※※※※※※.*/
			(Player.Get_NaviMesh()->SetUp_CurrentIndex(iCellIndx));
		}
	}
	else if (Player.Get_Direction() == 3)
	{
		_uint		iCellIndx = 0;
		if (true == (Player.Get_NaviMesh()->Move_OnNavigation(Player.Get_Transform()->Get_StateInfo(CTransform::STATE_POSITION), Player.Get_Transform()->Get_StateInfo(CTransform::STATE_LOOK), PLAYER_SPEED * fTimeDelta, &iCellIndx)))
		{
			Player.Get_TransformMove()->Move(3, PLAYER_SPEED, fTimeDelta * 1.8);

			/* ※※※※※※※진짜 이동하면 꼭 호출해야합니다※※※※※※.*/
			(Player.Get_NaviMesh()->SetUp_CurrentIndex(iCellIndx));
		}
	}
}

void CPlayer_Skill_Tumbling::MovePlayerPosition(CPlayer & Player, _float fPlayerSpeed, const float & fTimeDelta, void * pArg, _int iMoveDir)
{
	_uint		iCellIndx = 0;
	if (true == ((CNavigation*)(pArg))->Move_OnNavigation(Player.Get_Transform()->Get_StateInfo(CTransform::STATE_POSITION), Player.Get_Transform()->Get_StateInfo(CTransform::STATE_LOOK), 30.0f * fTimeDelta, &iCellIndx))
	{
		Player.Get_TransformMove()->Move(iMoveDir, fPlayerSpeed, fTimeDelta);

		/* ※※※※※※※진짜 이동하면 꼭 호출해야합니다※※※※※※.*/
		((CNavigation*)(pArg))->SetUp_CurrentIndex(iCellIndx);
	}
}

CPlayer_Skill_Tumbling * CPlayer_Skill_Tumbling::Create(LPDIRECT3DDEVICE9 pGraphicDevice, CPlayer & Player, void * pArg)
{
	CPlayer_Skill_Tumbling* pInstance = new CPlayer_Skill_Tumbling(pGraphicDevice);

	if (*(_int*)(pArg) == 0)
		Player.Set_Direction(0);

	else if (*(_int*)(pArg) == 1)
		Player.Set_Direction(1);

	else if (*(_int*)(pArg) == 2)
		Player.Set_Direction(2);

	else if (*(_int*)(pArg) == 3)
		Player.Set_Direction(3);

	if (FAILED(pInstance->Initialize_State(Player)))
		Safe_Release(pInstance);

	return pInstance;
}

void CPlayer_Skill_Tumbling::Free()
{
	CPlayerState::Free();
}
