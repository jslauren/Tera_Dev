#pragma once
#include "UI.h"

_BEGIN(Client)

class UI_DamageFont : public CUI
{
public:
	explicit UI_DamageFont(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit UI_DamageFont(const UI_DamageFont& rhs);
	virtual ~UI_DamageFont() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	virtual _int	Update_GameObject(const _float& fTimeDelta);
	virtual _int	LateUpdate_GameObject(const _float& fTimeDelta);
	virtual HRESULT Render_GameObject();

public:
	virtual HRESULT	Add_Component();
	virtual HRESULT	SetUp_ConstantTable(LPD3DXEFFECT pEffect, const _uint iTargetTextureIdx = 1);

private:
	list<CGameObject*>	m_DamageFontList;

public:
	static UI_DamageFont*		Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*		Clone(void * pArg = nullptr) override;
	virtual void				Free();

};

_END
