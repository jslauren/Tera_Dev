#include "stdafx.h"
#include "..\Headers\UI_Inventory.h"
#include "Input_Device.h"
#include "UI_MouseCursor.h"
#include "UI_Item.h"
#include "Object_Manager.h"
#include "Player.h"

_USING(Client)

CUI_Inventory::CUI_Inventory(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUI(pGraphic_Device)
{
	GetCursorPos(&m_ptFirstMousePos);
	ScreenToClient(g_hWnd, &m_ptFirstMousePos);

	m_ptFirstMousePos.x = (_long)(m_ptFirstMousePos.x - (g_iWinCX * 0.5)) + 10;
	m_ptFirstMousePos.y = (_long)(m_ptFirstMousePos.y - (g_iWinCY * 0.5));

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

	InventorySetting();

	m_pTransformCom->Set_Scaling((g_iWinCX * 0.25f), (g_iWinCY * 0.5f), 0.f);
	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(0.f, 0.f, 0.f));

	return NOERROR;
}

_int CUI_Inventory::Update_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pTransformCom)
		return -1;

	m_fTimeDelta = fTimeDelta;

	ClickedEvent();
		
	if (CInput_Device::GetInstance()->Get_DIKeyDown(DIK_I))
	{
		//ValueFree();
		HideInventory();
	}

	// 인벤토리가 활성화되어 있을때만 CUI_Item 클래스를 Update해준다.
	if (m_bIsRender == true)
		UpdateInventory();

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

HRESULT CUI_Inventory::InventorySetting()
{
	RECT	rcTemp = { 500, 325, 530, 355 };

	CUI_Item*	pItem = CUI_Item::Create(m_pGraphic_Device, &rcTemp);
	if (nullptr == pItem)
		return E_FAIL;

	m_ItemList.push_back(pItem);

	return NOERROR;
}

void CUI_Inventory::UpdateInventory()
{
	auto iter = m_ItemList.begin();
	auto iter_end = m_ItemList.end();

	for (; iter != iter_end; ++iter)
	{
		(*iter)->Update_GameObject(m_fTimeDelta);
	}
}

void CUI_Inventory::HideInventory()
{
	m_bIsRender = !m_bIsRender;
	CUI_MouseCursor* pUI_Mouse = dynamic_cast<CUI_MouseCursor*>(CObject_Manager::GetInstance()->Get_Object(SCENE_STATIC, L"Layer_UI", 3));

	pUI_Mouse->Set_MouseRenderInfo();
}

void CUI_Inventory::ClickedEvent()
{
	if (CInput_Device::GetInstance()->Get_DIMouseDown(CInput_Device::MOUSEBUTTON::DIM_LBUTTON))
	{
		GetCursorPos(&m_ptFirstMousePos);
		ScreenToClient(g_hWnd, &m_ptFirstMousePos);

		MoveInventory();
	}
	else if (CInput_Device::GetInstance()->Get_DIMouseUp(CInput_Device::MOUSEBUTTON::DIM_LBUTTON))
		m_bIsClicked = false;

	if (m_bIsClicked == true)
	{
		GetCursorPos(&m_ptCurrentMousePos);
		ScreenToClient(g_hWnd, &m_ptCurrentMousePos);

		m_vMovedPos.x = (_float)m_ptFirstMousePos.x - m_ptCurrentMousePos.x;
		m_vMovedPos.y = (_float)m_ptFirstMousePos.y - m_ptCurrentMousePos.y;

		if ((m_vMovedPos.x != 0) ||
			(m_vMovedPos.y != 0))
		{
			m_vCurrentPos.x += m_vMovedPos.x;
			m_vCurrentPos.y += m_vMovedPos.y;
			m_ptFirstMousePos.x = m_ptCurrentMousePos.x;
			m_ptFirstMousePos.y = m_ptCurrentMousePos.y;

			m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(-m_vCurrentPos.x, m_vCurrentPos.y, 0.f));
		}
	}
}

void CUI_Inventory::MoveInventory()
{
	RECT rcFrameTemp;
	rcFrameTemp.left = (_long)(m_rcFrame.left - m_vCurrentPos.x);
	rcFrameTemp.top = (_long)(m_rcFrame.top - m_vCurrentPos.y);
	rcFrameTemp.right = (_long)(m_rcFrame.right - m_vCurrentPos.x);
	rcFrameTemp.bottom = (_long)(m_rcFrame.bottom - m_vCurrentPos.y);

	if (PtInRect(&rcFrameTemp, m_ptFirstMousePos) == TRUE)
		m_bIsClicked = true;

	// X버튼 이동 부분.
	RECT rcExitTemp;
	rcExitTemp.left = (_long)(m_rcExitBtn.left - m_vCurrentPos.x);
	rcExitTemp.top = (_long)(m_rcExitBtn.top - m_vCurrentPos.y);
	rcExitTemp.right = (_long)(m_rcExitBtn.right - m_vCurrentPos.x);
	rcExitTemp.bottom = (_long)(m_rcExitBtn.bottom - m_vCurrentPos.y);

	// 인벤토리 칸 이동 부분.
	for (auto iter : m_ItemList)
	{
		RECT* rcItem = (iter->Get_RectInfo());
		RECT  rcItemTemp = { 0, 0, 0, 0 };

		rcItemTemp.left = (_long)(rcItem->left - m_vCurrentPos.x);
		rcItemTemp.top = (_long)(rcItem->top - m_vCurrentPos.y);
		rcItemTemp.right = (_long)(rcItem->right - m_vCurrentPos.x);
		rcItemTemp.bottom = (_long)(rcItem->bottom - m_vCurrentPos.y);

		iter->Set_MovedRectInfo(&rcItemTemp);
	}

	// X버튼 누르면 인벤토리 끄기.
	if (PtInRect(&rcExitTemp, m_ptFirstMousePos) == TRUE)
	{
		HideInventory();
		dynamic_cast<CPlayer*>(CObject_Manager::GetInstance()->Get_Object(SCENE_STATIC, L"Layer_Player"))->Set_HideInventoryInfo(true);
	}
}

void CUI_Inventory::ValueFree()
{
	m_pTransformCom->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(0.f, 0.f, 0.f));

	m_ptFirstMousePos.x = 0;
	m_ptFirstMousePos.y = 0;

	m_ptCurrentMousePos.x = 0;
	m_ptCurrentMousePos.y = 0;

	m_vCurrentPos.x = 0;
	m_vCurrentPos.y = 0;

	m_vMovedPos.x = 0;
	m_vMovedPos.y = 0;

	m_bIsClicked = false;
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

	for (auto& iter : m_ItemList)
		Safe_Release(iter);

	m_ItemList.clear();

	CUI::Free();
}
