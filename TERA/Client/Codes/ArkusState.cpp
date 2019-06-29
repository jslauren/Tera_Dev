#include "stdafx.h"
#include "..\Headers\ArkusState.h"

_USING(Client)

CArkusState::CArkusState(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device(pGraphic_Device)
{
	Safe_AddRef(m_pGraphic_Device);
}

HRESULT CArkusState::Initialize_State(CArkus & Player)
{
	return NOERROR;
}

CArkusState * CArkusState::Input_State(CArkus & Player, const float & fTimeDelta, BYTE KeyID, void * pAgr)
{
	return nullptr;
}

void CArkusState::Update_State(CArkus & Player, const float & fTimeDelta)
{
}

HRESULT CArkusState::Enter_State(CArkus & Player)
{
	return NOERROR;
}

void CArkusState::AttackAvailableCheck(CPlayer * pPlayer, CArkus * pArkus)
{
	if (pPlayer->CollisionCheck() == true)
	{
	//	Player->DamageCalculator((CPlayer::PLAYER_ANI)Player->Get_Mesh_Bone()->Get_NowPlayAniIndex());
		if (pPlayer->Get_HitCount() < m_iAvailableHitNumber)
			m_bIsDamageAvailable = true;
	}
}

void CArkusState::AttackEvent(CPlayer * pPlayer, CArkus * pArkus, _uint iAvailableHitNumber, _uint iAttackNumber)
{
	m_iAvailableHitNumber = iAvailableHitNumber;

	if (m_bIsDamageAvailable == true)
	{
		CEventManager::GetInstance()->Notify_Event(L"Arkus_Attack", &iAttackNumber);

		pPlayer->Set_AddHitCount();

		m_bIsDamageAvailable = false;
	}
}

void CArkusState::AttackEventFree(CPlayer* pPlayer, CArkus * pArkus)
{
	m_bIsDamageAvailable = false;
	pPlayer->Set_FreeHitCount();
}

void CArkusState::Free()
{
	Safe_Release(m_pGraphic_Device);
}
