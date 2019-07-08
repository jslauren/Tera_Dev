#include "stdafx.h"
#include "..\Headers\TerrainObject.h"
#include "Object_Manager.h"
#include "Light_Manager.h"
#include "Management.h"

_USING(Client)

CTerrainObject::CTerrainObject(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CTerrainObject::CTerrainObject(const CTerrainObject & rhs)
	: CGameObject(rhs)
{
}

HRESULT CTerrainObject::Ready_GameObject_Prototype()
{
	if (FAILED(CGameObject::Ready_GameObject_Prototype()))
		return E_FAIL;

	return NOERROR;
}

HRESULT CTerrainObject::Ready_GameObject(void * pArg)
{
	if (FAILED(Add_Component(pArg)))
		return E_FAIL;

	m_pTransformCom->Set_Scaling(0.04f, 0.04f, 0.04f);
	m_pTransformCom->Set_WorldMatrix(((OBJECTMESHDATA*)pArg)->matWorld);

	return NOERROR;
}

_int CTerrainObject::Update_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pTransformCom)
		return -1;

	return _int();
}

_int CTerrainObject::LateUpdate_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	Compute_ViewZ(m_pTransformCom);

	//if (FAILED(SetUp_HeightOnTerrain()))
	//	return -1;

	m_fTimeDelta = fTimeDelta;
	


	if (true == m_pFrustumCom->WorldPt_InFrustum(m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION), m_pTransformCom, m_fCulling))
	{
		if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONEALPHA, this)))
			return -1;
	}

	return _int();
}

HRESULT CTerrainObject::Render_GameObject()
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

	for (size_t i = 0; i < m_pMeshCom->Get_NumMaterials(); ++i)
	{
		if (FAILED(m_pMeshCom->SetTexture_OnShader(pEffect, i, "g_BaseTexture", MESHTEXTURE::TYPE_DIFFUSE)))
			return E_FAIL;

		pEffect->CommitChanges();

		pEffect->BeginPass(0);

		m_pMeshCom->Render_Mesh(i);

		pEffect->EndPass();
	}

	pEffect->End();

	Safe_Release(pEffect);

	return NOERROR;
}

HRESULT CTerrainObject::Add_Component(void* pArg)
{
	// For.Com_Transform
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	//_tchar* pStrArg = static_cast<_tchar*>(pArg);
	
	// For.Com_Renderer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	// SCENE_STAGE 을 준비하려면 SCENE_LOGO가 현재 씬이 맞다.
	// 다음 씬을 준비하기위해 이전 씬의 조건을 검사하자.
	if (CManagement::GetInstance()->Get_PreScene() == SCENE_LOGO ||
		CManagement::GetInstance()->Get_PreScene() == SCENE_DRAGON)
	{
		// For.Com_TerrainData
		if (FAILED(CGameObject::Add_Component(SCENE_STAGE, ((OBJECTMESHDATA*)pArg)->strComProtoTag.c_str(), L"Com_Mesh", (CComponent**)&m_pMeshCom)))
			return E_FAIL;

		// For.Com_Shader
		if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Shader_Mesh", L"Com_Shader", (CComponent**)&m_pShaderCom)))
			return E_FAIL;
	}
	else if (CManagement::GetInstance()->Get_PreScene() == SCENE_STAGE)
	{
		//const _tchar* p = ((OBJECTMESHDATA*)pArg)->strComProtoTag.c_str();
		int iNoneCullingObject = lstrcmp(((OBJECTMESHDATA*)pArg)->strComProtoTag.c_str(), L"Component_Mesh_Static_Arena_Main_B");
		if (0 == iNoneCullingObject)
			m_fCulling = 500.f;

		// For.Com_TerrainData
		if (FAILED(CGameObject::Add_Component(SCENE_DRAGON, ((OBJECTMESHDATA*)pArg)->strComProtoTag.c_str(), L"Com_Mesh", (CComponent**)&m_pMeshCom)))
			return E_FAIL;

		// For.Com_Shader
		if (FAILED(CGameObject::Add_Component(SCENE_DRAGON, L"Component_Shader_Mesh", L"Com_Shader", (CComponent**)&m_pShaderCom)))
			return E_FAIL;
	}

	// For.Com_Frustum
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Frustum", L"Com_Frustum", (CComponent**)&m_pFrustumCom)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CTerrainObject::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	if (nullptr == pEffect)
		return E_FAIL;

	pEffect->AddRef();

	pEffect->SetMatrix("g_matWorld", m_pTransformCom->Get_WorldMatrixPointer());
	pEffect->SetMatrix("g_matView", &CGameObject::Get_Transform(D3DTS_VIEW));
	pEffect->SetMatrix("g_matProj", &CGameObject::Get_Transform(D3DTS_PROJECTION));


	Safe_Release(pEffect);

	return NOERROR;
}

CTerrainObject * CTerrainObject::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTerrainObject* pInstance = new CTerrainObject(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		_MSGBOX("CTerrainObject Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CTerrainObject::Clone(void * pArg)
{
	CTerrainObject* pInstance = new CTerrainObject(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		_MSGBOX("CTerrainObject Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTerrainObject::Free()
{
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pMeshCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pFrustumCom);

	CGameObject::Free();
}
