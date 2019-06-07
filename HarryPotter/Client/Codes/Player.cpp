#include "stdafx.h"
#include "..\Headers\Player.h"
#include "Object_Manager.h"
#include "Light_Manager.h"
#include "Player_Idle.h"
#include "Player_Move.h"

_USING(Client)

#define PLAYER_SPEED	5.f
#define	GRAVITY			9.8f
#define	JUMP_POWER		10.f

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUnit(pGraphic_Device)
//	, m_pKeyManager(CKeyManager::GetInstance())
{
//	Safe_AddRef(m_pKeyManager);
}

CPlayer::CPlayer(const CPlayer & rhs)
	: CUnit(rhs)
//	, m_pKeyManager(rhs.m_pKeyManager)
//	, m_isMove(rhs.m_isMove)
{
//	Safe_AddRef(m_pKeyManager);
}

// 원본객체에 필요한 데이터를 셋팅한다.
HRESULT CPlayer::Ready_GameObject_Prototype()
{
	// 파일입출력을 통해 객체의 정보를 셋팅한다.
	if (FAILED(CGameObject::Ready_GameObject_Prototype()))
		return E_FAIL;

	return NOERROR;
}

// 실제 씬에서 사용할 객체가 호출하는 함수.
// 원본객체 복제외에도 추가적인 셋팅이필요하면 여기서 셋팅해라.
HRESULT CPlayer::Ready_GameObject(void* pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pTransformCom->Set_Scaling(0.05f, 0.05f, 0.05f);
	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(0.2, 0.f, 0.2));
	m_pMeshCom->SetUp_AnimationSet(LUMOSSTRAFELEFT);

	m_pState = CPlayer_Idle::Create(m_pGraphic_Device, *this);

	//m_pMeshCom->ChangePivot(_vec3(1.f, 0.f, 0.f), -90);
	m_pMeshCom->ChangePivot(_vec3(0.f, 1.f, 0.f), 180);
	
	return NOERROR;
}

_int CPlayer::Update_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pTransformCom)
		return -1;

	m_fTimeDelta = fTimeDelta;

	m_fPlayerPosY = m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION)->y;

	ViewChanage();
	KeyInput();

	//m_fAniSpeed = 1.f;
	m_pMeshCom->Play_Animation(fTimeDelta, m_fAniSpeed);

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
	if (nullptr == m_pShaderCom ||
		nullptr == m_pTransformCom ||
		nullptr == m_pMeshCom)
		return E_FAIL;

	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	if (nullptr == pEffect)
		return E_FAIL;

	pEffect->AddRef();

	if (FAILED(SetUp_ConstantTable(pEffect)))
		return E_FAIL;

	pEffect->Begin(nullptr, 0);
	
	for (size_t i = 0; i < m_pMeshCom->Get_NumMeshContainer(); ++i)
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

	// 콜라이더 렌더
	m_pColliderCom->Render_Collider();

	return NOERROR;
}

//void CPlayer::handleInput(P_STATE eInput)
//{
//	CPlayerState* State = pState->HandleInput(*this, eInput);
//	
//	if (pState != NULL)
//	{
//		Safe_Delete(pState);
//		pState = State;
//	}
//	pState->Enter(*this);
//}

HRESULT CPlayer::Add_Component()
{
	// For.Com_Transform
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	// For.Com_Mesh
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Mesh_Player", L"Com_Mesh", (CComponent**)&m_pMeshCom)))
		return E_FAIL;

	// For.Com_Renderer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	// For.Com_Shader
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Shader_Mesh", L"Com_Shader", (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	// For.Com_BodyCollider
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Collider_OBB", L"Com_BodyCollider", (CComponent**)&m_pColliderCom,
		&CCollider::COLLIDERDESC(CCollider::COLLIDERDESC::TYPE_TRANSFORM, m_pTransformCom->Get_WorldMatrixPointer(), nullptr,
			_vec3(1.f, 2.f, 1.f), _vec3(0.0f, 1.f, 0.f)))))
		return E_FAIL;

	//// For.Com_HandCollider
	//if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Collider_Sphere", L"Com_HandCollider", (CComponent**)&m_pHandColliderCom, &CCollider::COLLIDERDESC(CCollider::COLLIDERDESC::TYPE_FRAME, m_pTransformCom->Get_WorldMatrixPointer(), &(m_pMeshCom->Get_FrameDesc("Body_rFingerMidTop")->CombinedTransformationMatrix)
	//	, _vec3(0.3f, 0.3f, 0.3f), _vec3(0.f, 0.f, 0.f)))))
	//	return E_FAIL;

	return NOERROR;
}

