#include "stdafx.h"
#include "..\Headers\Player_Skill_DrawSword_Move.h"
#include "Player.h"
#include "Input_Device.h"

#include "Player_Skill_DrawSword.h"
#include "Player_Skill_DrawSword_Loop.h"

_USING(Client)

CPlayer_Skill_DrawSword_Move::CPlayer_Skill_DrawSword_Move(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CPlayerState(pGraphic_Device)
{
}

HRESULT CPlayer_Skill_DrawSword_Move::Initialize_State(CPlayer & Player)
{
	Player.Set_AniIndex(CPlayer::PLAYER_ANI::DrawSwordMove);

	CTransform* pWeaponTransformCom = dynamic_cast<CWeapon*>(CObject_Manager::GetInstance()->Get_Object(SCENE_STATIC, L"Layer_Weapon", -1))->Get_TransformCom();

	pWeaponTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(0.f, 5.f, -10.f));
	pWeaponTransformCom->Set_Rotation_YawPitchRoll(D3DXToRadian(0), D3DXToRadian(-27.5f), D3DXToRadian(0));

	return NOERROR;
}

CPlayerState * CPlayer_Skill_DrawSword_Move::Input_Keyboard(CPlayer & Player, const float & fTimeDelta, BYTE KeyID, void * pArg)
{
	if (Player.Get_Mesh_Bone()->Get_NowPlayAniIndex() == CPlayer::PLAYER_ANI::DrawSwordMove)
	{
		if (CInput_Device::GetInstance()->GetDIMouseState(CInput_Device::MOUSEBUTTON::DIM_LBUTTON))
		{
			if (CInput_Device::GetInstance()->GetDIKeyState(DIK_W) & 0x80)
			{
				_uint		iCellIndx = 0;
				// 여기서 Move함수의 스피드랑 Move_OnNavigation함수의 인자값인 fTimeDelta랑 값을 동기화 해줘야 안끼고 잘간다.
				if (true == ((CNavigation*)(pArg))->Move_OnNavigation(Player.Get_Transform()->Get_StateInfo(CTransform::STATE_POSITION), Player.Get_Transform()->Get_StateInfo(CTransform::STATE_LOOK), 10.0f * fTimeDelta, &iCellIndx))
				{
					Player.Get_TransformMove()->Move(0, 10.f, fTimeDelta);

					/* ※※※※※※※진짜 이동하면 꼭 호출해야합니다※※※※※※.*/
					((CNavigation*)(pArg))->SetUp_CurrentIndex(iCellIndx);
				}

				if (CInput_Device::GetInstance()->GetDIKeyState(DIK_A) & 0x80)
				{
					Player.Get_TransformMove()->Rotation_Axis(_vec3(0.f, 1.f, 0.f), D3DXToRadian(-100.f), fTimeDelta);
				}

				else if (CInput_Device::GetInstance()->GetDIKeyState(DIK_D) & 0x80)
				{
					Player.Get_TransformMove()->Rotation_Axis(_vec3(0.f, 1.f, 0.f), D3DXToRadian(100.f), fTimeDelta);
				}

				Player.Get_TransformRotation()->Set_Angle_Axis(_vec3(0.f, 1.f, 0.f), D3DXToRadian(0));

				return nullptr;
			}
			else if (CInput_Device::GetInstance()->GetDIKeyState(DIK_S) & 0x80)
			{
				//if (Engine::Get_DIKeyDown(DIK_SPACE))
				//	return CPlayer_Jump::Create(m_pGraphicDevice, Player);

				_uint		iCellIndx = 0;
				// 여기서 Move함수의 스피드랑 Move_OnNavigation함수의 인자값인 fTimeDelta랑 값을 동기화 해줘야 안끼고 잘간다.
				if (true == ((CNavigation*)(pArg))->Move_OnNavigation(Player.Get_Transform()->Get_StateInfo(CTransform::STATE_POSITION), Player.Get_Transform()->Get_StateInfo(CTransform::STATE_LOOK), 10.0f * fTimeDelta, &iCellIndx))
				{
					Player.Get_TransformMove()->Move(1, 10.f, fTimeDelta);

					/* ※※※※※※※진짜 이동하면 꼭 호출해야합니다※※※※※※.*/
					((CNavigation*)(pArg))->SetUp_CurrentIndex(iCellIndx);
				}

				if (CInput_Device::GetInstance()->GetDIKeyState(DIK_A) & 0x80)
				{
					Player.Get_TransformMove()->Rotation_Axis(_vec3(0.f, 1.f, 0.f), D3DXToRadian(-100.f), fTimeDelta);
				}

				else if (CInput_Device::GetInstance()->GetDIKeyState(DIK_D) & 0x80)
				{
					Player.Get_TransformMove()->Rotation_Axis(_vec3(0.f, 1.f, 0.f), D3DXToRadian(100.f), fTimeDelta);
				}

				Player.Get_TransformRotation()->Set_Angle_Axis(_vec3(0.f, 1.f, 0.f), D3DXToRadian(180));

				return nullptr;
			}
		}
		else if (CInput_Device::GetInstance()->Get_DIMouseUp(CInput_Device::MOUSEBUTTON::DIM_LBUTTON))
		{
			if (Player.Get_Mesh_Bone()->IsAnimationEnded(0.75f))
			{
				return CPlayer_Skill_DrawSword::Create(m_pGraphic_Device, Player, &m_iAniState);
			}
		}
	}

	return CPlayer_Skill_DrawSword_Loop::Create(m_pGraphic_Device, Player, &m_iAniState);
}

void CPlayer_Skill_DrawSword_Move::Update_State(CPlayer & Player, const float & fTimeDelta)
{
}

void CPlayer_Skill_DrawSword_Move::MovePlayerPosition(CPlayer & Player, _float fPlayerSpeed, const float & fTimeDelta, void * pArg, _int iMoveDir)
{
	_uint		iCellIndx = 0;
	if (true == ((CNavigation*)(pArg))->Move_OnNavigation(Player.Get_Transform()->Get_StateInfo(CTransform::STATE_POSITION), Player.Get_Transform()->Get_StateInfo(CTransform::STATE_LOOK), 30.0f * fTimeDelta, &iCellIndx))
	{
		Player.Get_TransformMove()->Move(Player.Get_Transform()->Get_StateInfo(CTransform::STATE_LOOK), fPlayerSpeed, fTimeDelta);

		/* ※※※※※※※진짜 이동하면 꼭 호출해야합니다※※※※※※.*/
		((CNavigation*)(pArg))->SetUp_CurrentIndex(iCellIndx);
	}
}

CPlayer_Skill_DrawSword_Move * CPlayer_Skill_DrawSword_Move::Create(LPDIRECT3DDEVICE9 pGraphicDevice, CPlayer & Player, void * pArg)
{
	CPlayer_Skill_DrawSword_Move* pInstance = new CPlayer_Skill_DrawSword_Move(pGraphicDevice);

	if (FAILED(pInstance->Initialize_State(Player)))
		Safe_Release(pInstance);

	return pInstance;
}

void CPlayer_Skill_DrawSword_Move::Free()
{
	CPlayerState::Free();
}
