#include "stdafx.h"
#include "..\Headers\Arkus_Run.h"
#include "Arkus.h"
#include "Input_Device.h"
#include "Time.h"

//#include "Arkus_Idle.h"
#include "Arkus_Attack.h"

_USING(Client)

CArkus_Run::CArkus_Run(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CArkusState(pGraphic_Device)
{
}

HRESULT CArkus_Run::Initialize_State(CArkus & Arkus)
{
	Arkus.Set_AniIndex(CArkus::ARKUS_ANI::Run_Battle);
	Arkus.Set_ActionID(CArkus::ACTION_ID::ACTION_READY);

	return NOERROR;
}

CArkusState * CArkus_Run::Input_State(CArkus & Arkus, const float & fTimeDelta, BYTE KeyID, void * pArg)
{
	if (Arkus.Get_Mesh()->Get_NowPlayAniIndex() == CArkus::ARKUS_ANI::Run_Battle)
	{
		MovePlayerPosition(Arkus, 40.f, fTimeDelta, pArg, 0);

		srand((unsigned)time(NULL));

		if (Arkus.Get_Mesh()->IsAnimationEnded(0.85f))
		{
			if (Arkus.Get_CollisionPartCheck(Arkus.COLL_ATKAREA) == true)
			{
				m_iAniState = 5;// rand() % 8 + 2;
				return CArkus_Attack::Create(m_pGraphic_Device, Arkus, &m_iAniState);
			}
		}
		else
			return nullptr;
	}

	return nullptr;
}

void CArkus_Run::Update_State(CArkus & Arkus, const float & fTimeDelta)
{
}

void CArkus_Run::MovePlayerPosition(CArkus & Arkus, const _float fArkusSpeed, const _float & fTimeDelta, void * pArg, _int iMoveDir)
{
	_uint		iCellIndx = 0;
	if (true == ((CNavigation*)(pArg))->Move_OnNavigation(Arkus.Get_Transform()->Get_StateInfo(CTransform::STATE_POSITION), Arkus.Get_Transform()->Get_StateInfo(CTransform::STATE_LOOK), 30.0f * fTimeDelta, &iCellIndx))
	{
		_vec3 vPlayerPos, vDir;
		vPlayerPos = *dynamic_cast<CPlayer*>(CObject_Manager::GetInstance()->Get_Object(SCENE_STATIC, L"Layer_Player"))->Get_Transform()->Get_StateInfo(CTransform::STATE_POSITION);
		vDir = vPlayerPos - *Arkus.Get_Transform()->Get_StateInfo(CTransform::STATE_POSITION);

		Arkus.Get_Transform()->Move(&vDir, fArkusSpeed, fTimeDelta);

		/* �ءءءءءء���¥ �̵��ϸ� �� ȣ���ؾ��մϴ١ءءءءء�.*/
		((CNavigation*)(pArg))->SetUp_CurrentIndex(iCellIndx);
	}
}

CArkus_Run * CArkus_Run::Create(LPDIRECT3DDEVICE9 pGraphicDevice, CArkus & Arkus, void * pArg)
{
	CArkus_Run* pInstance = new CArkus_Run(pGraphicDevice);

	if (FAILED(pInstance->Initialize_State(Arkus)))
		Safe_Release(pInstance);

	return pInstance;
}

void CArkus_Run::Free()
{
	CArkusState::Free();
}
