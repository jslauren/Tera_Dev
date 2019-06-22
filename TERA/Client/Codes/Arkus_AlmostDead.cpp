#include "stdafx.h"
#include "..\Headers\Arkus_AlmostDead.h"
#include "Arkus.h"
#include "Input_Device.h"

#include "Arkus_Idle.h"

_USING(Client)

CArkus_AlmostDead::CArkus_AlmostDead(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CArkusState(pGraphic_Device)
{
}

HRESULT CArkus_AlmostDead::Initialize_State(CArkus & Arkus)
{
	Arkus.Set_AniIndex(CArkus::ARKUS_ANI::AlmostDead);
	Arkus.Set_ActionID(CArkus::ACTION_ID::ACTION_STUN);

	return NOERROR;
}

CArkusState * CArkus_AlmostDead::Input_State(CArkus & Arkus, const float & fTimeDelta, BYTE KeyID, void * pArg)
{
	if (Arkus.Get_Mesh()->Get_NowPlayAniIndex() == CArkus::ARKUS_ANI::AlmostDead)
	{
		if (Arkus.Get_Mesh()->IsAnimationEnded(0.95f))
		{
			m_iAniState = 1;
			return CArkus_Idle::Create(m_pGraphic_Device, Arkus, &m_iAniState);
		}
	}
}

void CArkus_AlmostDead::Update_State(CArkus & Arkus, const float & fTimeDelta)
{
}

CArkus_AlmostDead * CArkus_AlmostDead::Create(LPDIRECT3DDEVICE9 pGraphicDevice, CArkus & Arkus, void * pArg)
{
	CArkus_AlmostDead* pInstance = new CArkus_AlmostDead(pGraphicDevice);

	if (FAILED(pInstance->Initialize_State(Arkus)))
		Safe_Release(pInstance);

	return pInstance;
}

void CArkus_AlmostDead::Free()
{
	CArkusState::Free();
}
