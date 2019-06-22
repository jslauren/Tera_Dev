#include "stdafx.h"
#include "..\Headers\Arkus_Idle.h"
#include "Arkus.h"
#include "Input_Device.h"

#include "Arkus_Run.h"
#include "Arkus_Attack.h"
#include "Arkus_Hit.h"
#include "Arkus_Groggy.h"
#include "Arkus_AlmostDead.h"
#include "Arkus_Death.h"

_USING(Client)

CArkus_Idle::CArkus_Idle(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CArkusState(pGraphic_Device)
{
}

HRESULT CArkus_Idle::Initialize_State(CArkus & Arkus)
{
	Arkus.Set_AniIndex(CArkus::ARKUS_ANI::Idle);
	Arkus.Set_ActionID(CArkus::ACTION_ID::ACTION_IDLE);

	return NOERROR;
}

CArkusState * CArkus_Idle::Input_State(CArkus & Arkus, const float & fTimeDelta, BYTE KeyID, void * pAgr)
{
	if (Arkus.Get_Mesh()->Get_NowPlayAniIndex() == CArkus::ARKUS_ANI::Idle)
	{
		if (Arkus.CollisionCheck() == true)
		{
			if (Arkus.Get_Mesh()->IsAnimationEnded(0.45f))
				return CArkus_Hit::Create(m_pGraphic_Device, Arkus, &m_iAniState);
		}

		if (Arkus.Get_CollisionPartCheck(Arkus.COLL_ATTACK_AREA) == false)
		{
			if (Arkus.Get_Mesh()->IsAnimationEnded(0.6f))
				return CArkus_Run::Create(m_pGraphic_Device, Arkus, &m_iAniState);
		}
		else if (Arkus.Get_CollisionPartCheck(Arkus.COLL_ATTACK_AREA) == true)
		{
			if (Arkus.Get_Mesh()->IsAnimationEnded(0.6f))
			{
				m_iAniState = rand() % 5 + 4;
				return CArkus_Attack::Create(m_pGraphic_Device, Arkus, &m_iAniState);
			//	return CArkus_Idle::Create(m_pGraphic_Device, Arkus, &m_iAniState);

				//if (EnemyPositionCheck(Arkus) == true)
				//{
				//	m_iAniState = rand() % 5 + 4;
				//	return CArkus_Attack::Create(m_pGraphic_Device, Arkus, &m_iAniState);
				//}
				//else if (EnemyPositionCheck(Arkus) == false)
				//{
				//	if (m_bIsTurnRight == true)
				//	{
				//		m_iAniState = 1;
				//		return CArkus_Attack::Create(m_pGraphic_Device, Arkus, &m_iAniState);
				//	}
				//	else if (m_bIsTurnRight == false)
				//	{
				//		m_iAniState = 2;
				//		return CArkus_Attack::Create(m_pGraphic_Device, Arkus, &m_iAniState);
				//	}
				//}
			}
		}
		else
			return nullptr;
	}

	return nullptr;
}

void CArkus_Idle::Update_State(CArkus & Arkus, const float & fTimeDelta)
{
}

_bool CArkus_Idle::EnemyPositionCheck(CArkus& Arkus)
{
	_vec3 vPlayerPos = *dynamic_cast<CPlayer*>(CObject_Manager::GetInstance()->Get_Object(SCENE_STATIC, L"Layer_Player"))->Get_Transform()->Get_StateInfo(CTransform::STATE_POSITION);
	
	_vec3 vDir = vPlayerPos - (*Arkus.Get_Transform()->Get_StateInfo(CTransform::STATE_POSITION));
	D3DXVec3Normalize(&vDir, &vDir);

	_vec3 vArkusView = *Arkus.Get_Transform()->Get_StateInfo(CTransform::STATE_LOOK);
	D3DXVec3Normalize(&vArkusView, &vArkusView);

	_float fDegree = D3DXVec3Dot(&vDir, &(vArkusView));

	// 플레이어가 앞에 있으면,
	if (fDegree > 0.f)
		return true;

	// 뒤에 있으면,
	else if (fDegree <= 0.f)
	{		
		fDegree = D3DXVec3Dot(&vDir, Arkus.Get_Transform()->Get_StateInfo(CTransform::STATE_RIGHT));

		if (fDegree < 0)
			m_bIsTurnRight = false;

		else if (fDegree > 0)
			m_bIsTurnRight = true;

		return false;
	}
}

CArkus_Idle * CArkus_Idle::Create(LPDIRECT3DDEVICE9 pGraphicDevice, CArkus & Arkus, void * pArg)
{
	CArkus_Idle* pInstance = new CArkus_Idle(pGraphicDevice);

	if (FAILED(pInstance->Initialize_State(Arkus)))
		Safe_Release(pInstance);

	return pInstance;
}

void CArkus_Idle::Free()
{
	CArkusState::Free();
}
