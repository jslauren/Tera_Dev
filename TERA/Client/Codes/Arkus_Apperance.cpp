#include "stdafx.h"
#include "..\Headers\Arkus_Apperance.h"
#include "Arkus.h"
#include "Input_Device.h"

#include "Arkus_Rush.h"
#include "Arkus_Attack.h"

#include "UI_BossPoint.h"

_USING(Client)

CArkus_Apperance::CArkus_Apperance(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CArkusState(pGraphic_Device)
{
}

HRESULT CArkus_Apperance::Initialize_State(CArkus & Arkus)
{
	//Arkus.Set_AniIndex(CArkus::ARKUS_ANI::Apperance01);
	Arkus.Set_ActionID(CArkus::ACTION_ID::ACTION_READY);

	return NOERROR;
}

CArkusState * CArkus_Apperance::Input_State(CArkus & Arkus, const float & fTimeDelta, BYTE KeyID, void * pArg)
{
	if (Arkus.Get_Mesh()->Get_NowPlayAniIndex() == CArkus::ARKUS_ANI::Apperance01)
	{
		if (Arkus.Get_Mesh()->IsAnimationEnded(0.85f))
		{
			dynamic_cast<CUI_BossPoint*>(CObject_Manager::GetInstance()->Get_Object(SCENE_DRAGON, L"Layer_UI"))->Set_CutSceneStart(true);

			m_iAniState = 2;
			return CArkus_Apperance::Create(m_pGraphic_Device, Arkus, &m_iAniState);
		}
	}
	else if (Arkus.Get_Mesh()->Get_NowPlayAniIndex() == CArkus::ARKUS_ANI::Apperance02)
	{
		if (Arkus.Get_Mesh()->IsAnimationEnded(0.98f))
		{
			m_iAniState = 1;
			return CArkus_Rush::Create(m_pGraphic_Device, Arkus, &m_iAniState);
		}
	}

	return nullptr;
}

void CArkus_Apperance::Update_State(CArkus & Arkus, const float & fTimeDelta)
{
}

CArkus_Apperance * CArkus_Apperance::Create(LPDIRECT3DDEVICE9 pGraphicDevice, CArkus & Arkus, void * pArg)
{
	CArkus_Apperance* pInstance = new CArkus_Apperance(pGraphicDevice);

	if (FAILED(pInstance->Initialize_State(Arkus)))
		Safe_Release(pInstance);

	if (*(_int*)(pArg) == 1)
		Arkus.Set_AniIndex(CArkus::ARKUS_ANI::Apperance01);

	else if ((*(_int*)(pArg) == 2))
		Arkus.Set_AniIndex(CArkus::ARKUS_ANI::Apperance02);

	return pInstance;
}

void CArkus_Apperance::Free()
{
	CArkusState::Free();
}