//HRESULT CPlayer::SetUp_HeightOnTerrain()
//{
//	CObject_Manager*	pObject_Manager = CObject_Manager::GetInstance();
//
//	if (nullptr == pObject_Manager)
//		return E_FAIL;
//	pObject_Manager->AddRef();
//
//	CBuffer_Terrain* pBufferCom = (CBuffer_Terrain*)pObject_Manager->Get_Component(SCENE_STAGE, L"Layer_BackGround", L"Com_Buffer", 0);
//	if (nullptr == pBufferCom)
//		return E_FAIL;
//
//	// 플레이어의 Y값과 이 지형의 Y값을 비교해서, 점프를 구현하면 된다.
//	_float	fY = pBufferCom->Compute_HeightOnBuffer(m_pTransformCom);
//
//	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION)->x, fY + 0.5f, m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION)->z));
//
//	Safe_Release(pObject_Manager);
//
//	return NOERROR;
//}

HRESULT CPlayer::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	if (nullptr == pEffect)
		return E_FAIL;

	pEffect->AddRef();

	pEffect->SetMatrix("g_matWorld", m_pTransformCom->Get_WorldMatrixPointer());
	pEffect->SetMatrix("g_matView", &CGameObject::Get_Transform(D3DTS_VIEW));
	pEffect->SetMatrix("g_matProj", &CGameObject::Get_Transform(D3DTS_PROJECTION));

	CLight_Manager*	pLight_Manager = CLight_Manager::GetInstance();
	if (nullptr == pLight_Manager)
		return E_FAIL;

	pLight_Manager->AddRef();

	const D3DLIGHT9* pLightInfo = pLight_Manager->Get_LightInfo(0);
	if (nullptr == pLightInfo)
		return E_FAIL;

	pEffect->SetVector("g_vLightDir", &_vec4(pLightInfo->Direction, 0.f));
	pEffect->SetVector("g_vLightDiffuse", (_vec4*)&pLightInfo->Diffuse);
	pEffect->SetVector("g_vLightAmbient", (_vec4*)&pLightInfo->Ambient);
	pEffect->SetVector("g_vLightSpecular", (_vec4*)&pLightInfo->Specular);

	Safe_Release(pLight_Manager);

	_matrix		matView = CGameObject::Get_Transform(D3DTS_VIEW);
	D3DXMatrixInverse(&matView, nullptr, &matView);

	pEffect->SetVector("g_vCamPosition", (_vec4*)&matView.m[3][0]);

	Safe_Release(pEffect);

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
	CPlayerState* pState = m_pState->Input_Keyboard(*this, m_fTimeDelta, 0);

	if (nullptr != pState)
	{
		Safe_Release(m_pState);
		m_pState = pState;
	}

	if (true == m_pMeshCom->IsAnimationEnded() || m_eOldAnimationIndex != m_eAnimationIndex)
	{
		m_pMeshCom->SetUp_AnimationSet(m_eAnimationIndex);
	}

	m_eOldAnimationIndex = m_eAnimationIndex;

	m_pState->Update_State(*this, m_fTimeDelta);

}

