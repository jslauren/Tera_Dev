#pragma once
#include "UI.h"

_BEGIN(Client)

class CPlayer;
class CUI_PlayerPoint final : public CUI
{
private:
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

	CTransform*			m_pTransformHpCom = nullptr;
	CBuffer_RcTex*		m_pBufferHpCom = nullptr;
	CTexture*			m_pTextureHpCom = nullptr;

	CTransform*			m_pTransformMpCom = nullptr;
	CBuffer_RcTex*		m_pBufferMpCom = nullptr;
	CTexture*			m_pTextureMpCom = nullptr;

private:
	HRESULT				NullCheck();
	void				PointCalculater(_bool bIsHP, _float fCurrentValue);

private:
	_float				m_fTimeDelta = 1.f;

	_float				m_fHP = 10686.f;
	_float				m_fMP = 3250.f;

	_float				m_fCalculatedHP = 0.f;
	_float				m_fCalculatedMP = 1.f;

	_float				m_fHPRatio = 0.f;
	_float				m_fMPRatio = 1.f;

	_tchar				m_szHP[64];

public:
	static CUI_PlayerPoint*		Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*		Clone(void * pArg = nullptr) override;
	virtual void				Free();

};

_END