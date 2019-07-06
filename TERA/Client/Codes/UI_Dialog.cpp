#include "stdafx.h"
#include "..\Headers\UI_Dialog.h"
#include "FontManager.h"

_USING(Client)

CUI_Dialog::CUI_Dialog(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUI(pGraphic_Device)
{
	ZeroMemory(m_szDialogTitle, sizeof(_tchar) * 64);
}

CUI_Dialog::CUI_Dialog(const CUI_Dialog & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Dialog::Ready_GameObject_Prototype()
{
	if (FAILED(CGameObject::Ready_GameObject_Prototype()))
		return E_FAIL;

	return NOERROR;
}

HRESULT CUI_Dialog::Ready_GameObject(void * pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pTransformCom->Set_Scaling((g_iWinCX * 0.2f), (g_iWinCY * 0.5f), 0.f);
	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3((g_iWinCX * -(0.4f)), (g_iWinCY * 0.17f), 0.f));

	return NOERROR;
}

_int CUI_Dialog::Update_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pTransformCom)
		return -1;

	return _int();
}

_int CUI_Dialog::LateUpdate_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this)))
		return -1;

	return _int();
}

HRESULT CUI_Dialog::Render_GameObject()
{
	if (m_bIsTalkEventAvaliable != true)
		return NOERROR;

	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	if (nullptr == pEffect)
		return E_FAIL;

	pEffect->AddRef();

	if (FAILED(SetUp_ConstantTable(pEffect)))
		return E_FAIL;

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);

	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);

	lstrcpy(m_szDialogTitle, L"아르커스 사냥");

	// Dialog Script //
	CFontManager::GetInstance()->RenderFont(CFontManager::FONT_NAME, _vec3((g_iWinCX * 0.05f), (g_iWinCY * 0.175f), 0.f), m_szDialogTitle);


	return NOERROR;
}

HRESULT CUI_Dialog::Add_Component()
{
	CUI::Add_Component();

	// For.Com_Buffer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Buffer_UI_Dialog", L"Com_Buffer_Dialog", (CComponent**)&m_pBufferCom)))
		return E_FAIL;

	// For.Com_Texture
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Texture_UI_Dialog", L"Com_Texture_Dialog", (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CUI_Dialog::SetUp_ConstantTable(LPD3DXEFFECT pEffect, const _uint iTargetTextureIdx)
{
	if (nullptr == pEffect)
		return E_FAIL;

	pEffect->AddRef();

	_matrix		matTmp, matProj;
	D3DXMatrixIdentity(&matTmp);

	// UI는 직교투영을 해야하기 때문에 이렇게 처리해준다.
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixOrthoLH(&matProj, (_float)g_iWinCX, (_float)g_iWinCY, 0.f, 1.f);

	if (1 == iTargetTextureIdx)
	{
		m_pTextureCom->SetUp_OnShader(pEffect, "g_BaseTexture");

		pEffect->SetMatrix("g_matWorld", m_pTransformCom->Get_WorldMatrixPointer());
		pEffect->SetMatrix("g_matView", &matTmp);
		pEffect->SetMatrix("g_matProj", &matProj);
	}

	Safe_Release(pEffect);

	return NOERROR;
}

CUI_Dialog * CUI_Dialog::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUI_Dialog* pInstance = new CUI_Dialog(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		_MSGBOX("CUI_Dialog Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CUI_Dialog::Clone(void * pArg)
{
	CUI_Dialog* pInstance = new CUI_Dialog(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		_MSGBOX("CUI_Dialog Cloned Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Dialog::Free()
{
	Safe_Release(m_pBufferCom);
	Safe_Release(m_pTextureCom);

	CUI::Free();
}
