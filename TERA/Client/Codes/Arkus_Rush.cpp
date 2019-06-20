#include "stdafx.h"
#include "..\Headers\Arkus_Rush.h"
#include "Arkus.h"
#include "Input_Device.h"

#include "Arkus_Idle.h"

_USING(Client)

CArkus_Rush::CArkus_Rush(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CArkusState(pGraphic_Device)
{
}

HRESULT CArkus_Rush::Initialize_State(CArkus & Arkus)
{
	Arkus.Set_ActionID(CArkus::ACTION_ID::ACTION_PHYSICSSKILL);

	return NOERROR;
}

CArkusState * CArkus_Rush::Input_State(CArkus & Arkus, const float & fTimeDelta, BYTE KeyID, void * pArg)
{
	if (Arkus.Get_Mesh()->Get_NowPlayAniIndex() == CArkus::ARKUS_ANI::MoveAtkStart)
	{
		if (Arkus.Get_Mesh()->IsAnimationEnded(0.85f))
		{
			m_iAniState = 2;
			return CArkus_Rush::Create(m_pGraphic_Device, Arkus, &m_iAniState);
		}
	}
	else if (Arkus.Get_Mesh()->Get_NowPlayAniIndex() == CArkus::ARKUS_ANI::MoveAtkLoop)
	{
		MovePlayerPosition(Arkus, 40.f, fTimeDelta, pArg, 0);

		if (Arkus.Get_Mesh()->IsAnimationEnded(0.6f))
		{
			if (Arkus.Get_CollisionPartCheck(6) == true)
			{
				m_iAniState = 3;
				return CArkus_Rush::Create(m_pGraphic_Device, Arkus, &m_iAniState);
			}
			else
				return nullptr;
		}
	}
	else if (Arkus.Get_Mesh()->Get_NowPlayAniIndex() == CArkus::ARKUS_ANI::MoveAtkEnd)
	{
		MovePlayerPosition(Arkus, 30.f, fTimeDelta, pArg, 0);

		if (Arkus.Get_Mesh()->IsAnimationEnded(0.85f))
		{
			m_iAniState = 2;
			return CArkus_Idle::Create(m_pGraphic_Device, Arkus, &m_iAniState);
		}
	}

	return nullptr;
}

void CArkus_Rush::Update_State(CArkus & Arkus, const float & fTimeDelta)
{
}

void CArkus_Rush::MovePlayerPosition(CArkus & Arkus, const _float fArkusSpeed, const _float & fTimeDelta, void * pArg, _int iMoveDir)
{
	_uint		iCellIndx = 0;
	if (true == ((CNavigation*)(pArg))->Move_OnNavigation(Arkus.Get_Transform()->Get_StateInfo(CTransform::STATE_POSITION), Arkus.Get_Transform()->Get_StateInfo(CTransform::STATE_LOOK), 30.0f * fTimeDelta, &iCellIndx))
	{
		_vec3 vPlayerPos, vDir;
		vPlayerPos = *dynamic_cast<CPlayer*>(CObject_Manager::GetInstance()->Get_Object(SCENE_STATIC, L"Layer_Player"))->Get_Transform()->Get_StateInfo(CTransform::STATE_POSITION);
		vDir = vPlayerPos - *Arkus.Get_Transform()->Get_StateInfo(CTransform::STATE_POSITION);

		Arkus.Get_Transform()->Move(&vDir, fArkusSpeed, fTimeDelta);

		/* ※※※※※※※진짜 이동하면 꼭 호출해야합니다※※※※※※.*/
		((CNavigation*)(pArg))->SetUp_CurrentIndex(iCellIndx);
	}
}

CArkus_Rush * CArkus_Rush::Create(LPDIRECT3DDEVICE9 pGraphicDevice, CArkus & Arkus, void * pArg)
{
	CArkus_Rush* pInstance = new CArkus_Rush(pGraphicDevice);

	if (FAILED(pInstance->Initialize_State(Arkus)))
		Safe_Release(pInstance);

	if (*(_int*)(pArg) == 1)
		Arkus.Set_AniIndex(CArkus::ARKUS_ANI::MoveAtkStart);

	else if ((*(_int*)(pArg) == 2))
		Arkus.Set_AniIndex(CArkus::ARKUS_ANI::MoveAtkLoop);

	else if ((*(_int*)(pArg) == 3))
		Arkus.Set_AniIndex(CArkus::ARKUS_ANI::MoveAtkEnd);

	return pInstance;
}

void CArkus_Rush::Free()
{
	CArkusState::Free();
}
