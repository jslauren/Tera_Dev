#include "stdafx.h"
#include "..\Headers\UI_MouseCursor.h"

_USING(Client)

CUI_MouseCursor::CUI_MouseCursor(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUI(pGraphic_Device)
{
	m_ptMousePos.x = 0;
	m_ptMousePos.y = 0;
}

CUI_MouseCursor::CUI_MouseCursor(const CUI_MouseCursor & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_MouseCursor::Ready_GameObject_Prototype()
{
	if (FAILED(CGameObject::Ready_GameObject_Prototype()))
		return E_FAIL;

	return NOERROR;
}

HRESULT CUI_MouseCursor::Ready_GameObject(void * pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pTransformCom->Set_Scaling((g_iWinCX * 0.025f), (g_iWinCY * 0.05f), 0.f);
	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3((g_iWinCX * 0.05f), (g_iWinCY * 0.05f), 0.f));

	return NOERROR;
}

_int CUI_MouseCursor::Update_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pTransformCom)
		return -1;

	GetCursorPos(&m_ptMousePos);
	ScreenToClient(g_hWnd, &m_ptMousePos);

	_float fX = (m_ptMousePos.x - (g_iWinCX * 0.5f)) + 10.f;
	_float fY = (m_ptMousePos.y - (g_iWinCY * 0.5f));

	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(fX , -fY, 0.f));
	
	return _int();
}

_int CUI_MouseCursor::LateUpdate_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this)))
		return -1;

	return _int();
}

HRESULT CUI_MouseCursor::Render_GameObject()
{
	if (m_bIsRender == false)
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

	return NOERROR;
}

HRESULT CUI_MouseCursor::Add_Component()
{
	CUI::Add_Component();

	// For.Com_Buffer_Mouse
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Buffer_UI_Mouse", L"Com_Buffer_Mouse", (CComponent**)&m_pBufferCom)))
		return E_FAIL;

	// For.Com_Texture_Mouse
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Texture_UI_Mouse", L"Com_Texture_Mouse", (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CUI_MouseCursor::SetUp_ConstantTable(LPD3DXEFFECT pEffect, const _uint iTargetTextureIdx)
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

CUI_MouseCursor * CUI_MouseCursor::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUI_MouseCursor* pInstance = new CUI_MouseCursor(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		_MSGBOX("CUI_MouseCursor Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CUI_MouseCursor::Clone(void * pArg)
{
	CUI_MouseCursor* pInstance = new CUI_MouseCursor(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		_MSGBOX("CUI_MouseCursor Cloned Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_MouseCursor::Free()
{
	Safe_Release(m_pBufferCom);

	CUI::Free();
}
