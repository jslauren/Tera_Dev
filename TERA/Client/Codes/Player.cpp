#include "stdafx.h"
#include "..\Headers\Player.h"
#include "Object_Manager.h"
#include "Light_Manager.h"
#include "Player_Idle.h"
#include "Player_Move.h"
#include "Layer.h"

#define PLAYER_SCALING	0.33f

_USING(Client)

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUnit(pGraphic_Device)
{
}

CPlayer::CPlayer(const CPlayer & rhs)
	: CUnit(rhs)
{
}

HRESULT CPlayer::Ready_GameObject_Prototype()
{
	if (FAILED(CGameObject::Ready_GameObject_Prototype()))
		return E_FAIL;

	return NOERROR;
}

HRESULT CPlayer::Ready_GameObject(void* pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	/*m_pTransformCom->Set_Scaling(0.3f, 0.3f, 0.3f);
	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(100.f, 0.f, 100.f));*/

	m_pTransformMoveCom->Set_Scaling(PLAYER_SCALING, PLAYER_SCALING, PLAYER_SCALING);
	m_pTransformMoveCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(100.f, 0.f, 100.f));

	m_pMeshCom->SetUp_AnimationSet(R05UNARMEDWAIT);

	int iIdleState = 1;
	m_pState = CPlayer_Idle::Create(m_pGraphic_Device, *this, &iIdleState);

	m_pMeshCom->ChangePivot(_vec3(0.f, 1.f, 0.f), -90);

	return NOERROR;
}

_int CPlayer::Update_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pTransformCom)
		return -1;

	m_fTimeDelta = fTimeDelta;

	m_fPlayerPosY = m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION)->y;

	KeyInput();

	m_pMeshCom->Play_Animation(fTimeDelta, m_fAniSpeed);

	m_pTransformCom->Set_WorldMatrix((*m_pTransformRotateCom->Get_WorldMatrixPointer()) * (*m_pTransformMoveCom->Get_WorldMatrixPointer()));

//	Compute_HeightOnNavi();

	return _int();
}

_int CPlayer::LateUpdate_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONEALPHA, this)))
		return -1;

	return _int();
}

HRESULT CPlayer::Render_GameObject()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pTransformCom ||
		nullptr == m_pMeshCom)
		return E_FAIL;

	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	if (nullptr == pEffect)
		return E_FAIL;

	pEffect->AddRef();

	if (FAILED(SetUp_ConstantTable(pEffect)))
		return E_FAIL;

	pEffect->Begin(nullptr, 0);
	
	for (size_t i = 0; i < m_pMeshCom->Get_NumMeshContainer(); ++i)
	{
		if (FAILED(m_pMeshCom->Update_SkinnedMesh(i)))
			break;

		for (size_t j = 0; j < m_pMeshCom->Get_NumSubSet(i); ++j)
		{
			if (FAILED(m_pMeshCom->SetTexture_OnShader(pEffect, i, j, "g_BaseTexture", MESHTEXTURE::TYPE_DIFFUSE)))
				return E_FAIL;

			pEffect->CommitChanges();

			pEffect->BeginPass(0);

			m_pMeshCom->Render_Mesh(i, j);

			pEffect->EndPass();
		}
	}
	
	pEffect->End();

	Safe_Release(pEffect);

	// 콜라이더 렌더
	m_pColliderCom->Render_Collider();

	// 네비메쉬 렌더
	m_pNavigationCom->Render_Navigation();

	return NOERROR;
}

HRESULT CPlayer::Add_Component()
{
	// For.Com_Transform
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	// For.Com_TransformRotation
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_TransformRotation", (CComponent**)&m_pTransformRotateCom)))
		return E_FAIL;

	// For.Com_TransformMove
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_TransformMove", (CComponent**)&m_pTransformMoveCom)))
		return E_FAIL;

	// For.Com_Mesh
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Mesh_Player", L"Com_Mesh", (CComponent**)&m_pMeshCom)))
		return E_FAIL;

	// For.Com_Renderer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	// For.Com_Shader
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Shader_Mesh", L"Com_Shader", (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	// For.Com_BodyCollider
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Collider_OBB", L"Com_BodyCollider", (CComponent**)&m_pColliderCom,
		&CCollider::COLLIDERDESC(CCollider::COLLIDERDESC::TYPE_TRANSFORM, m_pTransformCom->Get_WorldMatrixPointer(), nullptr,
			_vec3(1.f, 2.f, 1.f), _vec3(0.0f, 1.f, 0.f)))))
		return E_FAIL;

	//// For.Com_HandCollider
	//if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Collider_Sphere", L"Com_HandCollider", (CComponent**)&m_pHandColliderCom, &CCollider::COLLIDERDESC(CCollider::COLLIDERDESC::TYPE_FRAME, m_pTransformCom->Get_WorldMatrixPointer(), &(m_pMeshCom->Get_FrameDesc("Body_rFingerMidTop")->CombinedTransformationMatrix)
	//	, _vec3(0.3f, 0.3f, 0.3f), _vec3(0.f, 0.f, 0.f)))))
	//	return E_FAIL;

	// For.Com_Navigation
	_uint		iIndex = 0;
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Navigation_Stage", L"Com_Navigation", (CComponent**)&m_pNavigationCom, &iIndex)))
		return E_FAIL;

	return NOERROR;
}

