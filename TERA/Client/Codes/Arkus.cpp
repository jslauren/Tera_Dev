#include "stdafx.h"
#include "..\Headers\Arkus.h"
#include "Object_Manager.h"
#include "Light_Manager.h"

_USING(Client)

CArkus::CArkus(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUnit(pGraphic_Device)
{
}

CArkus::CArkus(const CArkus & rhs)
	: CUnit(rhs)
{
}

HRESULT CArkus::Ready_GameObject_Prototype()
{
	if (FAILED(CGameObject::Ready_GameObject_Prototype()))
		return E_FAIL;

	return NOERROR;
}

HRESULT CArkus::Ready_GameObject(void * pArg)
{
	//tObjectMeshData = *((OBJECTMESHDATA*)pArg);

	if (FAILED(Add_Component()))
		return E_FAIL;

//	_matrix matWorld = *(_matrix*)pArg;

	m_pTransformCom->Set_Scaling(0.4f, 0.4f, 0.4f);
	m_pTransformCom->Set_Rotation_YawPitchRoll(D3DXToRadian(90.f), D3DXToRadian(0.f), D3DXToRadian(0.f));
	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(200.f, 0.f, 200.f));
//	m_pTransformCom->Set_WorldMatrix(matWorld);

//	m_pTransformCom->Set_WorldMatrix(tObjectMeshData.matWorld);
//	m_pTransformCom->Set_WorldMatrix(((OBJECTMESHDATA*)pArg)->matWorld);

	m_pMeshCom->SetUp_AnimationSet(Apperance01);
	m_eAnimationIndex = Apperance01;
	m_eOldAnimationIndex = End;
	m_eCurActionID = ACTION_READY;
}

_int CArkus::Update_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pTransformCom)
		return -1;

	const CCollider* pPlayerCollider = (const CCollider*)CObject_Manager::GetInstance()->Get_Component(SCENE_STATIC, L"Layer_Player", L"Com_BodyCollider");
	const CCollider* pWeaponCollider = (const CCollider*)CObject_Manager::GetInstance()->Get_Component(SCENE_STATIC, L"Layer_Weapon", L"Com_WeaponCollider");

	m_pColliderCom->Collision_OBB(pPlayerCollider);
	m_pColliderCom->Collision_OBB(pWeaponCollider);

	return _int();
}

_int CArkus::LateUpdate_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	Compute_ViewZ(m_pTransformCom);

	//if (FAILED(SetUp_HeightOnTerrain(1)))
	//	return -1;

	m_fTimeDelta = fTimeDelta;

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONEALPHA, this)))
		return -1;

	return _int();
}

HRESULT CArkus::Render_GameObject()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pTransformCom ||
		nullptr == m_pMeshCom ||
		nullptr == m_pColliderCom)
		return E_FAIL;

	m_pMeshCom->Play_Animation(m_fTimeDelta);

	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	if (nullptr == pEffect)
		return E_FAIL;

	pEffect->AddRef();

	if (FAILED(SetUp_ConstantTable(pEffect)))
		return E_FAIL;

	pEffect->Begin(nullptr, 0);


	for (size_t i = 0; i < 1; ++i)
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

	m_pColliderCom->Render_Collider();

	return NOERROR;
}

HRESULT CArkus::Add_Component()
{
	// For.Com_Transform
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom)))
		return E_FAIL;
		
	// For.Com_Renderer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom)))
		return E_FAIL;
	
	// For.Com_Shader
	if (FAILED(CGameObject::Add_Component(SCENE_DRAGON, L"Component_Shader_Mesh", L"Com_Shader", (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	// For.Com_Mesh
	if (FAILED(CGameObject::Add_Component(SCENE_DRAGON, L"Component_Mesh_Arkus", L"Com_Mesh", (CComponent**)&m_pMeshCom)))
		return E_FAIL;
	
	// For.Com_Collider
	CCollider::COLLIDERDESC		ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	
	ColliderDesc.eType = CCollider::COLLIDERDESC::TYPE_TRANSFORM;
	ColliderDesc.pTransformMatrix = m_pTransformCom->Get_WorldMatrixPointer();
	ColliderDesc.pFrameMatrix = nullptr;
	ColliderDesc.vScale = _vec3(10.0f, 20.f, 10.f);
	ColliderDesc.vPivot = _vec3(0.0f, 1.f, 0.f);
	
	if (FAILED(CGameObject::Add_Component(SCENE_DRAGON, L"Component_Collider_AABB", L"Com_Collider", (CComponent**)&m_pColliderCom, &ColliderDesc)))
		return E_FAIL;

	// For.Com_Frustum
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Frustum", L"Com_Frustum", (CComponent**)&m_pFrustumCom)))
		return E_FAIL;
		
	return NOERROR;
}

HRESULT CArkus::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
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

HRESULT CArkus::SetUp_HeightOnTerrain(_uint iIndex)
{
	CUnit::SetUp_HeightOnTerrain(1);

	return NOERROR;
}

void CArkus::AI()
{
	if (m_pMeshCom->IsAnimationEnded() &&
		m_pMeshCom->Get_NowPlayAniIndex() == ARKUS_ANI::Apperance01)
	{
		if (m_pMeshCom->IsAnimationEnded(8.5f))
		{
			m_eAnimationIndex = Idle;
			m_pMeshCom->SetUp_AnimationSet(Idle);
		}
	}

	if (true == m_pMeshCom->IsAnimationEnded() || m_eOldAnimationIndex != m_eAnimationIndex)
		m_pMeshCom->SetUp_AnimationSet(m_eAnimationIndex);
}

CArkus * CArkus::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CArkus* pInstance = new CArkus(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		_MSGBOX("CArkus Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CArkus::Clone(void * pArg)
{
	CArkus* pInstance = new CArkus(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		_MSGBOX("CArkus Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CArkus::Free()
{
	CUnit::Free();
}
