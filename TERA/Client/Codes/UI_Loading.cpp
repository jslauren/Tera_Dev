#include "stdafx.h"
#include "..\Headers\UI_Loading.h"

_USING(Client)

CUI_Loading::CUI_Loading(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUI(pGraphic_Device)
{
}

CUI_Loading::CUI_Loading(const CUI_Loading & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Loading::Ready_GameObject_Prototype()
{
	if (FAILED(CGameObject::Ready_GameObject_Prototype()))
		return E_FAIL;

	return NOERROR;
}

HRESULT CUI_Loading::Ready_GameObject(void * pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pTransformCom->Set_Scaling(2.f, 2.f, 2.f);
	
	return NOERROR;
}

_int CUI_Loading::Update_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pTransformCom)
		return -1;

	return _int();
}

_int CUI_Loading::LateUpdate_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_PRIORITY, this)))
		return -1;

	return _int();
}

HRESULT CUI_Loading::Render_GameObject()
{
	//if (FAILED(NullCheck()))
	//	return E_FAIL;

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

	//{
	//	if (FAILED(SetUp_ConstantTable(pEffect, 2)))
	//		return E_FAIL;

	//	pEffect->Begin(nullptr, 0);
	//	pEffect->BeginPass(0);

	//	m_pBufferBarFrameCom->Render_Buffer();

	//	pEffect->EndPass();
	//	pEffect->End();
	//}

	//{
	//	if (FAILED(SetUp_ConstantTable(pEffect, 3)))
	//		return E_FAIL;

	//	pEffect->Begin(nullptr, 0);
	//	pEffect->BeginPass(0);

	//	m_pBufferBarFilterCom->Render_Buffer();

	//	pEffect->EndPass();
	//	pEffect->End();
	//}

	//{
	//	if (FAILED(SetUp_ConstantTable(pEffect, 4)))
	//		return E_FAIL;

	//	pEffect->Begin(nullptr, 0);
	//	pEffect->BeginPass(0);

	//	m_pBufferBarGaugeCom->Render_Buffer();

	//	pEffect->EndPass();
	//	pEffect->End();
	//}


	Safe_Release(pEffect);

	return NOERROR;
}

HRESULT CUI_Loading::Add_Component()
{
	CUI::Add_Component();

	// For.Com_Transform
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_TransformBarFrame", (CComponent**)&m_pTransformBarFrameCom)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_TransformBarFilter", (CComponent**)&m_pTransformBarFilterCom)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_TransformBarGauge", (CComponent**)&m_pTransformBarGaugeCom)))
		return E_FAIL;

	// For.Com_Buffer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Buffer_Background", L"Com_Buffer", (CComponent**)&m_pBufferBGCom)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Buffer_UI_Bar_Frame", L"Com_BufferBarFrame", (CComponent**)&m_pBufferBarFrameCom)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Buffer_UI_Loading_Filter", L"Com_BufferBarFilter", (CComponent**)&m_pBufferBarFilterCom)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Buffer_UI_Loading_Gauge", L"Com_BufferBarGauge", (CComponent**)&m_pBufferBarGaugeCom)))
		return E_FAIL;

	// For.Com_Texture
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Texture_Loading_Main", L"Com_Texture", (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Texture_Bar_Frame", L"Com_TextureBarFrame", (CComponent**)&m_pTextureBarFrameCom)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Texture_Bar_Filter", L"Com_TextureBarFilter", (CComponent**)&m_pTextureBarFilterCom)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Texture_Bar_Gauge", L"Com_TextureBarGauge", (CComponent**)&m_pTextureBarGaugeCom)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CUI_Loading::SetUp_ConstantTable(LPD3DXEFFECT pEffect, const _uint iTargetTextureIdx)
{
	if (nullptr == pEffect)
		return E_FAIL;

	if (1 == iTargetTextureIdx)
	{
		pEffect->AddRef();

		_matrix		matTmp;
		D3DXMatrixIdentity(&matTmp);

		m_pTextureCom->SetUp_OnShader(pEffect, "g_BaseTexture");

		pEffect->SetMatrix("g_matWorld", m_pTransformCom->Get_WorldMatrixPointer());
		pEffect->SetMatrix("g_matView", &matTmp);
		pEffect->SetMatrix("g_matProj", &matTmp);

		Safe_Release(pEffect);
	}

	return NOERROR;
}

HRESULT CUI_Loading::NullCheck()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	if (nullptr == m_pTextureCom)
		return E_FAIL;

	if (nullptr == m_pBufferBGCom)
		return E_FAIL;
	
	if (nullptr == m_pTransformBarFrameCom)
		return E_FAIL;

	if (nullptr == m_pBufferBarFrameCom)
		return E_FAIL;

	if (nullptr == m_pTextureBarFrameCom)
		return E_FAIL;

	if (nullptr == m_pTransformBarFilterCom)
		return E_FAIL;

	if (nullptr == m_pBufferBarFilterCom)
		return E_FAIL;

	if (nullptr == m_pTextureBarFilterCom)
		return E_FAIL;

	if (nullptr == m_pTransformBarGaugeCom)
		return E_FAIL;

	if (nullptr == m_pBufferBarGaugeCom)
		return E_FAIL;

	if (nullptr == m_pTextureBarGaugeCom)
		return E_FAIL;

	return NOERROR;
}

CUI_Loading * CUI_Loading::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUI_Loading* pInstance = new CUI_Loading(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		_MSGBOX("CUI_Loading Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CUI_Loading::Clone(void * pArg)
{
	CUI_Loading* pInstance = new CUI_Loading(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		_MSGBOX("CUI_Loading Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Loading::Free()
{
	Safe_Release(m_pBufferBGCom);
	Safe_Release(m_pBufferBarFrameCom);
	Safe_Release(m_pBufferBarFilterCom);
	Safe_Release(m_pBufferBarGaugeCom);

	Safe_Release(m_pTransformBarFrameCom);
	Safe_Release(m_pTransformBarFilterCom);
	Safe_Release(m_pTransformBarGaugeCom);

	Safe_Release(m_pTextureBarFrameCom);
	Safe_Release(m_pTextureBarFilterCom);
	Safe_Release(m_pTextureBarGaugeCom);

	CUI::Free();
}
