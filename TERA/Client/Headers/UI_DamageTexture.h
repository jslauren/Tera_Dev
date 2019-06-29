#pragma once
#include "UI.h"

_BEGIN(Client)

class UI_DamageTexture : public CUI
{
public:
	explicit UI_DamageTexture(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit UI_DamageTexture(const UI_DamageTexture& rhs);
	virtual ~UI_DamageTexture() = default;

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
	CBuffer_RcTex*	m_pBufferDamageCom = nullptr;
	CTexture*		m_pTextureDamageTexCom = nullptr;

public:
	static UI_DamageTexture*	Create(LPDIRECT3DDEVICE9 pGraphic_Device, void * pArg = nullptr);
	virtual CGameObject*		Clone(void * pArg = nullptr) override;
	virtual void				Free();

};

_END
