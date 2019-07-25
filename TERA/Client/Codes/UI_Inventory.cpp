#include "stdafx.h"
#include "..\Headers\UI_Inventory.h"
#include "Input_Device.h"
#include "UI_MouseCursor.h"

_USING(Client)

CUI_Inventory::CUI_Inventory(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUI(pGraphic_Device)
{
	GetCursorPos(&m_ptFirstMousePos);
	ScreenToClient(g_hWnd, &m_ptFirstMousePos);

	m_ptFirstMousePos.x = (m_ptFirstMousePos.x - (g_iWinCX * 0.5f)) + 10.f;
	m_ptFirstMousePos.y = (m_ptFirstMousePos.y - (g_iWinCY * 0.5f));

	m_ptCurrentMousePos.x = 0;
	m_ptCurrentMousePos.y = 0;
}

CUI_Inventory::CUI_Inventory(const CUI_Inventory & rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Inventory::Ready_GameObject_Prototype()
{
	if (FAILED(CGameObject::Ready_GameObject_Prototype()))
		return E_FAIL;

	return NOERROR;
}

HRESULT CUI_Inventory::Ready_GameObject(void * pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pTransformCom->Set_Scaling((g_iWinCX * 0.25f), (g_iWinCY * 0.5f), 0.f);
	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(0.f, 0.f, 0.f));

	return NOERROR;
}

_int CUI_Inventory::Update_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pTransformCom)
		return -1;

	ClickedEvent();
	
	if (CInput_Device::GetInstance()->Get_DIKeyDown(DIK_I))
	{
		m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(0.f, 0.f, 0.f));

		ValueFree();

		m_bIsRender = !m_bIsRender;
		CUI_MouseCursor* pUI_Mouse = dynamic_cast<CUI_MouseCursor*>(CObject_Manager::GetInstance()->Get_Object(SCENE_STATIC, L"Layer_UI", 3));

		pUI_Mouse->Set_MouseRenderInfo();
	}

	return _int();
}

_int CUI_Inventory::LateUpdate_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this)))
		return -1;

	return _int();
}

HRESULT CUI_Inventory::Render_GameObject()
{
	if(m_bIsRender == false)
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

HRESULT CUI_Inventory::Add_Component()
{
	CUI::Add_Component();

	// For.Com_Buffer_Inventory
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Buffer_UI_Inventory", L"Com_Buffer_Inventory", (CComponent**)&m_pBufferCom)))
		return E_FAIL;

	// For.Com_Texture_Inventory
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Texture_UI_Inventory", L"Com_Texture_Inventory", (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CUI_Inventory::SetUp_ConstantTable(LPD3DXEFFECT pEffect, const _uint iTargetTextureIdx)
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

void CUI_Inventory::ClickedEvent()
{
	if (CInput_Device::GetInstance()->Get_DIMouseDown(CInput_Device::MOUSEBUTTON::DIM_LBUTTON))
	{
		GetCursorPos(&m_ptFirstMousePos);
		ScreenToClient(g_hWnd, &m_ptFirstMousePos);

		RECT rcTemp;
		rcTemp.left = (_long)(m_rcFrame.left - m_vCurrentPos.x);
		rcTemp.top = (_long)(m_rcFrame.top - m_vCurrentPos.y);
		rcTemp.right = (_long)(m_rcFrame.right - m_vCurrentPos.x);
		rcTemp.bottom = (_long)(m_rcFrame.bottom - m_vCurrentPos.y);

		if (PtInRect(&rcTemp, m_ptFirstMousePos) == true)
			m_bIsClicked = true;
	}
	else if (CInput_Device::GetInstance()->Get_DIMouseUp(CInput_Device::MOUSEBUTTON::DIM_LBUTTON))
		m_bIsClicked = false;


	if (m_bIsClicked == true)
	{
		GetCursorPos(&m_ptCurrentMousePos);
		ScreenToClient(g_hWnd, &m_ptCurrentMousePos);

		m_vMovedPos.x = m_ptFirstMousePos.x - m_ptCurrentMousePos.x;
		m_vMovedPos.y = m_ptFirstMousePos.y - m_ptCurrentMousePos.y;

		if ((m_vMovedPos.x != 0) ||
			(m_vMovedPos.y != 0))
		{
			m_bIsAcc = true;

			if (m_bIsAcc == true)
			{
				m_vCurrentPos.x += m_vMovedPos.x;
				m_vCurrentPos.y += m_vMovedPos.y;
				m_ptFirstMousePos.x = m_ptCurrentMousePos.x;
				m_ptFirstMousePos.y = m_ptCurrentMousePos.y;
				m_bIsAcc = false;
			}

			m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(-m_vCurrentPos.x, m_vCurrentPos.y, 0.f));
		}
	}
}

void CUI_Inventory::ValueFree()
{
	m_ptFirstMousePos.x = 0;
	m_ptFirstMousePos.y = 0;

	m_ptCurrentMousePos.x = 0;
	m_ptCurrentMousePos.y = 0;

	m_vCurrentPos.x = 0;
	m_vCurrentPos.y = 0;

	m_vMovedPos.x = 0;
	m_vMovedPos.y = 0;

	m_bIsClicked = false;
	m_bIsAcc = false;
}

CUI_Inventory * CUI_Inventory::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUI_Inventory* pInstance = new CUI_Inventory(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		_MSGBOX("CUI_Inventory Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CUI_Inventory::Clone(void * pArg)
{
	CUI_Inventory* pInstance = new CUI_Inventory(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		_MSGBOX("CUI_Inventory Cloned Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Inventory::Free()
{
	Safe_Release(m_pBufferCom);

	CUI::Free();
}
