#include "stdafx.h"
#include "..\Headers\Arkus.h"
#include "Object_Manager.h"
#include "Light_Manager.h"
#include "Layer.h"
#include "Input_Device.h"
#include "ArkusState.h"

#include "Arkus_Apperance.h"
#include "Arkus_Idle.h"

_USING(Client)

CArkus::CArkus(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUnit(pGraphic_Device)
{
}

CArkus::CArkus(const CArkus & rhs)
	: CUnit(rhs)
{
}

HRESULT CArkus::Ready_GameObject_Prototype()
{
	if (FAILED(CGameObject::Ready_GameObject_Prototype()))
		return E_FAIL;

	return NOERROR;
}

HRESULT CArkus::Ready_GameObject(void * pArg)
{
	//tObjectMeshData = *((OBJECTMESHDATA*)pArg);

	if (FAILED(Add_Component()))
		return E_FAIL;

//	_matrix matWorld = *(_matrix*)pArg;

	m_pTransformCom->Set_Scaling(0.4f, 0.4f, 0.4f);
	m_pTransformCom->Set_Rotation_YawPitchRoll(D3DXToRadian(90.f), D3DXToRadian(0.f), D3DXToRadian(0.f));
	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(200.f, 0.f, 200.f));
//	m_pTransformCom->Set_WorldMatrix(matWorld);

//	m_pTransformCom->Set_WorldMatrix(tObjectMeshData.matWorld);
//	m_pTransformCom->Set_WorldMatrix(((OBJECTMESHDATA*)pArg)->matWorld);

	m_pMeshCom->SetUp_AnimationSet(Apperance01);
	m_eAnimationIndex = Apperance01;
	m_eOldAnimationIndex = End;
	m_eCurActionID = ACTION_READY;

	int iIdleState = 1;
	m_pState = CArkus_Apperance::Create(m_pGraphic_Device, *this, &iIdleState);

}

_int CArkus::Update_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pTransformCom)
		return -1;

	m_fTimeDelta = fTimeDelta;

	CollisionCheck();
	AI();

	return _int();
}

_int CArkus::LateUpdate_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	Compute_ViewZ(m_pTransformCom);

	//if (FAILED(SetUp_HeightOnTerrain(1)))
	//	return -1;

	const CCollider* pPlayerCollider = (const CCollider*)CObject_Manager::GetInstance()->Get_Component(SCENE_STATIC, L"Layer_Player", L"Com_Player_Collider");
	const CCollider* pWeaponCollider = (const CCollider*)CObject_Manager::GetInstance()->Get_Component(SCENE_STATIC, L"Layer_Weapon", L"Com_WeaponCollider");

	m_pColliderCom->Collision_OBB(pPlayerCollider);
	m_pColliderCom->Collision_OBB(pWeaponCollider);

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONEALPHA, this)))
		return -1;

	return _int();
}

HRESULT CArkus::Render_GameObject()
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
	m_pColliderHeadCom->Render_Collider();
	m_pColliderNeckCom->Render_Collider();
	m_pColliderTail01Com->Render_Collider();
	m_pColliderTail02Com->Render_Collider();

	return NOERROR;
}

