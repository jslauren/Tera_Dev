#include "stdafx.h"
#include "..\Headers\PlayerState.h"

_USING(Client)

CPlayerState::CPlayerState(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device(pGraphic_Device)
{
	Safe_AddRef(m_pGraphic_Device);
}

HRESULT CPlayerState::Initialize_State(CPlayer & Player)
{
	return NOERROR;
}

CPlayerState * CPlayerState::Input_Keyboard(CPlayer & Player, const float & fTimeDelta, BYTE KeyID, void* pAgr)
{
	return nullptr;
}

void CPlayerState::Update_State(CPlayer & Player, const float & fTimeDelta)
{
}

HRESULT CPlayerState::Enter_State(CPlayer & Player)
{
	return NOERROR;
}

void CPlayerState::Free()
{
	Safe_Release(m_pGraphic_Device);
}
