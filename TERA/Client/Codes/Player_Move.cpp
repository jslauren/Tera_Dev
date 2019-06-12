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
//	Player.Set_AniIndex(CPlayer::PLAYER_ANI::UNARMEDRUN);
	Player.Set_ActionID(CPlayer::ACTION_ID::ACTION_RUN);

	return NOERROR;
}

CPlayerState * CPlayer_Move::Input_Keyboard(CPlayer & Player, const float & fTimeDelta, BYTE KeyID, void* pAgr)
{
	if (CInput_Device::GetInstance()->GetDIKeyState(DIK_W) & 0x80)
	{
		//if (CInput_Device::GetInstance()->GetDIMouseState(CInput_Device::MOUSEBUTTON::DIM_LBUTTON))
			//return CPlayer_Jump::Create(m_pGraphicDevice, Player);

		//Player.Set_AniIndex(CPlayer::PLAYER_ANI::UNARMEDRUN);

		_uint		iCellIndx = 0;
		// ���⼭ Move�Լ��� ���ǵ�� Move_OnNavigation�Լ��� ���ڰ��� fTimeDelta�� ���� ����ȭ ����� �ȳ��� �߰���.
		if (true == ((CNavigation*)(pAgr))->Move_OnNavigation(Player.Get_Transform()->Get_StateInfo(CTransform::STATE_POSITION), Player.Get_Transform()->Get_StateInfo(CTransform::STATE_LOOK), 30.0f * fTimeDelta, &iCellIndx))
		{
			Player.Get_TransformMove()->Move(0, 30.f, fTimeDelta);

			/* �ءءءءءء���¥ �̵��ϸ� �� ȣ���ؾ��մϴ١ءءءءء�.*/
			((CNavigation*)(pAgr))->SetUp_CurrentIndex(iCellIndx);
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

	//	Player.Set_AniIndex(CPlayer::PLAYER_ANI::UNARMEDRUN);
		
		_uint		iCellIndx = 0;
		// ���⼭ Move�Լ��� ���ǵ�� Move_OnNavigation�Լ��� ���ڰ��� fTimeDelta�� ���� ����ȭ ����� �ȳ��� �߰���.
		if (true == ((CNavigation*)(pAgr))->Move_OnNavigation(Player.Get_Transform()->Get_StateInfo(CTransform::STATE_POSITION), Player.Get_Transform()->Get_StateInfo(CTransform::STATE_LOOK), 30.0f * fTimeDelta, &iCellIndx))
		{
			Player.Get_TransformMove()->Move(1, 30.f, fTimeDelta);

			/* �ءءءءءء���¥ �̵��ϸ� �� ȣ���ؾ��մϴ١ءءءءء�.*/
			((CNavigation*)(pAgr))->SetUp_CurrentIndex(iCellIndx);
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
	if (CInput_Device::GetInstance()->GetDIKeyState(DIK_A) & 0x80)
	{
		/*if (Engine::Get_DIKeyDown(DIK_SPACE))
			return CPlayer_Jump::Create(m_pGraphicDevice, Player);*/

		_uint		iCellIndx = 0;
		// ���⼭ Move�Լ��� ���ǵ�� Move_OnNavigation�Լ��� ���ڰ��� fTimeDelta�� ���� ����ȭ ����� �ȳ��� �߰���.
		if (true == ((CNavigation*)(pAgr))->Move_OnNavigation(Player.Get_Transform()->Get_StateInfo(CTransform::STATE_POSITION), Player.Get_Transform()->Get_StateInfo(CTransform::STATE_LOOK), 30.0f * fTimeDelta, &iCellIndx))
		{
			Player.Get_TransformMove()->Move(2, 30.f, fTimeDelta);

			/* �ءءءءءء���¥ �̵��ϸ� �� ȣ���ؾ��մϴ١ءءءءء�.*/
			((CNavigation*)(pAgr))->SetUp_CurrentIndex(iCellIndx);
		}

		Player.Get_TransformRotation()->Set_Angle_Axis(_vec3(0.f, 1.f, 0.f), D3DXToRadian(-90));

		return nullptr;
	}
	else if (CInput_Device::GetInstance()->GetDIKeyState(DIK_D) & 0x80)
	{
		/*if (Engine::Get_DIKeyDown(DIK_SPACE))
		return CPlayer_Jump::Create(m_pGraphicDevice, Player);*/

		_uint		iCellIndx = 0;
		// ���⼭ Move�Լ��� ���ǵ�� Move_OnNavigation�Լ��� ���ڰ��� fTimeDelta�� ���� ����ȭ ����� �ȳ��� �߰���.
		if (true == ((CNavigation*)(pAgr))->Move_OnNavigation(Player.Get_Transform()->Get_StateInfo(CTransform::STATE_POSITION), Player.Get_Transform()->Get_StateInfo(CTransform::STATE_LOOK), 30.0f * fTimeDelta, &iCellIndx))
		{
			Player.Get_TransformMove()->Move(3, 30.f, fTimeDelta);

			/* �ءءءءءء���¥ �̵��ϸ� �� ȣ���ؾ��մϴ١ءءءءء�.*/
			((CNavigation*)(pAgr))->SetUp_CurrentIndex(iCellIndx);
		}

		Player.Get_TransformRotation()->Set_Angle_Axis(_vec3(0.f, 1.f, 0.f), D3DXToRadian(90));

		return nullptr;
	}
	else // Go to Idle
	{
		if (Player.Get_Mesh()->Get_NowPlayAniIndex() == CPlayer::PLAYER_ANI::UNARMEDRUN)
		{
			m_iAniState = 1;
			return CPlayer_Idle::Create(m_pGraphic_Device, Player, &m_iAniState);
		}
		else if (Player.Get_Mesh()->Get_NowPlayAniIndex() == CPlayer::PLAYER_ANI::RUN)
		{
			m_iAniState = 2;
			return CPlayer_Idle::Create(m_pGraphic_Device, Player, &m_iAniState);
		}

	}

	return nullptr;
}

void CPlayer_Move::Update_State(CPlayer & Player, const float & fTimeDelta)
{
}

CPlayer_Move * CPlayer_Move::Create(LPDIRECT3DDEVICE9 pGraphicDevice, CPlayer & Player, void* pArg)
{
	CPlayer_Move* pInstance = new CPlayer_Move(pGraphicDevice);

	if (*(_int*)(pArg) == 1)
		Player.Set_AniIndex(CPlayer::PLAYER_ANI::UNARMEDRUN);
	else
		Player.Set_AniIndex(CPlayer::PLAYER_ANI::RUN);

	if (FAILED(pInstance->Initialize_State(Player)))
		Safe_Release(pInstance);

	return pInstance;
}

void CPlayer_Move::Free()
{
	CPlayerState::Free();
}
