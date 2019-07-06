#include "stdafx.h"
#include "..\Headers\Player.h"
#include "Object_Manager.h"
#include "Light_Manager.h"
#include "EventManager.h"
#include "Management.h"
#include "Player_Idle.h"
#include "Player_Move.h"
#include "UI_Dialog.h"
#include "Layer.h"
#include "Arkus.h"
#include "Input_Device.h"
#include "Time.h"

#define PLAYER_SCALING	0.33f
#define	PLAYER_HP	10686
#define	PLAYER_MP	3250

_USING(Client)

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUnit(pGraphic_Device)
{
}

CPlayer::CPlayer(const CPlayer & rhs)
	: CUnit(rhs)
{
}

const _bool & CPlayer::Get_SkillAvailable(PLAYER_ANI eAttackAni)
{
	_float fCurrentMP = Get_MP();

	switch (eAttackAni)
	{
	case Client::CPlayer::CutHead:
		if (fCurrentMP >= m_iRequirementSkillMP[0])
			return true;
		else
			return false;
		break;

	case Client::CPlayer::CuttingSlash:
		if (fCurrentMP >= m_iRequirementSkillMP[1])
			return true;
		else
			return false;
		break;

	case Client::CPlayer::FlatBlade:
		if (fCurrentMP >= m_iRequirementSkillMP[2])
			return true;
		else
			return false;
		break;

	case Client::CPlayer::HandySlash:
		if (fCurrentMP >= m_iRequirementSkillMP[3])
			return true;
		else
			return false;
		break;

	case Client::CPlayer::JawBreaker:
		if (fCurrentMP >= m_iRequirementSkillMP[4])
			return true;
		else
			return false;
		break;

	case Client::CPlayer::StingerBlade:
		if (fCurrentMP >= m_iRequirementSkillMP[5])
			return true;
		else
			return false;
		break;

	case Client::CPlayer::RagingStrike:
		if (fCurrentMP >= m_iRequirementSkillMP[6])
			return true;
		else
			return false;
		break;

	case Client::CPlayer::DrawSword:
		if (fCurrentMP >= m_iRequirementSkillMP[7])
			return true;
		else
			return false;
		break;

	case Client::CPlayer::Tumbling:
		if (fCurrentMP >= m_iRequirementSkillMP[8])
			return true;
		else
			return false;
		break;
	}

	return false;
}

const _uint & CPlayer::Get_Requirement_SkillMP_Info(PLAYER_ANI eAttackAni)
{
	switch (eAttackAni)
	{
	case Client::CPlayer::CutHead:
		return m_iRequirementSkillMP[0];
		break;

	case Client::CPlayer::CuttingSlash:
		return m_iRequirementSkillMP[1];
		break;

	case Client::CPlayer::FlatBlade:
		return m_iRequirementSkillMP[2];
		break;

	case Client::CPlayer::HandySlash:
		return m_iRequirementSkillMP[3];
		break;

	case Client::CPlayer::JawBreaker:
		return m_iRequirementSkillMP[4];
		break;

	case Client::CPlayer::StingerBlade:
		return m_iRequirementSkillMP[5];
		break;

	case Client::CPlayer::RagingStrike:
		return m_iRequirementSkillMP[6];
		break;

	case Client::CPlayer::DrawSword:
		return m_iRequirementSkillMP[7];
		break;

	case Client::CPlayer::Tumbling:
		return m_iRequirementSkillMP[8];
		break;
	}
	
	return 0;
}

const _float & CPlayer::Get_CurrentCoolTimeInfo(PLAYER_ANI eAttackAni)
{
	switch (eAttackAni)
	{
	case Client::CPlayer::CutHead:
		return m_fCurrentCoolTime[0];
		break;

	case Client::CPlayer::CuttingSlash:
		return m_fCurrentCoolTime[1];
		break;

	case Client::CPlayer::FlatBlade:
		return m_fCurrentCoolTime[2];
		break;

	case Client::CPlayer::HandySlash:
		return m_fCurrentCoolTime[3];
		break;

	case Client::CPlayer::JawBreaker:
		return m_fCurrentCoolTime[4];
		break;

	case Client::CPlayer::StingerBlade:
		return m_fCurrentCoolTime[5];
		break;

	case Client::CPlayer::RagingStrike:
		return m_fCurrentCoolTime[6];
		break;

	case Client::CPlayer::DrawSword:
		return m_fCurrentCoolTime[7];
		break;

	case Client::CPlayer::Tumbling:
		return m_fCurrentCoolTime[8];
		break;
	}

	return 0;
}

const _float & CPlayer::Get_MaxCoolTimeInfo(PLAYER_ANI eAttackAni)
{
	switch (eAttackAni)
	{
	case Client::CPlayer::CutHead:
		return m_fMaxCoolTime[0];
		break;

	case Client::CPlayer::CuttingSlash:
		return m_fMaxCoolTime[1];
		break;

	case Client::CPlayer::FlatBlade:
		return m_fMaxCoolTime[2];
		break;

	case Client::CPlayer::HandySlash:
		return m_fMaxCoolTime[3];
		break;

	case Client::CPlayer::JawBreaker:
		return m_fMaxCoolTime[4];
		break;

	case Client::CPlayer::StingerBlade:
		return m_fMaxCoolTime[5];
		break;

	case Client::CPlayer::RagingStrike:
		return m_fMaxCoolTime[6];
		break;

	case Client::CPlayer::DrawSword:
		return m_fMaxCoolTime[7];
		break;

	case Client::CPlayer::Tumbling:
		return m_fMaxCoolTime[8];
		break;
	}

	return 0;
}

