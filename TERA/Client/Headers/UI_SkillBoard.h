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
	virtual HRESULT	OnEvent(const _tchar * _szEventTag, void * _pMsg);

public:
	virtual HRESULT	Add_Component();
	virtual HRESULT	SetUp_ConstantTable(LPD3DXEFFECT pEffect, const _uint iTargetTextureIdx = 1);

private:
	CTexture*			m_pTextureSkillBoardCom = nullptr;
	CTransform*			m_pTransformSkillBoardCom = nullptr;
	CBuffer_RcTex*		m_pBufferSkillBoardCom = nullptr;

	CTexture*			m_pTextureSkillFilterCom = nullptr;
	CTransform*			m_pTransformSkillFilterCom = nullptr;
	CBuffer_RcTex*		m_pBufferSkillFilterCom = nullptr;

private:
	HRESULT				NullCheck();

public:
	static CUI_SkillBoard*		Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*		Clone(void * pArg = nullptr) override;
	virtual void				Free();

};

_END