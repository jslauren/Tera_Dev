#include "stdafx.h"
#include "..\Headers\Player_Move.h"
#include "Player.h"
#include "Input_Device.h"
#include "Navigation.h"

#include "Player_Idle.h"

_USING(Client)

CPlayer_Move::CPlayer_Move(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CPlayerState(pGraphic_Device)
{
}

HRESULT CPlayer_Move::Initialize_State(CPlayer & Player)
{
	Player.Set_AniIndex(CPlayer::PLAYER_STATE::RUN);
	Player.Set_ActionID(CPlayer::ACTION_ID::ACTION_RUN);

	return NOERROR;
}

CPlayerState * CPlayer_Move::Input_Keyboard(CPlayer & Player, const float & fTimeDelta, BYTE KeyID, void* pAgr)
{
	if (CInput_Device::GetInstance()->GetDIKeyState(DIK_W) & 0x80)
	{
		if (CInput_Device::GetInstance()->GetDIMouseState(CInput_Device::MOUSEBUTTON::DIM_LBUTTON))
			//return CPlayer_Jump::Create(m_pGraphicDevice, Player);

		Player.Set_AniIndex(CPlayer::PLAYER_STATE::RUN);

		_uint		iCellIndx = 0;
		// 여기서 Move함수의 스피드랑 Move_OnNavigation함수의 인자값인 fTimeDelta랑 값을 동기화 해줘야 안끼고 잘간다.
		if (true == ((CNavigation*)(pAgr))->Move_OnNavigation(Player.Get_Transform()->Get_StateInfo(CTransform::STATE_POSITION), Player.Get_Transform()->Get_StateInfo(CTransform::STATE_LOOK), 30.0f * fTimeDelta, &iCellIndx))
		{
			Player.Get_Transform()->Move(0, 30.f, fTimeDelta);

			/* ※※※※※※※진짜 이동하면 꼭 호출해야합니다※※※※※※.*/
			((CNavigation*)(pAgr))->SetUp_CurrentIndex(iCellIndx);
		}

		//if (CInput_Device::GetInstance()->GetDIKeyState(DIK_A) & 0x80)
		//	Player.Set_AniIndex(CPlayer::PLAYER_STATE::LUMOSSTRAFELEFT);

		//else if (CInput_Device::GetInstance()->GetDIKeyState(DIK_D) & 0x80)
		//	Player.Set_AniIndex(CPlayer::PLAYER_STATE::LUMOSSTRAFERIGHT);

		return nullptr;
	}
	else if (CInput_Device::GetInstance()->GetDIKeyState(DIK_S) & 0x80)
	{
		//if (Engine::Get_DIKeyDown(DIK_SPACE))
		//	return CPlayer_Jump::Create(m_pGraphicDevice, Player);

		Player.Set_AniIndex(CPlayer::PLAYER_STATE::RUN_BACK);

		return nullptr;
	}
	if (CInput_Device::GetInstance()->GetDIKeyState(DIK_A) & 0x80)
	{
		/*if (Engine::Get_DIKeyDown(DIK_SPACE))
			return CPlayer_Jump::Create(m_pGraphicDevice, Player);*/

		Player.Set_AniIndex(CPlayer::PLAYER_STATE::RUN_LEFT);

		return nullptr;
	}
	else if (CInput_Device::GetInstance()->GetDIKeyState(DIK_D) & 0x80)
	{
		/*if (Engine::Get_DIKeyDown(DIK_SPACE))
		return CPlayer_Jump::Create(m_pGraphicDevice, Player);*/

		Player.Set_AniIndex(CPlayer::PLAYER_STATE::RUN_LEFT);
	}
	else
		return CPlayer_Idle::Create(m_pGraphic_Device, Player);

	return nullptr;
}

void CPlayer_Move::Update_State(CPlayer & Player, const float & fTimeDelta)
{
}

CPlayer_Move * CPlayer_Move::Create(LPDIRECT3DDEVICE9 pGraphicDevice, CPlayer & Player)
{
	CPlayer_Move* pInstance = new CPlayer_Move(pGraphicDevice);

	if (FAILED(pInstance->Initialize_State(Player)))
		Safe_Release(pInstance);

	return pInstance;
}

void CPlayer_Move::Free()
{
	CPlayerState::Free();
}
