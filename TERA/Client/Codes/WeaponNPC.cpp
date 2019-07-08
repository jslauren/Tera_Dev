#include "stdafx.h"
#include "..\Headers\WeaponNPC.h"

_USING(Client)

CWeaponNPC::CWeaponNPC(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CNPC(pGraphic_Device)
{
}

CWeaponNPC::CWeaponNPC(const CWeaponNPC & rhs)
	: CNPC(rhs)
{
}

HRESULT CWeaponNPC::Ready_GameObject_Prototype()
{
	if (FAILED(CGameObject::Ready_GameObject_Prototype()))
		return E_FAIL;

	return NOERROR;
}

HRESULT CWeaponNPC::Ready_GameObject(void * pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pTransformCom->Set_Scaling(0.5f, 0.5f, 0.5f);
	m_pTransformCom->Set_Angle_Axis(_vec3(0.f, 1.f, 0.f), D3DXToRadian(0.f));
	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(271.f, 0.f, 63.f));

	m_pMeshCom->SetUp_AnimationSet(1);

	ScriptInfo();
}

_int CWeaponNPC::Update_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pTransformCom)
		return -1;

	CollisionCheck(false);
	TalkWithPlayer(1, 1, 1, false, 0);

	return _int();
}

_int CWeaponNPC::LateUpdate_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	Compute_ViewZ(m_pTransformCom);

	m_fTimeDelta = fTimeDelta;

	if (true == m_pFrustumCom->WorldPt_InFrustum(m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION), m_pTransformCom, m_fCulling))
	{
		if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONEALPHA, this)))
			return -1;
	}

	return _int();
}

HRESULT CWeaponNPC::Render_GameObject()
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

HRESULT CWeaponNPC::Add_Component()
{
	CNPC::Add_Component();

	// For.Component_Mesh_WeaponNPC
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Mesh_WeaponNPC", L"Com_Mesh", (CComponent**)&m_pMeshCom)))
		return E_FAIL;

	// For.Com_Collider_WeaponNPC_Body
	_float fBodyScale = 30.f;
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Collider_Sphere", L"Com_Collider_WeaponNPC_Body",
		(CComponent**)&m_pColliderCom, &CCollider::COLLIDERDESC(CCollider::COLLIDERDESC::TYPE_FRAME,
			m_pTransformCom->Get_WorldMatrixPointer(), &(m_pMeshCom->Get_FrameDesc("Bip01-Spine")->CombinedTransformationMatrix)
			, _vec3(fBodyScale, fBodyScale, fBodyScale), _vec3(0.f, 0.f, 0.f)))))
		return E_FAIL;

	return NOERROR;
}

HRESULT CWeaponNPC::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	CNPC::SetUp_ConstantTable(pEffect);

	return NOERROR;
}

void CWeaponNPC::ScriptInfo()
{
	// NPC Title //
	m_pTitleScript = L"무기상인 실라";

	// Quest Start //
	m_pMainScript[0] = L"후후 처음보는 얼굴이네?\n\n아직 무기상점은 오픈 전이야";
	m_pReplyScript[0] = L"알겠습니다.";

}

CWeaponNPC * CWeaponNPC::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CWeaponNPC* pInstance = new CWeaponNPC(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		_MSGBOX("CWeaponNPC Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CWeaponNPC::Clone(void * pArg)
{
	CWeaponNPC* pInstance = new CWeaponNPC(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		_MSGBOX("CWeaponNPC Cloned Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWeaponNPC::Free()
{
	CNPC::Free();
}
