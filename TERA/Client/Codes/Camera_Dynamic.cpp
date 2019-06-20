#include "stdafx.h"
#include "..\Headers\Camera_Dynamic.h"
#include "Input_Device.h"
#include "Management.h"
#include "Scene_Dragon.h"
#include "Camera_Static.h"
#include "SkyBox_Dragon.h"
#include "Layer.h"
#include "Arkus.h"
#include "Player.h"

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

	CCamera::CAMERAINFO*	pCameraInfo = (CCamera::CAMERAINFO*)pArg;

	_vec3		vRight, vUp, vLook;

	vLook = pCameraInfo->vAt - pCameraInfo->vEye;
	vLook.y = 0;

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

	CutSceneEvent();

	m_fTimeDelta = fTimeDelta;

	KeyInput();

	TracingTarget();
	
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
	if (m_bIsDynamicCamOnAir == true)
	{
		if (nullptr == m_pTransformCom)
			return E_FAIL;

		_matrix matView = *m_pTransformCom->Compute_InverseWorldMatrixPointer();

		Set_Transform(D3DTS_VIEW, &matView);
	}

	return NOERROR;
}

HRESULT CCamera_Dynamic::SetUp_ProjectionMatrix()
{
	if (m_bIsDynamicCamOnAir == true)
	{
		_matrix matProj;

		D3DXMatrixPerspectiveFovLH(&matProj, m_CameraInfo.fFovY, m_CameraInfo.fAspect, m_CameraInfo.fNear, m_CameraInfo.fFar);

		Set_Transform(D3DTS_PROJECTION, &matProj);
	}
	
	return NOERROR;
}

void CCamera_Dynamic::TracingTarget()
{
	if (m_bIsDynamicCamOnAir == true)
	{
		CTransform*	vTargetTransformCom = ((CTransform*)(CObject_Manager::GetInstance()->Get_Component(SCENE_DRAGON, L"Layer_Monster", L"Com_Transform")));

		// 타겟 포지션을 가져와서 카메라포지션 변수를 하나 만들어 넣어준다.
		_vec3 vCameraPos = *vTargetTransformCom->Get_StateInfo(CTransform::STATE_POSITION);

		_vec3 vCameraLook;
		
		// 카메라의 Look 정보를 가져온 뒤
		vCameraLook = *m_pTransformCom->Get_StateInfo(CTransform::STATE_LOOK);

		// 카메라의 Look의 방향을 반대로 바꾼 뒤, 내가 멀어지고 싶은 임의의 값을 fDistance를 통해 준다.
		// 그걸 카메라의 포지션에 더해준다.
		// 결국 플레이어의 포지션에서 부터 반대로 Distance값 만큼 멀어진 값이 카메라 포지션이 되는것이다.

		vCameraPos += (vCameraLook * -1) * m_fCameraDistance;

		vCameraPos.y += m_fCameraHeight;

		// 그 후, 그 값을 다시 내 포지션에 넣어준다.
		m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &vCameraPos);
	}
}

void CCamera_Dynamic::KeyInput()
{
	if (GetKeyState('I') & 0x8000)
		m_pTransformCom->Move(0, 10.f, m_fTimeDelta);

	if (GetKeyState('K') & 0x8000)
		m_pTransformCom->Move(1, 10.f, m_fTimeDelta);

	if (GetKeyState('J') & 0x8000)
		m_pTransformCom->Move(2, 10.f, m_fTimeDelta);

	if (GetKeyState('L') & 0x8000)
		m_pTransformCom->Move(3, 10.f, m_fTimeDelta);

	//_long			dwMouseMove = 0;

	//if (dwMouseMove = m_pInput_Device->GetDIMouseMove(CInput_Device::DIMM_Y))
	//	m_pTransformCom->Rotation_Axis(*m_pTransformCom->Get_StateInfo(CTransform::STATE_RIGHT), D3DXToRadian(dwMouseMove) * 10.f, m_fTimeDelta);

	//if (dwMouseMove = m_pInput_Device->GetDIMouseMove(CInput_Device::DIMM_X))
	//	m_pTransformCom->Rotation_Axis(_vec3(0.f, 1.f, 0.f), D3DXToRadian(dwMouseMove) * 10.f, m_fTimeDelta);

	//POINT			ptMouse = { g_iWinCX >> 1, g_iWinCY >> 1 };

	//ClientToScreen(g_hWnd, &ptMouse);
	//SetCursorPos(ptMouse.x, ptMouse.y);
}

