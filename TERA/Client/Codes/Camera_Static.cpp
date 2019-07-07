#include "stdafx.h"
#include "..\Headers\Camera_Static.h"
#include "Input_Device.h"
#include "EventManager.h"
#include "UI_Dialog.h"
#include "Layer.h"
#include "Player.h"
#include "Arkus.h"

_USING(Client)

CCamera_Static::CCamera_Static(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCamera(pGraphic_Device)
	, m_pInput_Device(CInput_Device::GetInstance())
{
	m_pInput_Device->AddRef();
}

CCamera_Static::CCamera_Static(const CCamera_Static & rhs)
	: CCamera(rhs)
	, m_pInput_Device(rhs.m_pInput_Device)
{
	m_pInput_Device->AddRef();
}

HRESULT CCamera_Static::Ready_GameObject_Prototype()
{
	return NOERROR;
}

HRESULT CCamera_Static::Ready_GameObject(void * pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	CCamera::CAMERAINFO*	pCameraInfo = (CCamera::CAMERAINFO*)pArg;

	_vec3		vLook, vRight, vUp;
	vLook = pCameraInfo->vAt - pCameraInfo->vEye;
	D3DXVec3Cross(&vRight, &pCameraInfo->vAxisY, &vLook);
	D3DXVec3Cross(&vUp, &vLook, &vRight);

	m_pTransformCom->Set_StateInfo(CTransform::STATE_RIGHT, D3DXVec3Normalize(&vRight, &vRight));
	m_pTransformCom->Set_StateInfo(CTransform::STATE_UP, D3DXVec3Normalize(&vUp, &vUp));
	m_pTransformCom->Set_StateInfo(CTransform::STATE_LOOK, D3DXVec3Normalize(&vLook, &vLook));
	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(pCameraInfo->vEye));

	m_CameraInfo = *pCameraInfo;

	SetUp_ViewMatrix();
	SetUp_ProjectionMatrix();

	EventRegist();

	return NOERROR;
}

_int CCamera_Static::Update_GameObject(const _float & fTimeDelta)
{
	m_fTimeDelta = fTimeDelta;

	if (CInput_Device::GetInstance()->Get_DIKeyDown(DIK_Z))
	{
		if (m_bIsCameraReverting != true)
			m_bIsCameraReverting = true;
	}

	ChangeView();
	TracingPlayer();
	RevertInitValue();
	TalkEvent();

	return _int();
}

_int CCamera_Static::LateUpdate_GameObject(const _float & fTimeDelta)
{
	SetUp_ViewMatrix();
	SetUp_ProjectionMatrix();
	
	return _int();
}

HRESULT CCamera_Static::Render_GameObject()
{
	return NOERROR;
}

HRESULT CCamera_Static::OnEvent(const _tchar * _szEventTag, void * _pMsg)
{
	if (!lstrcmp(_szEventTag, L"Player_Inside_Event"))
	{
		if (*(_uint*)(_pMsg) == 0)
		{
			m_bIsPlayerInside = true;
			m_bIsCameraReverting = false;
			PlayerInsideEvent();
		}
		else if (*(_uint*)(_pMsg) == 1)
		{
			m_bIsPlayerInside = false;
			m_bIsCameraReverting = true;
			RevertInitValue();
		}
	}


	return NOERROR;
}

