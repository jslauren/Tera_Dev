#include "stdafx.h"
#include "..\Headers\UI_DamageTexture.h"

_USING(Client)

UI_DamageTexture::UI_DamageTexture(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUI(pGraphic_Device)
{
}

UI_DamageTexture::UI_DamageTexture(const UI_DamageTexture & rhs)
	: CUI(rhs)
{
}

HRESULT UI_DamageTexture::Ready_GameObject_Prototype()
{
	if (FAILED(CGameObject::Ready_GameObject_Prototype()))
		return E_FAIL;

	return NOERROR;
}

HRESULT UI_DamageTexture::Ready_GameObject(void * pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	_vec4 vInfo = (*(_vec4*)(pArg));

	// 앞에 3개는 포지션, 뒤에 하나는 텍스쳐 번호.

	return NOERROR;
}

_int UI_DamageTexture::Update_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pTransformCom)
		return -1;

	return _int();
}

_int UI_DamageTexture::LateUpdate_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this)))
		return -1;

	return _int();
}

HRESULT UI_DamageTexture::Render_GameObject()
{
	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	if (nullptr == pEffect)
		return E_FAIL;

	pEffect->AddRef();


	if (FAILED(SetUp_ConstantTable(pEffect)))
		return E_FAIL;

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);

	//m_pBufferSkillBoardCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);

	return NOERROR;
}

HRESULT UI_DamageTexture::Add_Component()
{
	CUI::Add_Component();

	// For.Com_Buffer_Damage_Texture
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Buffer_UI_Damage_Texture", L"Com_Buffer_Damage_Texture", (CComponent**)&m_pBufferDamageCom)))
		return E_FAIL;

	// For.Com_Damage_Texture
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Texture_UI_Damage", L"Com_Damage_Texture", (CComponent**)&m_pTextureDamageTexCom)))
		return E_FAIL;

	return NOERROR;
}

HRESULT UI_DamageTexture::SetUp_ConstantTable(LPD3DXEFFECT pEffect, const _uint iTargetTextureIdx)
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
		m_pTextureDamageTexCom->SetUp_OnShader(pEffect, "g_BaseTexture");

		pEffect->SetMatrix("g_matWorld", m_pTransformCom->Get_WorldMatrixPointer());
		pEffect->SetMatrix("g_matView", &matTmp);
		pEffect->SetMatrix("g_matProj", &matProj);
	}
	
	Safe_Release(pEffect);

	return NOERROR;
}

UI_DamageTexture * UI_DamageTexture::Create(LPDIRECT3DDEVICE9 pGraphic_Device, void * pArg)
{
	UI_DamageTexture* pInstance = new UI_DamageTexture(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		_MSGBOX("UI_DamageTexture Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * UI_DamageTexture::Clone(void * pArg)
{
	UI_DamageTexture* pInstance = new UI_DamageTexture(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		_MSGBOX("UI_DamageTexture Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void UI_DamageTexture::Free()
{
	Safe_Release(m_pBufferDamageCom);
	Safe_Release(m_pTextureDamageTexCom);

	CUI::Free();
}