HRESULT CArkus::Add_Component()
{
	// For.Com_Transform
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom)))
		return E_FAIL;
		
	// For.Com_Renderer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom)))
		return E_FAIL;
	
	// For.Com_Shader
	if (FAILED(CGameObject::Add_Component(SCENE_DRAGON, L"Component_Shader_Mesh", L"Com_Shader", (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	// For.Com_Mesh
	if (FAILED(CGameObject::Add_Component(SCENE_DRAGON, L"Component_Mesh_Arkus", L"Com_Mesh", (CComponent**)&m_pMeshCom)))
		return E_FAIL;
	
	//// For.Com_Collider
	//CCollider::COLLIDERDESC		ColliderDesc;
	//ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	//
	//ColliderDesc.eType = CCollider::COLLIDERDESC::TYPE_TRANSFORM;
	//ColliderDesc.pTransformMatrix = m_pTransformCom->Get_WorldMatrixPointer();
	//ColliderDesc.pFrameMatrix = nullptr;
	//ColliderDesc.vScale = _vec3(10.0f, 20.f, 10.f);
	//ColliderDesc.vPivot = _vec3(0.0f, 1.f, 0.f);

	// [콜라이더 부위 별 추가]
	// For.Com_Collider_Arkus_Body
	_float fBodyScale = 50.f;
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Collider_Sphere", L"Com_Collider_Arkus_Body", 
		(CComponent**)&m_pColliderCom, &CCollider::COLLIDERDESC(CCollider::COLLIDERDESC::TYPE_FRAME,
		m_pTransformCom->Get_WorldMatrixPointer(), &(m_pMeshCom->Get_FrameDesc("Bip01-Spine")->CombinedTransformationMatrix)
		, _vec3(fBodyScale, fBodyScale, fBodyScale), _vec3(0.f, 0.f, 0.f)))))
		return E_FAIL;

	// For.Com_Collider_Arkus_Head
	_float fHeadScale = 24.f;
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Collider_Sphere", L"Com_Collider_Arkus_Head",
		(CComponent**)&m_pColliderHeadCom, &CCollider::COLLIDERDESC(CCollider::COLLIDERDESC::TYPE_FRAME,
			m_pTransformCom->Get_WorldMatrixPointer(), &(m_pMeshCom->Get_FrameDesc("Bip01-Head")->CombinedTransformationMatrix)
			, _vec3(fHeadScale, fHeadScale, fHeadScale), _vec3(0.f, 0.f, -5.f)))))
		return E_FAIL;

	// For.Com_Collider_Arkus_Neck
	_float fNeckScale = 20.f;
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Collider_Sphere", L"Com_Collider_Arkus_Neck",
		(CComponent**)&m_pColliderNeckCom, &CCollider::COLLIDERDESC(CCollider::COLLIDERDESC::TYPE_FRAME,
			m_pTransformCom->Get_WorldMatrixPointer(), &(m_pMeshCom->Get_FrameDesc("Bip01-Neck1")->CombinedTransformationMatrix)
			, _vec3(fNeckScale, fNeckScale, fNeckScale), _vec3(0.f, 0.f, 0.f)))))
		return E_FAIL;

	// For.Com_Collider_Arkus_Tail01
	_float fTail01Scale = 33.f;
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Collider_Sphere", L"Com_Collider_Arkus_Tail01",
		(CComponent**)&m_pColliderTail01Com, &CCollider::COLLIDERDESC(CCollider::COLLIDERDESC::TYPE_FRAME,
			m_pTransformCom->Get_WorldMatrixPointer(), &(m_pMeshCom->Get_FrameDesc("Bip01-Tail1")->CombinedTransformationMatrix)
			, _vec3(fTail01Scale, fTail01Scale, fTail01Scale), _vec3(0.f, 0.f, 5.f)))))
		return E_FAIL;
	
	// For.Com_Collider_Arkus_Tail02
	_float fTail02Scale = 42.f;
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Collider_Sphere", L"Com_Collider_Arkus_Tail02",
		(CComponent**)&m_pColliderTail02Com, &CCollider::COLLIDERDESC(CCollider::COLLIDERDESC::TYPE_FRAME,
			m_pTransformCom->Get_WorldMatrixPointer(), &(m_pMeshCom->Get_FrameDesc("Bip01-Tail3")->CombinedTransformationMatrix)
			, _vec3(fTail02Scale, fTail02Scale, fTail02Scale), _vec3(0.f, 0.f, 8.f)))))
		return E_FAIL;

	// For.Com_Frustum
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Frustum", L"Com_Frustum", (CComponent**)&m_pFrustumCom)))
		return E_FAIL;
		
	return NOERROR;
}

HRESULT CArkus::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
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

HRESULT CArkus::SetUp_HeightOnTerrain(_uint iIndex)
{
	CUnit::SetUp_HeightOnTerrain(1);

	return NOERROR;
}

