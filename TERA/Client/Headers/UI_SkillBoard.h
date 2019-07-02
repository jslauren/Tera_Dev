#pragma once
#include "UI.h"

_BEGIN(Client)

class CUI_SkillBoard : public CUI
{
private:
	explicit CUI_SkillBoard(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CUI_SkillBoard(const CUI_SkillBoard& rhs);
	virtual ~CUI_SkillBoard() = default;

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
	CTexture*			m_pTextureSkillBoardCom = nullptr;
	CTransform*			m_pTransformSkillBoardCom = nullptr;
	CBuffer_RcTex*		m_pBufferSkillBoardCom = nullptr;

	CTexture*			m_pTextureSkillFilterCom = nullptr;

	CTransform*			m_pTransformSkillFilter1Com = nullptr;
	CBuffer_RcTex*		m_pBufferSkillFilter1Com = nullptr;

	CTransform*			m_pTransformSkillFilter2Com = nullptr;
	CBuffer_RcTex*		m_pBufferSkillFilter2Com = nullptr;

	CTransform*			m_pTransformSkillFilter3Com = nullptr;
	CBuffer_RcTex*		m_pBufferSkillFilter3Com = nullptr;

	CTransform*			m_pTransformSkillFilter4Com = nullptr;
	CBuffer_RcTex*		m_pBufferSkillFilter4Com = nullptr;

	CTransform*			m_pTransformSkillFilter5Com = nullptr;
	CBuffer_RcTex*		m_pBufferSkillFilter5Com = nullptr;

	CTransform*			m_pTransformSkillFilter6Com = nullptr;
	CBuffer_RcTex*		m_pBufferSkillFilter6Com = nullptr;

	CTransform*			m_pTransformSkillFilter7Com = nullptr;
	CBuffer_RcTex*		m_pBufferSkillFilter7Com = nullptr;

	CTransform*			m_pTransformSkillFilter8Com = nullptr;
	CBuffer_RcTex*		m_pBufferSkillFilter8Com = nullptr;

	CTransform*			m_pTransformSkillFilter9Com = nullptr;
	CBuffer_RcTex*		m_pBufferSkillFilter9Com = nullptr;

private:
	HRESULT				NullCheck();
	void				Init_Filters_Transform();
	void				Renewal_PlayerSkill_CoolTime();

private:
	_float				m_fMaxCoolTime[9] = { 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f };
	_float				m_fCurrentCoolTime[9] = { 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f };
	_float				m_fCalculatedCoolTime[9] = { 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f };

	_tchar				m_szCutHead_CT[32];
	_tchar				m_szCuttingSlash_CT[32];
	_tchar				m_szFlatBlade_CT[32];
	_tchar				m_szHandySlash_CT[32];
	_tchar				m_szJawBreaker_CT[32];
	_tchar				m_szStingerBlade_CT[32];
	_tchar				m_szRagingStrike_CT[32];
	_tchar				m_szDrawSword_CT[32];
	_tchar				m_szTumbling_CT[32];


public:
	static CUI_SkillBoard*		Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*		Clone(void * pArg = nullptr) override;
	virtual void				Free();

};

_END