//HRESULT CPlayer::SetUp_HeightOnTerrain()
//{
//	CObject_Manager*	pObject_Manager = CObject_Manager::GetInstance();
//
//	if (nullptr == pObject_Manager)
//		return E_FAIL;
//	pObject_Manager->AddRef();
//
//	CBuffer_Terrain* pBufferCom = (CBuffer_Terrain*)pObject_Manager->Get_Component(SCENE_STAGE, L"Layer_BackGround", L"Com_Buffer", 0);
//	if (nullptr == pBufferCom)
//		return E_FAIL;
//
//	// 플레이어의 Y값과 이 지형의 Y값을 비교해서, 점프를 구현하면 된다.
//	_float	fY = pBufferCom->Compute_HeightOnBuffer(m_pTransformCom);
//
//	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION)->x, fY + 0.5f, m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION)->z));
//
//	Safe_Release(pObject_Manager);
//
//	return NOERROR;
//}

HRESULT CPlayer::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	if (nullptr == pEffect)
		return E_FAIL;

	pEffect->AddRef();

	pEffect->SetMatrix("g_matWorld", m_pTransformCom->Get_WorldMatrixPointer());
	pEffect->SetMatrix("g_matView", &CGameObject::Get_Transform(D3DTS_VIEW));
	pEffect->SetMatrix("g_matProj", &CGameObject::Get_Transform(D3DTS_PROJECTION));

	CLight_Manager*	pLight_Manager = CLight_Manager::GetInstance();
	if (nullptr == pLight_Manager)
		return E_FAIL;

	pLight_Manager->AddRef();

	const D3DLIGHT9* pLightInfo = pLight_Manager->Get_LightInfo(0);
	if (nullptr == pLightInfo)
		return E_FAIL;

	pEffect->SetVector("g_vLightDir", &_vec4(pLightInfo->Direction, 0.f));
	pEffect->SetVector("g_vLightDiffuse", (_vec4*)&pLightInfo->Diffuse);
	pEffect->SetVector("g_vLightAmbient", (_vec4*)&pLightInfo->Ambient);
	pEffect->SetVector("g_vLightSpecular", (_vec4*)&pLightInfo->Specular);

	Safe_Release(pLight_Manager);

	_matrix		matView = CGameObject::Get_Transform(D3DTS_VIEW);
	D3DXMatrixInverse(&matView, nullptr, &matView);

	pEffect->SetVector("g_vCamPosition", (_vec4*)&matView.m[3][0]);

	Safe_Release(pEffect);

	return NOERROR;
}

void CPlayer::ViewChanage()
{
	// 마우스 커서를 기준으로 플레이어의 뷰를 변환한다 //
	// Feat.형진이
	CTransform*	vCameraTransformCom = ((CTransform*)(CObject_Manager::GetInstance()->Get_Component(SCENE_STAGE, L"Layer_Camera", L"Com_Transform", 1)));
	_vec3 vCameraLook = *vCameraTransformCom->Get_StateInfo(CTransform::STATE_LOOK);
	
	_vec3	vPlayerRight = *m_pTransformCom->Get_StateInfo(CTransform::STATE_RIGHT);
	_vec3	vPlayerUp = *m_pTransformCom->Get_StateInfo(CTransform::STATE_UP);
	_vec3	vPlayerLook = *m_pTransformCom->Get_StateInfo(CTransform::STATE_LOOK);
	_float	fPlayerRightSize = D3DXVec3Length(&vPlayerRight);
	_float	fPlayerLookSize = D3DXVec3Length(&vPlayerLook);

	vCameraLook.y = 0.f;
	D3DXVec3Normalize(&vCameraLook, &vCameraLook);

	m_pTransformCom->Set_StateInfo(CTransform::STATE_LOOK, &(vCameraLook * fPlayerLookSize));

	_vec3 vPlayerNewRight;
	D3DXVec3Cross(&vPlayerNewRight, &vPlayerUp, &vCameraLook);

	D3DXVec3Normalize(&vPlayerNewRight, &vPlayerNewRight);

	m_pTransformCom->Set_StateInfo(CTransform::STATE_RIGHT, &(vPlayerNewRight * fPlayerRightSize));

}

void CPlayer::KeyInput()
{
	CPlayerState* pState = m_pState->Input_Keyboard(*this, m_fTimeDelta, 0, m_pNavigationCom);

	if (nullptr != pState)
	{
		Safe_Release(m_pState);
		m_pState = pState;
	}

	if (true == m_pMeshCom->IsAnimationEnded() || m_eOldAnimationIndex != m_eAnimationIndex)
	{
		m_pMeshCom->SetUp_AnimationSet(m_eAnimationIndex);
	}

	m_eOldAnimationIndex = m_eAnimationIndex;

	m_pState->Update_State(*this, m_fTimeDelta);

}

void CPlayer::Compute_HeightOnNavi()
{
	_vec3	pPlayerPos = *m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION);
	_float fDist = m_pNavigationCom->Compute_HeightOnNavi(&pPlayerPos);

	pPlayerPos.y = fDist;

	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &pPlayerPos);
}

CPlayer * CPlayer::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPlayer* pInstance = new CPlayer(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		_MSGBOX("CPlayer Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CPlayer::Clone(void* pArg)
{
	CPlayer* pInstance = new CPlayer(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		_MSGBOX("CPlayer Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer::Free()
{
	Safe_Release(m_pState);

	CUnit::Free();
}
