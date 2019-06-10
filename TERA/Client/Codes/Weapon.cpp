#include "stdafx.h"
#include "..\Headers\Weapon.h"
#include "Object_Manager.h"
#include "Light_Manager.h"

_USING(Client)

CWeapon::CWeapon(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
	, m_fFrame(0.f)
{
}

CWeapon::CWeapon(const CWeapon & rhs)
	: CGameObject(rhs)
	, m_fFrame(rhs.m_fFrame)
{
}

HRESULT CWeapon::Ready_GameObject_Prototype()
{
	// ����������� ���� ��ü�� ������ �����Ѵ�.
	if (FAILED(CGameObject::Ready_GameObject_Prototype()))
		return E_FAIL;

	return NOERROR;
}

HRESULT CWeapon::Ready_GameObject(void * pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	CObject_Manager*	pObject_Manager = CObject_Manager::GetInstance();
	if (nullptr == pObject_Manager)
		return E_FAIL;

	Safe_AddRef(pObject_Manager);

	CMesh_Dynamic* pPlayerMeshCom = (CMesh_Dynamic*)pObject_Manager->Get_Component(SCENE_STATIC, L"Layer_Player", L"Com_Mesh");
	if (nullptr == pPlayerMeshCom)
		return E_FAIL;

	Safe_AddRef(pPlayerMeshCom);


	m_pBoneMatrix = &(pPlayerMeshCom->Get_FrameDesc("Body_rFingerMidTop")->CombinedTransformationMatrix);
	if (nullptr == m_pBoneMatrix)
		return E_FAIL;

	Safe_Release(pPlayerMeshCom);


	CTransform* pPlayerTransform = (CTransform*)pObject_Manager->Get_Component(SCENE_STATIC, L"Layer_Player", L"Com_Transform");
	if (nullptr == pPlayerTransform)
		return E_FAIL;

	Safe_AddRef(pPlayerTransform);


	m_pParentMatrix = pPlayerTransform->Get_WorldMatrixPointer();
	if (nullptr == m_pParentMatrix)
		return E_FAIL;

	Safe_Release(pPlayerTransform);

	Safe_Release(pObject_Manager);

	m_pTransformCom->Set_Angle_Axis(_vec3(1.f, 0.f, 0.f),D3DXToRadian(300.0f));

	return NOERROR;
}

_int CWeapon::Update_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pTransformCom)
		return -1;

	return _int();
}

_int CWeapon::LateUpdate_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	Compute_ViewZ(m_pTransformCom);

	//if (FAILED(SetUp_HeightOnTerrain()))
	//	return -1;

	m_fTimeDelta = fTimeDelta;

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONEALPHA, this)))
		return -1;

	return _int();
}

HRESULT CWeapon::Render_GameObject()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pTransformCom ||
		nullptr == m_pMeshCom )
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

//	m_pColliderCom->Render_Collider();

	return NOERROR;
}

HRESULT CWeapon::Add_Component()
{
	// For.Com_Transform
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	// For.Com_Mesh
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Mesh_Weapon", L"Com_Mesh", (CComponent**)&m_pMeshCom)))
		return E_FAIL;

	// For.Com_Renderer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	// For.Com_Shader
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Shader_Mesh", L"Com_Shader", (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	//// For.Com_Collider
	//if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Collider_OBB", L"Com_Collider", (CComponent**)&m_pColliderCom,
	//	&CCollider::COLLIDERDESC(CCollider::COLLIDERDESC::TYPE_TRANSFORM, &m_matWorld, nullptr, _vec3(0.2f, 0.2f, 1.5f), _vec3(0.0f, 0.f, -0.75f)))))
	//	return E_FAIL;

	return NOERROR;
}

HRESULT CWeapon::SetUp_HeightOnTerrain()
{
	CObject_Manager*	pObject_Manager = CObject_Manager::GetInstance();

	if (nullptr == pObject_Manager)
		return E_FAIL;
	pObject_Manager->AddRef();

	CBuffer_Terrain* pBufferCom = (CBuffer_Terrain*)pObject_Manager->Get_Component(SCENE_STAGE, L"Layer_BackGround", L"Com_Buffer", 0);
	if (nullptr == pBufferCom)
		return E_FAIL;

	_float	fY = pBufferCom->Compute_HeightOnBuffer(m_pTransformCom);

	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION)->x, fY + 0.5f, m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION)->z));

	Safe_Release(pObject_Manager);

	return NOERROR;
}

HRESULT CWeapon::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	if (nullptr == pEffect)
		return E_FAIL;

	pEffect->AddRef();

	//_matrix		matWorld = *m_pTransformCom->Get_WorldMatrixPointer() * *m_pBoneMatrix * *m_pParentMatrix;
	m_matWorld = *m_pTransformCom->Get_WorldMatrixPointer() * *m_pBoneMatrix * *m_pParentMatrix;

	pEffect->SetMatrix("g_matWorld", &m_matWorld);
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

CWeapon * CWeapon::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CWeapon* pInstance = new CWeapon(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		_MSGBOX("CWeapon Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CWeapon::Clone(void * pArg)
{
	CWeapon* pInstance = new CWeapon(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		_MSGBOX("CWeapon Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWeapon::Free()
{
//	Safe_Release(m_pColliderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pMeshCom);
	Safe_Release(m_pShaderCom);

	CGameObject::Free();
}