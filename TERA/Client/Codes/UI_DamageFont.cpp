#include "stdafx.h"
#include "..\Headers\UI_DamageFont.h"
#include "Management.h"
#include "Camera_Dynamic.h"
#include "UI_DamageTexture.h"

_USING(Client)


UI_DamageFont::UI_DamageFont(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUI(pGraphic_Device)
{
}

UI_DamageFont::UI_DamageFont(const UI_DamageFont & rhs)
	: CUI(rhs)
{
}

HRESULT UI_DamageFont::Ready_GameObject_Prototype()
{
	return E_NOTIMPL;
}

HRESULT UI_DamageFont::Ready_GameObject(void * pArg)
{
	_vec4 vInfo = (*(_vec4*)(pArg));
	
	_vec3 vInfoV;
	vInfoV.x = vInfo.x;
	vInfoV.y = vInfo.y;
	vInfoV.z = vInfo.z;

	_vec3 vOut;

	_matrix	matWorld, matView, matProj;
	D3DVIEWPORT9 viewPort;

	ZeroMemory(&viewPort, sizeof(D3DVIEWPORT9));

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &matView);
	m_pGraphic_Device->GetViewport(&viewPort);

	D3DXMatrixIdentity(&matWorld);

	D3DXVec3Project(&vOut, &vInfoV, &viewPort, &matProj, &matView, &matWorld);

	vOut.x -= (g_iWinCX / 2);
	vOut.y *= -1;
	vOut.y += (g_iWinCY / 2);

	m_DamageFontList.push_back(UI_DamageTexture::Create(m_pGraphic_Device, &_vec4(vOut, vInfo.w)));
	
	return E_NOTIMPL;
}

_int UI_DamageFont::Update_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pTransformCom)
		return -1;

	for (auto iter : m_DamageFontList)
		iter->Update_GameObject(fTimeDelta);

	return _int();
}

_int UI_DamageFont::LateUpdate_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	for (auto iter : m_DamageFontList)
		iter->LateUpdate_GameObject(fTimeDelta);

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this)))
		return -1;

	return _int();
}

HRESULT UI_DamageFont::Render_GameObject()
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

HRESULT UI_DamageFont::Add_Component()
{
	return NOERROR;
}

HRESULT UI_DamageFont::SetUp_ConstantTable(LPD3DXEFFECT pEffect, const _uint iTargetTextureIdx)
{
	return NOERROR;
}

UI_DamageFont * UI_DamageFont::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	UI_DamageFont* pInstance = new UI_DamageFont(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		_MSGBOX("UI_DamageFont Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * UI_DamageFont::Clone(void * pArg)
{
	UI_DamageFont* pInstance = new UI_DamageFont(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		_MSGBOX("UI_DamageFont Cloned Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void UI_DamageFont::Free()
{
	CUI::Free();
}
