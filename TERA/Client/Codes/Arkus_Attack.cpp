#include "stdafx.h"
#include "..\Headers\Arkus_Attack.h"
#include "Arkus.h"
#include "Input_Device.h"
#include "Camera_Static.h"

#include "Arkus_Idle.h"
#include "Arkus_Rush.h"

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
	CCamera_Static* pCamera_Static = dynamic_cast<CCamera_Static*>(CObject_Manager::GetInstance()->Get_Object(SCENE_DRAGON, L"Layer_Camera", 1));

	if (Arkus.Get_Mesh()->Get_NowPlayAniIndex() == CArkus::ARKUS_ANI::RoundAtk01)
	{
		if (Arkus.Get_Mesh()->IsAnimationEnded(0.85f))
			return CArkus_Idle::Create(m_pGraphic_Device, Arkus, &m_iAniState);
	}
	else if (Arkus.Get_Mesh()->Get_NowPlayAniIndex() == CArkus::ARKUS_ANI::RoundAtk02)
	{
		if (Arkus.Get_Mesh()->IsAnimationEnded(0.85f))
			return CArkus_Idle::Create(m_pGraphic_Device, Arkus, &m_iAniState);
	}
	if (Arkus.Get_Mesh()->Get_NowPlayAniIndex() == CArkus::ARKUS_ANI::Atk01)
	{
		if (Arkus.Get_Mesh()->IsAnimationEnded(0.85f))
			return CArkus_Idle::Create(m_pGraphic_Device, Arkus, &m_iAniState);
	}
	else if (Arkus.Get_Mesh()->Get_NowPlayAniIndex() == CArkus::ARKUS_ANI::HeavyAtk02)
	{
		if (Arkus.Get_Mesh()->IsAnimationEnded(0.85f))
			return CArkus_Idle::Create(m_pGraphic_Device, Arkus, &m_iAniState);
	}
	else if (Arkus.Get_Mesh()->Get_NowPlayAniIndex() == CArkus::ARKUS_ANI::FlyAtk01)
	{
		// 카메라 제어.
		if (pCamera_Static->Get_CameraModInfo() == true)
		{
			if (pCamera_Static->Get_CemeraDistance() < 130.f &&
				pCamera_Static->Get_CameraHeightValue() < 45.f)
			{
				pCamera_Static->Set_CameraDistance(pCamera_Static->Get_CemeraDistance() + 30.f * fTimeDelta);
				pCamera_Static->Set_CameraHeightValue(pCamera_Static->Get_CameraHeightValue() + 10.f * fTimeDelta);
			}
			// 여기 또한 배수로 설정해 줘야 한다.
			else if (pCamera_Static->Get_CemeraDistance() >= 125.f &&
				pCamera_Static->Get_CameraHeightValue() >= 35.f)
			{
				// 원하는 위치까지 카메라 조작을 완료하면,
				// 이제 원래 카메라 위치로 돌아가기 위해 불 변수를 준다.
				// 원래대로 돌아갈꺼야 라는 뜻은 => m_bCameraModOn가 false라는 뜻이다.
				pCamera_Static->Set_CameraModInfo(false);
			}
		}
		// m_bCameraModOn가 false이므로 원래 위치로 카메라가 돌아갈 시간이 되었다는 뜻이다.
		else if (pCamera_Static->Get_CameraModInfo() == false)
		{
			// 카메라를 멀리 줌 아웃 했다가 바로 줌인 해버리면,
			// 몬스터의 애니메이션과 위치를 파악할 수 없기 때문에,
			// 밑에서 IsAnimationEnded(0.8f)로 일정 시간이 흐른 뒤에,
			// 카메라를 원래 위치로 돌리고 싶어 불 변수를 또 하나 준다.
			if(pCamera_Static->Get_CameraBackTimeInfo() == true)
			{
				// 돌아갈 준비가 모두 마쳐졌다면, 초기 값 만큼만 돌아가야 한다.
				if (pCamera_Static->Get_CemeraDistance() >= 30.f &&
					pCamera_Static->Get_CameraHeightValue() >= 10.f)
				{
					pCamera_Static->Set_CameraDistance(pCamera_Static->Get_CemeraDistance() - 30.f * fTimeDelta);
					pCamera_Static->Set_CameraHeightValue(pCamera_Static->Get_CameraHeightValue() - 10.f * fTimeDelta);
				}
			}
		}
		// 애니메이션이 이 정도 시간 진척이 되었다면, 
		// 돌아갈 준비가 모두 완료 되었다.
		if (Arkus.Get_Mesh()->IsAnimationEnded(0.7f))
			pCamera_Static->Set_CameraBackTimeInfo(true);

		if (Arkus.Get_Mesh()->IsAnimationEnded(0.9f))
		{
			// 애니메이션이 종료될 때
			if (pCamera_Static->Get_CameraModInfo() == false)
			{
				// 모든 불 변수 값을 초기 값으로 되돌려 준다.
				pCamera_Static->Set_CameraModInfo(true);
				pCamera_Static->Set_CameraBackTimeInfo(false);

				pCamera_Static->Set_CameraDistance(30.f);
				pCamera_Static->Set_CameraHeightValue(10.f);

				return CArkus_Idle::Create(m_pGraphic_Device, Arkus, &m_iAniState);
			}
		}
	}
	else if (Arkus.Get_Mesh()->Get_NowPlayAniIndex() == CArkus::ARKUS_ANI::FlyAtk02Start)
	{
		if (Arkus.Get_Mesh()->IsAnimationEnded(0.85f))
		{
			m_iAniState = 7;
			return CArkus_Attack::Create(m_pGraphic_Device, Arkus, &m_iAniState);
		}
	}
	else if (Arkus.Get_Mesh()->Get_NowPlayAniIndex() == CArkus::ARKUS_ANI::FlyAtk02End)
	{
		MovePlayerPosition(Arkus, 40.f, fTimeDelta, pArg, 0);

		if (Arkus.Get_Mesh()->IsAnimationEnded(0.85f))
		{
			return CArkus_Idle::Create(m_pGraphic_Device, Arkus, &m_iAniState);
		}
	}
	else if (Arkus.Get_Mesh()->Get_NowPlayAniIndex() == CArkus::ARKUS_ANI::MoveAtkStart)
	{
		m_iAniState = 1;
		return CArkus_Rush::Create(m_pGraphic_Device, Arkus, &m_iAniState);
	}

	return nullptr;
}

