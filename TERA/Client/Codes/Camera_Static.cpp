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

	// NPC�� ��ȭ���϶� �ƹ���� ��Ÿ��? //
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

		// ���� �� ���� 1 or -1 �̸� vWorldYvalue �� fDotValue�� �� ��ġ�� ���̴�.
		// 0�̸� 90�� �̴�.

		if (dwMouseMove = m_pInput_Device->GetDIMouseMove(CInput_Device::DIMM_Y))
		{
			// ���콺 Ŀ���� ���� �ø� ��.
			// �� ���� �ø��� ������ �ؿ� 0.3f ���� �� ũ�� �ָ� �ȴ�.
			if (dwMouseMove < 0 && fDotValue < 0.3f)
			{
				//// ���콺 Ŀ���� �������� ������ ������ ī�޶� �ܾƿ� �ϴ� ����.
				//if (fDotValue < 0.1f && fDotValue > m_fDotValuePri)
				//	m_fCameraDistance -= 0.01f;

				m_pTransformCom->Rotation_Axis(*m_pTransformCom->Get_StateInfo(CTransform::STATE_RIGHT), D3DXToRadian(dwMouseMove) * 7.f, m_fTimeDelta);
			}
			else if (dwMouseMove > 0 && fDotValue > -0.1f)
			{
				//// ī�޶� ���� ����.
				//if (fDotValue < -0.5f && fDotValue < m_fDotValuePri)
				//	m_fCameraDistance += 0.03f;

				m_pTransformCom->Rotation_Axis(*m_pTransformCom->Get_StateInfo(CTransform::STATE_RIGHT), D3DXToRadian(dwMouseMove) * 7.f, m_fTimeDelta);
			}
		}
		if (dwMouseMove = m_pInput_Device->GetDIMouseMove(CInput_Device::DIMM_X))
			m_pTransformCom->Rotation_Axis(_vec3(0.f, 1.f, 0.f), D3DXToRadian(dwMouseMove) * 10.f, m_fTimeDelta);

		// Arkus ���߰��� ��, ī�޶� ��� �������� �ϱ� ������
		// �� Ÿ�ֿ̹� ���� ��� �������� ����� ���� �� ����.
		if (m_bIsCameraCtrlAvailable == true)
			MouseWheelPlay();

		// ī�޶� �� ��, �ƿ��� �ϱ����� �ʿ��� ����.
		m_fDotValuePri = fDotValue;
	}
}

void CCamera_Static::TracingPlayer()
{
	if (m_bIsStaticCamOnAir == true)
	{
		CTransform*	vPlayerTransformCom = ((CTransform*)(CObject_Manager::GetInstance()->Get_Component(SCENE_STATIC, L"Layer_Player", L"Com_Transform")));

		// �÷��̾� �������� �����ͼ� ī�޶������� ������ �ϳ� ����� �־��ش�.
		_vec3 vCameraPos = *vPlayerTransformCom->Get_StateInfo(CTransform::STATE_POSITION);

		_vec3 vCameraLook;


		// ī�޶��� Look ������ ������ ��
		vCameraLook = *m_pTransformCom->Get_StateInfo(CTransform::STATE_LOOK);

		// ī�޶��� Look�� ������ �ݴ�� �ٲ� ��, ���� �־����� ���� ������ ���� fDistance�� ���� �ش�.
		// �װ� ī�޶��� �����ǿ� �����ش�.
		// �ᱹ �÷��̾��� �����ǿ��� ���� �ݴ�� Distance�� ��ŭ �־��� ���� ī�޶� �������� �Ǵ°��̴�.

		//if (fCameraDistance > 3.8f)
		//	fCameraDistance = 3.8f;
		//
		//if (fCameraDistance < 2.7f)
		//	fCameraDistance = 2.7f;

		vCameraPos += (vCameraLook * -1) * m_fCameraDistance;

		vCameraPos.y += m_fCameraHeightValue;

		// �� ��, �� ���� �ٽ� �� �����ǿ� �־��ش�.
		m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &vCameraPos);
	}
}

void CCamera_Static::MouseWheelPlay()
{
	// ���콺 �� ������ ���� �� ī�޶� �������� �����ϴ� �Լ��̴�.

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
	// ī�޶��� �������� �ʱⰪ���� �ǵ����� �Լ��̴�.

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
	// ���ϴ� ��ġ���� ī�޶� �ű�� ���̻� �̵����� �ʴ´�.
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