HRESULT CCamera_Static::Add_Component()
{
	// For.Com_Transform
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CCamera_Static::SetUp_ViewMatrix()
{
	if (m_bIsStaticCamOnAir == true)
	{
		if (nullptr == m_pTransformCom)
			return E_FAIL;

		_matrix matView = *m_pTransformCom->Compute_InverseWorldMatrixPointer();

		Set_Transform(D3DTS_VIEW, &matView);
	}

	return NOERROR;
}

HRESULT CCamera_Static::SetUp_ProjectionMatrix()
{
	if (m_bIsStaticCamOnAir == true)
	{
		_matrix matProj;

		D3DXMatrixPerspectiveFovLH(&matProj, m_CameraInfo.fFovY, m_CameraInfo.fAspect, m_CameraInfo.fNear, m_CameraInfo.fFar);

		Set_Transform(D3DTS_PROJECTION, &matProj);
	}

	return NOERROR;
}

void CCamera_Static::EventRegist()
{
	CEventManager::GetInstance()->Register_Object(L"Player_Inside_Event", this);
}

void CCamera_Static::ChangeView()
{
	POINT	ptMouse = { g_iWinCX >> 1, g_iWinCY >> 1 };

	ClientToScreen(g_hWnd, &ptMouse);
	SetCursorPos(ptMouse.x, ptMouse.y);

	// NPC와 대화중일땐 아무고또 모타죠? //
	CUI_Dialog* pUI_Dialog = dynamic_cast<CUI_Dialog*>(CObject_Manager::GetInstance()->Get_Object(SCENE_STAGE, L"Layer_UI", 1));

	if (pUI_Dialog->Get_TalkEventAvaliable() == true)
		return;
	///////////////////////////////////////

	if (m_bIsStaticCamOnAir == true)
	{
		if (nullptr == m_pInput_Device)
			return;

		_long	dwMouseMove = 0;

		_vec3	vCameraLook;
		_vec3	vWorldYvalue = { 0.f, 1.f, 0.f };

		vCameraLook = *m_pTransformCom->Get_StateInfo(CTransform::STATE_LOOK);

		D3DXVec3Normalize(&vCameraLook, &vCameraLook);

		_float fDotValue = D3DXVec3Dot(&vWorldYvalue, &vCameraLook);

		// 내적 한 값이 1 or -1 이면 vWorldYvalue 와 fDotValue가 딱 겹치는 것이다.
		// 0이면 90도 이다.

		if (dwMouseMove = m_pInput_Device->GetDIMouseMove(CInput_Device::DIMM_Y))
		{
			// 마우스 커서를 위로 올릴 때.
			// 더 많이 올리고 싶으면 밑에 0.3f 값을 더 크게 주면 된다.
			if (dwMouseMove < 0 && fDotValue < 0.3f)
			{
				//// 마우스 커서를 일정영역 밑으로 내리면 카메라 줌아웃 하는 구문.
				//if (fDotValue < 0.1f && fDotValue > m_fDotValuePri)
				//	m_fCameraDistance -= 0.01f;

				m_pTransformCom->Rotation_Axis(*m_pTransformCom->Get_StateInfo(CTransform::STATE_RIGHT), D3DXToRadian(dwMouseMove) * 7.f, m_fTimeDelta);
			}
			else if (dwMouseMove > 0 && fDotValue > -0.1f)
			{
				//// 카메라 줌인 구문.
				//if (fDotValue < -0.5f && fDotValue < m_fDotValuePri)
				//	m_fCameraDistance += 0.03f;

				m_pTransformCom->Rotation_Axis(*m_pTransformCom->Get_StateInfo(CTransform::STATE_RIGHT), D3DXToRadian(dwMouseMove) * 7.f, m_fTimeDelta);
			}
		}
		if (dwMouseMove = m_pInput_Device->GetDIMouseMove(CInput_Device::DIMM_X))
			m_pTransformCom->Rotation_Axis(_vec3(0.f, 1.f, 0.f), D3DXToRadian(dwMouseMove) * 10.f, m_fTimeDelta);

		// Arkus 공중공격 시, 카메라 제어를 수동으로 하기 때문에
		// 그 타이밍에 수동 제어를 막으려고 만들어 놓은 불 변수.
		if (m_bIsCameraCtrlAvailable == true)
			MouseWheelPlay();

		// 카메라 줌 인, 아웃을 하기위해 필요한 구문.
		m_fDotValuePri = fDotValue;
	}
}

void CCamera_Static::TracingPlayer()
{
	if (m_bIsStaticCamOnAir == true)
	{
		CTransform*	vPlayerTransformCom = ((CTransform*)(CObject_Manager::GetInstance()->Get_Component(SCENE_STATIC, L"Layer_Player", L"Com_Transform")));

		// 플레이어 포지션을 가져와서 카메라포지션 변수를 하나 만들어 넣어준다.
		_vec3 vCameraPos = *vPlayerTransformCom->Get_StateInfo(CTransform::STATE_POSITION);

		_vec3 vCameraLook;


		// 카메라의 Look 정보를 가져온 뒤
		vCameraLook = *m_pTransformCom->Get_StateInfo(CTransform::STATE_LOOK);

		// 카메라의 Look의 방향을 반대로 바꾼 뒤, 내가 멀어지고 싶은 임의의 값을 fDistance를 통해 준다.
		// 그걸 카메라의 포지션에 더해준다.
		// 결국 플레이어의 포지션에서 부터 반대로 Distance값 만큼 멀어진 값이 카메라 포지션이 되는것이다.

		//if (fCameraDistance > 3.8f)
		//	fCameraDistance = 3.8f;
		//
		//if (fCameraDistance < 2.7f)
		//	fCameraDistance = 2.7f;

		vCameraPos += (vCameraLook * -1) * m_fCameraDistance;

		vCameraPos.y += m_fCameraHeightValue;

		// 그 후, 그 값을 다시 내 포지션에 넣어준다.
		m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &vCameraPos);
	}
}