void CArkus_Attack::Update_State(CArkus & Arkus, const float & fTimeDelta)
{
}

void CArkus_Attack::MovePlayerPosition(CArkus & Arkus, const _float fArkusSpeed, const _float & fTimeDelta, void * pArg, _int iMoveDir)
{
	_uint		iCellIndx = 0;
	if (true == ((CNavigation*)(pArg))->Move_OnNavigation(Arkus.Get_Transform()->Get_StateInfo(CTransform::STATE_POSITION), Arkus.Get_Transform()->Get_StateInfo(CTransform::STATE_LOOK), 30.0f * fTimeDelta, &iCellIndx))
	{
		_vec3 vPlayerPos, vDir;
		vPlayerPos = *dynamic_cast<CPlayer*>(CObject_Manager::GetInstance()->Get_Object(SCENE_STATIC, L"Layer_Player"))->Get_Transform()->Get_StateInfo(CTransform::STATE_POSITION);
		vDir = vPlayerPos - *Arkus.Get_Transform()->Get_StateInfo(CTransform::STATE_POSITION);

		Arkus.Get_Transform()->Move(&vDir, fArkusSpeed, fTimeDelta);

		/* ※※※※※※※진짜 이동하면 꼭 호출해야합니다※※※※※※.*/
		((CNavigation*)(pArg))->SetUp_CurrentIndex(iCellIndx);
	}
}

CArkus_Attack * CArkus_Attack::Create(LPDIRECT3DDEVICE9 pGraphicDevice, CArkus & Arkus, void * pArg)
{
	CArkus_Attack* pInstance = new CArkus_Attack(pGraphicDevice);

	if (*(_int*)(pArg) == 1)
		Arkus.Set_AniIndex(CArkus::ARKUS_ANI::RoundAtk01);

	else if ((*(_int*)(pArg) == 2))
		Arkus.Set_AniIndex(CArkus::ARKUS_ANI::RoundAtk02);

	else if ((*(_int*)(pArg) == 3))
		Arkus.Set_AniIndex(CArkus::ARKUS_ANI::Atk01);

	else if ((*(_int*)(pArg) == 4))
		Arkus.Set_AniIndex(CArkus::ARKUS_ANI::HeavyAtk02);

	else if ((*(_int*)(pArg) == 5))
		Arkus.Set_AniIndex(CArkus::ARKUS_ANI::FlyAtk01);

	else if ((*(_int*)(pArg) == 6))
		Arkus.Set_AniIndex(CArkus::ARKUS_ANI::FlyAtk02Start);

	else if ((*(_int*)(pArg) == 7))
		Arkus.Set_AniIndex(CArkus::ARKUS_ANI::FlyAtk02Start);

	else if ((*(_int*)(pArg) == 8))
		Arkus.Set_AniIndex(CArkus::ARKUS_ANI::MoveAtkStart);

	if (FAILED(pInstance->Initialize_State(Arkus)))
		Safe_Release(pInstance);

	return pInstance;
}

void CArkus_Attack::Free()
{
	CArkusState::Free();
}
