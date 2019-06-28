#include "stdafx.h"
#include "..\Headers\Player_AttackCombo.h"
#include "Player.h"
#include "Input_Device.h"
#include "Time.h"

#include "Player_Hit.h"
#include "Player_Idle.h"
#include "Player_AttackCombo_R.h"
#include "Player_KnockDown.h"

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
	CArkus*	pArkus = dynamic_cast<CArkus*>(CObject_Manager::GetInstance()->Get_Object(SCENE_DRAGON, L"Layer_Monster"));

	if (CInput_Device::GetInstance()->Get_DIMouseDown(CInput_Device::MOUSEBUTTON::DIM_LBUTTON))
	{
		if (Player.Get_Mesh_Bone()->Get_NowPlayAniIndex() == CPlayer::PLAYER_ANI::Combo1)
		{
			if (Player.Get_Mesh_Bone()->IsAnimationEnded(0.65f))
			{
				if (Player.CollisionCheck() == true)
				{
					if (pArkus->Get_AniIndex() == CArkus::ARKUS_ANI::RoundAtk01 ||
						pArkus->Get_AniIndex() == CArkus::ARKUS_ANI::RoundAtk02 ||
						pArkus->Get_AniIndex() == CArkus::ARKUS_ANI::FlyAtk02End ||
						pArkus->Get_AniIndex() == CArkus::ARKUS_ANI::MoveAtkEnd)
					{
						return CPlayer_KnockDown::Create(m_pGraphic_Device, Player, &m_iAniState);
					}
					else
						return CPlayer_Hit::Create(m_pGraphic_Device, Player, &m_iAniState);
				}

				else if (Player.CollisionCheck() == false)
				{
					m_iAniState = 2;
					return CPlayer_AttackCombo::Create(m_pGraphic_Device, Player, &m_iAniState);
				}
			}
		}
		else if (Player.Get_Mesh_Bone()->Get_NowPlayAniIndex() == CPlayer::PLAYER_ANI::Combo2)
		{
			Player.DamageCalculator(Player.Combo2);

			if (Player.Get_Mesh_Bone()->IsAnimationEnded(0.65f))
			{
				if (Player.CollisionCheck() == true)
				{
					if (pArkus->Get_AniIndex() == CArkus::ARKUS_ANI::RoundAtk01 ||
						pArkus->Get_AniIndex() == CArkus::ARKUS_ANI::RoundAtk02 ||
						pArkus->Get_AniIndex() == CArkus::ARKUS_ANI::FlyAtk02End ||
						pArkus->Get_AniIndex() == CArkus::ARKUS_ANI::MoveAtkEnd)
					{
						return CPlayer_KnockDown::Create(m_pGraphic_Device, Player, &m_iAniState);
					}
					else
						return CPlayer_Hit::Create(m_pGraphic_Device, Player, &m_iAniState);
				}

				else if (Player.CollisionCheck() == false)
				{
					m_iAniState = 3;
					return CPlayer_AttackCombo::Create(m_pGraphic_Device, Player, &m_iAniState);
				}
			}
		}
		else if (Player.Get_Mesh_Bone()->Get_NowPlayAniIndex() == CPlayer::PLAYER_ANI::Combo3)
		{
			Player.DamageCalculator(Player.Combo3);

			if (Player.Get_Mesh_Bone()->IsAnimationEnded(0.65f))
			{
				if (Player.CollisionCheck() == true)
				{
					if (pArkus->Get_AniIndex() == CArkus::ARKUS_ANI::RoundAtk01 ||
						pArkus->Get_AniIndex() == CArkus::ARKUS_ANI::RoundAtk02 ||
						pArkus->Get_AniIndex() == CArkus::ARKUS_ANI::FlyAtk02End ||
						pArkus->Get_AniIndex() == CArkus::ARKUS_ANI::MoveAtkEnd)
					{
						return CPlayer_KnockDown::Create(m_pGraphic_Device, Player, &m_iAniState);
					}
					else
						return CPlayer_Hit::Create(m_pGraphic_Device, Player, &m_iAniState);
				}

				else if (Player.CollisionCheck() == false)
				{
					m_iAniState = 4;
					return CPlayer_AttackCombo::Create(m_pGraphic_Device, Player, &m_iAniState);
				}
			}
		}
		else if (Player.Get_Mesh_Bone()->Get_NowPlayAniIndex() == CPlayer::PLAYER_ANI::Combo4)
		{
			Player.DamageCalculator(Player.Combo4);

			if (Player.Get_Mesh_Bone()->IsAnimationEnded(0.85f))
			{
				if (Player.CollisionCheck() == true)
				{
					if (pArkus->Get_AniIndex() == CArkus::ARKUS_ANI::RoundAtk01 ||
						pArkus->Get_AniIndex() == CArkus::ARKUS_ANI::RoundAtk02 ||
						pArkus->Get_AniIndex() == CArkus::ARKUS_ANI::FlyAtk02End ||
						pArkus->Get_AniIndex() == CArkus::ARKUS_ANI::MoveAtkEnd)
					{
						return CPlayer_KnockDown::Create(m_pGraphic_Device, Player, &m_iAniState);
					}
					else
						return CPlayer_Hit::Create(m_pGraphic_Device, Player, &m_iAniState);
				}

				else if (Player.CollisionCheck() == false)
				{
					m_iAniState = 2;
					return CPlayer_Idle::Create(m_pGraphic_Device, Player, &m_iAniState);
				}
			}
		}
	}

	else // 아무 키도 안눌렸을 때
	{
		if (Player.Get_Mesh_Bone()->Get_NowPlayAniIndex() == CPlayer::PLAYER_ANI::Combo1)
		{
			MovePlayerPosition(Player, 0.33f, 20.f, 0.002, pAgr);

			if (Player.Get_Mesh_Bone()->IsAnimationEnded(0.8f))
			{
				if (Player.CollisionCheck() == true)
				{
					if (pArkus->Get_AniIndex() == CArkus::ARKUS_ANI::RoundAtk01 ||
						pArkus->Get_AniIndex() == CArkus::ARKUS_ANI::RoundAtk02 ||
						pArkus->Get_AniIndex() == CArkus::ARKUS_ANI::FlyAtk02End ||
						pArkus->Get_AniIndex() == CArkus::ARKUS_ANI::MoveAtkEnd)
					{
						return CPlayer_KnockDown::Create(m_pGraphic_Device, Player, &m_iAniState);
					}
					else
						return CPlayer_Hit::Create(m_pGraphic_Device, Player, &m_iAniState);
				}
				else if (Player.CollisionCheck() == false)
				{
					m_iAniState = 1;
					return CPlayer_AttackCombo_R::Create(m_pGraphic_Device, Player, &m_iAniState);
				}
			}
		}
		else if (Player.Get_Mesh_Bone()->Get_NowPlayAniIndex() == CPlayer::PLAYER_ANI::Combo2)
		{
			MovePlayerPosition(Player, 0.1f, 15.f, fTimeDelta, pAgr);

			if (Player.Get_Mesh_Bone()->IsAnimationEnded(0.8f))
			{
				if (Player.CollisionCheck() == true)
				{
					if (pArkus->Get_AniIndex() == CArkus::ARKUS_ANI::RoundAtk01 ||
						pArkus->Get_AniIndex() == CArkus::ARKUS_ANI::RoundAtk02 ||
						pArkus->Get_AniIndex() == CArkus::ARKUS_ANI::FlyAtk02End ||
						pArkus->Get_AniIndex() == CArkus::ARKUS_ANI::MoveAtkEnd)
					{
						return CPlayer_KnockDown::Create(m_pGraphic_Device, Player, &m_iAniState);
					}
					else
						return CPlayer_Hit::Create(m_pGraphic_Device, Player, &m_iAniState);
				}
				else if (Player.CollisionCheck() == false)
				{
					m_iAniState = 2;
					return CPlayer_AttackCombo_R::Create(m_pGraphic_Device, Player, &m_iAniState);
				}
			}
		}
		else if (Player.Get_Mesh_Bone()->Get_NowPlayAniIndex() == CPlayer::PLAYER_ANI::Combo3)
		{
			MovePlayerPosition(Player, 0.1f, 15.f, fTimeDelta, pAgr);

			if (Player.Get_Mesh_Bone()->IsAnimationEnded(0.99f))
			{
				if (Player.CollisionCheck() == true)
				{
					if (pArkus->Get_AniIndex() == CArkus::ARKUS_ANI::RoundAtk01 ||
						pArkus->Get_AniIndex() == CArkus::ARKUS_ANI::RoundAtk02 ||
						pArkus->Get_AniIndex() == CArkus::ARKUS_ANI::FlyAtk02End ||
						pArkus->Get_AniIndex() == CArkus::ARKUS_ANI::MoveAtkEnd)
					{
						return CPlayer_KnockDown::Create(m_pGraphic_Device, Player, &m_iAniState);
					}
					else
						return CPlayer_Hit::Create(m_pGraphic_Device, Player, &m_iAniState);
				}
				else if (Player.CollisionCheck() == false)
				{
					m_iAniState = 3;
					return CPlayer_AttackCombo_R::Create(m_pGraphic_Device, Player, &m_iAniState);
				}
			}
		}
		else if (Player.Get_Mesh_Bone()->Get_NowPlayAniIndex() == CPlayer::PLAYER_ANI::Combo4)
		{
			MovePlayerPosition(Player, 0.1f, 15.f, fTimeDelta, pAgr);

			if (Player.Get_Mesh_Bone()->IsAnimationEnded(0.85f))
			{
				if (Player.CollisionCheck() == true)
				{
					if (pArkus->Get_AniIndex() == CArkus::ARKUS_ANI::RoundAtk01 ||
						pArkus->Get_AniIndex() == CArkus::ARKUS_ANI::RoundAtk02 ||
						pArkus->Get_AniIndex() == CArkus::ARKUS_ANI::FlyAtk02End ||
						pArkus->Get_AniIndex() == CArkus::ARKUS_ANI::MoveAtkEnd)
					{
						return CPlayer_KnockDown::Create(m_pGraphic_Device, Player, &m_iAniState);
					}
					else
						return CPlayer_Hit::Create(m_pGraphic_Device, Player, &m_iAniState);
				}
				else if (Player.CollisionCheck() == false)
				{
					m_iAniState = 2;
					return CPlayer_Idle::Create(m_pGraphic_Device, Player, &m_iAniState);
				}
			}
		}
	}
	
	return nullptr;
}

