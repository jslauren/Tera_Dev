#pragma once
#include "UI.h"

_BEGIN(Client)

class CUI_PlayerPoint : public CUI
{
public:
	explicit CUI_PlayerPoint(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CUI_PlayerPoint(const CUI_PlayerPoint& rhs);
	virtual ~CUI_PlayerPoint() = default;

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
	CTransform*			m_pTransformPointBoardCom = nullptr;
	CBuffer_RcTex*		m_pBufferPointBoardCom = nullptr;
	CTexture*			m_pTexturePointBoardCom = nullptr;

	CTransform*			m_pTransformHpFilterCom = nullptr;
	CBuffer_RcTex*		m_pBufferHpFilterCom = nullptr;
	CTexture*			m_pTextureHpFilterCom = nullptr;

	CTransform*			m_pTransformMpFilterCom = nullptr;
	CBuffer_RcTex*		m_pBufferMpFilterCom = nullptr;
	CTexture*			m_pTextureMpFilterCom = nullptr;

private:
	HRESULT				NullCheck();

public:
	static CUI_PlayerPoint*		Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*		Clone(void * pArg = nullptr) override;
	virtual void				Free();

};

_END