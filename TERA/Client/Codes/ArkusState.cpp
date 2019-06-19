#include "stdafx.h"
#include "..\Headers\ArkusState.h"

_USING(Client)

CArkusState::CArkusState(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device(pGraphic_Device)
{
	Safe_AddRef(m_pGraphic_Device);
}

HRESULT CArkusState::Initialize_State(CArkus & Player)
{
	return NOERROR;
}

CArkusState * CArkusState::Input_State(CArkus & Player, const float & fTimeDelta, BYTE KeyID, void * pAgr)
{
	return nullptr;
}

void CArkusState::Update_State(CArkus & Player, const float & fTimeDelta)
{
}

HRESULT CArkusState::Enter_State(CArkus & Player)
{
	return NOERROR;
}

void CArkusState::Free()
{
	Safe_Release(m_pGraphic_Device);
}
