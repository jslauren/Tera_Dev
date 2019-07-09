#include "stdafx.h"
#include "..\Headers\NPC.h"
#include "Object_Manager.h"
#include "EventManager.h"
#include "SoundManager.h"
#include "Input_Device.h"
#include "Camera_Static.h"
#include "UI_Dialog.h"
#include "QMark.h"
#include "Player.h"

_USING(Client)

CNPC::CNPC(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUnit(pGraphic_Device)
{
	ZeroMemory(m_pMainScript, sizeof(m_pMainScript));
	ZeroMemory(m_pReplyScript, sizeof(m_pReplyScript));
}

CNPC::CNPC(const CNPC & rhs)
	: CUnit(rhs)
	, m_pCameraStatic(dynamic_cast<CCamera_Static*>(CObject_Manager::GetInstance()->Get_Object(SCENE_STAGE, L"Layer_Camera", 1)))
	, m_pQMark(dynamic_cast<CQMark*>(CObject_Manager::GetInstance()->Get_Object(SCENE_STAGE, L"Layer_UI", 0)))
	, m_pPlayer(dynamic_cast<CPlayer*>(CObject_Manager::GetInstance()->Get_Object(SCENE_STATIC, L"Layer_Player")))
{
	Safe_AddRef(m_pCameraStatic);
	Safe_AddRef(m_pQMark);
	Safe_AddRef(m_pPlayer);
}

HRESULT CNPC::Ready_GameObject_Prototype()
{
	return NOERROR;
}

HRESULT CNPC::Ready_GameObject(void * pArg)
{
	return NOERROR;
}

_int CNPC::Update_GameObject(const _float & fTimeDelta)
{
	CUnit::Update_GameObject(fTimeDelta);
	
	return _int();
}

_int CNPC::LateUpdate_GameObject(const _float & fTimeDelta)
{
	Compute_ViewZ(m_pTransformCom);

	m_fTimeDelta = fTimeDelta;

	if (true == m_pFrustumCom->WorldPt_InFrustum(m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION), m_pTransformCom, m_fCulling))
	{
		if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONEALPHA, this)))
			return -1;
	}

	return _int();
}

HRESULT CNPC::Render_GameObject()
{
	return NOERROR;
}

HRESULT CNPC::Add_Component()
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

	// For.Com_Frustum
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Frustum", L"Com_Frustum", (CComponent**)&m_pFrustumCom)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CNPC::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	if (nullptr == pEffect)
		return E_FAIL;

	pEffect->AddRef();

	pEffect->SetMatrix("g_matWorld", m_pTransformCom->Get_WorldMatrixPointer());
	pEffect->SetMatrix("g_matView", &CGameObject::Get_Transform(D3DTS_VIEW));
	pEffect->SetMatrix("g_matProj", &CGameObject::Get_Transform(D3DTS_PROJECTION));

	Safe_Release(pEffect);

	return NOERROR;
}

void CNPC::CollisionCheck(_bool _bIsPlayerInside, _bool _bIsLookPlayer)
{
	// �÷��̾ �������ִ� �ݶ��̴� ��ü�� ������ �����ʾƼ�,
	// �ش� �ݶ��̴��� ���� ��Ʈ���� ���� �����Ⱑ �ǹ��ȴ�..
	CCollider* pPlayerCollider = const_cast<CCollider*>((const CCollider*)CObject_Manager::GetInstance()->Get_Component(SCENE_STATIC, L"Layer_Player", L"Com_Player_Collider"));

	// �׷��� ������ �ѹ� ���� �� (Feat.������)
	if (m_bPlayerRenderingFirst == true)
	{
		pPlayerCollider->Render_Collider();
		m_bPlayerRenderingFirst = false;
	}

	if (_bIsPlayerInside == true)
	{
		m_bIsArchitecture = true;
		PlayerInside(pPlayerCollider);
	}

	TalkEvent(pPlayerCollider, _bIsLookPlayer);
}

void CNPC::PlayerInside(const CCollider * _PlayerCollider)
{
	if (m_pColliderEventCom->Collision_Sphere(_PlayerCollider) == true)
	{
		m_bIsPlayerInside = true;
		m_iEventArgValue = 0;
		CEventManager::GetInstance()->Notify_Event(L"Player_Inside_Event", &m_iEventArgValue);
	}
	else
	{
		// �̺�Ʈ�� ��� ������ �ʰ�, �÷��̾ �ǹ� ������ ����� �ÿ� �ѹ��� ������ ����
		// �ش� bool ���� �̿��� ������ ��� ��.
		if (m_bIsPlayerInside == true)
		{
			m_iEventArgValue = 1;
			CEventManager::GetInstance()->Notify_Event(L"Player_Inside_Event", &m_iEventArgValue);
			m_bIsPlayerInside = false;
		}
	}
}

void CNPC::TalkEvent(const CCollider * _PlayerCollider, _bool _bIsLookPlayer)
{
	if (m_bIsEventCollisionFirst == true)
	{
		if (m_pColliderCom->Collision_Sphere(_PlayerCollider) == true)
		{
			if (CInput_Device::GetInstance()->Get_DIKeyDown(DIK_F))
			{
				m_pMeshCom->SetUp_AnimationSet(0.f);
				m_pCameraStatic->Set_TalkingInfo(true);
				
				if(_bIsLookPlayer == true)
					LookAtPlayer();

				m_bIsTalking = true;
				m_bIsTalkEnded = false;
				m_bIsEventCollisionFirst = false;
			}
		}
	}
}

