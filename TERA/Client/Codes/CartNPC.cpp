#include "stdafx.h"
#include "..\Headers\CartNPC.h"
#include "QuestNPC.h"
#include "Management.h"
#include "Scene_Loading.h"
#include "Player.h"

_USING(Client)

CCartNPC::CCartNPC(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CNPC(pGraphic_Device)
{
}

CCartNPC::CCartNPC(const CCartNPC & rhs)
	: CNPC(rhs)
{
}

HRESULT CCartNPC::Ready_GameObject_Prototype()
{
	if (FAILED(CGameObject::Ready_GameObject_Prototype()))
		return E_FAIL;

	return NOERROR;
}

HRESULT CCartNPC::Ready_GameObject(void * pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pTransformCom->Set_Scaling(0.4f, 0.4f, 0.4f);
	m_pTransformCom->Set_Angle_Axis(_vec3(0.f, 1.f, 0.f), D3DXToRadian(0.f));
	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(155.f, 0.f, 85.f));

	m_pMeshCom->SetUp_AnimationSet(1);

	ScriptInfo();

	return NOERROR;
}

_int CCartNPC::Update_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pTransformCom)
		return -1;

	CUnit::Update_GameObject(fTimeDelta);

	CollisionCheck(false, false);

	if (dynamic_cast<CQuestNPC*>(CObject_Manager::GetInstance()->Get_Object(SCENE_STATIC, L"Layer_NPC", 0))->GetCurrentQuestStateInfo() == CQuestNPC::QUEST_STATE::QUEST_ONGOING)
	{
		if (m_iEndScriptNum != 4)
		{
			m_iScriptNumber = 0;
			m_iEndScriptNum = 4; 
			m_iLoopScriptNum = 4;
		}
		if (m_iScriptNumber == 4)
			m_bIsSceneChangeAvailable = true;
	}

	TalkWithPlayer(m_iEndScriptNum, m_iLoopScriptNum, 1, false, 0);
	SoundPlay("Horse_Idle01.ogg");

	return _int();
}

_int CCartNPC::LateUpdate_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	CNPC::LateUpdate_GameObject(fTimeDelta);

	return _int();
}

HRESULT CCartNPC::Render_GameObject()
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

HRESULT CCartNPC::Add_Component()
{
	CNPC::Add_Component();

	// For.Component_Mesh_CartNPC
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Mesh_CartNPC", L"Com_Mesh", (CComponent**)&m_pMeshCom)))
		return E_FAIL;

	// For.Com_Collider_CartNPC_Body
	_float fBodyScale = 50.f;
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Collider_Sphere", L"Com_Collider_CartNPC_Body",
		(CComponent**)&m_pColliderCom, &CCollider::COLLIDERDESC(CCollider::COLLIDERDESC::TYPE_FRAME,
			m_pTransformCom->Get_WorldMatrixPointer(), &(m_pMeshCom->Get_FrameDesc("Bip01-Pelvis01")->CombinedTransformationMatrix)
			, _vec3(fBodyScale, fBodyScale, fBodyScale), _vec3(0.f, 0.f, 0.f)))))
		return E_FAIL;

	return NOERROR;
}

HRESULT CCartNPC::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	CNPC::SetUp_ConstantTable(pEffect);

	return NOERROR;
}

void CCartNPC::ScriptInfo()
{
	// NPC Title //
	m_pTitleScript = L"�̽��͸� ���� ��";

	// Quest Start //
	m_pMainScript[0] = L"�żӹ��!! ���� �̵� ����!!\n\n���ϼ�.";
	m_pReplyScript[0] = L"�ű��ϱ�.";

	m_pMainScript[1] = L"���� ������..!\n\n��ġ�� �����ϱ� ���Ͽ�\n\nõ���� ������ �����ٰ�?";
	m_pReplyScript[1] = L"�׷�. �ߺ�Ź����.";

	m_pMainScript[2] = L"���� ����� �������� �������";
	m_pReplyScript[2] = L"�嵥.. ���� ������ Ÿ�� ����?";

	m_pMainScript[3] = L"�ƴ� �ڳ� ����..\n\n�� �༮���� ������ ������ �ʴ°�?\n\n�μ��� ������� ������ ���δٴ�\n\n���� ����̾������� ����";
	m_pReplyScript[3] = L"(...)�� ���̳�. ���� �׸� �������.";

}

CCartNPC * CCartNPC::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCartNPC* pInstance = new CCartNPC(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		_MSGBOX("CCartNPC Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CCartNPC::Clone(void * pArg)
{
	CCartNPC* pInstance = new CCartNPC(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		_MSGBOX("CCartNPC Cloned Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCartNPC::Free()
{
	CNPC::Free();
}
