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
			
			DamageFontUI(pArkus, Player);
			CheckCollisionPart(pArkus);

			// 평타 일때만 검상 이펙트 On, 스킬들은 추후에 다른 화려한 이펙트 추가하자.
			if (Player->Get_Mesh_Bone()->Get_NowPlayAniIndex() == CPlayer::PLAYER_ANI::Combo1 ||
				Player->Get_Mesh_Bone()->Get_NowPlayAniIndex() == CPlayer::PLAYER_ANI::Combo2 ||
				Player->Get_Mesh_Bone()->Get_NowPlayAniIndex() == CPlayer::PLAYER_ANI::Combo3 ||
				Player->Get_Mesh_Bone()->Get_NowPlayAniIndex() == CPlayer::PLAYER_ANI::Combo4)
			{
				SwordScarEffect(pArkus, Player);
			}

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

void CPlayerState::DamageFontUI(CArkus * pArkus, CPlayer* Player)
{
	CArkus::ARKUS_COLLISION eArkusCollisionPart = pArkus->Get_CurrentCollisionPart();

	CCollider* pCollider = dynamic_cast<CWeapon*>(CObject_Manager::GetInstance()->Get_Object(SCENE_STATIC, L"Layer_Weapon", 0))->Get_ColliderTop();

	Safe_AddRef(pCollider);

	_vec3 vColliderPos = pCollider->CalculateCollisionPos(pArkus->Check_ColliderParts(pArkus->Get_CurrentCollisionPart()));

	CUI_DamageFont_Manager::GetInstance()->Create_DamageFont(m_pGraphic_Device, vColliderPos, (_float)Player->Get_PlayerOffenceValue());
	m_iHitCount++;

	Safe_Release(pCollider);
}

void CPlayerState::CheckCollisionPart(CArkus * pArkus)
{
	CArkus::ARKUS_COLLISION eArkusCollisionPart = pArkus->Get_CurrentCollisionPart();

	CCollider* pCollider = dynamic_cast<CWeapon*>(CObject_Manager::GetInstance()->Get_Object(SCENE_STATIC, L"Layer_Weapon", 0))->Get_ColliderTop();

	Safe_AddRef(pCollider);

	_vec3 vColliderPos = pCollider->CalculateCollisionPos(pArkus->Check_ColliderParts(pArkus->Get_CurrentCollisionPart()));

	CObject_Manager::GetInstance()->Add_Object(SCENE_STATIC, L"GameObject_HitEffect", SCENE_STATIC, L"Layer_Effect", &vColliderPos);

	Safe_Release(pCollider);
}

void CPlayerState::SwordScarEffect(CArkus * pArkus, CPlayer* Player)
{
	CArkus::ARKUS_COLLISION eArkusCollisionPart = pArkus->Get_CurrentCollisionPart();

	CCollider* pCollider = dynamic_cast<CWeapon*>(CObject_Manager::GetInstance()->Get_Object(SCENE_STATIC, L"Layer_Weapon", 0))->Get_ColliderTop();

	Safe_AddRef(pCollider);

	_vec4 vColliderPos = pCollider->CalculateCollisionPos(pArkus->Check_ColliderParts(pArkus->Get_CurrentCollisionPart()));
	
	if (Player->Get_Mesh_Bone()->Get_NowPlayAniIndex() == CPlayer::PLAYER_ANI::Combo1 ||
		Player->Get_Mesh_Bone()->Get_NowPlayAniIndex() == CPlayer::PLAYER_ANI::Combo2)
		vColliderPos.w = 0;
	else if (Player->Get_Mesh_Bone()->Get_NowPlayAniIndex() == CPlayer::PLAYER_ANI::Combo3 ||
		Player->Get_Mesh_Bone()->Get_NowPlayAniIndex() == CPlayer::PLAYER_ANI::Combo4)
		vColliderPos.w = 1;

	CObject_Manager::GetInstance()->Add_Object(SCENE_STATIC, L"GameObject_SwordScarEffect", SCENE_STATIC, L"Layer_Effect", &vColliderPos);

	Safe_Release(pCollider);
}

void CPlayerState::Free()
{
	Safe_Release(m_pGraphic_Device);
}
