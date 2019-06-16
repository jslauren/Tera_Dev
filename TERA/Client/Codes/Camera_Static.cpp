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
	
	// ���� �� ���� 1 or -1 �̸� vWorldYvalue �� fDotValue�� �� ��ġ�� ���̴�.
	// 0�̸� 90�� �̴�.

	if (dwMouseMove = m_pInput_Device->GetDIMouseMove(CInput_Device::DIMM_Y))
	{
		if (dwMouseMove < 0 && fDotValue < 0.1f)
		{
			// ���콺 Ŀ���� �������� ������ ������ ī�޶� �ܾƿ� �ϴ� ����.
			if (fDotValue < 0.1f && fDotValue > m_fDotValuePri)
				fCameraDistance -= 0.01f;

			m_pTransformCom->Rotation_Axis(*m_pTransformCom->Get_StateInfo(CTransform::STATE_RIGHT), D3DXToRadian(dwMouseMove) * 7.f, m_fTimeDelta);
		}
		else if (dwMouseMove > 0 && fDotValue > -0.5f)
		{
			// ī�޶� ���� ����.
			if (fDotValue < -0.5f && fDotValue < m_fDotValuePri)
				fCameraDistance += 0.03f;

			m_pTransformCom->Rotation_Axis(*m_pTransformCom->Get_StateInfo(CTransform::STATE_RIGHT), D3DXToRadian(dwMouseMove) * 7.f, m_fTimeDelta);
		}
	}
	if (dwMouseMove = m_pInput_Device->GetDIMouseMove(CInput_Device::DIMM_X))
		m_pTransformCom->Rotation_Axis(_vec3(0.f, 1.f, 0.f), D3DXToRadian(dwMouseMove) * 10.f, m_fTimeDelta);

	POINT	ptMouse = { g_iWinCX >> 1, g_iWinCY >> 1 };
	
	// ī�޶� �� ��, �ƿ��� �ϱ����� �ʿ��� ����.
	m_fDotValuePri = fDotValue;

	ClientToScreen(g_hWnd, &ptMouse);
	SetCursorPos(ptMouse.x, ptMouse.y);
}

void CCamera_Static::TracingPlayer()
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

	vCameraPos += (vCameraLook * -1) * fCameraDistance;

	vCameraPos.y += 10.f;

	// �� ��, �� ���� �ٽ� �� �����ǿ� �־��ش�.
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