//void CPlayer::KeyInput()
//{
//	// 걷고 뛰기 토글 플래그.
//	if (m_pKeyManager->KeyDown(CKeyManager::KEY_R))
//		m_bIsRun = !m_bIsRun;
//
//	if (m_bIsJumped == false)
//	{
//		if (m_pKeyManager->KeyPressing(CKeyManager::KEY_W))
//		{
//			if (m_pKeyManager->KeyPressing(CKeyManager::KEY_LBUTTON))
//			{
//				m_pTransformCom->Move(0, PLAYER_SPEED, m_fTimeDelta);
//				m_dwPlayerState = STATE_LUMOSSTRAFEFORWARD;
//			}
//			else
//			{
//				if (m_bIsRun == false)
//				{
//					m_pTransformCom->Move(0, PLAYER_SPEED - 2, m_fTimeDelta);
//					m_dwPlayerState = STATE_WALK;
//				}
//				else if (m_bIsRun == true)
//				{
//					m_pTransformCom->Move(0, PLAYER_SPEED, m_fTimeDelta);
//					m_dwPlayerState = STATE_RUN;
//				}
//			}
//		}
//		else if (m_pKeyManager->KeyPressing(CKeyManager::KEY_S))
//		{
//			if (m_pKeyManager->KeyPressing(CKeyManager::KEY_LBUTTON))
//			{
//				m_pTransformCom->Move(1, PLAYER_SPEED, m_fTimeDelta);
//				m_dwPlayerState = STATE_LUMOSSTRAFEBACK;
//			}
//			else
//			{
//				if (m_bIsRun == false)
//				{
//					m_pTransformCom->Move(1, PLAYER_SPEED - 2, m_fTimeDelta);
//					m_dwPlayerState = STATE_WALK;
//				}
//				else if (m_bIsRun == true)
//				{
//					m_pTransformCom->Move(1, PLAYER_SPEED, m_fTimeDelta);
//					m_dwPlayerState = STATE_RUN;
//				}
//			}
//		}
//		else if (m_pKeyManager->KeyPressing(CKeyManager::KEY_A))
//		{
//			if (m_pKeyManager->KeyPressing(CKeyManager::KEY_LBUTTON))
//			{
//				m_pTransformCom->Move(2, PLAYER_SPEED, m_fTimeDelta);
//				m_dwPlayerState = STATE_LUMOSSTRAFELEFT;
//			}
//			else
//			{
//				m_pTransformCom->Move(2, PLAYER_SPEED, m_fTimeDelta);
//				m_dwPlayerState = STATE_RUN_LEFT;
//			}
//		}
//		else if (m_pKeyManager->KeyPressing(CKeyManager::KEY_D))
//		{
//			if (m_pKeyManager->KeyPressing(CKeyManager::KEY_LBUTTON))
//			{
//				m_pTransformCom->Move(3, PLAYER_SPEED, m_fTimeDelta);
//				m_dwPlayerState = STATE_LUMOSSTRAFERIGHT;
//			}
//			else
//			{
//				m_pTransformCom->Move(3, PLAYER_SPEED, m_fTimeDelta);
//				m_dwPlayerState = STATE_RUN_RIGHT;
//			}
//		}
//		else if (m_pKeyManager->KeyPressing(CKeyManager::KEY_LBUTTON))
//		{
//			m_dwPlayerState = STATE_CASTAIM;
//		}
//		else if (m_pKeyManager->KeyDown(CKeyManager::KEY_RBUTTON))
//		{
//			m_dwPlayerState = STATE_JUMP_START;
//			m_bIsJumped = true;
//		}
//		else
//		{
//			m_dwPlayerState = STATE_IDLE;
//			m_bIsJumped = false;
//		}
//	}
//	else if (m_bIsJumped == true)
//	{
//		if (m_pMeshCom->IsAnimationEnded() == false)
//		{
//			if (m_pKeyManager->KeyPressing(CKeyManager::KEY_W))
//			{
//				if (m_bIsRun == false)
//					m_pTransformCom->Move(0, PLAYER_SPEED - 2, m_fTimeDelta);
//				else if (m_bIsRun == true)
//					m_pTransformCom->Move(0, PLAYER_SPEED, m_fTimeDelta);
//			}
//			else if (m_pKeyManager->KeyPressing(CKeyManager::KEY_S))
//			{
//				if (m_bIsRun == false)
//					m_pTransformCom->Move(1, PLAYER_SPEED - 2, m_fTimeDelta);
//				else if (m_bIsRun == true)
//					m_pTransformCom->Move(1, PLAYER_SPEED, m_fTimeDelta);
//			}
//			else if (m_pKeyManager->KeyPressing(CKeyManager::KEY_A))
//			{
//				m_pTransformCom->Move(2, PLAYER_SPEED, m_fTimeDelta);
//			}
//			else if (m_pKeyManager->KeyPressing(CKeyManager::KEY_D))
//			{
//				m_pTransformCom->Move(3, PLAYER_SPEED, m_fTimeDelta);
//			}
//			else
//			{
//				if (m_dwPlayerState == STATE_JUMP_START)
//				{
//					if (m_pMeshCom->IsAnimationEnded(0.2f) == true)
//						m_dwPlayerState = STATE_JUMP;
//				}
//				else if (m_dwPlayerState == STATE_JUMP)
//				{
//					if (m_pMeshCom->IsAnimationEnded(0.2f) == true)
//						m_dwPlayerState = STATE_BUNDIJUMP1;
//				}
//				else if (m_dwPlayerState == STATE_BUNDIJUMP1)
//				{
//					if (m_pMeshCom->IsAnimationEnded(0.2f) == true)
//						m_bIsJumped = false;
//				}
//				else
//				{
//					m_dwPlayerState = STATE_IDLE;
//					m_bIsJumped = false;
//				}
//			}
//		}
//		else if (m_pMeshCom->IsAnimationEnded(0.2f) == true)
//		{
//			if (m_dwPlayerState == STATE_JUMP_START)
//				m_dwPlayerState = STATE_JUMP;
//			else if (m_dwPlayerState == STATE_JUMP)
//				m_dwPlayerState = STATE_JUMPLANDTORUN;
//			else if (m_dwPlayerState == STATE_JUMPLANDTORUN)
//				m_bIsJumped = false;
//		}
//	}
//
//
//	//if (m_pKeyManager->KeyPressing(CKeyManager::KEY_W))
//	//{
//	//	if (m_bIsJumped == true)
//	//	{
//	//		if (m_pMeshCom->IsAnimationEnded() == false)
//	//		{
//	//			if (m_bIsRun == false)
//	//				m_pTransformCom->Move(0, PLAYER_SPEED - 2, m_fTimeDelta);
//	//			else if (m_bIsRun == true)
//	//				m_pTransformCom->Move(0, PLAYER_SPEED, m_fTimeDelta);
//	//		}
//	//		else if (m_pMeshCom->IsAnimationEnded(0.2f) == true)
//	//		{
//	//			if (m_dwPlayerState == STATE_JUMP_START)
//	//				m_dwPlayerState = STATE_JUMP; 
//	//			else if (m_dwPlayerState == STATE_JUMP)
//	//				m_dwPlayerState = STATE_JUMPLANDTOSTAND;
//	//			else if (m_dwPlayerState == STATE_JUMPLANDTOSTAND)
//	//			{
//	//				m_bIsJumped = false;
//	//				if (m_bIsRun == false)
//	//					m_dwPlayerState = STATE_WALK;
//	//			}
//	//		}
//	//	}
//	//	else
//	//	{
//	//		if (m_bIsRun == false)
//	//		{
//	//			m_pTransformCom->Move(0, PLAYER_SPEED - 2, m_fTimeDelta);
//	//			m_dwPlayerState = STATE_WALK;
//	//		}
//	//		else if (m_bIsRun == true)
//	//		{
//	//			m_pTransformCom->Move(0, PLAYER_SPEED, m_fTimeDelta);
//	//			m_dwPlayerState = STATE_RUN;
//	//		}
//	//	}
//	//}
//	//else if (m_pKeyManager->KeyPressing(CKeyManager::KEY_S))
//	//{
//	//	m_pTransformCom->Move(1, PLAYER_SPEED, m_fTimeDelta);
//	//	m_dwPlayerState = STATE_RUN_BACK;
//	//}
//	//else if (m_pKeyManager->KeyPressing(CKeyManager::KEY_A))
//	//{
//	//	m_pTransformCom->Move(2, PLAYER_SPEED, m_fTimeDelta);
//	//	m_dwPlayerState = STATE_RUN_LEFT;
//	//}
//	//else if (m_pKeyManager->KeyPressing(CKeyManager::KEY_D))
//	//{
//	//	m_pTransformCom->Move(3, PLAYER_SPEED, m_fTimeDelta);
//	//	m_dwPlayerState = STATE_RUN_RIGHT;
//	//}
//	//else if (m_pKeyManager->KeyPressing(CKeyManager::KEY_LBUTTON))
//	//{
//	//	m_dwPlayerState = STATE_CASTAIM;
//	//}
//	//else if (m_pKeyManager->KeyDown(CKeyManager::KEY_RBUTTON))
//	//{
//	//	if (m_bIsJumped == false)
//	//	{
//	//		m_dwPlayerState = STATE_JUMP_START;
//	//		m_bIsJumped = true;
//	//	}
//	//}
//	//else
//	//{
//	//	if (m_dwPlayerState == STATE_JUMP_START)
//	//	{
//	//		if (m_pMeshCom->IsAnimationEnded(0.2f) == true)
//	//			m_dwPlayerState = STATE_JUMP;
//	//	}
//	//	else if (m_dwPlayerState == STATE_JUMP)
//	//	{
//	//		if (m_pMeshCom->IsAnimationEnded(0.2f) == true)
//	//			m_dwPlayerState = STATE_JUMPLANDTOSTAND;
//	//	}
//	//	else if (m_dwPlayerState == STATE_JUMPLANDTOSTAND)
//	//	{
//	//		if (m_pMeshCom->IsAnimationEnded(0.2f) == true)
//	//		{
//	//			m_dwPlayerState = STATE_IDLE;
//	//			m_bIsJumped = false;
//	//		}
//	//	}
//	//	// 마법 태그랑 충돌 불 변수가 True 면
//	//	// 여기서 애니메이션 제어를 한다.
//	//	//if (스테이트 == 마법)
//	//	//{
//	//	//	애니메이션이 종료되었어?
//	//	//	되었으면 밑에 구문 실행.
//	//	//}
//	//	// 아무키도 누르지 않으면 이 Else문에 최종적으로 들어오게 된다.
//	//	else
//	//	{
//	//		m_dwPlayerState = STATE_IDLE;
//	//		m_bIsJumped = false;
//	//	}
//	//}
//}