void CArkus::CollisionCheck()
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(CObject_Manager::GetInstance()->Get_Object(SCENE_STATIC, L"Layer_Player"));
	
	if (pPlayer->Get_AniIndex() != CPlayer::PLAYER_ANI::Idle &&
		pPlayer->Get_AniIndex() != CPlayer::PLAYER_ANI::Idle_Battle &&
		pPlayer->Get_AniIndex() != CPlayer::PLAYER_ANI::Hit &&
		pPlayer->Get_AniIndex() != CPlayer::PLAYER_ANI::Hit_Wake &&
		pPlayer->Get_AniIndex() != CPlayer::PLAYER_ANI::Hit_Loop &&
		pPlayer->Get_AniIndex() != CPlayer::PLAYER_ANI::Hit_Rolling &&
		pPlayer->Get_AniIndex() != CPlayer::PLAYER_ANI::Hit_High &&
		pPlayer->Get_AniIndex() != CPlayer::PLAYER_ANI::Hit_Land &&
		pPlayer->Get_AniIndex() != CPlayer::PLAYER_ANI::DrawSwordCharge &&
		pPlayer->Get_AniIndex() != CPlayer::PLAYER_ANI::DrawSwordLoop &&
		pPlayer->Get_AniIndex() != CPlayer::PLAYER_ANI::DrawSwordMove &&
		pPlayer->Get_AniIndex() != CPlayer::PLAYER_ANI::Run &&
		pPlayer->Get_AniIndex() != CPlayer::PLAYER_ANI::Run_Battle)
	{
		// Static으로 있는 플레이어 레이어에서 Com_Player_Collider라는 콜라이더를 가져오겠다.
		const CCollider* pPlayerCollider = (const CCollider*)CObject_Manager::GetInstance()->Get_Component(SCENE_STATIC, L"Layer_Player", L"Com_Player_Collider");
		const CCollider* pWeaponCollider01 = (const CCollider*)CObject_Manager::GetInstance()->Get_Component(SCENE_STATIC, L"Layer_Weapon", L"Com_WeaponCollider01");
		const CCollider* pWeaponCollider02 = (const CCollider*)CObject_Manager::GetInstance()->Get_Component(SCENE_STATIC, L"Layer_Weapon", L"Com_WeaponCollider02");
		const CCollider* pWeaponCollider03 = (const CCollider*)CObject_Manager::GetInstance()->Get_Component(SCENE_STATIC, L"Layer_Weapon", L"Com_WeaponCollider03");
		const CCollider* pWeaponCollider04 = (const CCollider*)CObject_Manager::GetInstance()->Get_Component(SCENE_STATIC, L"Layer_Weapon", L"Com_WeaponCollider04");

		m_pColliderCom->Collision_Sphere(pPlayerCollider);
		m_pColliderHeadCom->Collision_Sphere(pPlayerCollider);
		m_pColliderNeckCom->Collision_Sphere(pPlayerCollider);
		m_pColliderTail01Com->Collision_Sphere(pPlayerCollider);
		m_pColliderTail02Com->Collision_Sphere(pPlayerCollider);

		{
			if (m_pColliderCom->Collision_Sphere(pWeaponCollider01) == true)
				m_bCollisionPartsCheck[0] = true;

			if (m_pColliderCom->Collision_Sphere(pWeaponCollider02) == true)
				m_bCollisionPartsCheck[0] = true;

			if (m_pColliderCom->Collision_Sphere(pWeaponCollider03) == true)
				m_bCollisionPartsCheck[0] = true;

			if (m_pColliderCom->Collision_Sphere(pWeaponCollider04) == true)
				m_bCollisionPartsCheck[0] = true;
		}
		{
			if (m_pColliderHeadCom->Collision_Sphere(pWeaponCollider01) == true)
				m_bCollisionPartsCheck[1] = true;

			if (m_pColliderHeadCom->Collision_Sphere(pWeaponCollider02) == true)
				m_bCollisionPartsCheck[1] = true;

			if (m_pColliderHeadCom->Collision_Sphere(pWeaponCollider03) == true)
				m_bCollisionPartsCheck[1] = true;

			if (m_pColliderHeadCom->Collision_Sphere(pWeaponCollider04) == true)
				m_bCollisionPartsCheck[1] = true;
		}
		{
			if (m_pColliderNeckCom->Collision_Sphere(pWeaponCollider01) == true)
				m_bCollisionPartsCheck[2] = true;

			if (m_pColliderNeckCom->Collision_Sphere(pWeaponCollider02) == true)
				m_bCollisionPartsCheck[2] = true;

			if (m_pColliderNeckCom->Collision_Sphere(pWeaponCollider03) == true)
				m_bCollisionPartsCheck[2] = true;

			if (m_pColliderNeckCom->Collision_Sphere(pWeaponCollider04) == true)
				m_bCollisionPartsCheck[2] = true;

		}
		{
			if (m_pColliderTail01Com->Collision_Sphere(pWeaponCollider01) == true)
				m_bCollisionPartsCheck[3] = true;

			if (m_pColliderTail01Com->Collision_Sphere(pWeaponCollider02) == true)
				m_bCollisionPartsCheck[3] = true;

			if (m_pColliderTail01Com->Collision_Sphere(pWeaponCollider03) == true)
				m_bCollisionPartsCheck[3] = true;

			if (m_pColliderTail01Com->Collision_Sphere(pWeaponCollider04) == true)
				m_bCollisionPartsCheck[3] = true;
		}
		{
			if (m_pColliderTail01Com->Collision_Sphere(pWeaponCollider01) == true)
				m_bCollisionPartsCheck[4] = true;

			if (m_pColliderTail01Com->Collision_Sphere(pWeaponCollider02) == true)
				m_bCollisionPartsCheck[4] = true;

			if (m_pColliderTail01Com->Collision_Sphere(pWeaponCollider03) == true)
				m_bCollisionPartsCheck[4] = true;

			if (m_pColliderTail01Com->Collision_Sphere(pWeaponCollider04) == true)
				m_bCollisionPartsCheck[4] = true;
		}

		for (size_t i = 0; i < 5; ++i)
		{
			if (m_bCollisionPartsCheck[i] == false)
				m_bCollisionCheck = false;
			else
				m_bCollisionCheck = true;
		}
	}
}

void CArkus::AI()
{
	CArkusState* pState = m_pState->Input_State(*this, m_fTimeDelta, 0, m_pNavigationCom);

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

CArkus * CArkus::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CArkus* pInstance = new CArkus(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		_MSGBOX("CArkus Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CArkus::Clone(void * pArg)
{
	CArkus* pInstance = new CArkus(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		_MSGBOX("CArkus Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CArkus::Free()
{
	Safe_Release(m_pColliderHeadCom);
	Safe_Release(m_pColliderNeckCom);
	Safe_Release(m_pColliderTail01Com);
	Safe_Release(m_pColliderTail02Com);

	Safe_Release(m_pState);
	
	CUnit::Free();
}
