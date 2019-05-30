#include "stdafx.h"
#include "..\Headers\Player.h"
#include "Object_Manager.h"
#include "Light_Manager.h"

_USING(Client)

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
	, m_pKeyManager(CKeyManager::GetInstance())
{
}

CPlayer::CPlayer(const CPlayer & rhs)
	: CGameObject(rhs)
	, m_isMove(rhs.m_isMove)
{
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

	m_pTransformCom->Set_Scaling(0.01f, 0.01f, 0.01f);
	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(0.2, 0.f, 0.2));
	m_pMeshCom->SetUp_AnimationSet(1);

	m_pMeshCom->ChangePivot(_vec3(1.f, 0.f, 0.f), -90);
	m_pMeshCom->ChangePivot(_vec3(0.f, 1.f, 0.f), 180);
	
	return NOERROR;
}

_int CPlayer::Update_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pTransformCom)
		return -1;

	if (GetKeyState('W') & 0x8000)
	{
		m_pTransformCom->Move(0, 5.f, fTimeDelta);
		m_pMeshCom->SetUp_AnimationSet(9);
	}
	else if (GetKeyState('S') & 0x8000)
	{
		m_pTransformCom->Move(1, 5.f, fTimeDelta);
		m_pMeshCom->SetUp_AnimationSet(5);
	}
	else if (GetKeyState('A') & 0x8000)
	{
		m_pTransformCom->Move(2, 5.f, fTimeDelta);
		m_pMeshCom->SetUp_AnimationSet(7);
	}
	else if (GetKeyState('D') & 0x8000)
	{
		m_pTransformCom->Move(3, 5.f, fTimeDelta);
		m_pMeshCom->SetUp_AnimationSet(6);
	}
	else if (GetKeyState(VK_SPACE) & 0x8000)
	{
		m_pTransformCom->Move(3, 5.f, fTimeDelta);
		m_pMeshCom->SetUp_AnimationSet(6);
	}
	else
	{
		m_pMeshCom->SetUp_AnimationSet(29);
	}

	CKeyManager::GetInstance()->UpdateKey();

	if (true == m_isMove)
	{
		_bool isFinish = false;

		m_pTransformCom->Move_Target(&m_vTargetPos, 10.f, fTimeDelta, &isFinish);

		if (true == isFinish)
			m_isMove = false;
	}

	m_pMeshCom->Play_Animation(fTimeDelta);

	//if (FAILED(SetUp_HeightOnTerrain()))
	//	return -1;

	return _int();
}

_int CPlayer::LateUpdate_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHA, this)))
		return -1;

	return _int();
}

HRESULT CPlayer::Render_GameObject()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pTransformCom ||
		nullptr == m_pMeshCom ||
		nullptr == m_pTextureCom)
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

	return NOERROR;
}

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

	// For.Com_Texture
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Texture_Default", L"Com_Texture", (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	// For.Com_Shader
	if (FAILED(CGameObject::Add_Component(SCENE_STAGE, L"Component_Shader_Mesh", L"Com_Shader", (CComponent**)&m_pShaderCom)))
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

	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION)->x, fY + 0.5f, m_pTransformCom->Get_StateInfo(CTransform::STATE_POSITION)->z));

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

//HRESULT CPlayer::SetUp_RenderState()
//{
//	CGameObject::Set_RenderState(D3DRS_CULLMODE, D3DCULL_NONE);
//
//	return NOERROR;
//}
//
//HRESULT CPlayer::Release_RenderState()
//{
//	CGameObject::Set_RenderState(D3DRS_CULLMODE, D3DCULL_CCW);
//
//	return NOERROR;
//}

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
	Safe_Release(m_pKeyManager);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pMeshCom);
	Safe_Release(m_pShaderCom);

	CGameObject::Free();
}
