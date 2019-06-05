#include "stdafx.h"
#include "..\Headers\Monster.h"
#include "Object_Manager.h"
#include "Light_Manager.h"

_USING(Client)

CMonster::CMonster(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUnit(pGraphic_Device)
//	, m_fFrame(0.f)
{
}

CMonster::CMonster(const CMonster & rhs)
	: CUnit(rhs)
//	, m_fFrame(rhs.m_fFrame)
{
}

// 원본객체에 필요한 데이터를 셋팅한다.
HRESULT CMonster::Ready_GameObject_Prototype()
{
	// 파일입출력을 통해 객체의 정보를 셋팅한다.
	if (FAILED(CGameObject::Ready_GameObject_Prototype()))
		return E_FAIL;

	return NOERROR;
}

// 실제 씬에서 사용할 객체가 호출하는 함수.
// 원본객체 복제외에도 추가적인 셋팅이필요하면 여기서 셋팅해라.
HRESULT CMonster::Ready_GameObject(void* pArg)
{
	_matrix matWorld = *(_matrix*)pArg;


	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pTransformCom->Set_Scaling(0.01f, 0.01f, 0.01f);

	m_pTransformCom->Set_WorldMatrix(matWorld);
	m_pMeshCom->SetUp_AnimationSet(rand() % 10);

	return NOERROR;
}

_int CMonster::Update_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pTransformCom)
		return -1;

	const CCollider* pPlayerCollider = (const CCollider*)CObject_Manager::GetInstance()->Get_Component(SCENE_STATIC, L"Layer_Player", L"Com_BodyCollider");

	//const CCollider* pWeaponCollider = (const CCollider*)CObject_Manager::GetInstance()->Get_Component(SCENE_STATIC, L"Layer_Player", L"Com_Collider", 1);

	m_pColliderCom->Collision_OBB(pPlayerCollider);

	return _int();
}

_int CMonster::LateUpdate_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	Compute_ViewZ(m_pTransformCom);

	if (FAILED(SetUp_HeightOnTerrain(1)))
		return -1;

	m_fTimeDelta = fTimeDelta;

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONEALPHA, this)))
		return -1;

	return _int();
}

HRESULT CMonster::Render_GameObject()
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

HRESULT CMonster::Add_Component()
{
	// For.Com_Transform
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	// For.Com_Mesh
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Mesh_Monster", L"Com_Mesh", (CComponent**)&m_pMeshCom)))
		return E_FAIL;

	// For.Com_Renderer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	// For.Com_Shader
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Shader_Mesh", L"Com_Shader", (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	// For.Com_Collider
	CCollider::COLLIDERDESC		ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.eType = CCollider::COLLIDERDESC::TYPE_TRANSFORM;
	ColliderDesc.pTransformMatrix = m_pTransformCom->Get_WorldMatrixPointer();
	ColliderDesc.pFrameMatrix = nullptr;
	ColliderDesc.vScale = _vec3(1.0f, 2.f, 1.f);
	ColliderDesc.vPivot = _vec3(0.0f, 1.f, 0.f);

	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Collider_AABB", L"Com_Collider", (CComponent**)&m_pColliderCom, &ColliderDesc)))
		return E_FAIL;
	
	return NOERROR;
}

//HRESULT CMonster::SetUp_HeightOnTerrain()
//{
//	CObject_Manager*	pObject_Manager = CObject_Manager::GetInstance();
//
//	if (nullptr == pObject_Manager)
//		return E_FAIL;
//	pObject_Manager->AddRef();
//
//	// 인덱스 1번으로 해야함.
//	CBuffer_Terrain* pBufferCom = (CBuffer_Terrain*)pObject_Manager->Get_Component(SCENE_STAGE, L"Layer_BackGround", L"Com_Buffer", 1);
//	if (nullptr == pBufferCom)
//		return E_FAIL;
//
//	_float	fY = pBufferCom->Compute_HeightOnBuffer(m_pTransformCom);
//
//	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION)->x, fY + 0.5f, m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION)->z));
//
//	Safe_Release(pObject_Manager);
//
//	return NOERROR;
//}

//HRESULT CMonster::SetUp_BillBoard()
//{
//	CObject_Manager*	pObject_Manager = CObject_Manager::GetInstance();
//
//	if (nullptr == pObject_Manager)
//		return E_FAIL;
//	pObject_Manager->AddRef();
//
//	CTransform* pCamTransformCom = (CTransform*)pObject_Manager->Get_Component(SCENE_STAGE, L"Layer_Camera", L"Com_Transform", 0);
//	if (nullptr == pCamTransformCom)
//		return E_FAIL;
//
//	// 카메라의 월드행렬 == 뷰스페이스 변환행렬 역행렬
//	m_pTransformCom->Set_StateInfo(CTransform::STATE_RIGHT
//		, pCamTransformCom->Get_StateInfo(CTransform::STATE_RIGHT));
//	//m_pTransformCom->Set_StateInfo(CTransform::STATE_UP
//	//	, pCamTransformCom->Get_StateInfo(CTransform::STATE_UP));
//	m_pTransformCom->Set_StateInfo(CTransform::STATE_LOOK
//		, pCamTransformCom->Get_StateInfo(CTransform::STATE_LOOK));
//
//	Safe_Release(pObject_Manager);
//
//	return NOERROR;
//}

HRESULT CMonster::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
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

// 원본객체를 생성한다.
CMonster * CMonster::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CMonster* pInstance = new CMonster(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		_MSGBOX("CMonster Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CMonster::Clone(void* pArg)
{
	CMonster* pInstance = new CMonster(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		_MSGBOX("CMonster Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMonster::Free()
{
	//Safe_Release(m_pColliderCom);
	//Safe_Release(m_pTransformCom);
	//Safe_Release(m_pRendererCom);
	//Safe_Release(m_pMeshCom);
	//Safe_Release(m_pShaderCom);

	CUnit::Free();
}