const _bool & CPlayer::Get_CoolTimeAvailable(PLAYER_ANI eAttackAni)
{
	switch (eAttackAni)
	{
	case Client::CPlayer::CutHead:
		return m_bIsCoolTimeAvailable[0];
		break;

	case Client::CPlayer::CuttingSlash:
		return m_bIsCoolTimeAvailable[1];
		break;

	case Client::CPlayer::FlatBlade:
		return m_bIsCoolTimeAvailable[2];
		break;

	case Client::CPlayer::HandySlash:
		return m_bIsCoolTimeAvailable[3];
		break;

	case Client::CPlayer::JawBreaker:
		return m_bIsCoolTimeAvailable[4];
		break;

	case Client::CPlayer::StingerBlade:
		return m_bIsCoolTimeAvailable[5];
		break;

	case Client::CPlayer::RagingStrike:
		return m_bIsCoolTimeAvailable[6];
		break;

	case Client::CPlayer::DrawSword:
		return m_bIsCoolTimeAvailable[7];
		break;

	case Client::CPlayer::Tumbling:
		return m_bIsCoolTimeAvailable[8];
		break;
	}

	return 0;
}

const _bool & CPlayer::GetSkillAvailable(PLAYER_ANI eAttackAni)
{
	switch (eAttackAni)
	{
	case Client::CPlayer::CutHead:
		return m_bIsSkillAvailable[0];
		break;

	case Client::CPlayer::CuttingSlash:
		return m_bIsSkillAvailable[1];
		break;

	case Client::CPlayer::FlatBlade:
		return m_bIsSkillAvailable[2];
		break;

	case Client::CPlayer::HandySlash:
		return m_bIsSkillAvailable[3];
		break;

	case Client::CPlayer::JawBreaker:
		return m_bIsSkillAvailable[4];
		break;

	case Client::CPlayer::StingerBlade:
		return m_bIsSkillAvailable[5];
		break;

	case Client::CPlayer::RagingStrike:
		return m_bIsSkillAvailable[6];
		break;

	case Client::CPlayer::DrawSword:
		return m_bIsSkillAvailable[7];
		break;

	case Client::CPlayer::Tumbling:
		return m_bIsSkillAvailable[8];
		break;
	}

	return 0;
}

HRESULT CPlayer::Set_Navigation_Component(SCENEID eScene)
{
	// For.Com_Navigation
	// 씬 별 네비게이션 컴포넌트 셋팅 부분.
	_uint		iIndex = 0;

	switch (eScene)
	{
	case SCENE_STAGE:
		iIndex = 0;
		if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Navigation_Stage", L"Com_Navigation_Stage", (CComponent**)&m_pNavigationCom, &iIndex)))
			return E_FAIL;
		break;

	case SCENE_DRAGON:
		iIndex = 0;
		if (FAILED(CGameObject::Add_Component(SCENE_DRAGON, L"Component_Navigation_Dragon", L"Com_Navigation_Dragon", (CComponent**)&m_pNavigationCom, &iIndex)))
			return E_FAIL;
		break;

	case SCENE_BOSS:
		break;
	}

	Safe_Release(m_pNavigationCom);

	m_pMeshCom_Bone->SetUp_AnimationSet(Idle);

	return NOERROR;
}

void CPlayer::Set_CoolTimeAvailable(PLAYER_ANI eAttackAni, _bool bButton)
{
	switch (eAttackAni)
	{
	case Client::CPlayer::CutHead:
		m_bIsCoolTimeAvailable[0] = bButton;
		break;

	case Client::CPlayer::CuttingSlash:
		m_bIsCoolTimeAvailable[1] = bButton;
		break;

	case Client::CPlayer::FlatBlade:
		m_bIsCoolTimeAvailable[2] = bButton;
		break;

	case Client::CPlayer::HandySlash:
		m_bIsCoolTimeAvailable[3] = bButton;
		break;

	case Client::CPlayer::JawBreaker:
		m_bIsCoolTimeAvailable[4] = bButton;
		break;

	case Client::CPlayer::StingerBlade:
		m_bIsCoolTimeAvailable[5] = bButton;
		break;

	case Client::CPlayer::RagingStrike:
		m_bIsCoolTimeAvailable[6] = bButton;
		break;

	case Client::CPlayer::DrawSword:
		m_bIsCoolTimeAvailable[7] = bButton;
		break;

	case Client::CPlayer::Tumbling:
		m_bIsCoolTimeAvailable[8] = bButton;
		break;
	}
}

void CPlayer::Set_CoolTimeFree(PLAYER_ANI eAttackAni)
{
	switch (eAttackAni)
	{
	case Client::CPlayer::CutHead:
		m_fCurrentCoolTime[0] = m_fMaxCoolTime[0];
		break;

	case Client::CPlayer::CuttingSlash:
		m_fCurrentCoolTime[1] = m_fMaxCoolTime[1];
		break;

	case Client::CPlayer::FlatBlade:
		m_fCurrentCoolTime[2] = m_fMaxCoolTime[2];
		break;

	case Client::CPlayer::HandySlash:
		m_fCurrentCoolTime[3] = m_fMaxCoolTime[3];
		break;

	case Client::CPlayer::JawBreaker:
		m_fCurrentCoolTime[4] = m_fMaxCoolTime[4];
		break;

	case Client::CPlayer::StingerBlade:
		m_fCurrentCoolTime[5] = m_fMaxCoolTime[5];
		break;

	case Client::CPlayer::RagingStrike:
		m_fCurrentCoolTime[6] = m_fMaxCoolTime[6];
		break;

	case Client::CPlayer::DrawSword:
		m_fCurrentCoolTime[7] = m_fMaxCoolTime[7];
		break;

	case Client::CPlayer::Tumbling:
		m_fCurrentCoolTime[8] = m_fMaxCoolTime[8];
		break;
	}
}

