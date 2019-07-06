#pragma once
#include "UI.h"

_BEGIN(Client)

class CUI_Dialog final : public CUI
{
public:
	enum SCRIPT
	{
		SCRIPT_TITLE, SCRIPT_MAIN, SCRIPT_REPLY, SCRIPT_END
	};

private:
	explicit CUI_Dialog(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CUI_Dialog(const CUI_Dialog& rhs);
	virtual ~CUI_Dialog() = default;
	
public:	// Getter
	_bool			Get_TalkEventAvaliable() { return m_bIsTalkEventAvaliable; }

public:	// Setter
	void			Set_TalkEventAvaliable(_bool bButton) { m_bIsTalkEventAvaliable = bButton; }

public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	virtual _int	Update_GameObject(const _float& fTimeDelta);
	virtual _int	LateUpdate_GameObject(const _float& fTimeDelta);
	virtual HRESULT Render_GameObject();

public:
	void			MakeScript(SCRIPT eScriptKinds, _tchar* pScriptContents);

private:
	virtual HRESULT	Add_Component();
	virtual HRESULT	SetUp_ConstantTable(LPD3DXEFFECT pEffect, const _uint iTargetTextureIdx = 1);

private:
	CBuffer_RcTex*		m_pBufferCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;

private:
	_tchar				m_szTitle[32];
	_tchar				m_szMain[MAX_PATH];
	_tchar				m_szReply[MAX_PATH];
	_bool				m_bIsTalkEventAvaliable = false;

public:
	static CUI_Dialog*		Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone(void * pArg = nullptr) override;
	virtual void			Free();

};

_END