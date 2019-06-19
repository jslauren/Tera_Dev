#include "stdafx.h"
#include "..\Headers\Arkus_Idle.h"
#include "Arkus.h"
#include "Input_Device.h"

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
	return nullptr;
}

void CArkus_Idle::Update_State(CArkus & Arkus, const float & fTimeDelta)
{
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