void CPlayer::Set_SkillAvailable(PLAYER_ANI eAttackAni, _bool bButton)
{
	switch (eAttackAni)
	{
	case Client::CPlayer::CutHead:
		m_bIsSkillAvailable[0] = bButton;
		break;

	case Client::CPlayer::CuttingSlash:
		m_bIsSkillAvailable[1] = bButton;
		break;

	case Client::CPlayer::FlatBlade:
		m_bIsSkillAvailable[2] = bButton;
		break;

	case Client::CPlayer::HandySlash:
		m_bIsSkillAvailable[3] = bButton;
		break;

	case Client::CPlayer::JawBreaker:
		m_bIsSkillAvailable[4] = bButton;
		break;

	case Client::CPlayer::StingerBlade:
		m_bIsSkillAvailable[5] = bButton;
		break;

	case Client::CPlayer::RagingStrike:
		m_bIsSkillAvailable[6] = bButton;
		break;

	case Client::CPlayer::DrawSword:
		m_bIsSkillAvailable[7] = bButton;
		break;

	case Client::CPlayer::Tumbling:
		m_bIsSkillAvailable[8] = bButton;
		break;
	}
}

HRESULT CPlayer::Ready_GameObject_Prototype()
{
	if (FAILED(CGameObject::Ready_GameObject_Prototype()))
		return E_FAIL;

	return NOERROR;
}

HRESULT CPlayer::Ready_GameObject(void* pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pMeshCom_Bone->Set_HeadFrame(m_pMeshCom_Head->GetRootFrame());
	m_pMeshCom_Bone->Set_BodyFrame(m_pMeshCom_Body->GetRootFrame());
	m_pMeshCom_Bone->Set_HandFrame(m_pMeshCom_Hand->GetRootFrame());
	m_pMeshCom_Bone->Set_LegFrame(m_pMeshCom_Leg->GetRootFrame());
	m_pMeshCom_Bone->Set_TailFrame(m_pMeshCom_Tail->GetRootFrame());

	m_pTransformMoveCom->Set_Scaling(PLAYER_SCALING, PLAYER_SCALING, PLAYER_SCALING);
	m_pTransformMoveCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(12.f, 0.f, 233.f));

	//	m_pTransformMoveCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(100.f, 0.f, 100.f));

	m_pMeshCom_Bone->SetUp_AnimationSet(Idle);

	int iIdleState = 1;
	m_pState = CPlayer_Idle::Create(m_pGraphic_Device, *this, &iIdleState);

	m_pMeshCom_Bone->ChangePivot(_vec3(0.f, 1.f, 0.f), 0);
	
	CEventManager::GetInstance()->Register_Object(L"Arkus_Attack", this);

	m_pArkus = dynamic_cast<CArkus*>(CObject_Manager::GetInstance()->Get_Object(SCENE_DRAGON, L"Layer_Monster"));
	Safe_AddRef(m_pArkus);
	
	return NOERROR;
}

_int CPlayer::Update_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pTransformCom)
		return -1;

	m_fTimeDelta = fTimeDelta;

	AutoHealing(fTimeDelta);
	KeyInput();
	CollisionCheck();
	DecreaseSkillCoolTime();

	if (CInput_Device::GetInstance()->Get_DIKeyDown(DIK_N))
		m_bIsNavigationRender = !m_bIsNavigationRender;

	m_fPlayerPosY = m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION)->y;
	m_pTransformCom->Set_WorldMatrix((*m_pTransformRotateCom->Get_WorldMatrixPointer()) * (*m_pTransformMoveCom->Get_WorldMatrixPointer()));

	// 네비메쉬 타는거는 서식때문에 반드시 맨 밑에 위치해야 한다.
	Compute_HeightOnNavi();	

	//// 이 부분은 추후에 인벤토리 구현 시 참고할 구문이다.

	//if (CInput_Device::GetInstance()->Get_DIKeyDown(DIK_8))
	//{
	//	if (m_bTest == false)
	//	{
	//		Safe_Release(m_pMeshCom_Body);
	//		m_pMeshCom_Body = (CMesh_Dynamic_Parts*)m_pComponent_Manager->Clone_Component(SCENE_STATIC, L"Component_Mesh_Player_Body2");
	//		m_pMeshCom_Bone->Set_BodyFrame(m_pMeshCom_Body->GetRootFrame());
	//	}
	//	else
	//	{
	//		Safe_Release(m_pMeshCom_Body);
	//		m_pMeshCom_Body = (CMesh_Dynamic_Parts*)m_pComponent_Manager->Clone_Component(SCENE_STATIC, L"Component_Mesh_Player_Body");
	//		m_pMeshCom_Bone->Set_BodyFrame(m_pMeshCom_Body->GetRootFrame());
	//	}
	//	m_bTest = !m_bTest;
	//}

	return _int();
}

_int CPlayer::LateUpdate_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONEALPHA, this)))
		return -1;

	return _int();
}