void CCamera_Dynamic::CutSceneEvent()
{
	if (m_bIsCutSceneEventEnded == false)
	{
		if (m_eCurrentScene == SCENE_DRAGON)
			DragonTrialCutSceneEvent();
	}
}

void CCamera_Dynamic::EndCutSceneEvent()
{
	// 컷 신 이벤트가 종료됨을 설정한다.
	m_bIsCutSceneEventEnded = true;
	// 컷 신 이벤트가 종료되며, 다이나믹 카메라의 방송을 중지한다.
	m_bIsDynamicCamOnAir = false;
	// 다이나믹 카메라의 방송을 중지하는 동시에, 플레이어를 찍고있는 스태틱 카메라의 방송을 시작한다.
	dynamic_cast<CCamera_Static*>(CObject_Manager::GetInstance()->Get_Object(SCENE_DRAGON, L"Layer_Camera", 1))->Set_TurnOnStaticCam(true);
	// 컷 신 이벤트 중, 즉 다이나믹 카메라가 방송중일때는 플레이어의 키조작이 불가능하게 조취했는데,
	// 이를 풀어준다.
	dynamic_cast<CPlayer*>(CObject_Manager::GetInstance()->Get_Object(SCENE_STATIC, L"Layer_Player"))->Set_CutSceneInfo(false);
	
	// 해당하는 각 씬의 스카이박스 포지션 정보를 기본값인 다이나믹 카메라에서 스태틱 카메라로 변경해 준다.
	if (CManagement::GetInstance()->Get_CurrentScene() == SCENE_DRAGON)
		dynamic_cast<CSkyBox_Dragon*>(CObject_Manager::GetInstance()->Get_Object(SCENE_DRAGON, L"Layer_BackGround"))->Set_CameraNumber(1);

}

void CCamera_Dynamic::DragonTrialCutSceneEvent()
{
	CMesh_Dynamic*		ArkusMeshCom = dynamic_cast<CArkus*>(CObject_Manager::GetInstance()->Get_Object(SCENE_DRAGON, L"Layer_Monster"))->Get_Mesh();
	_vec3				vArkusPos = *dynamic_cast<CArkus*>(CObject_Manager::GetInstance()->Get_Object(SCENE_DRAGON, L"Layer_Monster"))->Get_Transform()->Get_StateInfo(CTransform::STATE_POSITION);
	CArkus::ARKUS_ANI	eArkusCurrentAni = dynamic_cast<CArkus*>(CObject_Manager::GetInstance()->Get_Object(SCENE_DRAGON, L"Layer_Monster"))->Get_AniIndex();

	if (eArkusCurrentAni == CArkus::ARKUS_ANI::Apperance01 &&
		m_fCameraHeight > 40)
	{
		m_fCameraHeight -= (m_fTimeDelta * 20.f);
	}

	if (ArkusMeshCom->IsAnimationEnded(9.f))
		m_bIsAnimationEneded = true;

	if (eArkusCurrentAni == CArkus::ARKUS_ANI::Apperance02)
	{
		m_fCameraDistance -= m_fTimeDelta * 35.f;

		if (m_fCameraDistance < 175.f)
		{
			m_bCameraShaking = !m_bCameraShaking;

			if (m_bCameraShaking == true)
				m_fCameraHeight += m_fTimeDelta * 150;
			else if (m_bCameraShaking == false)
				m_fCameraHeight -= m_fTimeDelta * 150;
		}
	}

	if (eArkusCurrentAni == CArkus::ARKUS_ANI::MoveAtkStart)
		EndCutSceneEvent();

	// 여기다가 카메라 동선 if문으로 주욱주욱 추가하자.
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
