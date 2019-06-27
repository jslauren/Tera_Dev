#include "stdafx.h"
#include "..\Headers\Arkus_JumpEvasion.h"
#include "Arkus.h"
#include "Input_Device.h"

#include "Arkus_Idle.h"

_USING(Client)

CArkus_JumpEvasion::CArkus_JumpEvasion(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CArkusState(pGraphic_Device)
{
}

HRESULT CArkus_JumpEvasion::Initialize_State(CArkus & Arkus)
{
	Arkus.Set_AniIndex(CArkus::ARKUS_ANI::JumpEvasion);
	Arkus.Set_ActionID(CArkus::ACTION_ID::ACTION_PHYSICSSKILL);

	return NOERROR;
}

CArkusState * CArkus_JumpEvasion::Input_State(CArkus & Arkus, const float & fTimeDelta, BYTE KeyID, void * pArg)
{
	if (Arkus.Get_Mesh()->Get_NowPlayAniIndex() == CArkus::ARKUS_ANI::JumpEvasion)
	{
		if (Arkus.Get_Mesh()->IsAnimationEnded(0.25f))
			MoveArkusPosition(Arkus, 40.f, fTimeDelta, pArg, 1);

		if (Arkus.Get_Mesh()->IsAnimationEnded(0.85f))
		{
			m_iAniState = 1;
			return CArkus_Idle::Create(m_pGraphic_Device, Arkus, &m_iAniState);
		}
		else
			return nullptr;
	}

	return nullptr;
}

void CArkus_JumpEvasion::Update_State(CArkus & Arkus, const float & fTimeDelta)
{
}

void CArkus_JumpEvasion::MoveArkusPosition(CArkus & Arkus, const _float fArkusSpeed, const _float & fTimeDelta, void * pArg, _int iMoveDir)
{
	_uint		iCellIndx = 0;
	if (true == ((CNavigation*)(pArg))->Move_OnNavigation(Arkus.Get_Transform()->Get_StateInfo(CTransform::STATE_POSITION), Arkus.Get_Transform()->Get_StateInfo(CTransform::STATE_LOOK), 30.0f * fTimeDelta, &iCellIndx))
	{
		_vec3 vPlayerPos, vDir;
		vPlayerPos = *dynamic_cast<CPlayer*>(CObject_Manager::GetInstance()->Get_Object(SCENE_STATIC, L"Layer_Player"))->Get_Transform()->Get_StateInfo(CTransform::STATE_POSITION);
		vDir = -(vPlayerPos - *Arkus.Get_Transform()->Get_StateInfo(CTransform::STATE_POSITION));

		Arkus.Get_Transform()->Move(&vDir, fArkusSpeed, fTimeDelta);

		/* ※※※※※※※진짜 이동하면 꼭 호출해야합니다※※※※※※.*/
		((CNavigation*)(pArg))->SetUp_CurrentIndex(iCellIndx);
	}
}

CArkus_JumpEvasion * CArkus_JumpEvasion::Create(LPDIRECT3DDEVICE9 pGraphicDevice, CArkus & Arkus, void * pArg)
{
	CArkus_JumpEvasion* pInstance = new CArkus_JumpEvasion(pGraphicDevice);

	if (FAILED(pInstance->Initialize_State(Arkus)))
		Safe_Release(pInstance);

	return pInstance;
}

void CArkus_JumpEvasion::Free()
{
	CArkusState::Free();
}