HRESULT CPlayer::Render_GameObject()
{
	if (CManagement::GetInstance()->Get_CurrentScene() != SCENE_LOADING)
	{
		if (nullptr == m_pShaderCom ||
			nullptr == m_pTransformCom ||
			nullptr == m_pMeshCom_Body)
			return E_FAIL;

		m_pMeshCom_Bone->Play_Animation(m_fTimeDelta, m_fAniSpeed);		// 몬스터 할 때 이 위치 기억해라!!

		LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
		if (nullptr == pEffect)
			return E_FAIL;

		pEffect->AddRef();

		if (FAILED(SetUp_ConstantTable(pEffect)))
			return E_FAIL;

		pEffect->Begin(nullptr, 0);

		// 이 포문이 파츠 개수만큼 추가 되어야 함
		// [Head]
		for (size_t i = 0; i < m_pMeshCom_Head->Get_NumMeshContainer(); ++i)
		{
			if (FAILED(m_pMeshCom_Head->Update_SkinnedMesh(i)))
				break;

			for (size_t j = 0; j < m_pMeshCom_Head->Get_NumSubSet(i); ++j)
			{
				if (FAILED(m_pMeshCom_Head->SetTexture_OnShader(pEffect, i, j, "g_BaseTexture", MESHTEXTURE::TYPE_DIFFUSE)))
					return E_FAIL;

				pEffect->CommitChanges();

				pEffect->BeginPass(0);

				m_pMeshCom_Head->Render_Mesh(i, j);

				pEffect->EndPass();
			}
		}
		// [Body]
		for (size_t i = 0; i < m_pMeshCom_Body->Get_NumMeshContainer(); ++i)
		{
			if (FAILED(m_pMeshCom_Body->Update_SkinnedMesh(i)))
				break;

			for (size_t j = 0; j < m_pMeshCom_Body->Get_NumSubSet(i); ++j)
			{
				if (FAILED(m_pMeshCom_Body->SetTexture_OnShader(pEffect, i, j, "g_BaseTexture", MESHTEXTURE::TYPE_DIFFUSE)))
					return E_FAIL;

				pEffect->CommitChanges();

				pEffect->BeginPass(0);

				m_pMeshCom_Body->Render_Mesh(i, j);

				pEffect->EndPass();
			}
		}
		// [Hand]
		for (size_t i = 0; i < m_pMeshCom_Hand->Get_NumMeshContainer(); ++i)
		{
			if (FAILED(m_pMeshCom_Hand->Update_SkinnedMesh(i)))
				break;

			for (size_t j = 0; j < m_pMeshCom_Hand->Get_NumSubSet(i); ++j)
			{
				if (FAILED(m_pMeshCom_Hand->SetTexture_OnShader(pEffect, i, j, "g_BaseTexture", MESHTEXTURE::TYPE_DIFFUSE)))
					return E_FAIL;

				pEffect->CommitChanges();

				pEffect->BeginPass(0);

				m_pMeshCom_Hand->Render_Mesh(i, j);

				pEffect->EndPass();
			}
		}
		// [Leg]
		for (size_t i = 0; i < m_pMeshCom_Leg->Get_NumMeshContainer(); ++i)
		{
			if (FAILED(m_pMeshCom_Leg->Update_SkinnedMesh(i)))
				break;

			for (size_t j = 0; j < m_pMeshCom_Leg->Get_NumSubSet(i); ++j)
			{
				if (FAILED(m_pMeshCom_Leg->SetTexture_OnShader(pEffect, i, j, "g_BaseTexture", MESHTEXTURE::TYPE_DIFFUSE)))
					return E_FAIL;

				pEffect->CommitChanges();

				pEffect->BeginPass(0);

				m_pMeshCom_Leg->Render_Mesh(i, j);

				pEffect->EndPass();
			}
		}
		// [Tail]
		for (size_t i = 0; i < m_pMeshCom_Tail->Get_NumMeshContainer(); ++i)
		{
			if (FAILED(m_pMeshCom_Tail->Update_SkinnedMesh(i)))
				break;

			for (size_t j = 0; j < m_pMeshCom_Tail->Get_NumSubSet(i); ++j)
			{
				if (FAILED(m_pMeshCom_Tail->SetTexture_OnShader(pEffect, i, j, "g_BaseTexture", MESHTEXTURE::TYPE_DIFFUSE)))
					return E_FAIL;

				pEffect->CommitChanges();

				pEffect->BeginPass(0);

				m_pMeshCom_Tail->Render_Mesh(i, j);

				pEffect->EndPass();
			}
		}

		pEffect->End();

		Safe_Release(pEffect);

		// 콜라이더 렌더
		m_pColliderCom->Render_Collider();

		// 네비메쉬 렌더
		if (m_bIsNavigationRender == true)
			m_pNavigationCom->Render_Navigation();
	}

	return NOERROR;
}

HRESULT CPlayer::OnEvent(const _tchar * _szEventTag, void * _pMsg)
{
	if (!lstrcmp(_szEventTag, L"Arkus_Attack"))
	{
		if (m_fHP <= 0.f)
			m_fHP = 0.f;
		else 
		{
			if (m_pMeshCom_Bone->Get_NowPlayAniIndex() != PLAYER_ANI::Tumbling)
			{
				_float fDamage = 0.f;

				if ((*(_uint*)(_pMsg) == CArkus::ARKUS_ANI::Atk01))
					fDamage = 500.f;

				else if ((*(_uint*)(_pMsg) == CArkus::ARKUS_ANI::HeavyAtk02))
					fDamage = 700.f;

				else if ((*(_uint*)(_pMsg) == CArkus::ARKUS_ANI::FlyAtk02End))
					fDamage = 900.f;

				else if ((*(_uint*)(_pMsg) == CArkus::ARKUS_ANI::MoveAtkEnd))
					fDamage = 1300.f;

				// [ HP 조절 구문 ] //
				// 현재 체력을 상회하는 데미지가 들어오면,
				// 그냥 현제 체력을 0으로 만들어버린다.
				if ((m_fHP - fDamage) <= 0.f)
					m_fHP = 0.f;
				else // 아니라면 그냥 들어온 데미지만큼 현제 체력을 깎는다.
					m_fHP -= fDamage;
			}
		}
	}

	return NOERROR;
}

