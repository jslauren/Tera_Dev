#pragma once
#include "Defines.h"
#include "GameObject.h"
#include "Buffer_RcTex.h"
#include "Buffer_ScreenTex.h"
#include "UI.h"

_BEGIN(Client)

class CUI_Loading : public CUI
{
public:
	explicit CUI_Loading(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CUI_Loading(const CUI_Loading& rhs);
	virtual ~CUI_Loading() = default;

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
	CBuffer_RcTex*		m_pBufferBGCom = nullptr;

	CTransform*			m_pTransformBarFrameCom = nullptr;
	CBuffer_ScreenTex*	m_pBufferBarFrameCom = nullptr;
	CTexture*			m_pTextureBarFrameCom = nullptr;

	CTransform*			m_pTransformBarFilterCom = nullptr;
	CBuffer_ScreenTex*	m_pBufferBarFilterCom = nullptr;
	CTexture*			m_pTextureBarFilterCom = nullptr;

	CTransform*			m_pTransformBarGaugeCom = nullptr;
	CBuffer_ScreenTex*	m_pBufferBarGaugeCom = nullptr;
	CTexture*			m_pTextureBarGaugeCom = nullptr;

private:
	HRESULT			NullCheck();

private:
	_int			m_iLoadingProgressValue = 0;

public:
	static CUI_Loading*		Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone(void * pArg = nullptr) override;
	virtual void			Free();

};

_END