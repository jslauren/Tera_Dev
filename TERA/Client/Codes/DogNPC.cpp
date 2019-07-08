#include "stdafx.h"
#include "..\Headers\DogNPC.h"

_USING(Client)

CDogNPC::CDogNPC(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CNPC(pGraphic_Device)
{
}

CDogNPC::CDogNPC(const CDogNPC & rhs)
	: CNPC(rhs)
{
}

HRESULT CDogNPC::Ready_GameObject_Prototype()
{
	if (FAILED(CGameObject::Ready_GameObject_Prototype()))
		return E_FAIL;

	return NOERROR;
}

HRESULT CDogNPC::Ready_GameObject(void * pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pTransformCom->Set_Scaling(0.8f, 0.8f, 0.8f);
	m_pTransformCom->Set_Angle_Axis(_vec3(0.f, 1.f, 0.f), D3DXToRadian(180.f));
	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(176.f, 0.f, 350.f));

	m_pMeshCom->SetUp_AnimationSet(1);

	//ScriptInfo();

	return NOERROR;
}

_int CDogNPC::Update_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pTransformCom)
		return -1;

	//CollisionCheck(false);
	//TalkWithPlayer(3, 1);

	return _int();
}

_int CDogNPC::LateUpdate_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	Compute_ViewZ(m_pTransformCom);

	m_fTimeDelta = fTimeDelta;

	if (true == m_pFrustumCom->WorldPt_InFrustum(m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION), m_pTransformCom, 350.f))
	{
		if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONEALPHA, this)))
			return -1;
	}

	return _int();
}

HRESULT CDogNPC::Render_GameObject()
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
//	m_pColliderCom->Render_Collider();

	return NOERROR;
}

HRESULT CDogNPC::Add_Component()
{
	CNPC::Add_Component();

	// For.Component_Mesh_DogNPC
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Mesh_DogNPC", L"Com_Mesh", (CComponent**)&m_pMeshCom)))
		return E_FAIL;

	//// For.Com_Collider_DogyNPC_Body
	//_float fBodyScale = 20.f;
	//if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Collider_Sphere", L"Com_Collider_DogyNPC_Body",
	//	(CComponent**)&m_pColliderCom, &CCollider::COLLIDERDESC(CCollider::COLLIDERDESC::TYPE_FRAME,
	//		m_pTransformCom->Get_WorldMatrixPointer(), &(m_pMeshCom->Get_FrameDesc("Bip01-Spine")->CombinedTransformationMatrix)
	//		, _vec3(fBodyScale, fBodyScale, fBodyScale), _vec3(0.f, 0.f, 0.f)))))
	//	return E_FAIL;

	return NOERROR;
}

HRESULT CDogNPC::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	CNPC::SetUp_ConstantTable(pEffect);

	return NOERROR;
}

void CDogNPC::ScriptInfo()
{
}

CDogNPC * CDogNPC::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CDogNPC* pInstance = new CDogNPC(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		_MSGBOX("CDogNPC Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CDogNPC::Clone(void * pArg)
{
	CDogNPC* pInstance = new CDogNPC(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		_MSGBOX("CDogNPC Cloned Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDogNPC::Free()
{
	CNPC::Free();
}