void CPlayer::DamageEvent(_float fSpeed)
{	
	if (m_pMeshCom_Bone->Get_NowPlayAniIndex() != PLAYER_ANI::Tumbling &&
		m_pMeshCom_Bone->Get_NowPlayAniIndex() != PLAYER_ANI::DrawSwordCharge &&
		m_pMeshCom_Bone->Get_NowPlayAniIndex() != PLAYER_ANI::DrawSwordLoop &&
		m_pMeshCom_Bone->Get_NowPlayAniIndex() != PLAYER_ANI::DrawSwordMove &&
		m_pMeshCom_Bone->Get_NowPlayAniIndex() != PLAYER_ANI::DrawSword &&
		m_pMeshCom_Bone->Get_NowPlayAniIndex() != PLAYER_ANI::DrawSwordEnd)
	{

	//	CArkus* pArkus = dynamic_cast<CArkus*>(CObject_Manager::GetInstance()->Get_Object(SCENE_DRAGON, L"Layer_Monster"));
		_vec3 vArkusPosition = *m_pArkus->Get_Transform()->Get_StateInfo(CTransform::STATE_POSITION);

		_vec3 vDir = (*m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION)) - vArkusPosition;

		D3DXVec3Normalize(&vDir, &vDir);

		_uint		iCellIndx = 0;
		if (true == m_pNavigationCom->Move_OnNavigation(m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION), &vDir, fSpeed * m_fTimeDelta, &iCellIndx))
		{
			m_pTransformMoveCom->Move(&vDir, fSpeed, m_fTimeDelta);

			/* ※※※※※※※진짜 이동하면 꼭 호출해야합니다※※※※※※.*/
			m_pNavigationCom->SetUp_CurrentIndex(iCellIndx);

			if ((m_fHP - 15.f) <= 0.f)
				m_fHP = 0.f;
			else // 아니라면 그냥 들어온 데미지만큼 현제 체력을 깎는다.
				m_fHP -= 15.f;
		}
	}
}

HRESULT CPlayer::Add_Component()
{
	// For.Com_Transform
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	// For.Com_TransformRotation
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_TransformRotation", (CComponent**)&m_pTransformRotateCom)))
		return E_FAIL;

	// For.Com_TransformMove
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_TransformMove", (CComponent**)&m_pTransformMoveCom)))
		return E_FAIL;

	// [ 플레이어 메쉬 추가 ]
	// For.Com_Mesh
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Mesh_Player_Bone", L"Com_Mesh_Bone", (CComponent**)&m_pMeshCom_Bone)))
		return E_FAIL;

	// For.Com_Mesh_Head
	m_pMeshCom_Head = (CMesh_Dynamic_Parts*)m_pComponent_Manager->Clone_Component(SCENE_STATIC, L"Component_Mesh_Player_Head03");
	if (nullptr == m_pMeshCom_Head)
		return E_FAIL;

	// For.Com_Mesh_Body
	m_pMeshCom_Body = (CMesh_Dynamic_Parts*)m_pComponent_Manager->Clone_Component(SCENE_STATIC, L"Component_Mesh_Player_BodyR29");
	if (nullptr == m_pMeshCom_Body)
		return E_FAIL;

	// For.Com_Mesh_Hand
	m_pMeshCom_Hand = (CMesh_Dynamic_Parts*)m_pComponent_Manager->Clone_Component(SCENE_STATIC, L"Component_Mesh_Player_HandR29");
	if (nullptr == m_pMeshCom_Hand)
		return E_FAIL;

	// For.Com_Mesh_Leg
	m_pMeshCom_Leg = (CMesh_Dynamic_Parts*)m_pComponent_Manager->Clone_Component(SCENE_STATIC, L"Component_Mesh_Player_LegR29");
	if (nullptr == m_pMeshCom_Leg)
		return E_FAIL;

	// For.Com_Mesh_Tail
	m_pMeshCom_Tail = (CMesh_Dynamic_Parts*)m_pComponent_Manager->Clone_Component(SCENE_STATIC, L"Component_Mesh_Player_Tail03");
	if (nullptr == m_pMeshCom_Tail)
		return E_FAIL;

	// For.Com_Renderer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	// For.Com_Shader
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Shader_Mesh", L"Com_Shader", (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	_float fColliderScale = 12.f;
	// For.Com_PlayerCollider
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Collider_Sphere", L"Com_Player_Collider", (CComponent**)&m_pColliderCom,
		&CCollider::COLLIDERDESC(CCollider::COLLIDERDESC::TYPE_TRANSFORM, m_pTransformCom->Get_WorldMatrixPointer(), nullptr,
			_vec3(fColliderScale, fColliderScale, fColliderScale), _vec3(0.0f, 5.f, 0.f)))))
		return E_FAIL;

	return NOERROR;
}

HRESULT CPlayer::SetUp_HeightOnTerrain()
{
	CObject_Manager*	pObject_Manager = CObject_Manager::GetInstance();

	if (nullptr == pObject_Manager)
		return E_FAIL;
	pObject_Manager->AddRef();

	CBuffer_Terrain* pBufferCom = (CBuffer_Terrain*)pObject_Manager->Get_Component(SCENE_STAGE, L"Layer_BackGround", L"Com_Buffer", 0);
	if (nullptr == pBufferCom)
		return E_FAIL;

	// 플레이어의 Y값과 이 지형의 Y값을 비교해서, 점프를 구현하면 된다.
	_float	fY = pBufferCom->Compute_HeightOnBuffer(m_pTransformCom);

	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION)->x, fY, m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION)->z));

	Safe_Release(pObject_Manager);

	return NOERROR;
}

