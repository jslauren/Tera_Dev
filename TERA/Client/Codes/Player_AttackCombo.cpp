#include "stdafx.h"
#include "..\Headers\Player_AttackCombo.h"
#include "Player.h"
#include "Input_Device.h"

#include "Player_Idle.h"
#include "Player_AttackCombo_R.h"

_USING(Client)

CPlayer_AttackCombo::CPlayer_AttackCombo(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CPlayerState(pGraphic_Device)
{
}

HRESULT CPlayer_AttackCombo::Initialize_State(CPlayer & Player)
{
	Player.Set_ActionID(CPlayer::ACTION_ID::ACTION_ATTACK);

	return NOERROR;
}

CPlayerState * CPlayer_AttackCombo::Input_Keyboard(CPlayer & Player, const float & fTimeDelta, BYTE KeyID, void * pAgr)
{
	if (CInput_Device::GetInstance()->Get_DIMouseDown(CInput_Device::MOUSEBUTTON::DIM_LBUTTON))
	{
		if (Player.Get_Mesh()->Get_NowPlayAniIndex() == CPlayer::PLAYER_ANI::COMBO1)
		{
			if (Player.Get_Mesh()->IsAnimationEnded(0.65f))
			{
				m_iAniState = 2;
				return CPlayer_AttackCombo::Create(m_pGraphic_Device, Player, &m_iAniState);
			}
		}
		else if (Player.Get_Mesh()->Get_NowPlayAniIndex() == CPlayer::PLAYER_ANI::COMBO2)
		{
			if (Player.Get_Mesh()->IsAnimationEnded(0.65f))
			{
				m_iAniState = 3;
				return CPlayer_AttackCombo::Create(m_pGraphic_Device, Player, &m_iAniState);
			}
		}
		else if (Player.Get_Mesh()->Get_NowPlayAniIndex() == CPlayer::PLAYER_ANI::COMBO3)
		{
			if (Player.Get_Mesh()->IsAnimationEnded(0.65f))
			{
				m_iAniState = 4;
				return CPlayer_AttackCombo::Create(m_pGraphic_Device, Player, &m_iAniState);
			}
		}
		else if (Player.Get_Mesh()->Get_NowPlayAniIndex() == CPlayer::PLAYER_ANI::COMBO4)
		{
			if (Player.Get_Mesh()->IsAnimationEnded(0.85f))
			{
				m_iAniState = 2;
				return CPlayer_Idle::Create(m_pGraphic_Device, Player, &m_iAniState);
			}
		}
	}

	else // 아무 키도 안눌렸을 때
	{
		if (Player.Get_Mesh()->Get_NowPlayAniIndex() == CPlayer::PLAYER_ANI::COMBO1)
		{
			MovePlayerPosition(Player, 0.33f, 10.f, fTimeDelta, pAgr);

			if (Player.Get_Mesh()->IsAnimationEnded(0.8f))
			{
				m_iAniState = 1;
				return CPlayer_AttackCombo_R::Create(m_pGraphic_Device, Player, &m_iAniState);
			}
		}
		else if (Player.Get_Mesh()->Get_NowPlayAniIndex() == CPlayer::PLAYER_ANI::COMBO2)
		{
			MovePlayerPosition(Player, 0.1f, 15.f, fTimeDelta, pAgr);

			if (Player.Get_Mesh()->IsAnimationEnded(0.8f))
			{
				m_iAniState = 2;
				return CPlayer_AttackCombo_R::Create(m_pGraphic_Device, Player, &m_iAniState);
			}
		}
		else if (Player.Get_Mesh()->Get_NowPlayAniIndex() == CPlayer::PLAYER_ANI::COMBO3)
		{
			MovePlayerPosition(Player, 0.1f, 15.f, fTimeDelta, pAgr);

			if (Player.Get_Mesh()->IsAnimationEnded(0.99f))
			{
				m_iAniState = 3;
				return CPlayer_AttackCombo_R::Create(m_pGraphic_Device, Player, &m_iAniState);
			}
		}
		else if (Player.Get_Mesh()->Get_NowPlayAniIndex() == CPlayer::PLAYER_ANI::COMBO4)
		{
			MovePlayerPosition(Player, 0.1f, 15.f, fTimeDelta, pAgr);

			if (Player.Get_Mesh()->IsAnimationEnded(0.85f))
			{
				m_iAniState = 2;
				return CPlayer_Idle::Create(m_pGraphic_Device, Player, &m_iAniState);
			}
		}
	}
	
	return nullptr;
}

void CPlayer_AttackCombo::Update_State(CPlayer & Player, const float & fTimeDelta)
{

}

void CPlayer_AttackCombo::MovePlayerPosition(CPlayer & Player, _float fPointTime, _float fPlayerSpeed, const float& fTimeDelta, void * pArg)
{
	if (Player.Get_Mesh()->IsAnimationEnded(fPointTime))
	{
		_uint		iCellIndx = 0;
		if (true == ((CNavigation*)(pArg))->Move_OnNavigation(Player.Get_Transform()->Get_StateInfo(CTransform::STATE_POSITION), Player.Get_Transform()->Get_StateInfo(CTransform::STATE_LOOK), 30.0f * fTimeDelta, &iCellIndx))
		{
			Player.Get_TransformMove()->Move(0, fPlayerSpeed, fTimeDelta);

			/* ※※※※※※※진짜 이동하면 꼭 호출해야합니다※※※※※※.*/
			((CNavigation*)(pArg))->SetUp_CurrentIndex(iCellIndx);
		}
	}
}

CPlayer_AttackCombo * CPlayer_AttackCombo::Create(LPDIRECT3DDEVICE9 pGraphicDevice, CPlayer & Player, void * pArg)
{
	CPlayer_AttackCombo* pInstance = new CPlayer_AttackCombo(pGraphicDevice);

	if (*(_int*)(pArg) == 1)
		Player.Set_AniIndex(CPlayer::PLAYER_ANI::COMBO1);

	else if (*(_int*)(pArg) == 2)
		Player.Set_AniIndex(CPlayer::PLAYER_ANI::COMBO2);

	else if (*(_int*)(pArg) == 3)
		Player.Set_AniIndex(CPlayer::PLAYER_ANI::COMBO3);

	else if (*(_int*)(pArg) == 4)
		Player.Set_AniIndex(CPlayer::PLAYER_ANI::COMBO4);

	if (FAILED(pInstance->Initialize_State(Player)))
		Safe_Release(pInstance);

	return pInstance;
}

void CPlayer_AttackCombo::Free()
{
	CPlayerState::Free();
}
