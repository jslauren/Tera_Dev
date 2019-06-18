#include "stdafx.h"
#include "..\Headers\Terrain_Dragon.h"
#include "Light_Manager.h"

_USING(Client)

CTerrain_Dragon::CTerrain_Dragon(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CTerrain_Dragon::CTerrain_Dragon(const CTerrain_Dragon & rhs)
	: CGameObject(rhs)
{
}

HRESULT CTerrain_Dragon::Ready_GameObject_Prototype()
{
	if (FAILED(CGameObject::Ready_GameObject_Prototype()))
		return E_FAIL;

	return NOERROR;
}

HRESULT CTerrain_Dragon::Ready_GameObject(void* pArg)
{
	_float fDetailValue = *(_float*)pArg;
	m_vDetail *= fDetailValue;

	if (FAILED(Add_Component()))
		return E_FAIL;

	ZeroMemory(&m_MtrlInfo, sizeof(D3DMATERIAL9));

	m_MtrlInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	m_MtrlInfo.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);
	m_MtrlInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);

	CGameObject::Set_Material(m_MtrlInfo);

	return NOERROR;
}

_int CTerrain_Dragon::Update_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pTransformCom)
		return -1;

	return _int();
}

_int CTerrain_Dragon::LateUpdate_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONEALPHA, this)))
		return -1;

	return _int();
}

HRESULT CTerrain_Dragon::Render_GameObject()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pTransformCom ||
		nullptr == m_pBufferCom ||
		nullptr == m_pTextureCom)
		return E_FAIL;

	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();

	if (nullptr == pEffect)
		return E_FAIL;

	pEffect->AddRef();

	if (SetUp_ConstantTable(pEffect))
		return E_FAIL;

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);

	// 행렬 = 행렬 * 행렬
	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);

	return NOERROR;
}

HRESULT CTerrain_Dragon::Add_Component()
{
	// For.Com_Transform
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	// For.Com_Buffer
	if (FAILED(CGameObject::Add_Component(SCENE_DRAGON, L"Component_Buffer_Terrain", L"Com_Buffer", (CComponent**)&m_pBufferCom)))
		return E_FAIL;

	// For.Com_Renderer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	// For.Com_Texture
	if (FAILED(CGameObject::Add_Component(SCENE_DRAGON, L"Component_Texture_Terrain", L"Com_Texture", (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	// For.Com_Shader
	if (FAILED(CGameObject::Add_Component(SCENE_DRAGON, L"Component_Shader_Terrain", L"Com_Shader", (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CTerrain_Dragon::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	if (nullptr == pEffect)
		return E_FAIL;

	pEffect->AddRef();

	pEffect->SetMatrix("g_matWorld", m_pTransformCom->Get_WorldMatrixPointer());
	pEffect->SetMatrix("g_matView", &CGameObject::Get_Transform(D3DTS_VIEW));
	pEffect->SetMatrix("g_matProj", &CGameObject::Get_Transform(D3DTS_PROJECTION));
	pEffect->SetVector("g_fDetail", &m_vDetail);

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

	m_pTextureCom->SetUp_OnShader(pEffect, "g_BaseTexture", 0);

	_matrix		matView = CGameObject::Get_Transform(D3DTS_VIEW);
	D3DXMatrixInverse(&matView, nullptr, &matView);

	pEffect->SetVector("g_vCamPosition", (_vec4*)&matView.m[3][0]);

	Safe_Release(pEffect);

	return NOERROR;
}

CTerrain_Dragon * CTerrain_Dragon::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTerrain_Dragon* pInstance = new CTerrain_Dragon(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		_MSGBOX("CTerrain_Dragon Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CTerrain_Dragon::Clone(void* pArg)
{
	CTerrain_Dragon* pInstance = new CTerrain_Dragon(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		_MSGBOX("CTerrain_Dragon Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTerrain_Dragon::Free()
{
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pBufferCom);

	CGameObject::Free();
}
