#include "stdafx.h"
#include "..\Headers\GirlNPC.h"

_USING(Client)

CGirlNPC::CGirlNPC(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CNPC(pGraphic_Device)
{
}

CGirlNPC::CGirlNPC(const CGirlNPC & rhs)
	: CNPC(rhs)
{
}

HRESULT CGirlNPC::Ready_GameObject_Prototype()
{
	if (FAILED(CGameObject::Ready_GameObject_Prototype()))
		return E_FAIL;

	return NOERROR;
}

HRESULT CGirlNPC::Ready_GameObject(void * pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pTransformCom->Set_Scaling(0.6f, 0.6f, 0.6f);
	m_pTransformCom->Set_Angle_Axis(_vec3(0.f, 1.f, 0.f), D3DXToRadian(180.f));
	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(500.f, 0.f, 335.f));

	m_pMeshCom->SetUp_AnimationSet(1);

	ScriptInfo();

	return NOERROR;
}

_int CGirlNPC::Update_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pTransformCom)
		return -1;

	CUnit::Update_GameObject(fTimeDelta);

	CollisionCheck(false);
	TalkWithPlayer(1, 1, 1, false, 180);

	return _int();
}

_int CGirlNPC::LateUpdate_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	CNPC::LateUpdate_GameObject(fTimeDelta);

	return _int();
}

HRESULT CGirlNPC::Render_GameObject()
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

HRESULT CGirlNPC::Add_Component()
{
	CNPC::Add_Component();

	// For.Component_Mesh_GirlNPC
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Mesh_GirlNPC", L"Com_Mesh", (CComponent**)&m_pMeshCom)))
		return E_FAIL;

	// For.Com_Collider_GirlNPC_Body
	_float fBodyScale = 15.f;
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Collider_Sphere", L"Com_Collider_GirlNPC_Body",
		(CComponent**)&m_pColliderCom, &CCollider::COLLIDERDESC(CCollider::COLLIDERDESC::TYPE_FRAME,
			m_pTransformCom->Get_WorldMatrixPointer(), &(m_pMeshCom->Get_FrameDesc("Bip01-Spine")->CombinedTransformationMatrix)
			, _vec3(fBodyScale, fBodyScale, fBodyScale), _vec3(0.f, 0.f, 0.f)))))
		return E_FAIL;

	return NOERROR;

}

HRESULT CGirlNPC::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	CNPC::SetUp_ConstantTable(pEffect);

	return NOERROR;
}

void CGirlNPC::ScriptInfo()
{
	// NPC Title //
	m_pTitleScript = L"마을 소녀";

	// Quest Start //
	m_pMainScript[0] = L"세상에 언리얼 강의라니..!";
	m_pReplyScript[0] = L"흥미롭군.";
}

CGirlNPC * CGirlNPC::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CGirlNPC* pInstance = new CGirlNPC(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		_MSGBOX("CGirlNPC Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CGirlNPC::Clone(void * pArg)
{
	CGirlNPC* pInstance = new CGirlNPC(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		_MSGBOX("CGirlNPC Cloned Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGirlNPC::Free()
{
	CNPC::Free();
}