HRESULT CPlayer::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	if (nullptr == pEffect)
		return E_FAIL;

	pEffect->AddRef();

	pEffect->SetMatrix("g_matWorld", m_pTransformCom->Get_WorldMatrixPointer());
	pEffect->SetMatrix("g_matView", &CGameObject::Get_Transform(D3DTS_VIEW));
	pEffect->SetMatrix("g_matProj", &CGameObject::Get_Transform(D3DTS_PROJECTION));

	Safe_Release(pEffect);

	return NOERROR;

	// Old
	{
		//if (nullptr == pEffect)
		//	return E_FAIL;

		//pEffect->AddRef();

		//pEffect->SetMatrix("g_matWorld", m_pTransformCom->Get_WorldMatrixPointer());
		//pEffect->SetMatrix("g_matView", &CGameObject::Get_Transform(D3DTS_VIEW));
		//pEffect->SetMatrix("g_matProj", &CGameObject::Get_Transform(D3DTS_PROJECTION));

		//CLight_Manager*	pLight_Manager = CLight_Manager::GetInstance();
		//if (nullptr == pLight_Manager)
		//	return E_FAIL;

		//pLight_Manager->AddRef();

		//const D3DLIGHT9* pLightInfo = pLight_Manager->Get_LightInfo(0);
		//if (nullptr == pLightInfo)
		//	return E_FAIL;

		//pEffect->SetVector("g_vLightDir", &_vec4(pLightInfo->Direction, 0.f));
		//pEffect->SetVector("g_vLightDiffuse", (_vec4*)&pLightInfo->Diffuse);
		//pEffect->SetVector("g_vLightAmbient", (_vec4*)&pLightInfo->Ambient);
		//pEffect->SetVector("g_vLightSpecular", (_vec4*)&pLightInfo->Specular);

		//Safe_Release(pLight_Manager);

		//_matrix		matView = CGameObject::Get_Transform(D3DTS_VIEW);
		//D3DXMatrixInverse(&matView, nullptr, &matView);

		//pEffect->SetVector("g_vCamPosition", (_vec4*)&matView.m[3][0]);

		//Safe_Release(pEffect);
	}

	return NOERROR;
}

void CPlayer::ViewChanage()
{
	// 마우스 커서를 기준으로 플레이어의 뷰를 변환한다 //
	// Feat.형진이
	CTransform*	vCameraTransformCom = ((CTransform*)(CObject_Manager::GetInstance()->Get_Component(SCENE_STAGE, L"Layer_Camera", L"Com_Transform", 1)));
	_vec3 vCameraLook = *vCameraTransformCom->Get_StateInfo(CTransform::STATE_LOOK);
	
	_vec3	vPlayerRight = *m_pTransformCom->Get_StateInfo(CTransform::STATE_RIGHT);
	_vec3	vPlayerUp = *m_pTransformCom->Get_StateInfo(CTransform::STATE_UP);
	_vec3	vPlayerLook = *m_pTransformCom->Get_StateInfo(CTransform::STATE_LOOK);
	_float	fPlayerRightSize = D3DXVec3Length(&vPlayerRight);
	_float	fPlayerLookSize = D3DXVec3Length(&vPlayerLook);

	vCameraLook.y = 0.f;
	D3DXVec3Normalize(&vCameraLook, &vCameraLook);

	m_pTransformCom->Set_StateInfo(CTransform::STATE_LOOK, &(vCameraLook * fPlayerLookSize));

	_vec3 vPlayerNewRight;
	D3DXVec3Cross(&vPlayerNewRight, &vPlayerUp, &vCameraLook);

	D3DXVec3Normalize(&vPlayerNewRight, &vPlayerNewRight);

	m_pTransformCom->Set_StateInfo(CTransform::STATE_RIGHT, &(vPlayerNewRight * fPlayerRightSize));

}

void CPlayer::KeyInput()
{
	// NPC와 대화중일땐 아무고또 모타죠? //
	CUI_Dialog* pUI_Dialog = dynamic_cast<CUI_Dialog*>(CObject_Manager::GetInstance()->Get_Object(SCENE_STAGE, L"Layer_UI", 1));

	if (pUI_Dialog->Get_TalkEventAvaliable() == true)
		return;
	///////////////////////////////////////

	CPlayerState* pState = m_pState->Input_Keyboard(*this, m_fTimeDelta, 0, m_pNavigationCom);

	if (nullptr != pState)
	{
		Safe_Release(m_pState);
		m_pState = pState;
	}

	if (true == m_pMeshCom_Bone->IsAnimationEnded() || m_eOldAnimationIndex != m_eAnimationIndex)
	{
		m_pMeshCom_Bone->SetUp_AnimationSet(m_eAnimationIndex);
	}

	m_eOldAnimationIndex = m_eAnimationIndex;

	m_pState->Update_State(*this, m_fTimeDelta);

}

void CPlayer::Compute_HeightOnNavi()
{
	_vec3	pPlayerPos = *m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION);
	
	m_pNavigationCom->SetUp_CurrentIndexLoop(pPlayerPos);
	
	m_fNaviDistValue = m_pNavigationCom->Compute_HeightOnNavi(&pPlayerPos);
	
	pPlayerPos.y = m_fNaviDistValue;

	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &pPlayerPos);
}

void CPlayer::AutoHealing(const _float& fTimeDelta)
{
	if (m_pMeshCom_Bone->Get_NowPlayAniIndex() == PLAYER_ANI::Idle_Battle ||
		m_pMeshCom_Bone->Get_NowPlayAniIndex() == PLAYER_ANI::SitLoop)
	{
		m_fAutoHealingAccTime += fTimeDelta;

		if (m_fAutoHealingAccTime > 4.f)
		{
			Set_HP_Add(PLAYER_HP * 5 / 100);
			Set_MP_Add(PLAYER_MP * 5 / 100);
			m_fAutoHealingAccTime = 0.f;
		}
	}
}

