#include "stdafx.h"
#include "..\Headers\Player_Idle.h"

_USING(Client)

CPlayer_Idle::CPlayer_Idle(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CPlayerState(pGraphic_Device)
{
}

HRESULT CPlayer_Idle::Initialize_State(CPlayer & Player)
{
	return E_NOTIMPL;
}

CPlayerState * CPlayer_Idle::Input_Keyboard(CPlayer & Player, const float & fTimeDelta, BYTE KeyID)
{
	return nullptr;
}

void CPlayer_Idle::Update_State(CPlayer & Player, const float & fTimeDelta)
{
}

CPlayer_Idle * CPlayer_Idle::Create(LPDIRECT3DDEVICE9 pGraphicDevice, CPlayer & Player)
{
	return nullptr;
}

void CPlayer_Idle::Free()
{
}
