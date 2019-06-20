#include "stdafx.h"
#include "..\Headers\Weapon.h"
#include "Object_Manager.h"
#include "Management.h"
#include "Light_Manager.h"

#define WEAPON_SCALING	0.7f

_USING(Client)

CWeapon::CWeapon(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
	, m_fFrame(0.f)
{
}

CWeapon::CWeapon(const CWeapon & rhs)
	: CGameObject(rhs)
	, m_fFrame(rhs.m_fFrame)
{
}

HRESULT CWeapon::Ready_GameObject_Prototype()
{
	// 파일입출력을 통해 객체의 정보를 셋팅한다.
	if (FAILED(CGameObject::Ready_GameObject_Prototype()))
		return E_FAIL;

	return NOERROR;
}

HRESULT CWeapon::Ready_GameObject(void * pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	CObject_Manager*	pObject_Manager = CObject_Manager::GetInstance();
	if (nullptr == pObject_Manager)
		return E_FAIL;

	Safe_AddRef(pObject_Manager);

	CMesh_Dynamic_Bone* pPlayerMeshCom = (CMesh_Dynamic_Bone*)pObject_Manager->Get_Component(SCENE_STATIC, L"Layer_Player", L"Com_Mesh_Bone");
	if (nullptr == pPlayerMeshCom)
		return E_FAIL;

	Safe_AddRef(pPlayerMeshCom);
	
	// R_Sword
	m_pBoneMatrix = &(pPlayerMeshCom->Get_FrameDesc("Weapon_Back")->CombinedTransformationMatrix);
	if (nullptr == m_pBoneMatrix)
		return E_FAIL;

	Safe_Release(pPlayerMeshCom);


	CTransform* pPlayerTransform = (CTransform*)pObject_Manager->Get_Component(SCENE_STATIC, L"Layer_Player", L"Com_Transform");
	if (nullptr == pPlayerTransform)
		return E_FAIL;

	Safe_AddRef(pPlayerTransform);


	m_pParentMatrix = pPlayerTransform->Get_WorldMatrixPointer();
	if (nullptr == m_pParentMatrix)
		return E_FAIL;

	Safe_Release(pPlayerTransform);

	Safe_Release(pObject_Manager);

//	m_pTransformCom->Set_Rotation_YawPitchRoll(D3DXToRadian(-90.0f), D3DXToRadian(180.f), D3DXToRadian(90.0f));

	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(-7.f, 7.f, 0.f));
	m_pTransformCom->Set_Angle_Axis(_vec3(0.f, 0.f, 1.f), D3DXToRadian(225));
	//m_pTransformCom->Set_Scaling(0.65f, 0.65f, 0.65f);
	m_pTransformCom->Set_Scaling(WEAPON_SCALING, WEAPON_SCALING, WEAPON_SCALING);

	return NOERROR;
}

_int CWeapon::Update_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pTransformCom)
		return -1;

	CollisionCheck();

	return _int();
}

_int CWeapon::LateUpdate_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	Compute_ViewZ(m_pTransformCom);

	//if (FAILED(SetUp_HeightOnTerrain()))
	//	return -1;

	m_fTimeDelta = fTimeDelta;

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONEALPHA, this)))
		return -1;

	return _int();
}

HRESULT CWeapon::Render_GameObject()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pTransformCom ||
		nullptr == m_pMeshCom )
		return E_FAIL;

	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	if (nullptr == pEffect)
		return E_FAIL;

	pEffect->AddRef();

	if (FAILED(SetUp_ConstantTable(pEffect)))
		return E_FAIL;

	pEffect->Begin(nullptr, 0);

	for (size_t i = 0; i < m_pMeshCom->Get_NumMaterials(); ++i)
	{
		if (FAILED(m_pMeshCom->SetTexture_OnShader(pEffect, i, "g_BaseTexture", MESHTEXTURE::TYPE_DIFFUSE)))
			return E_FAIL;

		pEffect->CommitChanges();

		pEffect->BeginPass(0);

		m_pMeshCom->Render_Mesh(i);

		pEffect->EndPass();
	}
	
	pEffect->End();

	Safe_Release(pEffect);

	m_pColliderCom01->Render_Collider();
	m_pColliderCom02->Render_Collider();
	m_pColliderCom03->Render_Collider();
	m_pColliderCom04->Render_Collider();

	return NOERROR;
}

