#pragma once
#include "UI.h"

_BEGIN(Client)

class CUI_DamageTexture : public CUI
{
public:
	explicit CUI_DamageTexture(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CUI_DamageTexture(const CUI_DamageTexture& rhs);
	virtual ~CUI_DamageTexture() = default;

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
	CTexture*		m_pTextureDamageCom = nullptr;

	CTransform*		m_pTransformOneCom = nullptr;
	CBuffer_RcTex*	m_pBufferOneCom = nullptr;

	CTransform*		m_pTransformTenCom = nullptr;
	CBuffer_RcTex*	m_pBufferTenCom = nullptr;

	CTransform*		m_pTransformHndrCom = nullptr;
	CBuffer_RcTex*	m_pBufferHndrCom = nullptr;

	CTransform*		m_pTransformThsnCom = nullptr;
	CBuffer_RcTex*	m_pBufferThsnCom = nullptr;

private:
	_vec3			m_vPosition;

	_uint			m_iDamageValue = 0.f;
	_tchar			m_szDamageValue[64];
	_uint			m_iSeprateDamage[4];
	_uint			m_iNumberUnit = 0;

	_float			m_fMaxTime = 3.7f;
	_float			m_fTimeAcc = 0.f;
	_float			m_fCurrentFontSize = 50.f;
	_float			m_fMinFontSize = 50.f;
	_float			m_fMaxFontSize = 75.f;
	_bool			m_bIsSub = false;
	_float			m_fFontPosition = 0.f;
	_float			m_fMaxFontPosition = 1.5f;
	_float			m_fFontAlpha = 0.f;
	_float			m_fFontStayTimeAcc = 0.f;


private:
	HRESULT			NullCheck();
	void			SeprateDamageValue();
	void			SetDamageTransform();

public:
	static CUI_DamageTexture*	Create(LPDIRECT3DDEVICE9 pGraphic_Device, void * pArg = nullptr);
	virtual CGameObject*		Clone(void * pArg = nullptr) override;
	virtual void				Free();

};

_END
