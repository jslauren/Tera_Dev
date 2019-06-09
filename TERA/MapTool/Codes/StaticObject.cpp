#include "stdafx.h"
#include "..\Headers\StaticObject.h"
#include "Object_Manager.h"
#include "Light_Manager.h"
#include "Buffer_Terrain_Tool.h"
#include "ViewManager.h"

CStaticObject::CStaticObject(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CStaticObject::CStaticObject(const CStaticObject & rhs)
	: CGameObject(rhs)
{
}

HRESULT CStaticObject::Ready_GameObject_Prototype()
{
	if (FAILED(CGameObject::Ready_GameObject_Prototype()))
		return E_FAIL;

	return NOERROR;
}

HRESULT CStaticObject::Ready_GameObject(void * pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(0.f, 0.f, 0.f));

	return NOERROR;
}

_int CStaticObject::Update_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pTransformCom)
		return -1;

	return _int();
}

_int CStaticObject::LateUpdate_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	Compute_ViewZ(m_pTransformCom);

	//if (FAILED(SetUp_HeightOnTerrain()))
	//	return -1;

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONEALPHA, this)))
		return -1;

	return _int();
}

HRESULT CStaticObject::Render_GameObject()
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

	// 행렬 = 행렬 * 행렬

	_uint	iNumMaterials = m_pMeshCom->Get_NumMaterials();

	for (size_t i = 0; i < iNumMaterials; ++i)
	{
		if (FAILED(m_pMeshCom->SetTexture_OnShader(pEffect, i, "g_BaseTexture", MESHTEXTURE::TYPE_DIFFUSE)))
			return E_FAIL;

		const D3DXMATERIAL* pMaterials = m_pMeshCom->Get_Materials();
		if (nullptr == pMaterials)
			return E_FAIL;

		/*	pEffect->SetVector("g_vMtrlDiffuse", (_vec4*)&pMaterials[i].MatD3D.Diffuse);
		pEffect->SetVector("g_vMtrlAmbient", (_vec4*)&pMaterials[i].MatD3D.Ambient);
		pEffect->SetVector("g_vMtrlSpecular", (_vec4*)&pMaterials[i].MatD3D.Specular);
		pEffect->SetFloat("g_fPower", pMaterials[i].MatD3D.Power);*/

		pEffect->CommitChanges();

		pEffect->BeginPass(0);

		m_pMeshCom->Render_Mesh(i);

		pEffect->EndPass();
	}

	pEffect->End();

	Safe_Release(pEffect);

	return NOERROR;
}

void CStaticObject::SetState(_vec3 vPos, _vec3 vScale)
{
	m_vPosition = vPos;

	m_pTransformCom->Set_Scaling(vScale.x, vScale.y, vScale.z);

	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &vPos);
	
}

void CStaticObject::SetPosition(_vec3 vPos)
{
	m_vPosition = vPos;
	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &vPos);
}

void CStaticObject::SetWorldMatrix(_matrix matWorld)
{
	m_pTransformCom->Set_WorldMatrix(matWorld);
}

HRESULT CStaticObject::Add_Component_Tool(const _tchar * pComponentPrototypeTag)
{
	// For.Com_Mesh_Tool
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, pComponentPrototypeTag, L"Com_Mesh", (CComponent**)&m_pMeshCom)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CStaticObject::Add_Component()
{
	// For.Com_Transform
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	// For.Com_Renderer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	// For.Com_Shader
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Shader_Mesh", L"Com_Shader", (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CStaticObject::SetUp_HeightOnTerrain()
{
	CObject_Manager*	pObject_Manager = CObject_Manager::GetInstance();

	if (nullptr == pObject_Manager)
		return E_FAIL;
	pObject_Manager->AddRef();

	CBuffer_Terrain_Tool* pBufferCom = (CBuffer_Terrain_Tool*)pObject_Manager->Get_Component(SCENE_STATIC, L"Layer_Terrain", L"Com_Buffer", 0);
	if (nullptr == pBufferCom)
		return E_FAIL;

	_float	fY = pBufferCom->Compute_HeightOnBuffer(m_pTransformCom);

	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION)->x, fY, m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION)->z));

	Safe_Release(pObject_Manager);

	return NOERROR;
}

HRESULT CStaticObject::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
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

CStaticObject * CStaticObject::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CStaticObject* pInstance = new CStaticObject(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		_MSGBOX("CStaticObject Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CStaticObject::Clone(void * pArg)
{
	CStaticObject* pInstance = new CStaticObject(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		_MSGBOX("CStaticObject Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStaticObject::Free()
{
	//Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pMeshCom);
	Safe_Release(m_pShaderCom);

	CGameObject::Free();
}
