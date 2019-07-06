#include "stdafx.h"
#include "..\Headers\QuestNPC.h"
#include "Object_Manager.h"
#include "Light_Manager.h"
#include "EventManager.h"
#include "Input_Device.h"
#include "Camera_Static.h"
#include "UI_Dialog.h"
#include "Player.h"


_USING(Client)

CQuestNPC::CQuestNPC(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUnit(pGraphic_Device)
{
	ZeroMemory(m_pMainScript, sizeof(m_pMainScript));
	ZeroMemory(m_pReplyScript, sizeof(m_pReplyScript));
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

	ScriptInfo();

	return NOERROR;
}

_int CQuestNPC::Update_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pTransformCom)
		return -1;

	CollisionCheck();
	TalkWithPlayer();

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
		_float fBodyScale = 40.f;
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
	// 플레이어가 가지고있는 콜라이더 객체의 렌더가 돌지않아서,
	// 해당 콜라이더의 월드 매트릭스 값이 쓰레기가 되버렸다..
	CCollider* pPlayerCollider = const_cast<CCollider*>((const CCollider*)CObject_Manager::GetInstance()->Get_Component(SCENE_STATIC, L"Layer_Player", L"Com_Player_Collider"));
	
	// 그래서 억지로 한번 돌림 ㅎ (Feat.시훤형)
	if (m_bPlayerRenderingFirst == true)
	{
		pPlayerCollider->Render_Collider();
		m_bPlayerRenderingFirst = false;
	}

	PlayerInside(pPlayerCollider);
	TalkEvent(pPlayerCollider);
}

void CQuestNPC::PlayerInside(const CCollider* _PlayerCollider)
{
	if (m_pColliderEventCom->Collision_Sphere(_PlayerCollider) == true)
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

void CQuestNPC::TalkEvent(const CCollider* _PlayerCollider)
{
	if (m_bIsEventCollisionFirst == true)
	{
		if (m_pColliderCom->Collision_Sphere(_PlayerCollider) == true)
		{
			if (CInput_Device::GetInstance()->Get_DIKeyDown(DIK_F))
			{
				CCamera_Static* pStaticCamera = dynamic_cast<CCamera_Static*>(CObject_Manager::GetInstance()->Get_Object(SCENE_STAGE, L"Layer_Camera", 1));
				pStaticCamera->Set_TalkingInfo(true);
				LookAtPlayer();
				m_bIsTalking = true;
				m_bIsEventCollisionFirst = false;
			}
		}
	}

}

void CQuestNPC::LookAtPlayer()
{
	_vec3 vRight, vUp, vLook, vDir, vPlayerPos, vPos;

	CPlayer*	pPlayer = dynamic_cast<CPlayer*>(CObject_Manager::GetInstance()->Get_Object(SCENE_STATIC, L"Layer_Player"));

	vPlayerPos = *pPlayer->Get_Transform()->Get_StateInfo(CTransform::STATE_POSITION);
	vPos = *m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION);
	vRight = *m_pTransformCom->Get_StateInfo(CTransform::STATE_RIGHT);
	vUp = *m_pTransformCom->Get_StateInfo(CTransform::STATE_UP);
	vLook = *m_pTransformCom->Get_StateInfo(CTransform::STATE_LOOK);

	vDir = vPlayerPos - vPos;

	_float fRightScale, fUpScale, fLookScale;

	fRightScale = D3DXVec3Length(&vRight);
	fUpScale = D3DXVec3Length(&vUp);
	fLookScale = D3DXVec3Length(&vLook);

	D3DXVec3Cross(&vRight, &_vec3(0.f, 1.f, 0.f), &vDir);
	D3DXVec3Cross(&vUp, &vDir, &vRight);

	D3DXVec3Normalize(&vRight, &vRight);
	D3DXVec3Normalize(&vUp, &vUp);
	D3DXVec3Normalize(&vDir, &vDir);

	m_pTransformCom->Set_StateInfo(CTransform::STATE_RIGHT, &(vRight * fRightScale));
	m_pTransformCom->Set_StateInfo(CTransform::STATE_UP, &(vUp * fUpScale));
	m_pTransformCom->Set_StateInfo(CTransform::STATE_LOOK, &(vDir * fLookScale));

}

void CQuestNPC::TalkWithPlayer()
{
	if (m_bIsTalking == true)
	{	
		// Layer_UI에 SCENE_STATIC 2개랑 SCENE_STAGE가 2개 있기 때문에,
		// 밑에 놈은 STAGE 2번째라 1을 넣어준다.
		// 3 넣으면 안된다 -_-...
		CUI_Dialog* pUI_Dialog = dynamic_cast<CUI_Dialog*>(CObject_Manager::GetInstance()->Get_Object(SCENE_STAGE, L"Layer_UI", 1));
		
		// 대화중에는 플레이어 키 조작을 못하게 하기 위한 bool 값 셋팅.
		pUI_Dialog->Set_TalkEventAvaliable(true);

		pUI_Dialog->MakeScript(CUI_Dialog::SCRIPT_TITLE, L"대주교 벨라");

		if (CInput_Device::GetInstance()->Get_DIKeyDown(DIK_F) && 0x80)
		{
			++m_iScriptNumber;

			if (m_iScriptNumber >= 2)
				--m_iScriptNumber;
		}

		pUI_Dialog->MakeScript(CUI_Dialog::SCRIPT_MAIN, m_pMainScript[m_iScriptNumber]);
		pUI_Dialog->MakeScript(CUI_Dialog::SCRIPT_REPLY, m_pReplyScript[m_iScriptNumber]);

	}
}

void CQuestNPC::ScriptInfo()
{
	m_pMainScript[0] = L"못보던 놈인데...\n\n네놈은 누구지?";
	m_pReplyScript[0] = L"취준생 빡정수다!!!";

	m_pMainScript[1] = L"(......)\n\n그동안 고생이 많았겠군.\n\n그래 무슨일로 날 찾아왔지?";
	m_pReplyScript[1] = L"나의 가치를 증명하러 왔다.";

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