void CCamera_Static::MouseWheelPlay()
{
	// 마우스 휠 조작을 했을 시 카메라 포지션을 조절하는 함수이다.

	if (m_bIsCameraReverting == false &&
		m_bIsPlayerInside != true &&
		m_bIsTalking != true)
	{
		if (0 < m_pInput_Device->GetDIMouseMove(CInput_Device::DIMM_WHEEL))
		{
			if (m_fCameraDistance <= (m_fOriginCameraDistance + 10.f))
				m_fCameraDistance += (5.f);

			if (m_fCameraHeightValue <= m_fOriginCameraHeightValue)
				m_fCameraHeightValue += (5.f);
		}
		else if (0 > m_pInput_Device->GetDIMouseMove(CInput_Device::DIMM_WHEEL))
		{
			if (m_fCameraDistance >= (m_fOriginCameraDistance - 10.f))
				m_fCameraDistance -= (5.f);

			if (m_fCameraHeightValue >= m_fOriginCameraHeightValue)
				m_fCameraHeightValue -= (5.f);
		}
	}
}

void CCamera_Static::RevertInitValue()
{
	// 카메라의 포지션을 초기값으로 되돌리는 함수이다.

	if (m_bIsCameraReverting == true)
	{
		if (m_fCameraDistance < m_fOriginCameraDistance ||
			m_fCameraHeightValue < m_fOriginCameraHeightValue)
		{
			m_fCameraDistance += (m_fTimeDelta * 15.f);
			m_fCameraHeightValue += (m_fTimeDelta * 10.f);

			if (m_fCameraDistance > m_fOriginCameraDistance)
				m_fCameraDistance = m_fOriginCameraDistance;

			if (m_fCameraHeightValue > m_fOriginCameraHeightValue)
				m_fCameraHeightValue = m_fOriginCameraHeightValue;
		}
		else if (m_fCameraDistance > m_fOriginCameraDistance ||
				m_fCameraHeightValue > m_fOriginCameraHeightValue)
		{
			m_fCameraDistance -= (m_fTimeDelta * 15.f);
			m_fCameraHeightValue -= (m_fTimeDelta * 10.f);

			if (m_fCameraDistance < m_fOriginCameraDistance)
				m_fCameraDistance = m_fOriginCameraDistance;

			if (m_fCameraHeightValue < m_fOriginCameraHeightValue)
				m_fCameraHeightValue = m_fOriginCameraHeightValue;
		}

		if (m_fCameraDistance == m_fOriginCameraDistance && 
			m_fCameraHeightValue == m_fOriginCameraHeightValue)
			m_bIsCameraReverting = false;
	}
}

void CCamera_Static::PlayerInsideEvent()
{
	if (m_fCameraDistance > 25.f)
	{
		m_fCameraDistance -= (m_fTimeDelta * 15.f);

		if (m_fCameraDistance < 25.f)
			m_fCameraDistance = 25.f;
	}

	if (m_bIsTalking != true)
	{
		if (m_fCameraHeightValue > 12.f)
		{
			m_fCameraHeightValue -= (m_fTimeDelta * 15.f);

			if (m_fCameraHeightValue < 12.f)
				m_fCameraHeightValue = 12.f;
		}
	}
}

void CCamera_Static::TalkEvent()
{
	// 원하는 위치까지 카메라를 옮기면 더이상 이동하지 않는다.
	if (m_fCameraDistance == 20.f &&
		m_fCameraHeightValue == 20.f)
		return;

	if (m_bIsTalking == true)
	{
		if (m_fCameraDistance > 20.f)
		{
			m_fCameraDistance -= (m_fTimeDelta * 15.f);

			if (m_fCameraDistance < 20.f)
				m_fCameraDistance = 20.f;
		}

		if (m_fCameraHeightValue < 16.f)
		{
			m_fCameraHeightValue += (m_fTimeDelta * 10.f);

			if (m_fCameraHeightValue > 16.f)
				m_fCameraHeightValue = 16.f;
		}
	}
}

CCamera_Static * CCamera_Static::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCamera_Static* pInstance = new CCamera_Static(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		_MSGBOX("Camera_Static Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CCamera_Static::Clone(void * pArg)
{
	CCamera_Static* pInstance = new CCamera_Static(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		_MSGBOX("Camera_Static Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Static::Free()
{
	CEventManager::GetInstance()->Remove_Object(L"Player_Inside_Event", this);

	Safe_Release(m_pInput_Device);
	Safe_Release(m_pTransformCom);

	CCamera::Free();
}
