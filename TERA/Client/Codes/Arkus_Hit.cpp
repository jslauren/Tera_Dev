#include "stdafx.h"
#include "..\Headers\Arkus_Hit.h"
#include "Arkus.h"
#include "Input_Device.h"
#include "Time.h"

#include "Arkus_Idle.h"
#include "Arkus_Attack.h"
#include "Arkus_JumpEvasion.h"

_USING(Client)

CArkus_Hit::CArkus_Hit(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CArkusState(pGraphic_Device)
{
}

HRESULT CArkus_Hit::Initialize_State(CArkus & Arkus)
{
	Arkus.Set_AniIndex(CArkus::ARKUS_ANI::Hit);

	return NOERROR;
}

CArkusState * CArkus_Hit::Input_State(CArkus & Arkus, const float & fTimeDelta, BYTE KeyID, void * pArg)
{
	if (Arkus.Get_Mesh()->Get_NowPlayAniIndex() == CArkus::ARKUS_ANI::Hit)
	{
		if (Arkus.Get_Mesh()->IsAnimationEnded(0.95f))
		{
			srand((unsigned)time(NULL));

			m_iRandomState = rand() % 10;

			if (m_iRandomState >= 0 && m_iRandomState < 6)
			{
				m_iAniState = rand() % 5 + 4;
				return CArkus_Attack::Create(m_pGraphic_Device, Arkus, &m_iAniState);
				/*m_iAniState = 1;
				return CArkus_Idle::Create(m_pGraphic_Device, Arkus, &m_iAniState);*/
			}
			else if(m_iRandomState >= 6 && m_iRandomState < 9)
				return CArkus_Idle::Create(m_pGraphic_Device, Arkus, &m_iAniState);
			else
				return CArkus_JumpEvasion::Create(m_pGraphic_Device, Arkus, &m_iAniState);
		}
	}
}

void CArkus_Hit::Update_State(CArkus & Arkus, const float & fTimeDelta)
{
}

CArkus_Hit * CArkus_Hit::Create(LPDIRECT3DDEVICE9 pGraphicDevice, CArkus & Arkus, void * pArg)
{
	CArkus_Hit* pInstance = new CArkus_Hit(pGraphicDevice);

	if (FAILED(pInstance->Initialize_State(Arkus)))
		Safe_Release(pInstance);

	return pInstance;
}

void CArkus_Hit::Free()
{
	CArkusState::Free();
}
