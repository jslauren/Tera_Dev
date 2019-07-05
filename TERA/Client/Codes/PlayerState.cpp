#include "stdafx.h"
#include "..\Headers\PlayerState.h"
#include "UI_DamageFont_Manager.h"

_USING(Client)

CPlayerState::CPlayerState(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device(pGraphic_Device)
{
	Safe_AddRef(m_pGraphic_Device);
}

HRESULT CPlayerState::Initialize_State(CPlayer & Player)
{
	return NOERROR;
}

CPlayerState * CPlayerState::Input_Keyboard(CPlayer & Player, const float & fTimeDelta, BYTE KeyID, void* pAgr)
{
	return nullptr;
}

void CPlayerState::Update_State(CPlayer & Player, const float & fTimeDelta)
{
}

HRESULT CPlayerState::Enter_State(CPlayer & Player)
{
	return NOERROR;
}

void CPlayerState::AttackAvailableCheck(CArkus* pArkus, CPlayer* Player)
{
	//if (pArkus != nullptr)
	//{
	//	if (pArkus->CollisionCheck() == true)
	//	{
	//		Player->DamageCalculator(eAttackAni);
	//		if (m_bIsDamageAvailable == true)
	//		{
	//			pArkus->Set_HP_Sub(Player->Get_PlayerOffenceValue());
	//			m_bIsDamageAvailable = false;
	//		}
	//	}
	//}

	if (pArkus != nullptr)
	{
		if (pArkus->CollisionCheck() == true)
		{
			Player->DamageCalculator((CPlayer::PLAYER_ANI)Player->Get_Mesh_Bone()->Get_NowPlayAniIndex());

			if (m_iHitCount < m_iAvailableHitNumber)
				m_bIsDamageAvailable = true;
		}
	}
}

void CPlayerState::AttackEvent(CArkus* pArkus, CPlayer* Player, _uint iAvailableHitNumber)
{
	if (pArkus != nullptr)
	{
		m_iAvailableHitNumber = iAvailableHitNumber;

		if (m_bIsDamageAvailable == true)
		{
			pArkus->Set_HP_Sub((_float)Player->Get_PlayerOffenceValue());
			
			CUI_DamageFont_Manager::GetInstance()->Create_DamageFont(m_pGraphic_Device, *Player->Get_Collider()->Get_CollisionPos(), (_float)Player->Get_PlayerOffenceValue());
			m_iHitCount++;

			m_bIsDamageAvailable = false;
		}
	}
}

void CPlayerState::AttackEventFree(CPlayer* Player, CPlayer::PLAYER_ANI ePlayerAni)
{
	if(ePlayerAni != CPlayer::PLAYER_ANI::END)
		Player->Set_SkillAvailable(ePlayerAni, true);

	Player->Set_LBtnClickedInfo(false);
	m_bIsDamageAvailable = false;
	m_iHitCount = 0;
}

void CPlayerState::Free()
{
	Safe_Release(m_pGraphic_Device);
}
