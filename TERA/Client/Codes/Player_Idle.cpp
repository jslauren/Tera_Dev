#include "stdafx.h"
#include "..\Headers\Player_Idle.h"
#include "Player.h"
#include "Input_Device.h"

#include "Player_Hit.h"
#include "Player_Sit.h"
#include "Player_Move.h"
#include "Player_KnockDown.h"
#include "Player_WeaponState.h"
#include "Player_AttackCombo.h"
#include "Player_Skill_CutHead.h"
#include "Player_Skill_Tumbling.h"
#include "Player_Skill_FlatBlade.h"
#include "Player_Skill_HandySlash.h"
#include "Player_Skill_JawBreaker.h"
#include "Player_Skill_CuttingSlash.h"
#include "Player_Skill_RagingStrike.h"
#include "Player_Skill_StingerBlade.h"
#include "Player_Skill_DrawSword_Charge.h"

#define PLAYER_SCALING	0.33f

_USING(Client)

CPlayer_Idle::CPlayer_Idle(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CPlayerState(pGraphic_Device)
{
}

HRESULT CPlayer_Idle::Initialize_State(CPlayer & Player)
{
	return NOERROR;
}

CPlayerState * CPlayer_Idle::Input_Keyboard(CPlayer & Player, const float & fTimeDelta, BYTE KeyID, void* pAgr)
{
	if (Player.Get_Mesh_Bone()->Get_NowPlayAniIndex() == CPlayer::PLAYER_ANI::Idle_Battle)
	{
		CArkus*	pArkus = dynamic_cast<CArkus*>(CObject_Manager::GetInstance()->Get_Object(SCENE_DRAGON, L"Layer_Monster"));

		if (Player.CollisionCheck() == true)
		{
			if (pArkus->Get_AniIndex() == CArkus::ARKUS_ANI::RoundAtk01 ||
				pArkus->Get_AniIndex() == CArkus::ARKUS_ANI::RoundAtk02 ||
				pArkus->Get_AniIndex() == CArkus::ARKUS_ANI::FlyAtk02End ||
				pArkus->Get_AniIndex() == CArkus::ARKUS_ANI::MoveAtkEnd)
			{
				return CPlayer_KnockDown::Create(m_pGraphic_Device, Player, &m_iAniState);
			}
			//else
			//	return CPlayer_Hit::Create(m_pGraphic_Device, Player, &m_iAniState);
		}
	}

	if (dynamic_cast<CPlayer*>(CObject_Manager::GetInstance()->Get_Object(SCENE_STATIC, L"Layer_Player"))->Get_CutSceneInfo() == true)
		return nullptr;

	if (CInput_Device::GetInstance()->GetDIKeyState(DIK_Q) & 0x80)
	{
		if (Player.Get_Mesh_Bone()->Get_NowPlayAniIndex() == CPlayer::PLAYER_ANI::Idle_Battle)
			Player.Set_DrawSwordBtn(true);
	}
	if (CInput_Device::GetInstance()->GetDIKeyState(DIK_W) & 0x80)
	{
		_matrix matView;
		_vec3 vTempRight, vTempUp, vTempLook;

		m_pGraphic_Device->GetTransform(D3DTS_VIEW, &matView);

		// 이렇게 m_pGraphic_Device의 view행렬을 가져와서 역행렬을 맥여버리면,
		// 카메라의 행렬이 만들어진다.
		D3DXMatrixInverse(&matView, nullptr, &matView);

		memcpy(&vTempLook, &matView.m[2][0], sizeof(_vec3));

		// 카메라껄 가져왔기 때문에, 카메라의 높이값을 빼주기 위한 구문.
		vTempLook.y = 0;

		D3DXVec3Normalize(&vTempLook, &vTempLook);

		D3DXVec3Cross(&vTempRight, &_vec3(0.f, 1.f, 0.f), &vTempLook);
		D3DXVec3Normalize(&vTempRight, &vTempRight);

		D3DXVec3Cross(&vTempUp, &vTempLook, &vTempRight);
		D3DXVec3Normalize(&vTempUp, &vTempUp);

		Player.Get_TransformMove()->Set_StateInfo(CTransform::STATE_RIGHT, &vTempRight);
		Player.Get_TransformMove()->Set_StateInfo(CTransform::STATE_UP, &vTempUp);
		Player.Get_TransformMove()->Set_StateInfo(CTransform::STATE_LOOK, &vTempLook);

		Player.Get_TransformMove()->Set_Scaling(PLAYER_SCALING, PLAYER_SCALING, PLAYER_SCALING);
		Player.Get_TransformRotation()->Set_Angle_Axis(_vec3(0.f, 1.f, 0.f), D3DXToRadian(0));		

		if (Player.Get_Mesh_Bone()->Get_NowPlayAniIndex() == CPlayer::PLAYER_ANI::Idle)
			return CPlayer_Move::Create(m_pGraphic_Device, Player, &m_iAniState);

		else if (Player.Get_Mesh_Bone()->Get_NowPlayAniIndex() == CPlayer::PLAYER_ANI::Idle_Battle)
		{
			m_iAniState = 2;
			return CPlayer_Move::Create(m_pGraphic_Device, Player, &m_iAniState);
		}

	}
	else if (CInput_Device::GetInstance()->GetDIKeyState(DIK_S) & 0x80)
	{
		// [Feat.현우 & 윤석 & 형진]
		_matrix matView;
		_vec3 vTempRight, vTempUp, vTempLook;

		m_pGraphic_Device->GetTransform(D3DTS_VIEW, &matView);

		D3DXMatrixInverse(&matView, nullptr, &matView);

		memcpy(&vTempLook, &matView.m[2][0], sizeof(_vec3));

		vTempLook.y = 0;

		D3DXVec3Normalize(&vTempLook, &vTempLook);

		D3DXVec3Cross(&vTempRight, &_vec3(0.f, 1.f, 0.f), &vTempLook);
		D3DXVec3Normalize(&vTempRight, &vTempRight);

		D3DXVec3Cross(&vTempUp, &vTempLook, &vTempRight);
		D3DXVec3Normalize(&vTempUp, &vTempUp);

		Player.Get_TransformMove()->Set_StateInfo(CTransform::STATE_RIGHT, &vTempRight);
		Player.Get_TransformMove()->Set_StateInfo(CTransform::STATE_UP, &vTempUp);
		Player.Get_TransformMove()->Set_StateInfo(CTransform::STATE_LOOK, &vTempLook);
			   
		Player.Get_TransformMove()->Set_Scaling(PLAYER_SCALING, PLAYER_SCALING, PLAYER_SCALING);
		Player.Get_TransformRotation()->Set_Angle_Axis(_vec3(0.f, 1.f, 0.f), D3DXToRadian(180));

		if (Player.Get_Mesh_Bone()->Get_NowPlayAniIndex() == CPlayer::PLAYER_ANI::Idle)
			return CPlayer_Move::Create(m_pGraphic_Device, Player, &m_iAniState);

		else if (Player.Get_Mesh_Bone()->Get_NowPlayAniIndex() == CPlayer::PLAYER_ANI::Idle_Battle)
		{
			m_iAniState = 2;
			return CPlayer_Move::Create(m_pGraphic_Device, Player, &m_iAniState);
		}
	}
	if (CInput_Device::GetInstance()->GetDIKeyState(DIK_A) & 0x80)
	{
		_matrix matView;
		_vec3 vTempRight, vTempUp, vTempLook;

		m_pGraphic_Device->GetTransform(D3DTS_VIEW, &matView);

		D3DXMatrixInverse(&matView, nullptr, &matView);

		memcpy(&vTempLook, &matView.m[2][0], sizeof(_vec3));

		vTempLook.y = 0;

		D3DXVec3Normalize(&vTempLook, &vTempLook);

		D3DXVec3Cross(&vTempRight, &_vec3(0.f, 1.f, 0.f), &vTempLook);
		D3DXVec3Normalize(&vTempRight, &vTempRight);

		D3DXVec3Cross(&vTempUp, &vTempLook, &vTempRight);
		D3DXVec3Normalize(&vTempUp, &vTempUp);

		Player.Get_TransformMove()->Set_StateInfo(CTransform::STATE_RIGHT, &vTempRight);
		Player.Get_TransformMove()->Set_StateInfo(CTransform::STATE_UP, &vTempUp);
		Player.Get_TransformMove()->Set_StateInfo(CTransform::STATE_LOOK, &vTempLook);

		Player.Get_TransformMove()->Set_Scaling(PLAYER_SCALING, PLAYER_SCALING, PLAYER_SCALING);
		Player.Get_TransformRotation()->Set_Angle_Axis(_vec3(0.f, 1.f, 0.f), D3DXToRadian(-90));

		if (Player.Get_Mesh_Bone()->Get_NowPlayAniIndex() == CPlayer::PLAYER_ANI::Idle)
			return CPlayer_Move::Create(m_pGraphic_Device, Player, &m_iAniState);

		else if (Player.Get_Mesh_Bone()->Get_NowPlayAniIndex() == CPlayer::PLAYER_ANI::Idle_Battle)
		{
			m_iAniState = 2;
			return CPlayer_Move::Create(m_pGraphic_Device, Player, &m_iAniState);
		}
	}
	else if (CInput_Device::GetInstance()->GetDIKeyState(DIK_D) & 0x80)
	{
		_matrix matView;
		_vec3 vTempRight, vTempUp, vTempLook;

		m_pGraphic_Device->GetTransform(D3DTS_VIEW, &matView);

		D3DXMatrixInverse(&matView, nullptr, &matView);

		memcpy(&vTempLook, &matView.m[2][0], sizeof(_vec3));

		vTempLook.y = 0;

		D3DXVec3Normalize(&vTempLook, &vTempLook);

		D3DXVec3Cross(&vTempRight, &_vec3(0.f, 1.f, 0.f), &vTempLook);
		D3DXVec3Normalize(&vTempRight, &vTempRight);

		D3DXVec3Cross(&vTempUp, &vTempLook, &vTempRight);
		D3DXVec3Normalize(&vTempUp, &vTempUp);

		Player.Get_TransformMove()->Set_StateInfo(CTransform::STATE_RIGHT, &vTempRight);
		Player.Get_TransformMove()->Set_StateInfo(CTransform::STATE_UP, &vTempUp);
		Player.Get_TransformMove()->Set_StateInfo(CTransform::STATE_LOOK, &vTempLook);

		Player.Get_TransformMove()->Set_Scaling(PLAYER_SCALING, PLAYER_SCALING, PLAYER_SCALING);
		Player.Get_TransformRotation()->Set_Angle_Axis(_vec3(0.f, 1.f, 0.f), D3DXToRadian(90));

		if (Player.Get_Mesh_Bone()->Get_NowPlayAniIndex() == CPlayer::PLAYER_ANI::Idle)
			return CPlayer_Move::Create(m_pGraphic_Device, Player, &m_iAniState);

		else if (Player.Get_Mesh_Bone()->Get_NowPlayAniIndex() == CPlayer::PLAYER_ANI::Idle_Battle)
		{
			m_iAniState = 2;
			return CPlayer_Move::Create(m_pGraphic_Device, Player, &m_iAniState);
		}
	}
	// [비 전투 중 = 칼 뽑기 / 전투 중 = 공격 ]
	if (CInput_Device::GetInstance()->Get_DIMouseDown(CInput_Device::MOUSEBUTTON::DIM_LBUTTON))
	{
		if (Player.Get_Mesh_Bone()->Get_NowPlayAniIndex() == CPlayer::PLAYER_ANI::Idle)
			return CPlayer_WeaponState::Create(m_pGraphic_Device, Player, &m_iAniState);

		else if (Player.Get_Mesh_Bone()->Get_NowPlayAniIndex() == CPlayer::PLAYER_ANI::Idle_Battle)
		{
			if (Player.Get_DrawSwordBtnState() == true)
			{
				m_iAniState = 1;
				return CPlayer_Skill_DrawSword_Charge::Create(m_pGraphic_Device, Player, &m_iAniState);
			}
			else
				return CPlayer_AttackCombo::Create(m_pGraphic_Device, Player, &m_iAniState);
		}
	}
	// [회피 기동 점프 스킬]
	if (CInput_Device::GetInstance()->Get_DIMouseDown(CInput_Device::MOUSEBUTTON::DIM_RBUTTON))
	{
		if (Player.Get_Mesh_Bone()->Get_NowPlayAniIndex() == CPlayer::PLAYER_ANI::Idle_Battle)
		{
			m_iAniState = 0;
			return CPlayer_Skill_Tumbling::Create(m_pGraphic_Device, Player, &m_iAniState);
		}

	}
	// [무기 발검, 착검]
	if (CInput_Device::GetInstance()->GetDIKeyState(DIK_F) & 0x80)
	{
		if (Player.Get_Mesh_Bone()->Get_NowPlayAniIndex() == CPlayer::PLAYER_ANI::Idle)
		{
			if (Player.Get_Mesh_Bone()->IsAnimationEnded())
				return CPlayer_WeaponState::Create(m_pGraphic_Device, Player, &m_iAniState);
		}

		else if (Player.Get_Mesh_Bone()->Get_NowPlayAniIndex() == CPlayer::PLAYER_ANI::Idle_Battle)
		{
			if (Player.Get_Mesh_Bone()->IsAnimationEnded())
			{
				m_iAniState = 2;
				return CPlayer_WeaponState::Create(m_pGraphic_Device, Player, &m_iAniState);
			}
		}
	}
	// [앉기, 서기]
	if (CInput_Device::GetInstance()->GetDIKeyState(DIK_R) & 0x80)
	{
		if (Player.Get_Mesh_Bone()->Get_NowPlayAniIndex() == CPlayer::PLAYER_ANI::Idle)
		{
			if (Player.Get_Mesh_Bone()->IsAnimationEnded())
			{
				m_iAniState = 1;
				return CPlayer_Sit::Create(m_pGraphic_Device, Player, &m_iAniState);
			}
		}
	}
	// [테스트]
	if (CInput_Device::GetInstance()->GetDIKeyState(DIK_T) & 0x80)
	{
		Player.Set_HP_Add(1000);
		//if (Player.Get_Mesh_Bone()->Get_NowPlayAniIndex() == CPlayer::PLAYER_ANI::Idle_Battle)
		//{
		//	if (Player.Get_Mesh_Bone()->IsAnimationEnded())
		//		return CPlayer_KnockDown::Create(m_pGraphic_Device, Player, &m_iAniState);
		//}
	}
	if (CInput_Device::GetInstance()->GetDIKeyState(DIK_Y) & 0x80)
		Player.Set_HP_Sub(100);

	// [스킬 넘버]
	if (CInput_Device::GetInstance()->GetDIKeyState(DIK_1) & 0x80)
	{
		if (Player.Get_Mesh_Bone()->Get_NowPlayAniIndex() == CPlayer::PLAYER_ANI::Idle_Battle)
		{
			if (Player.Get_Mesh_Bone()->IsAnimationEnded())
				return CPlayer_Skill_CutHead::Create(m_pGraphic_Device, Player, &m_iAniState);
		}
	}
	if (CInput_Device::GetInstance()->GetDIKeyState(DIK_2) & 0x80)
	{
		if (Player.Get_Mesh_Bone()->Get_NowPlayAniIndex() == CPlayer::PLAYER_ANI::Idle_Battle)
		{
			if (Player.Get_Mesh_Bone()->IsAnimationEnded())
				return CPlayer_Skill_CuttingSlash::Create(m_pGraphic_Device, Player, &m_iAniState);
		}
	}
	if (CInput_Device::GetInstance()->GetDIKeyState(DIK_3) & 0x80)
	{
		if (Player.Get_Mesh_Bone()->Get_NowPlayAniIndex() == CPlayer::PLAYER_ANI::Idle_Battle)
		{
			if (Player.Get_Mesh_Bone()->IsAnimationEnded())
				return Player_Skill_FlatBlade::Create(m_pGraphic_Device, Player, &m_iAniState);
		}
	}
	if (CInput_Device::GetInstance()->GetDIKeyState(DIK_4) & 0x80)
	{
		if (Player.Get_Mesh_Bone()->Get_NowPlayAniIndex() == CPlayer::PLAYER_ANI::Idle_Battle)
		{
			if (Player.Get_Mesh_Bone()->IsAnimationEnded())
				return CPlayer_Skill_HandySlash::Create(m_pGraphic_Device, Player, &m_iAniState);
		}
	}
	if (CInput_Device::GetInstance()->GetDIKeyState(DIK_5) & 0x80)
	{
		if (Player.Get_Mesh_Bone()->Get_NowPlayAniIndex() == CPlayer::PLAYER_ANI::Idle_Battle)
		{
			if (Player.Get_Mesh_Bone()->IsAnimationEnded())
				return CPlayer_Skill_JawBreaker::Create(m_pGraphic_Device, Player, &m_iAniState);
		}
	}
	if (CInput_Device::GetInstance()->GetDIKeyState(DIK_6) & 0x80)
	{
		if (Player.Get_Mesh_Bone()->Get_NowPlayAniIndex() == CPlayer::PLAYER_ANI::Idle_Battle)
		{
			if (Player.Get_Mesh_Bone()->IsAnimationEnded())
				return CPlayer_Skill_StingerBlade::Create(m_pGraphic_Device, Player, &m_iAniState);
		}
	}
	if (CInput_Device::GetInstance()->GetDIKeyState(DIK_7) & 0x80)
	{
		if (Player.Get_Mesh_Bone()->Get_NowPlayAniIndex() == CPlayer::PLAYER_ANI::Idle_Battle)
		{
			if (Player.Get_Mesh_Bone()->IsAnimationEnded())
			{
				m_iAniState = 1;
				return CPlayer_Skill_RagingStrike::Create(m_pGraphic_Device, Player, &m_iAniState);
			}
		}
	}
	// [ 시연을 위한 플레이어 HP, MP 치트 ] //
	if (CInput_Device::GetInstance()->GetDIKeyState(DIK_MINUS) & 0x80)
	{
		Player.Set_HP_Add(2000);
	}
	if (CInput_Device::GetInstance()->GetDIKeyState(DIK_EQUALS) & 0x80)
	{
		Player.Set_MP_Add(300);
	}

	return nullptr;
}

void CPlayer_Idle::Update_State(CPlayer & Player, const float & fTimeDelta)
{
}

CPlayer_Idle * CPlayer_Idle::Create(LPDIRECT3DDEVICE9 pGraphicDevice, CPlayer & Player, void* pArg)
{
	CPlayer_Idle* pInstance = new CPlayer_Idle(pGraphicDevice);

	if (*(_int*)(pArg) == 1)
		Player.Set_AniIndex(CPlayer::PLAYER_ANI::Idle);
	else
	{
		Player.Set_AniIndex(CPlayer::PLAYER_ANI::Idle_Battle);
		dynamic_cast<CWeapon*>(CObject_Manager::GetInstance()->Get_Object(SCENE_STATIC, L"Layer_Weapon", -1))->Set_BoneMatrix(2);
	}

	if (FAILED(pInstance->Initialize_State(Player)))
		Safe_Release(pInstance);

	return pInstance;
}

void CPlayer_Idle::Free()
{
	CPlayerState::Free();
}
