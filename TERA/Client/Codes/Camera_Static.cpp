#include "stdafx.h"
#include "..\Headers\Camera_Static.h"
#include "Input_Device.h"
#include "Layer.h"
#include "Player.h"

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

	return NOERROR;
}

_int CCamera_Static::Update_GameObject(const _float & fTimeDelta)
{
	m_fTimeDelta = fTimeDelta;

	ChangeView();

	TracingPlayer();

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

HRESULT CCamera_Static::Add_Component()
{
	// For.Com_Transform
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CCamera_Static::SetUp_ViewMatrix()
{
	if (nullptr == m_pTransformCom)
		return E_FAIL;

	_matrix matView = *m_pTransformCom->Compute_InverseWorldMatrixPointer();

	Set_Transform(D3DTS_VIEW, &matView);

	return NOERROR;
}

HRESULT CCamera_Static::SetUp_ProjectionMatrix()
{
	_matrix matProj;

	D3DXMatrixPerspectiveFovLH(&matProj, m_CameraInfo.fFovY, m_CameraInfo.fAspect, m_CameraInfo.fNear, m_CameraInfo.fFar);

	Set_Transform(D3DTS_PROJECTION, &matProj);

	return NOERROR;
}

void CCamera_Static::ChangeView()
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
		if (dwMouseMove < 0 && fDotValue < 0.1f)
		{
			// 마우스 커서를 일정영역 밑으로 내리면 카메라 줌아웃 하는 구문.
			if (fDotValue < 0.1f && fDotValue > m_fDotValuePri)
				fCameraDistance -= 0.01f;

			m_pTransformCom->Rotation_Axis(*m_pTransformCom->Get_StateInfo(CTransform::STATE_RIGHT), D3DXToRadian(dwMouseMove) * 7.f, m_fTimeDelta);
		}
		else if (dwMouseMove > 0 && fDotValue > -0.5f)
		{
			// 카메라 줌인 구문.
			if (fDotValue < -0.5f && fDotValue < m_fDotValuePri)
				fCameraDistance += 0.03f;

			m_pTransformCom->Rotation_Axis(*m_pTransformCom->Get_StateInfo(CTransform::STATE_RIGHT), D3DXToRadian(dwMouseMove) * 7.f, m_fTimeDelta);
		}
	}
	if (dwMouseMove = m_pInput_Device->GetDIMouseMove(CInput_Device::DIMM_X))
		m_pTransformCom->Rotation_Axis(_vec3(0.f, 1.f, 0.f), D3DXToRadian(dwMouseMove) * 10.f, m_fTimeDelta);

	POINT	ptMouse = { g_iWinCX >> 1, g_iWinCY >> 1 };
	
	// 카메라 줌 인, 아웃을 하기위해 필요한 구문.
	m_fDotValuePri = fDotValue;

	ClientToScreen(g_hWnd, &ptMouse);
	SetCursorPos(ptMouse.x, ptMouse.y);
}

void CCamera_Static::TracingPlayer()
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

	vCameraPos += (vCameraLook * -1) * fCameraDistance;

	vCameraPos.y += 10.f;

	// 그 후, 그 값을 다시 내 포지션에 넣어준다.
	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &vCameraPos);

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
	Safe_Release(m_pInput_Device);
	Safe_Release(m_pTransformCom);

	CCamera::Free();
}