void CNPC::LookAtPlayer()
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

void CNPC::TalkWithPlayer(_uint _iEndScriptNum, _uint _iLoopScriptNum, _uint _iInitAniNum, _bool _bIsQuestNPC, _float fResetViewAngle)
{
	if (m_bIsTalking == true)
	{
		if (m_pMeshCom->IsAnimationEnded(0.85f))
			m_pMeshCom->SetUp_AnimationSet(1);

		CUI_Dialog* pUI_Dialog = dynamic_cast<CUI_Dialog*>(CObject_Manager::GetInstance()->Get_Object(SCENE_STAGE, L"Layer_UI", 1));

		if(_bIsQuestNPC == true)
			m_bIsQuestNPC = true;

		if (m_bIsTalkEnded == true)
		{
			// ��ȭ�� ������ �ʱⰪ���� �������ش�.
			TalkEventFree(pUI_Dialog, _iInitAniNum, fResetViewAngle);

			if (m_bIsQuestNPC == true)
				m_pQMark->Set_CurrentMark(CQMark::QMARK_ONGOING);

			else
				m_iScriptNumber = -1;

			return;
		}

		// ��ȭ�߿��� �÷��̾� Ű ������ ���ϰ� �ϱ� ���� bool �� ����.
		pUI_Dialog->Set_TalkEventAvaliable(true);		

		if (CInput_Device::GetInstance()->Get_DIKeyDown(DIK_F) && 0x80)
		{
			if (m_bIsLoopOn == true)
			{
				m_bIsTalkEnded = true;
				m_bIsLoopOn = false;
				return;
			}

			++m_iScriptNumber;

			if (m_iScriptNumber == _iEndScriptNum)
			{
				m_bIsTalkEnded = true;
				return;
			}

			//m_pPlayer->Set_NPCSoundCheckInfo(true);
			//	SoundPlay(m_pSoundKey);

			if (m_iScriptNumber == _iLoopScriptNum)
			{
				m_bIsLoopOn = true;
				--m_iScriptNumber;
			}

		}

		pUI_Dialog->MakeScript(CUI_Dialog::SCRIPT_TITLE, m_pTitleScript);
		pUI_Dialog->MakeScript(CUI_Dialog::SCRIPT_MAIN, m_pMainScript[m_iScriptNumber]);
		pUI_Dialog->MakeScript(CUI_Dialog::SCRIPT_REPLY, m_pReplyScript[m_iScriptNumber]);
	}
}

void CNPC::TalkEventFree(CUI_Dialog * _pUI_Dialog, _uint iAniNum, _float fResetViewAngle)
{
//	m_pPlayer->Set_NPCSoundCheckInfo(false);
	m_pMeshCom->SetUp_AnimationSet(iAniNum);
	m_pCameraStatic->Set_TalkingInfo(false);
	m_bIsTalking = false;
	m_bIsTalkEnded = false;
	m_bIsEventCollisionFirst = true;
	_pUI_Dialog->Set_TalkEventAvaliable(false);
	m_pTransformCom->Set_Angle_Axis(_vec3(0.f, 1.f, 0.f), D3DXToRadian(fResetViewAngle));

	if (m_bIsArchitecture != true)
		m_pCameraStatic->Set_CameraRevertInfo(true);
}

void CNPC::ScriptInfo()
{
	//// NPC Title //
	//m_pTitleScript = L"���ֱ� ����";

	//// Quest Start //
	//m_pMainScript[0] = L"������ ���ε�...\n\n�׳��� ������?";
	//m_pReplyScript[0] = L"���ػ� ��������.";

	//m_pMainScript[1] = L"(......)\n\n�׵��� ����� ���Ұڱ�.\n\n�׷� �����Ϸ� �� ã�ƿ���?";
	//m_pReplyScript[1] = L"���� ��ġ�� �����Ϸ� �Դ�.";

	//m_pMainScript[2] = L"��ġ�� �����̶�..... ����.\n\n����ī���� õ���� ������̶��\n\n���� �ִ�.������ �������� ������\n\n�Ƹ�Ŀ���� �׿���.";
	//m_pReplyScript[2] = L"�ٳ����.";

	//// Quest Ongoing //
	//m_pMainScript[3] = L"���⼭ ���ϰ� �ִ°���?\n\n�׳��� ��ġ�� �̰ͻ��ΰ�?";
	//m_pReplyScript[3] = L"(...) �ٳ����.";
}

void CNPC::SoundPlay(const string& pSoundKey, CSoundManager::Channel_ID eID, bool bLoop)
{
	if (m_pPlayer == nullptr)
		return;

	if (m_pPlayer->Get_NPCSoundCheckInfo() == true)
	{
		CSoundManager::GetInstance()->Stop_Sound(eID);
		CSoundManager::GetInstance()->Play_SoundChannel(pSoundKey, eID, bLoop);

		m_pPlayer->Set_NPCSoundCheckInfo(false);
	}
}

void CNPC::Free()
{
	Safe_Release(m_pPlayer);
	Safe_Release(m_pQMark);
	Safe_Release(m_pCameraStatic);
	Safe_Release(m_pColliderEventCom);

	CUnit::Free();
}
