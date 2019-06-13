#include "stdafx.h"
#include "..\Headers\Player_Skill_GaiaCrush.h"
#include "Player.h"
#include "Input_Device.h"

#include "Player_Idle.h"

_USING(Client)

CPlayer_Skill_GaiaCrush::CPlayer_Skill_GaiaCrush(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CPlayerState(pGraphic_Device)
{
}

HRESULT CPlayer_Skill_GaiaCrush::Initialize_State(CPlayer & Player)
{
	Player.Set_ActionID(CPlayer::ACTION_ID::ACTION_ATTACK);

	return NOERROR;
}

CPlayerState * CPlayer_Skill_GaiaCrush::Input_Keyboard(CPlayer & Player, const float & fTimeDelta, BYTE KeyID, void * pArg)
{
	if (Player.Get_Mesh()->Get_NowPlayAniIndex() == CPlayer::PLAYER_ANI::GAIACRUSH01)
	{
		if (Player.Get_Mesh()->IsAnimationEnded(0.65f))
		{
			m_iAniState = 2;
			return CPlayer_Skill_GaiaCrush::Create(m_pGraphic_Device, Player, &m_iAniState);
		}
	}
	else if (Player.Get_Mesh()->Get_NowPlayAniIndex() == CPlayer::PLAYER_ANI::GAIACRUSH02)
	{
		if (Player.Get_Mesh()->IsAnimationEnded(0.65f))
		{
			m_iAniState = 3;
			return CPlayer_Skill_GaiaCrush::Create(m_pGraphic_Device, Player, &m_iAniState);
		}
	}
	else if (Player.Get_Mesh()->Get_NowPlayAniIndex() == CPlayer::PLAYER_ANI::GAIACRUSH03)
	{
		if (Player.Get_Mesh()->IsAnimationEnded(0.65f))
		{
			m_iAniState = 2;
			return CPlayer_Idle::Create(m_pGraphic_Device, Player, &m_iAniState);
		}
	}

	return nullptr;
}

void CPlayer_Skill_GaiaCrush::Update_State(CPlayer & Player, const float & fTimeDelta)
{
}

void CPlayer_Skill_GaiaCrush::MovePlayerPosition(CPlayer & Player, _float fPlayerSpeed, const float & fTimeDelta, void * pArg, _int iMoveDir)
{
	_uint		iCellIndx = 0;
	if (true == ((CNavigation*)(pArg))->Move_OnNavigation(Player.Get_Transform()->Get_StateInfo(CTransform::STATE_POSITION), Player.Get_Transform()->Get_StateInfo(CTransform::STATE_LOOK), 30.0f * fTimeDelta, &iCellIndx))
	{
		Player.Get_TransformMove()->Move(iMoveDir, fPlayerSpeed, fTimeDelta);

		/* ※※※※※※※진짜 이동하면 꼭 호출해야합니다※※※※※※.*/
		((CNavigation*)(pArg))->SetUp_CurrentIndex(iCellIndx);
	}
}

CPlayer_Skill_GaiaCrush * CPlayer_Skill_GaiaCrush::Create(LPDIRECT3DDEVICE9 pGraphicDevice, CPlayer & Player, void * pArg)
{
	CPlayer_Skill_GaiaCrush* pInstance = new CPlayer_Skill_GaiaCrush(pGraphicDevice);

	if (*(_int*)(pArg) == 1)
		Player.Set_AniIndex(CPlayer::PLAYER_ANI::GAIACRUSH01);

	else if (*(_int*)(pArg) == 2)
		Player.Set_AniIndex(CPlayer::PLAYER_ANI::GAIACRUSH02);

	else if (*(_int*)(pArg) == 3)
		Player.Set_AniIndex(CPlayer::PLAYER_ANI::GAIACRUSH03);

	if (FAILED(pInstance->Initialize_State(Player)))
		Safe_Release(pInstance);

	return pInstance;
}

void CPlayer_Skill_GaiaCrush::Free()
{
	CPlayerState::Free();
}
