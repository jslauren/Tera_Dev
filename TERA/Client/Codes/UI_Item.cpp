#include "stdafx.h"
#include "..\Headers\UI_Item.h"
#include "Input_Device.h"

_USING(Client)

CUI_Item::CUI_Item(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUI(pGraphic_Device)
{
	m_ptMousePos.x = 0;
	m_ptMousePos.y = 0;
}

CUI_Item::CUI_Item(const CUI_Item & rhs)
	: CUI(rhs)
{
}

void CUI_Item::Set_MovedRectInfo(RECT * rcPos)
{
	m_rcMovedFrame.left = rcPos->left;
	m_rcMovedFrame.top = rcPos->top;
	m_rcMovedFrame.right = rcPos->right;
	m_rcMovedFrame.bottom = rcPos->bottom;
}

HRESULT CUI_Item::Ready_GameObject_Prototype()
{
	if (FAILED(CGameObject::Ready_GameObject_Prototype()))
		return E_FAIL;

	return NOERROR;
}

HRESULT CUI_Item::Ready_GameObject(void * pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_rcFrame = (*(RECT*)(pArg));

	//m_pTransformCom->Set_Scaling((g_iWinCX * 0.025f), (g_iWinCY * 0.05f), 0.f);
	//m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3((g_iWinCX * 0.05f), (g_iWinCY * 0.05f), 0.f));

	return NOERROR;
}

_int CUI_Item::Update_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pTransformCom)
		return -1;

	ClickEvent();

	return _int();
}

_int CUI_Item::LateUpdate_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this)))
		return -1;

	return _int();
}

HRESULT CUI_Item::Render_GameObject()
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

HRESULT CUI_Item::Add_Component()
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

HRESULT CUI_Item::SetUp_ConstantTable(LPD3DXEFFECT pEffect, const _uint iTargetTextureIdx)
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

void CUI_Item::ClickEvent()
{
	if (CInput_Device::GetInstance()->Get_DIMouseDown(CInput_Device::MOUSEBUTTON::DIM_LBUTTON))
	{
		GetCursorPos(&m_ptMousePos);
		ScreenToClient(g_hWnd, &m_ptMousePos);

		if (PtInRect(&m_rcMovedFrame, m_ptMousePos) == TRUE)
		{
			m_bIsRenderStart = true;
		}
	}
}

CUI_Item * CUI_Item::Create(LPDIRECT3DDEVICE9 pGraphic_Device, void* pArg)
{
	CUI_Item* pInstance = new CUI_Item(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		_MSGBOX("CUI_Item Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CUI_Item::Clone(void * pArg)
{
	AddRef();

	return this;
}

void CUI_Item::Free()
{
	Safe_Release(m_pBufferCom);

	CUI::Free();
}
