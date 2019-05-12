#include "stdafx.h"
#include "..\Headers\Terrain.h"

CTerrain::CTerrain(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CTerrain::CTerrain(const CTerrain & rhs)
	: CGameObject(rhs)
{
}

// ������ü�� �ʿ��� �����͸� �����Ѵ�.
HRESULT CTerrain::Ready_GameObject_Prototype()
{
	// ����������� ���� ��ü�� ������ �����Ѵ�.
	if (FAILED(CGameObject::Ready_GameObject_Prototype()))
		return E_FAIL;

	return NOERROR;
}

// ���� ������ ����� ��ü�� ȣ���ϴ� �Լ�.
// ������ü �����ܿ��� �߰����� �������ʿ��ϸ� ���⼭ �����ض�.
HRESULT CTerrain::Ready_GameObject(void* pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	ZeroMemory(&m_MtrlInfo, sizeof(D3DMATERIAL9));

	m_MtrlInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	m_MtrlInfo.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);
	m_MtrlInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);

	CGameObject::Set_Material(m_MtrlInfo);

	return NOERROR;
}

_int CTerrain::Update_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pTransformCom)
		return -1;

	_vec3			vOut;

	if (GetKeyState(VK_LBUTTON) & 0x8000)
		m_pBufferCom->Picking(g_hWnd, m_pTransformCom, &vOut);

	return _int();
}

_int CTerrain::LateUpdate_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONEALPHA, this)))
		return -1;

	return _int();
}

HRESULT CTerrain::Render_GameObject()
{
	if (nullptr == m_pBufferCom ||
		nullptr == m_pTextureCom ||
		nullptr == m_pShaderCom)
		return E_FAIL;

	//m_pTextureCom->SetUp_OnGraphicDev(0);
	//SetUp_RenderState();

	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	if (nullptr == pEffect)
		return E_FAIL;

	pEffect->AddRef();

	_matrix			matTmp;
	D3DXMatrixIdentity(&matTmp);

	m_pTextureCom->SetUp_OnShader(pEffect, "g_BaseTexture");

	pEffect->SetMatrix("g_matWorld", m_pTransformCom->Get_WorldMatrixPointer());

	m_pGraphic_Device->GetTransform(D3DTS_VIEW,&matTmp);
	pEffect->SetMatrix("g_matView", &matTmp);

	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION,&matTmp);
	pEffect->SetMatrix("g_matProj", &matTmp);

	pEffect->SetVector("g_vLight", &_vec4(1.f, -1.f, 0.f, 0.f));

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &matTmp);
	D3DXMatrixInverse(&matTmp, nullptr, &matTmp);
	_vec3 vCameraPos = matTmp.m[3];
	pEffect->SetVector("g_vCamera", &(_vec4)vCameraPos);

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);

	// ��� = ��� * ���
	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);

//	Release_RenderState();

	return NOERROR;
}

HRESULT CTerrain::Add_Component()
{
	// For.Com_Transform
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	// For.Com_Buffer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Buffer_Terrain", L"Com_Buffer", (CComponent**)&m_pBufferCom)))
		return E_FAIL;

	// For.Com_Renderer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	// For.Com_Texture
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Texture_Terrain", L"Com_Texture", (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	// For.Com_Shader
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Shader_Default", L"Com_Shader", (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	return NOERROR;
}

//HRESULT CTerrain::SetUp_RenderState()
//{
//	CGameObject::Set_SamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
//	CGameObject::Set_SamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
//	CGameObject::Set_SamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
//	CGameObject::Set_SamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
//	CGameObject::Set_SamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
//	CGameObject::Set_RenderState(D3DRS_LIGHTING, TRUE);
//	CGameObject::Set_RenderState(D3DRS_NORMALIZENORMALS, true);
//
//	return NOERROR;
//}
//
//HRESULT CTerrain::Release_RenderState()
//{
//	CGameObject::Set_SamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_NONE);
//	CGameObject::Set_SamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_NONE);
//	CGameObject::Set_SamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
//	CGameObject::Set_RenderState(D3DRS_LIGHTING, FALSE);
//
//	return NOERROR;
//}

// ������ü�� �����Ѵ�.
CTerrain * CTerrain::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTerrain* pInstance = new CTerrain(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		_MSGBOX("CTerrain Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

// this? : 
// 1.����Լ��ȿ� ����. 
// 2.����Լ��� ��ü�κ��� ȣ��(��ü.����Լ�(), ��ü�ּ�->����Լ�())
// 3.����Լ��ȿ� �����ϴ� this�� ����Լ��� ȣ���� �����ϰ��� ��ü�� �ּҸ� �ǹ��Ѵ�.
CGameObject * CTerrain::Clone(void* pArg)
{
	CTerrain* pInstance = new CTerrain(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		_MSGBOX("CTerrain Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTerrain::Free()
{
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pBufferCom);
	Safe_Release(m_pShaderCom);

	CGameObject::Free();
}
