#include "stdafx.h"
#include "..\Headers\UI_PlayerPoint.h"

_USING(Client)

CUI_PlayerPoint::CUI_PlayerPoint(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUI(pGraphic_Device)
{
}

CUI_PlayerPoint::CUI_PlayerPoint(const CUI_PlayerPoint & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_PlayerPoint::Ready_GameObject_Prototype()
{
	if (FAILED(CGameObject::Ready_GameObject_Prototype()))
		return E_FAIL;

	return NOERROR;
}

HRESULT CUI_PlayerPoint::Ready_GameObject(void * pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	return NOERROR;
}

_int CUI_PlayerPoint::Update_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pTransformCom)
		return -1;

	return _int();
}

_int CUI_PlayerPoint::LateUpdate_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this)))
		return -1;

	return _int();
}

HRESULT CUI_PlayerPoint::Render_GameObject()
{
	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	if (nullptr == pEffect)
		return E_FAIL;

	pEffect->AddRef();


	if (FAILED(SetUp_ConstantTable(pEffect)))
		return E_FAIL;

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);

//	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);

	return NOERROR;
}

HRESULT CUI_PlayerPoint::Add_Component()
{
	CUI::Add_Component();

	return NOERROR;
}

HRESULT CUI_PlayerPoint::SetUp_ConstantTable(LPD3DXEFFECT pEffect, const _uint iTargetTextureIdx)
{
	if (nullptr == pEffect)
		return E_FAIL;

	pEffect->AddRef();

	_matrix		matTmp, matProj;
	D3DXMatrixIdentity(&matTmp);

	// UI는 직교투영을 해야하기 때문에 이렇게 처리해준다.
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixOrthoLH(&matProj, g_iWinCX, g_iWinCY, 0.f, 1.f);

	if (1 == iTargetTextureIdx)
	{
		m_pTextureCom->SetUp_OnShader(pEffect, "g_BaseTexture");

		pEffect->SetMatrix("g_matWorld", m_pTransformCom->Get_WorldMatrixPointer());
		pEffect->SetMatrix("g_matView", &matTmp);
		pEffect->SetMatrix("g_matProj", &matTmp);
	}

	Safe_Release(pEffect);

	return NOERROR;
}

CUI_PlayerPoint * CUI_PlayerPoint::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUI_PlayerPoint* pInstance = new CUI_PlayerPoint(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		_MSGBOX("CUI_PlayerPoint Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CUI_PlayerPoint::Clone(void * pArg)
{
	CUI_PlayerPoint* pInstance = new CUI_PlayerPoint(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		_MSGBOX("CUI_PlayerPoint Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_PlayerPoint::Free()
{
	CUI::Free();
}
