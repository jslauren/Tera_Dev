#include "stdafx.h"
#include "..\Headers\Back_Logo.h"

_USING(Client)

CBack_Logo::CBack_Logo(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUI(pGraphic_Device)
{
}

CBack_Logo::CBack_Logo(const CBack_Logo & rhs)
	: CUI(rhs)
{
}

// 원본객체에 필요한 데이터를 셋팅한다.
HRESULT CBack_Logo::Ready_GameObject_Prototype()
{
	// 파일입출력을 통해 객체의 정보를 셋팅한다.
	if (FAILED(CGameObject::Ready_GameObject_Prototype()))
		return E_FAIL;

	return NOERROR;
}

HRESULT CBack_Logo::Ready_GameObject(void* pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pTransformCom->Set_Scaling(2.f, 2.f, 2.f);

	return NOERROR;
}

_int CBack_Logo::Update_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pTransformCom)
		return -1;

	return _int();
}

_int CBack_Logo::LateUpdate_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_PRIORITY, this)))
		return -1;

	return _int();
}

HRESULT CBack_Logo::Render_GameObject()
{
	if (nullptr == m_pBufferBGCom ||
		nullptr == m_pTextureCom ||
		nullptr == m_pShaderCom)
		return E_FAIL;

	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	if (nullptr == pEffect)
		return E_FAIL;

	pEffect->AddRef();

	if (FAILED(SetUp_ConstantTable(pEffect)))
		return E_FAIL;

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);

	m_pBufferBGCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);

	return NOERROR;
}

HRESULT CBack_Logo::Add_Component()
{
	// For.Com_Transform
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	// For.Com_Renderer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	// For.Com_Buffer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Buffer_Background", L"Com_Buffer", (CComponent**)&m_pBufferBGCom)))
		return E_FAIL;

	// For.Com_Texture
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Texture_Loading_Main", L"Com_Texture", (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	// For.Com_Shader
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Shader_Default", L"Com_Shader", (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CBack_Logo::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	if (nullptr == pEffect)
		return E_FAIL;

	pEffect->AddRef();

	
	_matrix		matTmp;
	D3DXMatrixIdentity(&matTmp);

	m_pTextureCom->SetUp_OnShader(pEffect, "g_BaseTexture");

	pEffect->SetMatrix("g_matWorld", m_pTransformCom->Get_WorldMatrixPointer());
	pEffect->SetMatrix("g_matView", &matTmp);
	pEffect->SetMatrix("g_matProj", &matTmp);


	Safe_Release(pEffect);

	return NOERROR;
}

CBack_Logo * CBack_Logo::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBack_Logo* pInstance = new CBack_Logo(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		_MSGBOX("CBack_Logo Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CBack_Logo::Clone(void* pArg)
{
	CBack_Logo* pInstance = new CBack_Logo(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		_MSGBOX("CBack_Logo Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBack_Logo::Free()
{
	Safe_Release(m_pBufferBGCom);

	CUI::Free();
}
