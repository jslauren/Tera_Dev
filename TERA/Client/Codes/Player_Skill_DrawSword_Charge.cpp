#include "stdafx.h"
#include "..\Headers\Player_Skill_DrawSword_Charge.h"
#include "Player.h"
#include "Input_Device.h"

#include "Player_Skill_DrawSword_Loop.h"
#include "Player_Idle.h"

_USING(Client)

CPlayer_Skill_DrawSword_Charge::CPlayer_Skill_DrawSword_Charge(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CPlayerState(pGraphic_Device)
{
}

HRESULT CPlayer_Skill_DrawSword_Charge::Initialize_State(CPlayer & Player)
{
	Player.Set_AniIndex(CPlayer::PLAYER_ANI::DrawSwordCharge);
	Player.Set_ActionID(CPlayer::ACTION_ID::ACTION_ATTACK);

	return NOERROR;
}

CPlayerState * CPlayer_Skill_DrawSword_Charge::Input_Keyboard(CPlayer & Player, const float & fTimeDelta, BYTE KeyID, void * pArg)
{
	if (Player.Get_Mesh_Bone()->Get_NowPlayAniIndex() == CPlayer::PLAYER_ANI::DrawSwordCharge)
	{
		if (CInput_Device::GetInstance()->GetDIMouseState(CInput_Device::MOUSEBUTTON::DIM_LBUTTON))
		{
			if (Player.Get_Mesh_Bone()->IsAnimationEnded(0.2f))
			{
				CTransform* pWeaponTransformCom = dynamic_cast<CWeapon*>(CObject_Manager::GetInstance()->Get_Object(SCENE_STATIC, L"Layer_Weapon", -1))->Get_TransformCom();

				pWeaponTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(5.f, 5.f, 2.f));
				pWeaponTransformCom->Set_Rotation_YawPitchRoll(D3DXToRadian(0), D3DXToRadian(25), D3DXToRadian(-45));
			}
			if (Player.Get_Mesh_Bone()->IsAnimationEnded(0.85f))
				return CPlayer_Skill_DrawSword_Loop::Create(m_pGraphic_Device, Player, &m_iAniState);
		}
		else
		{
			if (Player.Get_Mesh_Bone()->IsAnimationEnded(0.4f))
			{
				m_iAniState = 2;
				Player.Set_DrawSwordBtn(false);
				dynamic_cast<CWeapon*>(CObject_Manager::GetInstance()->Get_Object(SCENE_STATIC, L"Layer_Weapon", -1))->Set_BoneMatrix(2);
				return CPlayer_Idle::Create(m_pGraphic_Device, Player, &m_iAniState);
			}
		}
	}

	return nullptr;
}

void CPlayer_Skill_DrawSword_Charge::Update_State(CPlayer & Player, const float & fTimeDelta)
{
}

void CPlayer_Skill_DrawSword_Charge::MovePlayerPosition(CPlayer & Player, _float fPlayerSpeed, const float & fTimeDelta, void * pArg, _int iMoveDir)
{
	_uint		iCellIndx = 0;
	if (true == ((CNavigation*)(pArg))->Move_OnNavigation(Player.Get_Transform()->Get_StateInfo(CTransform::STATE_POSITION), Player.Get_Transform()->Get_StateInfo(CTransform::STATE_LOOK), 30.0f * fTimeDelta, &iCellIndx))
	{
		Player.Get_TransformMove()->Move(Player.Get_Transform()->Get_StateInfo(CTransform::STATE_LOOK), fPlayerSpeed, fTimeDelta);

		/* ※※※※※※※진짜 이동하면 꼭 호출해야합니다※※※※※※.*/
		((CNavigation*)(pArg))->SetUp_CurrentIndex(iCellIndx);
	}
}

CPlayer_Skill_DrawSword_Charge * CPlayer_Skill_DrawSword_Charge::Create(LPDIRECT3DDEVICE9 pGraphicDevice, CPlayer & Player, void * pArg)
{
	CPlayer_Skill_DrawSword_Charge* pInstance = new CPlayer_Skill_DrawSword_Charge(pGraphicDevice);

	if (FAILED(pInstance->Initialize_State(Player)))
		Safe_Release(pInstance);

	return pInstance;
}

void CPlayer_Skill_DrawSword_Charge::Free()
{
	CPlayerState::Free();
}
