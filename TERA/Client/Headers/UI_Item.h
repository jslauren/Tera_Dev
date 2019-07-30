#pragma once
#include "UI.h"

_BEGIN(Client)

class CUI_Item final : public CUI
{
private:
	explicit CUI_Item(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CUI_Item(const CUI_Item& rhs);
	virtual ~CUI_Item() = default;

public:	// Getter
	_bool			Get_RenderStartInfo() { return m_bIsRenderStart; }
	RECT*			Get_RectInfo() { return &m_rcFrame; }

public:	// Setter
	void			Set_MovedRectInfo(RECT* rcPos);

public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	virtual _int	Update_GameObject(const _float& fTimeDelta);
	virtual _int	LateUpdate_GameObject(const _float& fTimeDelta);
	virtual HRESULT Render_GameObject();

private:
	virtual HRESULT	Add_Component();
	virtual HRESULT	SetUp_ConstantTable(LPD3DXEFFECT pEffect, const _uint iTargetTextureIdx = 1);

	void			ClickEvent();

private:
	CBuffer_RcTex*		m_pBufferCom = nullptr;

private:
	RECT				m_rcFrame;
	RECT				m_rcMovedFrame;
	_bool				m_bIsRender = false;
	_bool				m_bIsRenderStart = false;
	POINT				m_ptMousePos;

public:
	static CUI_Item*		Create(LPDIRECT3DDEVICE9 pGraphic_Device, void * pArg = nullptr);
	virtual CGameObject*	Clone(void * pArg = nullptr) override;
	virtual void			Free();

};

_END