HRESULT CWeapon::Set_BoneMatrix(_int iIndex)
{
	CObject_Manager*	pObject_Manager = CObject_Manager::GetInstance();
	if (nullptr == pObject_Manager)
		return E_FAIL;

	Safe_AddRef(pObject_Manager);

	CMesh_Dynamic_Bone* pPlayerMeshCom = (CMesh_Dynamic_Bone*)pObject_Manager->Get_Component(SCENE_STATIC, L"Layer_Player", L"Com_Mesh_Bone");
	if (nullptr == pPlayerMeshCom)
		return E_FAIL;

	Safe_AddRef(pPlayerMeshCom);

	if (iIndex == 1)
	{
		m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(-7.f, 7.f, 0.f));
		m_pTransformCom->Set_Angle_Axis(_vec3(0.f, 0.f, 1.f), D3DXToRadian(225));
		m_pTransformCom->Set_Scaling(WEAPON_SCALING, WEAPON_SCALING, WEAPON_SCALING);

		m_pBoneMatrix = &(pPlayerMeshCom->Get_FrameDesc("Weapon_Back")->CombinedTransformationMatrix);
		if (nullptr == m_pBoneMatrix)
			return E_FAIL;
	}
	else if (iIndex == 2)
	{
		m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(2.5f, -3.f, 0.f));
		m_pTransformCom->Set_Rotation_YawPitchRoll(D3DXToRadian(180), D3DXToRadian(0), D3DXToRadian(0));
		//m_pTransformCom->Set_Angle_Axis(_vec3(0.f, 0.f, 1.f), D3DXToRadian(0));
		m_pTransformCom->Set_Scaling(WEAPON_SCALING, WEAPON_SCALING, WEAPON_SCALING);

		m_pBoneMatrix = &(pPlayerMeshCom->Get_FrameDesc("R_Sword")->CombinedTransformationMatrix);
		if (nullptr == m_pBoneMatrix)
			return E_FAIL;
	}
	else if (iIndex == 3)	// 오른손
	{
		//m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(2.5f, -3.f, 0.f));
		//m_pTransformCom->Set_Rotation_YawPitchRoll(D3DXToRadian(180), D3DXToRadian(0), D3DXToRadian(0));
		//m_pTransformCom->Set_Angle_Axis(_vec3(0.f, 0.f, 1.f), D3DXToRadian(0));
		m_pTransformCom->Set_Scaling(WEAPON_SCALING, WEAPON_SCALING, WEAPON_SCALING);

		m_pBoneMatrix = &(pPlayerMeshCom->Get_FrameDesc("Bip01-R-Hand")->CombinedTransformationMatrix);
		if (nullptr == m_pBoneMatrix)
			return E_FAIL;
	}

	Safe_Release(pPlayerMeshCom);
	Safe_Release(pObject_Manager);
	
	return NOERROR;
}

HRESULT CWeapon::Add_Component()
{
	// For.Com_Transform
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	// For.Com_Mesh
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Mesh_Weapon", L"Com_Mesh", (CComponent**)&m_pMeshCom)))
		return E_FAIL;

	// For.Com_Renderer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	// For.Com_Shader
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Shader_Mesh", L"Com_Shader", (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	// For.Com_Collider
	//if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Collider_OBB", L"Com_Collider", (CComponent**)&m_pColliderCom,
	//	&CCollider::COLLIDERDESC(CCollider::COLLIDERDESC::TYPE_TRANSFORM, &m_matWorld, nullptr, _vec3(0.2f, 0.2f, 1.5f), _vec3(0.0f, 0.f, -0.75f)))))
	//	return E_FAIL;

	// For.Com_Collider
	_float fColliderScale = 3.f;
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Collider_Sphere", L"Com_WeaponCollider01", (CComponent**)&m_pColliderCom01,
		&CCollider::COLLIDERDESC(CCollider::COLLIDERDESC::TYPE_TRANSFORM, &m_matWorld, nullptr, _vec3(fColliderScale, fColliderScale, fColliderScale), _vec3(0.f, 2.5f, 0.f)))))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Collider_Sphere", L"Com_WeaponCollider02", (CComponent**)&m_pColliderCom02,
		&CCollider::COLLIDERDESC(CCollider::COLLIDERDESC::TYPE_TRANSFORM, &m_matWorld, nullptr, _vec3(fColliderScale, fColliderScale, fColliderScale), _vec3(0.f, 5.5f, 0.f)))))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Collider_Sphere", L"Com_WeaponCollider03", (CComponent**)&m_pColliderCom03,
		&CCollider::COLLIDERDESC(CCollider::COLLIDERDESC::TYPE_TRANSFORM, &m_matWorld, nullptr, _vec3(fColliderScale, fColliderScale, fColliderScale), _vec3(0.f, 8.5f, 0.f)))))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Collider_Sphere", L"Com_WeaponCollider04", (CComponent**)&m_pColliderCom04,
		&CCollider::COLLIDERDESC(CCollider::COLLIDERDESC::TYPE_TRANSFORM, &m_matWorld, nullptr, _vec3(fColliderScale, fColliderScale, fColliderScale), _vec3(0.f, 11.5f, 0.f)))))
		return E_FAIL;

	return NOERROR;
}

HRESULT CWeapon::SetUp_HeightOnTerrain()
{
	CObject_Manager*	pObject_Manager = CObject_Manager::GetInstance();

	if (nullptr == pObject_Manager)
		return E_FAIL;
	pObject_Manager->AddRef();

	CBuffer_Terrain* pBufferCom = (CBuffer_Terrain*)pObject_Manager->Get_Component(SCENE_STAGE, L"Layer_BackGround", L"Com_Buffer", 0);
	if (nullptr == pBufferCom)
		return E_FAIL;

	_float	fY = pBufferCom->Compute_HeightOnBuffer(m_pTransformCom);

	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION)->x, fY + 0.5f, m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION)->z));

	Safe_Release(pObject_Manager);

	return NOERROR;
}