void CPlayer_AttackCombo::Update_State(CPlayer & Player, const float & fTimeDelta)
{
	CArkus*	pArkus = dynamic_cast<CArkus*>(CObject_Manager::GetInstance()->Get_Object(SCENE_DRAGON, L"Layer_Monster"));

	AttackAvailableCheck(pArkus, &Player);
}

void CPlayer_AttackCombo::MovePlayerPosition(CPlayer & Player, _float fPointTime, _float fPlayerSpeed, const float& fTimeDelta, void * pArg)
{
	if (Player.Get_Mesh_Bone()->IsAnimationEnded(fPointTime))
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

CPlayer_AttackCombo * CPlayer_AttackCombo::Create(LPDIRECT3DDEVICE9 pGraphicDevice, CPlayer & Player, void * pArg)
{
	CPlayer_AttackCombo* pInstance = new CPlayer_AttackCombo(pGraphicDevice);

	if (*(_int*)(pArg) == 1)
		Player.Set_AniIndex(CPlayer::PLAYER_ANI::Combo1);

	else if (*(_int*)(pArg) == 2)
		Player.Set_AniIndex(CPlayer::PLAYER_ANI::Combo2);

	else if (*(_int*)(pArg) == 3)
		Player.Set_AniIndex(CPlayer::PLAYER_ANI::Combo3);

	else if (*(_int*)(pArg) == 4)
		Player.Set_AniIndex(CPlayer::PLAYER_ANI::Combo4);

	if (FAILED(pInstance->Initialize_State(Player)))
		Safe_Release(pInstance);

	return pInstance;
}

void CPlayer_AttackCombo::Free()
{
	CPlayerState::Free();
}
