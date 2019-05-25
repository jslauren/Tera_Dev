#include "stdafx.h"
#include "..\Headers\Camera_Dynamic.h"
#include "Input_Device.h"

_USING(Client)

CCamera_Dynamic::CCamera_Dynamic(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCamera(pGraphic_Device)
	, m_pInput_Device(CInput_Device::GetInstance())
{
	m_pInput_Device->AddRef();
}

CCamera_Dynamic::CCamera_Dynamic(const CCamera_Dynamic & rhs)
	: CCamera(rhs)
	, m_pInput_Device(rhs.m_pInput_Device)
{
	m_pInput_Device->AddRef();
}

HRESULT CCamera_Dynamic::Ready_GameObject_Prototype()
{
	return NOERROR;
}

HRESULT CCamera_Dynamic::Ready_GameObject(void* pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	CCamera::CAMERAINFO*		pCameraInfo = (CCamera::CAMERAINFO*)pArg;

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

_int CCamera_Dynamic::Update_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pInput_Device)
		return -1;

	if (GetKeyState(VK_LSHIFT) & 0x8000)
	{
		if (GetKeyState('W') & 0x8000)
			m_pTransformCom->Move(0, 10.f, fTimeDelta);

		if (GetKeyState('S') & 0x8000)
			m_pTransformCom->Move(1, 10.f, fTimeDelta);

		if (GetKeyState('A') & 0x8000)
			m_pTransformCom->Move(2, 10.f, fTimeDelta);

		if (GetKeyState('D') & 0x8000)
			m_pTransformCom->Move(3, 10.f, fTimeDelta);
	}

	_long			dwMouseMove = 0;

	if (dwMouseMove = m_pInput_Device->GetDIMouseMove(CInput_Device::DIMM_Y))
		m_pTransformCom->Rotation_Axis(*m_pTransformCom->Get_StateInfo(CTransform::STATE_RIGHT), D3DXToRadian(dwMouseMove) * 10.f, fTimeDelta);

	if (dwMouseMove = m_pInput_Device->GetDIMouseMove(CInput_Device::DIMM_X))
		m_pTransformCom->Rotation_Axis(_vec3(0.f, 1.f, 0.f), D3DXToRadian(dwMouseMove) * 10.f, fTimeDelta);

	POINT			ptMouse = { g_iWinCX >> 1, g_iWinCY >> 1 };

	ClientToScreen(g_hWnd, &ptMouse);
	SetCursorPos(ptMouse.x, ptMouse.y);

	return _int();
}

_int CCamera_Dynamic::LateUpdate_GameObject(const _float & fTimeDelta)
{
	SetUp_ViewMatrix();
	SetUp_ProjectionMatrix();

	return _int();
}

HRESULT CCamera_Dynamic::Render_GameObject()
{
	return NOERROR;
}

HRESULT CCamera_Dynamic::Add_Component()
{
	// For.Com_Transform
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CCamera_Dynamic::SetUp_ViewMatrix()
{
	if (nullptr == m_pTransformCom)
		return E_FAIL;

	_matrix matView = *m_pTransformCom->Compute_InverseWorldMatrixPointer();

	Set_Transform(D3DTS_VIEW, &matView);

	return NOERROR;
}

HRESULT CCamera_Dynamic::SetUp_ProjectionMatrix()
{
	_matrix matProj;

	D3DXMatrixPerspectiveFovLH(&matProj, m_CameraInfo.fFovY, m_CameraInfo.fAspect, m_CameraInfo.fNear, m_CameraInfo.fFar);

	Set_Transform(D3DTS_PROJECTION, &matProj);

	return NOERROR;
}

CCamera_Dynamic * CCamera_Dynamic::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCamera_Dynamic* pInstance = new CCamera_Dynamic(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		_MSGBOX("CCamera_Dynamic Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CCamera_Dynamic::Clone(void* pArg)
{
	CCamera_Dynamic* pInstance = new CCamera_Dynamic(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		_MSGBOX("CCamera_Dynamic Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Dynamic::Free()
{
	Safe_Release(m_pInput_Device);
	Safe_Release(m_pTransformCom);

	CCamera::Free();
}
