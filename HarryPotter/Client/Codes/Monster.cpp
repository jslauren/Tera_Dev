#include "stdafx.h"
#include "..\Headers\Monster.h"
#include "Object_Manager.h"
#include "Light_Manager.h"

_USING(Client)

CMonster::CMonster(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
	, m_fFrame(0.f)
{
}

CMonster::CMonster(const CMonster & rhs)
	: CGameObject(rhs)
	, m_fFrame(rhs.m_fFrame)
{
}

// ������ü�� �ʿ��� �����͸� �����Ѵ�.
HRESULT CMonster::Ready_GameObject_Prototype()
{
	// ����������� ���� ��ü�� ������ �����Ѵ�.
	if (FAILED(CGameObject::Ready_GameObject_Prototype()))
		return E_FAIL;

	return NOERROR;
}

// ���� ������ ����� ��ü�� ȣ���ϴ� �Լ�.
// ������ü �����ܿ��� �߰����� �������ʿ��ϸ� ���⼭ �����ض�.
HRESULT CMonster::Ready_GameObject(void* pArg)
{
	_matrix matWorld = *(_matrix*)pArg;


	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pTransformCom->Set_Scaling(0.01f, 0.01f, 0.01f);

	m_pTransformCom->Set_WorldMatrix(matWorld);
	//m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(rand() % 10 + 5, 0.f, rand() % 10 + 5));
	m_pMeshCom->SetUp_AnimationSet(rand() % 10);

	return NOERROR;
}

_int CMonster::Update_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pTransformCom)
		return -1;

	return _int();
}

_int CMonster::LateUpdate_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	Compute_ViewZ(m_pTransformCom);

	if (FAILED(SetUp_HeightOnTerrain()))
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
		nullptr == m_pMeshCom )
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

	//// For.Com_Texture
	//if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Texture_Effect", L"Com_Texture", (CComponent**)&m_pTextureCom)))
	//	return E_FAIL;

	// For.Com_Shader
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Shader_Mesh", L"Com_Shader", (CComponent**)&m_pShaderCom)))
		return E_FAIL;
	
	return NOERROR;
}

HRESULT CMonster::SetUp_HeightOnTerrain()
{
	CObject_Manager*	pObject_Manager = CObject_Manager::GetInstance();

	if (nullptr == pObject_Manager)
		return E_FAIL;
	pObject_Manager->AddRef();

	// �ε��� 1������ �ؾ���.
	CBuffer_Terrain* pBufferCom = (CBuffer_Terrain*)pObject_Manager->Get_Component(SCENE_STAGE, L"Layer_BackGround", L"Com_Buffer", 1);
	if (nullptr == pBufferCom)
		return E_FAIL;

	_float	fY = pBufferCom->Compute_HeightOnBuffer(m_pTransformCom);

	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION)->x, fY + 0.5f, m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION)->z));

	Safe_Release(pObject_Manager);

	return NOERROR;
}

HRESULT CMonster::SetUp_BillBoard()
{
	CObject_Manager*	pObject_Manager = CObject_Manager::GetInstance();

	if (nullptr == pObject_Manager)
		return E_FAIL;
	pObject_Manager->AddRef();

	CTransform* pCamTransformCom = (CTransform*)pObject_Manager->Get_Component(SCENE_STAGE, L"Layer_Camera", L"Com_Transform", 0);
	if (nullptr == pCamTransformCom)
		return E_FAIL;

	// ī�޶��� ������� == �佺���̽� ��ȯ��� �����
	m_pTransformCom->Set_StateInfo(CTransform::STATE_RIGHT
		, pCamTransformCom->Get_StateInfo(CTransform::STATE_RIGHT));
	//m_pTransformCom->Set_StateInfo(CTransform::STATE_UP
	//	, pCamTransformCom->Get_StateInfo(CTransform::STATE_UP));
	m_pTransformCom->Set_StateInfo(CTransform::STATE_LOOK
		, pCamTransformCom->Get_StateInfo(CTransform::STATE_LOOK));

	Safe_Release(pObject_Manager);

	return NOERROR;
}

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


// ������ü�� �����Ѵ�.
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

// this? : 
// 1.����Լ��ȿ� ����. 
// 2.����Լ��� ��ü�κ��� ȣ��(��ü.����Լ�(), ��ü�ּ�->����Լ�())
// 3.����Լ��ȿ� �����ϴ� this�� ����Լ��� ȣ���� �����ϰ��� ��ü�� �ּҸ� �ǹ��Ѵ�.
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
//	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pMeshCom);
	Safe_Release(m_pShaderCom);

	CGameObject::Free();
}
