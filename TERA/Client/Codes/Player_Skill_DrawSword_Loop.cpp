#include "stdafx.h"
#include "..\Headers\Player_Skill_DrawSword_Loop.h"
#include "Player.h"
#include "Input_Device.h"

#include "Player_Skill_DrawSword.h"
#include "Player_Skill_DrawSword_Move.h"

_USING(Client)

CPlayer_Skill_DrawSword_Loop::CPlayer_Skill_DrawSword_Loop(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CPlayerState(pGraphic_Device)
{
}

HRESULT CPlayer_Skill_DrawSword_Loop::Initialize_State(CPlayer & Player)
{
	Player.Set_AniIndex(CPlayer::PLAYER_ANI::DRAWSWORDLOOP);
	Player.Set_ActionID(CPlayer::ACTION_ID::ACTION_ATTACK);

	return NOERROR;
}

CPlayerState * CPlayer_Skill_DrawSword_Loop::Input_Keyboard(CPlayer & Player, const float & fTimeDelta, BYTE KeyID, void * pArg)
{
	if (Player.Get_Mesh()->Get_NowPlayAniIndex() == CPlayer::PLAYER_ANI::DRAWSWORDLOOP)
	{
		if (CInput_Device::GetInstance()->GetDIMouseState(CInput_Device::MOUSEBUTTON::DIM_LBUTTON))
		{
			if (CInput_Device::GetInstance()->Get_DIKeyDown(DIK_W) ||
				CInput_Device::GetInstance()->Get_DIKeyDown(DIK_S) ||
				CInput_Device::GetInstance()->Get_DIKeyDown(DIK_A) ||
				CInput_Device::GetInstance()->Get_DIKeyDown(DIK_D))
			{
				return CPlayer_Skill_DrawSword_Move::Create(m_pGraphic_Device, Player, &m_iAniState);
			}
			else
				return nullptr;
		}

		else if (CInput_Device::GetInstance()->Get_DIMouseUp(CInput_Device::MOUSEBUTTON::DIM_LBUTTON))
		{
			if (Player.Get_Mesh()->IsAnimationEnded(0.75f));
				return CPlayer_Skill_DrawSword::Create(m_pGraphic_Device, Player, &m_iAniState);
		}
	}
	return nullptr;
}

void CPlayer_Skill_DrawSword_Loop::Update_State(CPlayer & Player, const float & fTimeDelta)
{
}

void CPlayer_Skill_DrawSword_Loop::MovePlayerPosition(CPlayer & Player, _float fPlayerSpeed, const float & fTimeDelta, void * pArg, _int iMoveDir)
{
	_uint		iCellIndx = 0;
	if (true == ((CNavigation*)(pArg))->Move_OnNavigation(Player.Get_Transform()->Get_StateInfo(CTransform::STATE_POSITION), Player.Get_Transform()->Get_StateInfo(CTransform::STATE_LOOK), 30.0f * fTimeDelta, &iCellIndx))
	{
		Player.Get_TransformMove()->Move(iMoveDir, fPlayerSpeed, fTimeDelta);

		/* ※※※※※※※진짜 이동하면 꼭 호출해야합니다※※※※※※.*/
		((CNavigation*)(pArg))->SetUp_CurrentIndex(iCellIndx);
	}
}

CPlayer_Skill_DrawSword_Loop * CPlayer_Skill_DrawSword_Loop::Create(LPDIRECT3DDEVICE9 pGraphicDevice, CPlayer & Player, void * pArg)
{
	CPlayer_Skill_DrawSword_Loop* pInstance = new CPlayer_Skill_DrawSword_Loop(pGraphicDevice);

	if (FAILED(pInstance->Initialize_State(Player)))
		Safe_Release(pInstance);

	return pInstance;
}

void CPlayer_Skill_DrawSword_Loop::Free()
{
	CPlayerState::Free();
}
