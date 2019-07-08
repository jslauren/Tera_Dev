#include "stdafx.h"
#include "..\Headers\BoyNPC.h"

_USING(Client)

CBoyNPC::CBoyNPC(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CNPC(pGraphic_Device)
{
}

CBoyNPC::CBoyNPC(const CBoyNPC & rhs)
	: CNPC(rhs)
{
}

HRESULT CBoyNPC::Ready_GameObject_Prototype()
{
	if (FAILED(CGameObject::Ready_GameObject_Prototype()))
		return E_FAIL;

	return NOERROR;
}

HRESULT CBoyNPC::Ready_GameObject(void * pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL; 

	m_pTransformCom->Set_Scaling(0.6f, 0.6f, 0.6f);
	m_pTransformCom->Set_Angle_Axis(_vec3(0.f, 1.f, 0.f), D3DXToRadian(0.f));
	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(500.f, 0.f, 314.f));

	m_pMeshCom->SetUp_AnimationSet(1);

	ScriptInfo();

	return NOERROR;
}

_int CBoyNPC::Update_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pTransformCom)
		return -1;

	CUnit::Update_GameObject(fTimeDelta);

	CollisionCheck(false);
	TalkWithPlayer(1, 1, 1, false, 0);

	return _int();
}

_int CBoyNPC::LateUpdate_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	CNPC::LateUpdate_GameObject(fTimeDelta);

	return _int();
}

HRESULT CBoyNPC::Render_GameObject()
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

	return NOERROR;
}

HRESULT CBoyNPC::Add_Component()
{
	CNPC::Add_Component();

	// For.Component_Mesh_BoyNPC
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Mesh_BoyNPC", L"Com_Mesh", (CComponent**)&m_pMeshCom)))
		return E_FAIL;

	// For.Com_Collider_BoyNPC_Body
	_float fBodyScale = 15.f;
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Collider_Sphere", L"Com_Collider_BoyNPC_Body",
		(CComponent**)&m_pColliderCom, &CCollider::COLLIDERDESC(CCollider::COLLIDERDESC::TYPE_FRAME,
			m_pTransformCom->Get_WorldMatrixPointer(), &(m_pMeshCom->Get_FrameDesc("Bip01-Spine")->CombinedTransformationMatrix)
			, _vec3(fBodyScale, fBodyScale, fBodyScale), _vec3(0.f, 0.f, 0.f)))))
		return E_FAIL;

	return NOERROR;
}

HRESULT CBoyNPC::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	CNPC::SetUp_ConstantTable(pEffect);

	return NOERROR;
}

void CBoyNPC::ScriptInfo()
{
	// NPC Title //
	m_pTitleScript = L"마을 청년";

	// Quest Start //
	m_pMainScript[0] = L"그거 아세요?\n\n쥬신아카데미에서 언리얼 강의를!!";
	m_pReplyScript[0] = L"흥미롭군.";
}

CBoyNPC * CBoyNPC::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBoyNPC* pInstance = new CBoyNPC(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		_MSGBOX("CBoyNPC Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CBoyNPC::Clone(void * pArg)
{
	CBoyNPC* pInstance = new CBoyNPC(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		_MSGBOX("CBoyNPC Cloned Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBoyNPC::Free()
{
	CNPC::Free();
}
