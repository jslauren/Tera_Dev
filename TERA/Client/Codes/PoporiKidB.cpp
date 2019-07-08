#include "stdafx.h"
#include "..\Headers\PoporiKidB.h"

_USING(Client)

CPoporiKidB::CPoporiKidB(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CNPC(pGraphic_Device)
{
}

CPoporiKidB::CPoporiKidB(const CPoporiKidB & rhs)
	: CNPC(rhs)
{
}

HRESULT CPoporiKidB::Ready_GameObject_Prototype()
{
	if (FAILED(CGameObject::Ready_GameObject_Prototype()))
		return E_FAIL;

	return NOERROR;
}

HRESULT CPoporiKidB::Ready_GameObject(void * pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pTransformCom->Set_Scaling(0.8f, 0.8f, 0.8f);
	m_pTransformCom->Set_Angle_Axis(_vec3(0.f, 1.f, 0.f), D3DXToRadian(45.f));
	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(218.f, 0.f, 189.f));

	m_pMeshCom->SetUp_AnimationSet(2);

	ScriptInfo();

	return NOERROR;
}

_int CPoporiKidB::Update_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pTransformCom)
		return -1;

	CUnit::Update_GameObject(fTimeDelta);

	CollisionCheck(false);
	TalkWithPlayer(1, 1, 2, false, 45);

	return _int();
}

_int CPoporiKidB::LateUpdate_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	CNPC::LateUpdate_GameObject(fTimeDelta);

	return _int();
}

HRESULT CPoporiKidB::Render_GameObject()
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

	// [ÄÝ¶óÀÌ´õ ·»´õ]
	m_pColliderCom->Render_Collider();

	return NOERROR;
}

HRESULT CPoporiKidB::Add_Component()
{
	CNPC::Add_Component();

	// For.Component_Mesh_PoporiNPC_B
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Mesh_PoporiNPC_B", L"Com_Mesh", (CComponent**)&m_pMeshCom)))
		return E_FAIL;

	// For.Com_Collider_PoporiNPC_B_Body
	_float fBodyScale = 20.f;
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Collider_Sphere", L"Com_Collider_PoporiNPC_B_Body",
		(CComponent**)&m_pColliderCom, &CCollider::COLLIDERDESC(CCollider::COLLIDERDESC::TYPE_FRAME,
			m_pTransformCom->Get_WorldMatrixPointer(), &(m_pMeshCom->Get_FrameDesc("Bip01-Spine")->CombinedTransformationMatrix)
			, _vec3(fBodyScale, fBodyScale, fBodyScale), _vec3(0.f, 0.f, 0.f)))))
		return E_FAIL;

	return NOERROR;
}

HRESULT CPoporiKidB::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	CNPC::SetUp_ConstantTable(pEffect);

	return NOERROR;
}

void CPoporiKidB::ScriptInfo()
{
	// NPC Title //
	m_pTitleScript = L"Æ÷Æ÷¸® B";

	// Quest Start //
	m_pMainScript[0] = L"±ò±ò Ãë¾÷ÇØ¶ó Ãë¾÷.";
	m_pReplyScript[0] = L"°í¸¿±º.";
}

CPoporiKidB * CPoporiKidB::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPoporiKidB* pInstance = new CPoporiKidB(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		_MSGBOX("CPoporiKidB Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CPoporiKidB::Clone(void * pArg)
{
	CPoporiKidB* pInstance = new CPoporiKidB(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		_MSGBOX("CPoporiKidB Cloned Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPoporiKidB::Free()
{
	CNPC::Free();
}
