#include "stdafx.h"
#include "..\Headers\QuestNPC.h"
#include "Object_Manager.h"
#include "Light_Manager.h"
#include "EventManager.h"

_USING(Client)

CQuestNPC::CQuestNPC(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUnit(pGraphic_Device)
{
}

CQuestNPC::CQuestNPC(const CQuestNPC & rhs)
	: CUnit(rhs)
{
}

HRESULT CQuestNPC::Ready_GameObject_Prototype()
{
	if (FAILED(CGameObject::Ready_GameObject_Prototype()))
		return E_FAIL;

	return NOERROR;
}

HRESULT CQuestNPC::Ready_GameObject(void * pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pTransformCom->Set_Scaling(0.19f, 0.19f, 0.19f);
	m_pTransformCom->Set_Angle_Axis(_vec3(0.f, 1.f, 0.f), D3DXToRadian(180.f));
	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(364.f, 0.f, 364.f));

	m_pMeshCom->SetUp_AnimationSet(1);

	return NOERROR;
}

_int CQuestNPC::Update_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pTransformCom)
		return -1;

	CollisionCheck();

	return _int();
}

_int CQuestNPC::LateUpdate_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	Compute_ViewZ(m_pTransformCom);

	m_fTimeDelta = fTimeDelta;

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONEALPHA, this)))
		return -1;

	return _int();
}

HRESULT CQuestNPC::Render_GameObject()
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
	
	// [콜라이더 렌더]
	m_pColliderCom->Render_Collider();
	m_pColliderEventCom->Render_Collider();

	return NOERROR;
}

HRESULT CQuestNPC::Add_Component()
{
		// For.Com_Transform
		if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom)))
			return E_FAIL;
		
		// For.Com_Renderer
		if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom)))
			return E_FAIL;
	
		// For.Com_Shader
		if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Shader_Mesh", L"Com_Shader", (CComponent**)&m_pShaderCom)))
			return E_FAIL;
	
		// For.Component_Mesh_QuestNPC
		if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Mesh_QuestNPC", L"Com_Mesh", (CComponent**)&m_pMeshCom)))
			return E_FAIL;
	
		// For.Com_Collider_QuestNPC_Event
		_float fEventSphereScale = 100.f;
		if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Collider_Sphere", L"Com_Collider_QuestNPC_Event",
			(CComponent**)&m_pColliderEventCom, &CCollider::COLLIDERDESC(CCollider::COLLIDERDESC::TYPE_FRAME,
			m_pTransformCom->Get_WorldMatrixPointer(), &(m_pMeshCom->Get_FrameDesc("Bip01-Spine")->CombinedTransformationMatrix)
			, _vec3(fEventSphereScale, fEventSphereScale, fEventSphereScale), _vec3(0.f, 0.f, 0.f)))))
			return E_FAIL;

		// For.Com_Collider_QuestNPC_Body
		_float fBodyScale = 50.f;
		if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Collider_Sphere", L"Com_Collider_QuestNPC_Body",
			(CComponent**)&m_pColliderCom, &CCollider::COLLIDERDESC(CCollider::COLLIDERDESC::TYPE_FRAME,
				m_pTransformCom->Get_WorldMatrixPointer(), &(m_pMeshCom->Get_FrameDesc("Bip01-Spine")->CombinedTransformationMatrix)
				, _vec3(fBodyScale, fBodyScale, fBodyScale), _vec3(0.f, 0.f, 0.f)))))
			return E_FAIL;
		
		return NOERROR;
}

HRESULT CQuestNPC::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	if (nullptr == pEffect)
		return E_FAIL;

	pEffect->AddRef();

	pEffect->SetMatrix("g_matWorld", m_pTransformCom->Get_WorldMatrixPointer());
	pEffect->SetMatrix("g_matView", &CGameObject::Get_Transform(D3DTS_VIEW));
	pEffect->SetMatrix("g_matProj", &CGameObject::Get_Transform(D3DTS_PROJECTION));

	Safe_Release(pEffect);
}

void CQuestNPC::CollisionCheck()
{
	const CCollider* pPlayerCollider = (const CCollider*)CObject_Manager::GetInstance()->Get_Component(SCENE_STATIC, L"Layer_Player", L"Com_Player_Collider");

	if (m_pColliderEventCom->Collision_Sphere(pPlayerCollider) == true)
	{
		m_bIsPlayerInside = true;
		m_iEventArgValue = 0;
		CEventManager::GetInstance()->Notify_Event(L"Player_Inside_Event", &m_iEventArgValue);
	}
	else
	{
		// 이벤트를 계속 보내지 않고, 플레이어가 건물 밖으로 벗어났을 시에 한번만 보내기 위해
		// 해당 bool 값을 이용한 구문을 사용 함.
		if (m_bIsPlayerInside == true)
		{
			m_iEventArgValue = 1;
			CEventManager::GetInstance()->Notify_Event(L"Player_Inside_Event", &m_iEventArgValue);
			m_bIsPlayerInside = false;
		}
	}
}

CQuestNPC * CQuestNPC::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CQuestNPC* pInstance = new CQuestNPC(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		_MSGBOX("CQuestNPC Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CQuestNPC::Clone(void * pArg)
{
	CQuestNPC* pInstance = new CQuestNPC(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		_MSGBOX("CQuestNPC Cloned Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CQuestNPC::Free()
{
	Safe_Release(m_pColliderEventCom);

	CUnit::Free();
}
