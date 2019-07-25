#pragma once
#include "UI.h"

_BEGIN(Client)

class CUI_MouseCursor final : public CUI
{
private:
	explicit CUI_MouseCursor(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CUI_MouseCursor(const CUI_MouseCursor& rhs);
	virtual ~CUI_MouseCursor() = default;

public:	// Getter
	_bool			Get_MouseRenderInfo() { return m_bIsRender; }

public:	// Setter
	void			Set_MouseRenderInfo() { m_bIsRender = !m_bIsRender; }

public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	virtual _int	Update_GameObject(const _float& fTimeDelta);
	virtual _int	LateUpdate_GameObject(const _float& fTimeDelta);
	virtual HRESULT Render_GameObject();

private:
	virtual HRESULT	Add_Component();
	virtual HRESULT	SetUp_ConstantTable(LPD3DXEFFECT pEffect, const _uint iTargetTextureIdx = 1);

private:
	CBuffer_RcTex*		m_pBufferCom = nullptr;

private:
	_bool				m_bIsRender = false;
	POINT				m_ptMousePos;

public:
	static CUI_MouseCursor*	Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone(void * pArg = nullptr) override;
	virtual void			Free();

};

_END