//void CPlayer::AniChange()
//{
//	switch (m_dwPlayerState)
//	{
//	case STATE_LUMOSSTRAFERIGHT:
//		m_pMeshCom->SetUp_AnimationSet(STATE_LUMOSSTRAFERIGHT);
//		break;
//	case STATE_LUMOSSTRAFELEFT:
//		m_pMeshCom->SetUp_AnimationSet(STATE_LUMOSSTRAFELEFT);
//		break;
//	case STATE_LUMOSSTRAFEFORWARD:
//		m_pMeshCom->SetUp_AnimationSet(STATE_LUMOSSTRAFEFORWARD, 1.2f);
//		break;
//	case STATE_LUMOSSTRAFEBACK:
//		m_pMeshCom->SetUp_AnimationSet(STATE_LUMOSSTRAFEBACK);
//		break;
//	case STATE_JUMPLANDTORUN:
//		m_pMeshCom->SetUp_AnimationSet(STATE_JUMPLANDTORUN);
//		break;
//	case STATE_ACQUIREWIZARDCARD:
//		m_pMeshCom->SetUp_AnimationSet(STATE_ACQUIREWIZARDCARD);
//		break;
//	case STATE_ATTACKHITFALL:
//		m_pMeshCom->SetUp_AnimationSet(STATE_ATTACKHITFALL);
//		break;
//	case STATE_ATTACKHITSTAND:
//		m_pMeshCom->SetUp_AnimationSet(STATE_ATTACKHITSTAND);
//		break;
//	case STATE_BATTLECAST:
//		m_pMeshCom->SetUp_AnimationSet(STATE_BATTLECAST);
//		break;
//	case STATE_BATTLEIDLE:
//		m_pMeshCom->SetUp_AnimationSet(STATE_BATTLEIDLE);
//		break;
//	case STATE_BUNDIJUMP1:
//		m_pMeshCom->SetUp_AnimationSet(STATE_BUNDIJUMP1);
//		break;
//	case STATE_BUNDIJUMP2:
//		m_pMeshCom->SetUp_AnimationSet(STATE_BUNDIJUMP2);
//		break;
//	case STATE_CAST1:
//		m_pMeshCom->SetUp_AnimationSet(STATE_CAST1);
//		break;
//	case STATE_CASTAIM:
//		m_pMeshCom->SetUp_AnimationSet(STATE_CASTAIM);
//		break;
//	case STATE_CLIMB32:
//		m_pMeshCom->SetUp_AnimationSet(STATE_CLIMB32);
//		break;
//	case STATE_CLIMB64:
//		m_pMeshCom->SetUp_AnimationSet(STATE_CLIMB64);
//		break;
//	case STATE_CLIMB96START:
//		m_pMeshCom->SetUp_AnimationSet(STATE_CLIMB96START);
//		break;
//	case STATE_CLIMB96END:
//		m_pMeshCom->SetUp_AnimationSet(STATE_CLIMB96END);
//		break;
//	case STATE_COLLAPSE:
//		m_pMeshCom->SetUp_AnimationSet(STATE_COLLAPSE);
//		break;
//	case STATE_CR0CASTAIM:
//		m_pMeshCom->SetUp_AnimationSet(STATE_CR0CASTAIM);
//		break;
//	case STATE_CR1CATCH:
//		m_pMeshCom->SetUp_AnimationSet(STATE_CR1CATCH);
//		break;
//	case STATE_CR2FLY:
//		m_pMeshCom->SetUp_AnimationSet(STATE_CR2FLY);
//		break;
//	case STATE_CR3APEX:
//		m_pMeshCom->SetUp_AnimationSet(STATE_CR3APEX);
//		break;
//	case STATE_CR4FALL:
//		m_pMeshCom->SetUp_AnimationSet(STATE_CR4FALL);
//		break;
//	case STATE_CR6DANGLE:
//		m_pMeshCom->SetUp_AnimationSet(STATE_CR6DANGLE);
//		break;
//	case STATE_CRAWLCROUCH:
//		m_pMeshCom->SetUp_AnimationSet(STATE_CRAWLCROUCH);
//		break;
//	case STATE_CRAWLCIRCULARFORWARD:
//		m_pMeshCom->SetUp_AnimationSet(STATE_CRAWLCIRCULARFORWARD);
//		break;
//	case STATE_CRAWLSTAND:
//		m_pMeshCom->SetUp_AnimationSet(STATE_CRAWLSTAND);
//		break;
//	case STATE_CRPULL:
//		m_pMeshCom->SetUp_AnimationSet(STATE_CRPULL);
//		break;
//	case STATE_FAINT:
//		m_pMeshCom->SetUp_AnimationSet(STATE_FAINT);
//		break;
//	case STATE_IDLE:
//		m_pMeshCom->SetUp_AnimationSet(STATE_IDLE);
//		break;
//	case STATE_JUMP_START:
//		m_pMeshCom->SetUp_AnimationSet(STATE_JUMP_START, 1.2f);
//		break;
//	case STATE_JUMP:
//		m_pMeshCom->SetUp_AnimationSet(STATE_JUMP, 1.3f);
//		break;
//	case STATE_JUMPLANDTOSTAND:
//		m_pMeshCom->SetUp_AnimationSet(STATE_JUMPLANDTOSTAND);
//		break;
//	case STATE_LADDERBOTTOMMOUNTDISMOUNT:
//		m_pMeshCom->SetUp_AnimationSet(STATE_LADDERBOTTOMMOUNTDISMOUNT);
//		break;
//	case STATE_LADDERDOWNMOUNT:
//		m_pMeshCom->SetUp_AnimationSet(STATE_LADDERDOWNMOUNT);
//		break;
//	case STATE_LADDERUPDISMOUNT:
//		m_pMeshCom->SetUp_AnimationSet(STATE_LADDERUPDISMOUNT);
//		break;
//	case STATE_LADDERUPDOWN:
//		m_pMeshCom->SetUp_AnimationSet(STATE_LADDERUPDOWN);
//		break;
//	case STATE_PANELPUSH:
//		m_pMeshCom->SetUp_AnimationSet(STATE_PANELPUSH);
//		break;
//	case STATE_PROPHOLDIDLE:
//		m_pMeshCom->SetUp_AnimationSet(STATE_PROPHOLDIDLE);
//		break;
//	case STATE_PROPHOLDWALK:
//		m_pMeshCom->SetUp_AnimationSet(STATE_PROPHOLDWALK);
//		break;
//	case STATE_PROPHOLDWALKBACK:
//		m_pMeshCom->SetUp_AnimationSet(STATE_PROPHOLDWALKBACK);
//		break;
//	case STATE_PROPHOLDWALKLEFT:
//		m_pMeshCom->SetUp_AnimationSet(STATE_PROPHOLDWALKLEFT);
//		break;
//	case STATE_PROPHOLDWALKRIGHT:
//		m_pMeshCom->SetUp_AnimationSet(STATE_PROPHOLDWALKRIGHT);
//		break;
//	case STATE_PROPPICKUP:
//		m_pMeshCom->SetUp_AnimationSet(STATE_PROPPICKUP);
//		break;
//	case STATE_PROPTHROW:
//		m_pMeshCom->SetUp_AnimationSet(STATE_PROPTHROW);
//		break;
//	case STATE_RUN:
//		m_pMeshCom->SetUp_AnimationSet(STATE_RUN);
//		break;
//	case STATE_RUN_BACK:
//		m_pMeshCom->SetUp_AnimationSet(STATE_RUN_BACK);
//		break;
//	case STATE_RUN_LEFT:
//		m_pMeshCom->SetUp_AnimationSet(STATE_RUN_LEFT);
//		break;
//	case STATE_RUN_RIGHT:
//		m_pMeshCom->SetUp_AnimationSet(STATE_RUN_RIGHT);
//		break;
//	case STATE_RUNTOSTOP:
//		m_pMeshCom->SetUp_AnimationSet(STATE_RUNTOSTOP);
//		break;
//	case STATE_SEARCHCHEST:
//		m_pMeshCom->SetUp_AnimationSet(STATE_SEARCHCHEST);
//		break;
//	case STATE_SEARCHCHESTSTAND:
//		m_pMeshCom->SetUp_AnimationSet(STATE_SEARCHCHESTSTAND);
//		break;
//	case STATE_STANDTOWALKNONE:
//		m_pMeshCom->SetUp_AnimationSet(STATE_STANDTOWALKNONE);
//		break;
//	case STATE_WALK:
//		m_pMeshCom->SetUp_AnimationSet(STATE_WALK);
//		break;
//	}
//}

