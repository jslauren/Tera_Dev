#pragma once
#include "UI.h"
#include "UI_Item.h"

_BEGIN(Client)

class CUI_Inventory final : public CUI
{
private:
	explicit CUI_Inventory(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CUI_Inventory(const CUI_Inventory& rhs);
	virtual ~CUI_Inventory() = default;

public:	// Getter
	_bool			Get_InventoryRenderInfo() { return m_bIsRender; }

public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	virtual _int	Update_GameObject(const _float& fTimeDelta);
	virtual _int	LateUpdate_GameObject(const _float& fTimeDelta);
	virtual HRESULT Render_GameObject();

private:
	virtual HRESULT	Add_Component();
	virtual HRESULT	SetUp_ConstantTable(LPD3DXEFFECT pEffect, const _uint iTargetTextureIdx = 1);

	HRESULT			InventorySetting();
	void			UpdateInventory();
	void			HideInventory();
	void			ClickedEvent();
	void			MoveInventory();
	void			ValueFree();

private:
	CBuffer_RcTex*		m_pBufferCom = nullptr;

	list<CUI_Item*>		m_ItemList;

private:
	_float				m_fTimeDelta = 0.f;
	_bool				m_bIsRender = false;
	POINT				m_ptFirstMousePos;
	POINT				m_ptCurrentMousePos;
	RECT				m_rcFrame = { 580, 222, 750, 250 };
	RECT				m_rcExitBtn = {777, 226, 794, 244};

	_vec2				m_vCurrentPos = { 0, 0 };
	_vec2				m_vMovedPos = { 0, 0 };
	_bool				m_bIsClicked = false;

public:
	static CUI_Inventory*	Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone(void * pArg = nullptr) override;
	virtual void			Free();

};

_END