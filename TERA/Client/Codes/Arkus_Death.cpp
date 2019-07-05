#include "stdafx.h"
#include "..\Headers\Arkus_Death.h"
#include "Arkus.h"
#include "Input_Device.h"

#include "Arkus_Idle.h"

_USING(Client)

CArkus_Death::CArkus_Death(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CArkusState(pGraphic_Device)
{
}

HRESULT CArkus_Death::Initialize_State(CArkus & Arkus)
{
	Arkus.Set_AniIndex(CArkus::ARKUS_ANI::Death);

	return NOERROR;
}

CArkusState * CArkus_Death::Input_State(CArkus & Arkus, const float & fTimeDelta, BYTE KeyID, void * pArg)
{
	if (Arkus.Get_Mesh()->Get_NowPlayAniIndex() == CArkus::ARKUS_ANI::Death)
	{
		if (m_bIsDead == true)
			Arkus.Get_Mesh()->Play_Animation(0);

		if (Arkus.Get_Mesh()->IsAnimationEnded(0.95f))
			m_bIsDead = true;

		return nullptr;
	}
	return nullptr;
}

void CArkus_Death::Update_State(CArkus & Arkus, const float & fTimeDelta)
{
}

CArkus_Death * CArkus_Death::Create(LPDIRECT3DDEVICE9 pGraphicDevice, CArkus & Arkus, void * pArg)
{
	CArkus_Death* pInstance = new CArkus_Death(pGraphicDevice);

	if (FAILED(pInstance->Initialize_State(Arkus)))
		Safe_Release(pInstance);

	return pInstance;
}

void CArkus_Death::Free()
{
	CArkusState::Free();
}