_bool CPlayer::CollisionCheck()
{
	if (CManagement::GetInstance()->Get_CurrentScene() == SCENE_DRAGON)
	{
		//CArkus*	pArkus = dynamic_cast<CArkus*>(CObject_Manager::GetInstance()->Get_Object(SCENE_DRAGON, L"Layer_Monster"));
		if (m_pArkus == nullptr)
			m_pArkus = dynamic_cast<CArkus*>(CObject_Manager::GetInstance()->Get_Object(SCENE_DRAGON, L"Layer_Monster"));

		if (m_pArkus->EnemyPositionCheck() == true)
		{
			const CCollider* pArkusColliderBody = (const CCollider*)CObject_Manager::GetInstance()->Get_Component(SCENE_DRAGON, L"Layer_Monster", L"Com_Collider_Arkus_Body");
			const CCollider* pArkusColliderHead = (const CCollider*)CObject_Manager::GetInstance()->Get_Component(SCENE_DRAGON, L"Layer_Monster", L"Com_Collider_Arkus_Head");
			const CCollider* pArkusColliderNeck = (const CCollider*)CObject_Manager::GetInstance()->Get_Component(SCENE_DRAGON, L"Layer_Monster", L"Com_Collider_Arkus_Neck");
			const CCollider* pArkusColliderTail01 = (const CCollider*)CObject_Manager::GetInstance()->Get_Component(SCENE_DRAGON, L"Layer_Monster", L"Com_Collider_Arkus_Tail01");
			const CCollider* pArkusColliderTail02 = (const CCollider*)CObject_Manager::GetInstance()->Get_Component(SCENE_DRAGON, L"Layer_Monster", L"Com_Collider_Arkus_Tail02");

			if (m_pArkus->Get_AniIndex() != CArkus::ARKUS_ANI::Idle &&
				m_pArkus->Get_AniIndex() != CArkus::ARKUS_ANI::Hit &&
				m_pArkus->Get_AniIndex() != CArkus::ARKUS_ANI::Run_Battle &&
				m_pArkus->Get_AniIndex() != CArkus::ARKUS_ANI::JumpEvasion &&
				m_pArkus->Get_AniIndex() != CArkus::ARKUS_ANI::Groggy &&
				m_pArkus->Get_AniIndex() != CArkus::ARKUS_ANI::Death)
			{
				if (m_pColliderCom->Collision_Sphere(pArkusColliderBody) == true ||
					m_pColliderCom->Collision_Sphere(pArkusColliderHead) == true ||
					m_pColliderCom->Collision_Sphere(pArkusColliderNeck) == true ||
					m_pColliderCom->Collision_Sphere(pArkusColliderTail01) == true ||
					m_pColliderCom->Collision_Sphere(pArkusColliderTail02) == true)
				{
					m_bCollisionCheck = true;
				}
				else
					m_bCollisionCheck = false;
			}

			_uint	iCellIndx = 0;
			if (m_bCollisionCheck == true)
			{
				_float fRadius = m_pColliderCom->Get_Radius();
				_vec3 vInverseView = (*m_pColliderCom->Get_LookDistance());
				vInverseView.y = 0;

				if (true == m_pNavigationCom->Move_OnNavigation(m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION), &vInverseView, fRadius * m_fTimeDelta, &iCellIndx))
				{
					m_pTransformMoveCom->Move(&vInverseView, fRadius, m_fTimeDelta);

					/* ※※※※※※※진짜 이동하면 꼭 호출해야합니다※※※※※※.*/
					m_pNavigationCom->SetUp_CurrentIndex(iCellIndx);
				}
			}
		}
	}

	if (m_bCollisionCheck == true)
	{
		// 이걸 안해주면 계속 밀린다 ㅈ된다.
		m_bCollisionCheck = false;
		return true;
	}

	else if (m_bCollisionCheck == false)
		return false;

	return false;
}

void CPlayer::DamageCalculator(PLAYER_ANI eAttackAni)
{
	srand((unsigned)time(NULL));

	switch (eAttackAni)
	{
	case Client::CPlayer::Combo1:
		m_iOffencePower = rand() % 71 + 330;
		break;

	case Client::CPlayer::Combo2:
		m_iOffencePower = rand() % 51 + 350;
		break;

	case Client::CPlayer::Combo3:
		m_iOffencePower = rand() % 61 + 360;
		break;
		
	case Client::CPlayer::Combo4:
		m_iOffencePower = rand() % 71 + 320;
		break;

	case Client::CPlayer::CutHead:
		m_iOffencePower = rand() % 1601 + 4150;

		if (m_iOffencePower >= 5250 && m_iOffencePower <= 5750)
			m_bIsCriticalDamage[0] = true;
		break;

	case Client::CPlayer::CuttingSlash:
		m_iOffencePower = rand() % 1351 + 3850;

		if (m_iOffencePower >= 4700 && m_iOffencePower <= 5200)
			m_bIsCriticalDamage[1] = true;
		break;

	case Client::CPlayer::FlatBlade:
		m_iOffencePower = rand() % 1051 + 1500;

		if (m_iOffencePower >= 2050 && m_iOffencePower <= 2550)
			m_bIsCriticalDamage[2] = true;
		break;

	case Client::CPlayer::HandySlash:
		m_iOffencePower = rand() % 1651 + 4300;

		if (m_iOffencePower >= 5450 && m_iOffencePower <= 5950)
			m_bIsCriticalDamage[3]= true;
		break;

	case Client::CPlayer::JawBreaker:
		m_iOffencePower = rand() % 901 + 2900;

		if (m_iOffencePower >= 3300 && m_iOffencePower <= 3800)
			m_bIsCriticalDamage[4]= true;
		break;

	case Client::CPlayer::StingerBlade:
		m_iOffencePower = rand() % 1851 + 3850;

		if (m_iOffencePower >= 5200 && m_iOffencePower <= 5700)
			m_bIsCriticalDamage[5]= true;
		break;

	case Client::CPlayer::RagingStrike:
		m_iOffencePower = rand() % 851 + 1850;

		if (m_iOffencePower >= 2200 && m_iOffencePower <= 2700)
			m_bIsCriticalDamage[6]= true;
		break;

	case Client::CPlayer::DrawSword:
		m_iOffencePower = rand() % 1601 + 4500;

		if (m_iOffencePower >= 5600 && m_iOffencePower <= 6100)
			m_bIsCriticalDamage[7]= true;
		break;
	}

}

