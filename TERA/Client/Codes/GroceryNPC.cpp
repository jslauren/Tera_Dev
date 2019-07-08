#include "stdafx.h"
#include "..\Headers\GroceryNPC.h"

_USING(Client)

CGroceryNPC::CGroceryNPC(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CNPC(pGraphic_Device)
{
}

CGroceryNPC::CGroceryNPC(const CGroceryNPC & rhs)
	: CNPC(rhs)
{
}

HRESULT CGroceryNPC::Ready_GameObject_Prototype()
{
	if (FAILED(CGameObject::Ready_GameObject_Prototype()))
		return E_FAIL;

	return NOERROR;
}

HRESULT CGroceryNPC::Ready_GameObject(void * pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pTransformCom->Set_Scaling(0.8f, 0.8f, 0.8f);
	m_pTransformCom->Set_Angle_Axis(_vec3(0.f, 1.f, 0.f), D3DXToRadian(180.f));
	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(176.f, 0.f, 350.f));

	m_pMeshCom->SetUp_AnimationSet(1);

	ScriptInfo();
}

_int CGroceryNPC::Update_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pTransformCom)
		return -1;

	CollisionCheck(false);
	TalkWithPlayer(3, 1);

	return _int();
}

_int CGroceryNPC::LateUpdate_GameObject(const _float & fTimeDelta)
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

HRESULT CGroceryNPC::Render_GameObject()
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

	// [�ݶ��̴� ����]
	m_pColliderCom->Render_Collider();

	return NOERROR;
}

HRESULT CGroceryNPC::Add_Component()
{
	CNPC::Add_Component();

	// For.Component_Mesh_GroceryNPC
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Mesh_GroceryNPC", L"Com_Mesh", (CComponent**)&m_pMeshCom)))
		return E_FAIL;

	// For.Com_Collider_GroceryNPC_Body
	_float fBodyScale = 60.f;
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Collider_Sphere", L"Com_Collider_GroceryNPC_Body",
		(CComponent**)&m_pColliderCom, &CCollider::COLLIDERDESC(CCollider::COLLIDERDESC::TYPE_FRAME,
			m_pTransformCom->Get_WorldMatrixPointer(), &(m_pMeshCom->Get_FrameDesc("Bip01-Spine")->CombinedTransformationMatrix)
			, _vec3(fBodyScale, fBodyScale, fBodyScale), _vec3(0.f, 0.f, 0.f)))))
		return E_FAIL;

	return NOERROR;
}

HRESULT CGroceryNPC::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	CNPC::SetUp_ConstantTable(pEffect);

	return NOERROR;
}

void CGroceryNPC::ScriptInfo()
{
	// NPC Title //
	m_pTitleScript = L"���ȣ ����";

	// Quest Start //
	m_pMainScript[0] = L"������ ���ε�...\n\n�׳��� ������?";
	m_pReplyScript[0] = L"���ػ� ��������.";

	m_pMainScript[1] = L"(......)\n\n�׵��� ����� ���Ұڱ�.\n\n�׷� �����Ϸ� �� ã�ƿ���?";
	m_pReplyScript[1] = L"���� ��ġ�� �����Ϸ� �Դ�.";

	m_pMainScript[2] = L"��ġ�� �����̶�..... ����.\n\n����ī���� õ���� ������̶��\n\n���� �ִ�.������ �������� ������\n\n�Ƹ�Ŀ���� �׿���.";
	m_pReplyScript[2] = L"�ٳ����.";

	// Quest Ongoing //
	m_pMainScript[3] = L"���⼭ ���ϰ� �ִ°���?\n\n�׳��� ��ġ�� �̰ͻ��ΰ�?";
	m_pReplyScript[3] = L"(...) �ٳ����.";
}

CGroceryNPC * CGroceryNPC::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CGroceryNPC* pInstance = new CGroceryNPC(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		_MSGBOX("CGroceryNPC Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CGroceryNPC::Clone(void * pArg)
{
	CGroceryNPC* pInstance = new CGroceryNPC(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		_MSGBOX("CGroceryNPC Cloned Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGroceryNPC::Free()
{
	CNPC::Free();
}
