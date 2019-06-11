#include "stdafx.h"
#include "..\Headers\Player_Idle.h"
#include "Player.h"
#include "Input_Device.h"
#include "Layer.h"

#include "Player_Move.h"

_USING(Client)

CPlayer_Idle::CPlayer_Idle(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CPlayerState(pGraphic_Device)
{
}

HRESULT CPlayer_Idle::Initialize_State(CPlayer & Player)
{
	Player.Set_AniIndex(CPlayer::PLAYER_STATE::R05UNARMEDWAIT);
	Player.Set_ActionID(CPlayer::ACTION_ID::ACTION_IDLE);

	return NOERROR;
}

CPlayerState * CPlayer_Idle::Input_Keyboard(CPlayer & Player, const float & fTimeDelta, BYTE KeyID, void* pAgr)
{
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

		Player.Get_TransformMove()->Set_Scaling(0.3f, 0.3f, 0.3f);
		Player.Get_TransformRotation()->Set_Angle_Axis(_vec3(0.f, 1.f, 0.f), D3DXToRadian(0));		

		return CPlayer_Move::Create(m_pGraphic_Device, Player);
	}
	else if (CInput_Device::GetInstance()->GetDIKeyState(DIK_S) & 0x80)
	{
		// [Feat.현우 & 윤석]
		_matrix matView;
		_vec3 vTempRight, vTempUp, vTempLook;

		m_pGraphic_Device->GetTransform(D3DTS_VIEW, &matView);

		// 이렇게 m_pGraphic_Device의 view행렬을 가져와서 역행렬을 맥여버리면,
		// 카메라의 행렬이 만들어진다.
		D3DXMatrixInverse(&matView, nullptr, &matView);

		memcpy(&vTempLook, &matView.m[2][0], sizeof(_vec3));

		// 카메라껄 가져왔기 때문에, 카메라의 높이값을 빼주기 위한 구문.
		vTempLook.y = 0;

	//	vTempLook *= -1;

		D3DXVec3Normalize(&vTempLook, &vTempLook);

		D3DXVec3Cross(&vTempRight, &_vec3(0.f, 1.f, 0.f), &vTempLook);
		D3DXVec3Normalize(&vTempRight, &vTempRight);

		D3DXVec3Cross(&vTempUp, &vTempLook, &vTempRight);
		D3DXVec3Normalize(&vTempUp, &vTempUp);

		Player.Get_TransformMove()->Set_StateInfo(CTransform::STATE_RIGHT, &vTempRight);
		Player.Get_TransformMove()->Set_StateInfo(CTransform::STATE_UP, &vTempUp);
		Player.Get_TransformMove()->Set_StateInfo(CTransform::STATE_LOOK, &vTempLook);
			   
		Player.Get_TransformMove()->Set_Scaling(0.3f, 0.3f, 0.3f);
		Player.Get_TransformRotation()->Set_Angle_Axis(_vec3(0.f, 1.f, 0.f), D3DXToRadian(180));

		return CPlayer_Move::Create(m_pGraphic_Device, Player);
	}
	if (CInput_Device::GetInstance()->GetDIKeyState(DIK_A) & 0x80)
	{
		//_matrix		matRotate;
		//D3DXMatrixRotationAxis(&matRotate, &_vec3(0.f, 1.f, 0.f), D3DXToRadian(-90));

		//_vec3 vTempRight, vTempUp, vTempLook;
		//vTempLook.x = matRotate._31;
		//vTempLook.y = matRotate._32;
		//vTempLook.z = matRotate._33;

		//D3DXVec3Normalize(&vTempLook, &vTempLook);

		//D3DXVec3Cross(&vTempRight, &_vec3(0.f, 1.f, 0.f), &vTempLook);
		//D3DXVec3Normalize(&vTempRight, &vTempRight);

		//D3DXVec3Cross(&vTempUp, &vTempLook, &vTempRight);
		//D3DXVec3Normalize(&vTempUp, &vTempUp);

		//Player.Get_TransformRotation()->Set_StateInfo(CTransform::STATE_RIGHT, &vTempRight);
		//Player.Get_TransformRotation()->Set_StateInfo(CTransform::STATE_UP, &vTempUp);
		//Player.Get_TransformRotation()->Set_StateInfo(CTransform::STATE_LOOK, &vTempLook);

	//	Player.Get_Transform()->Set_Scaling(0.25f, 0.25f, 0.25f);

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

		Player.Get_TransformMove()->Set_Scaling(0.3f, 0.3f, 0.3f);
		Player.Get_TransformRotation()->Set_Angle_Axis(_vec3(0.f, 1.f, 0.f), D3DXToRadian(-90));

		return CPlayer_Move::Create(m_pGraphic_Device, Player);
	}
	else if (CInput_Device::GetInstance()->GetDIKeyState(DIK_D) & 0x80)
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

		Player.Get_TransformMove()->Set_Scaling(0.3f, 0.3f, 0.3f);
		Player.Get_TransformRotation()->Set_Angle_Axis(_vec3(0.f, 1.f, 0.f), D3DXToRadian(90));

		return CPlayer_Move::Create(m_pGraphic_Device, Player);
	}

	if (CInput_Device::GetInstance()->GetDIMouseState(CInput_Device::MOUSEBUTTON::DIM_LBUTTON))
	{
		Player.Set_AniIndex(CPlayer::PLAYER_STATE::OUTWEAPON);
	}

	return nullptr;
}

void CPlayer_Idle::Update_State(CPlayer & Player, const float & fTimeDelta)
{
}

CPlayer_Idle * CPlayer_Idle::Create(LPDIRECT3DDEVICE9 pGraphicDevice, CPlayer & Player)
{
	CPlayer_Idle* pInstance = new CPlayer_Idle(pGraphicDevice);

	if (FAILED(pInstance->Initialize_State(Player)))
		Safe_Release(pInstance);

	return pInstance;
}

void CPlayer_Idle::Free()
{
	CPlayerState::Free();
}