HRESULT CWeapon::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	if (nullptr == pEffect)
		return E_FAIL;

	pEffect->AddRef();

	//_matrix		matWorld = *m_pTransformCom->Get_WorldMatrixPointer() * *m_pBoneMatrix * *m_pParentMatrix;
	m_matWorld = *m_pTransformCom->Get_WorldMatrixPointer() * *m_pBoneMatrix * *m_pParentMatrix;

	pEffect->SetMatrix("g_matWorld", &m_matWorld);
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

void CWeapon::CollisionCheck()
{
	if (CManagement::GetInstance()->Get_CurrentScene() == SCENE_DRAGON)
	{
		const CCollider* pArkusBodyCollider = (const CCollider*)CObject_Manager::GetInstance()->Get_Component(SCENE_DRAGON, L"Layer_Monster", L"Com_Collider_Arkus_Body");
		const CCollider* pArkusHeadCollider = (const CCollider*)CObject_Manager::GetInstance()->Get_Component(SCENE_DRAGON, L"Layer_Monster", L"Com_Collider_Arkus_Head");
		const CCollider* pArkusNeckCollider = (const CCollider*)CObject_Manager::GetInstance()->Get_Component(SCENE_DRAGON, L"Layer_Monster", L"Com_Collider_Arkus_Neck");
		const CCollider* pArkusTail01Collider = (const CCollider*)CObject_Manager::GetInstance()->Get_Component(SCENE_DRAGON, L"Layer_Monster", L"Com_Collider_Arkus_Tail01");
		const CCollider* pArkusTail02Collider = (const CCollider*)CObject_Manager::GetInstance()->Get_Component(SCENE_DRAGON, L"Layer_Monster", L"Com_Collider_Arkus_Tail02");

		if (m_pColliderCom01->Collision_Sphere(pArkusBodyCollider) == true ||
			m_pColliderCom01->Collision_Sphere(pArkusHeadCollider) == true ||
			m_pColliderCom01->Collision_Sphere(pArkusNeckCollider) == true ||
			m_pColliderCom01->Collision_Sphere(pArkusTail01Collider) == true ||
			m_pColliderCom01->Collision_Sphere(pArkusTail02Collider) == true)
		{
			m_bCollisionCheck = true;
		}
		else if (m_pColliderCom02->Collision_Sphere(pArkusBodyCollider) == true ||
			m_pColliderCom02->Collision_Sphere(pArkusHeadCollider) == true ||
			m_pColliderCom02->Collision_Sphere(pArkusNeckCollider) == true ||
			m_pColliderCom02->Collision_Sphere(pArkusTail01Collider) == true ||
			m_pColliderCom02->Collision_Sphere(pArkusTail02Collider) == true)
		{
			m_bCollisionCheck = true;
		}
		else if (m_pColliderCom03->Collision_Sphere(pArkusBodyCollider) == true ||
			m_pColliderCom03->Collision_Sphere(pArkusHeadCollider) == true ||
			m_pColliderCom03->Collision_Sphere(pArkusNeckCollider) == true ||
			m_pColliderCom03->Collision_Sphere(pArkusTail01Collider) == true ||
			m_pColliderCom03->Collision_Sphere(pArkusTail02Collider) == true)
		{
			m_bCollisionCheck = true;
		}
		else if (m_pColliderCom04->Collision_Sphere(pArkusBodyCollider) == true ||
			m_pColliderCom04->Collision_Sphere(pArkusHeadCollider) == true ||
			m_pColliderCom04->Collision_Sphere(pArkusNeckCollider) == true ||
			m_pColliderCom04->Collision_Sphere(pArkusTail01Collider) == true ||
			m_pColliderCom04->Collision_Sphere(pArkusTail02Collider) == true)
		{
			m_bCollisionCheck = true;
		}
		else
			m_bCollisionCheck = false;

		//m_pColliderCom01->Collision_Sphere(pArkusBodyCollider);
		//m_pColliderCom01->Collision_Sphere(pArkusHeadCollider);
		//m_pColliderCom01->Collision_Sphere(pArkusNeckCollider);
		//m_pColliderCom01->Collision_Sphere(pArkusTail01Collider);
		//m_pColliderCom01->Collision_Sphere(pArkusTail02Collider);

	}
}

CWeapon * CWeapon::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CWeapon* pInstance = new CWeapon(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		_MSGBOX("CWeapon Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CWeapon::Clone(void * pArg)
{
	CWeapon* pInstance = new CWeapon(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		_MSGBOX("CWeapon Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWeapon::Free()
{
	Safe_Release(m_pColliderCom01);
	Safe_Release(m_pColliderCom02);
	Safe_Release(m_pColliderCom03);
	Safe_Release(m_pColliderCom04);

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pMeshCom);
	Safe_Release(m_pShaderCom);

	CGameObject::Free();
}