void CPlayer::DecreaseSkillCoolTime()
{
	if (Get_CoolTimeAvailable(CPlayer::PLAYER_ANI::CutHead) == false)
	{
		if (m_fCurrentCoolTime[0] > 0)
			m_fCurrentCoolTime[0] -= m_fTimeDelta;

		else if (m_fCurrentCoolTime[0] <= 0)
		{
			Set_CoolTimeFree(CPlayer::PLAYER_ANI::CutHead);
			Set_CoolTimeAvailable(CPlayer::PLAYER_ANI::CutHead, true);
		}
	}
	if (Get_CoolTimeAvailable(CPlayer::PLAYER_ANI::CuttingSlash) == false)
	{
		if (m_fCurrentCoolTime[1] > 0)
			m_fCurrentCoolTime[1] -= m_fTimeDelta;

		else if (m_fCurrentCoolTime[1] <= 0)
		{
			Set_CoolTimeFree(CPlayer::PLAYER_ANI::CuttingSlash);
			Set_CoolTimeAvailable(CPlayer::PLAYER_ANI::CuttingSlash, true);
		}
	}
	if (Get_CoolTimeAvailable(CPlayer::PLAYER_ANI::FlatBlade) == false)
	{
		if (m_fCurrentCoolTime[2] > 0)
			m_fCurrentCoolTime[2] -= m_fTimeDelta;

		else if (m_fCurrentCoolTime[2] <= 0)
		{
			Set_CoolTimeFree(CPlayer::PLAYER_ANI::FlatBlade);
			Set_CoolTimeAvailable(CPlayer::PLAYER_ANI::FlatBlade, true);
		}
	}
	if (Get_CoolTimeAvailable(CPlayer::PLAYER_ANI::HandySlash) == false)
	{
		if (m_fCurrentCoolTime[3] > 0)
			m_fCurrentCoolTime[3] -= m_fTimeDelta;

		else if (m_fCurrentCoolTime[3] <= 0)
		{
			Set_CoolTimeFree(CPlayer::PLAYER_ANI::HandySlash);
			Set_CoolTimeAvailable(CPlayer::PLAYER_ANI::HandySlash, true);
		}
	}
	if (Get_CoolTimeAvailable(CPlayer::PLAYER_ANI::JawBreaker) == false)
	{
		if (m_fCurrentCoolTime[4] > 0)
			m_fCurrentCoolTime[4] -= m_fTimeDelta;

		else if (m_fCurrentCoolTime[4] <= 0)
		{
			Set_CoolTimeFree(CPlayer::PLAYER_ANI::JawBreaker);
			Set_CoolTimeAvailable(CPlayer::PLAYER_ANI::JawBreaker, true);
		}
	}
	if (Get_CoolTimeAvailable(CPlayer::PLAYER_ANI::StingerBlade) == false)
	{
		if (m_fCurrentCoolTime[5] > 0)
			m_fCurrentCoolTime[5] -= m_fTimeDelta;

		else if (m_fCurrentCoolTime[5] <= 0)
		{
			Set_CoolTimeFree(CPlayer::PLAYER_ANI::StingerBlade);
			Set_CoolTimeAvailable(CPlayer::PLAYER_ANI::StingerBlade, true);
		}
	}
	if (Get_CoolTimeAvailable(CPlayer::PLAYER_ANI::RagingStrike) == false)
	{
		if (m_fCurrentCoolTime[6] > 0)
			m_fCurrentCoolTime[6] -= m_fTimeDelta;

		else if (m_fCurrentCoolTime[6] <= 0)
		{
			Set_CoolTimeFree(CPlayer::PLAYER_ANI::RagingStrike);
			Set_CoolTimeAvailable(CPlayer::PLAYER_ANI::RagingStrike, true);
		}
	}
	if (Get_CoolTimeAvailable(CPlayer::PLAYER_ANI::DrawSword) == false)
	{
		if (m_fCurrentCoolTime[7] > 0)
			m_fCurrentCoolTime[7] -= m_fTimeDelta;

		else if (m_fCurrentCoolTime[7] <= 0)
		{
			Set_CoolTimeFree(CPlayer::PLAYER_ANI::DrawSword);
			Set_CoolTimeAvailable(CPlayer::PLAYER_ANI::DrawSword, true);
		}
	}
	if (Get_CoolTimeAvailable(CPlayer::PLAYER_ANI::Tumbling) == false)
	{
		if (m_fCurrentCoolTime[8] > 0)
			m_fCurrentCoolTime[8] -= m_fTimeDelta;

		else if (m_fCurrentCoolTime[8] <= 0)
		{
			Set_CoolTimeFree(CPlayer::PLAYER_ANI::Tumbling);
			Set_CoolTimeAvailable(CPlayer::PLAYER_ANI::Tumbling, true);
		}
	}
}

CPlayer * CPlayer::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPlayer* pInstance = new CPlayer(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		_MSGBOX("CPlayer Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CPlayer::Clone(void* pArg)
{
	CPlayer* pInstance = new CPlayer(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		_MSGBOX("CPlayer Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer::Free()
{
	CEventManager::GetInstance()->Remove_Object(L"Arkus_Attack", this);

	Safe_Release(m_pArkus);

	Safe_Release(m_pTransformRotateCom);
	Safe_Release(m_pTransformMoveCom);
	Safe_Release(m_pMeshCom_Tail);
	Safe_Release(m_pMeshCom_Leg);
	Safe_Release(m_pMeshCom_Hand);
	Safe_Release(m_pMeshCom_Body);
	Safe_Release(m_pMeshCom_Head);
	Safe_Release(m_pMeshCom_Bone);
	Safe_Release(m_pState);

	CUnit::Free();
}
