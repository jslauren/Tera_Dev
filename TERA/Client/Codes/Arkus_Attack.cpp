#include "stdafx.h"
#include "..\Headers\Arkus_Attack.h"
#include "Arkus.h"
#include "Input_Device.h"

#include "Arkus_Idle.h"

_USING(Client)

CArkus_Attack::CArkus_Attack(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CArkusState(pGraphic_Device)
{
}

HRESULT CArkus_Attack::Initialize_State(CArkus & Arkus)
{
	Arkus.Set_ActionID(CArkus::ACTION_ID::ACTION_ATTACK);

	return NOERROR;
}

CArkusState * CArkus_Attack::Input_State(CArkus & Arkus, const float & fTimeDelta, BYTE KeyID, void * pArg)
{
	return nullptr;
}

void CArkus_Attack::Update_State(CArkus & Arkus, const float & fTimeDelta)
{
}

void CArkus_Attack::MovePlayerPosition(CArkus & Arkus, const _float fArkusSpeed, const _float & fTimeDelta, void * pArg, _int iMoveDir)
{
}

CArkus_Attack * CArkus_Attack::Create(LPDIRECT3DDEVICE9 pGraphicDevice, CArkus & Arkus, void * pArg)
{
	return nullptr;
}

void CArkus_Attack::Free()
{
}