void CPlayer::ETC()
{
	//// 걷고 뛰기 토글 플래그.
	//{
	//	if (GetAsyncKeyState('R') && m_bRunflag == false)
	//	{
	//		m_bRunflag = true;
	//		m_bIsRun = !m_bIsRun;
	//	}
	//	if (!GetAsyncKeyState('R') && m_bRunflag == true)
	//		m_bRunflag = false;
	//}

	// [Update_GameObject]
	//{
	//	if (true == m_isMove)
	//	{
	//		_bool isFinish = false;

	//		m_pTransformCom->Move_Target(&m_vTargetPos, 10.f, fTimeDelta, &isFinish);

	//		if (true == isFinish)
	//			m_isMove = false;
	//	}

	//	if (FAILED(SetUp_HeightOnTerrain()))
	//		return -1;
	//}
}

// 원본객체를 생성한다.
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

// this? : 
// 1.멤버함수안에 존재. 
// 2.멤버함수는 객체로부터 호출(객체.멤버함수(), 객체주소->멤버함수())
// 3.멤버함수안에 존재하는 this는 멤버함수의 호출을 가능하게한 객체의 주소를 의미한다.
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
	//Safe_Release(m_pKeyManager);
	//Safe_Release(m_pColliderCom);
	//Safe_Release(m_pTransformCom);
	//Safe_Release(m_pRendererCom);
	//Safe_Release(m_pMeshCom);
	//Safe_Release(m_pShaderCom);

	Safe_Release(m_pState);

	CUnit::Free();
}
