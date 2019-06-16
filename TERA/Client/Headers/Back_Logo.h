#pragma once
#include "Defines.h"
#include "GameObject.h"
#include "Buffer_RcTex.h"
#include "UI.h"

_BEGIN(Client)

class CBack_Logo final : public CUI
{
private:
	explicit CBack_Logo(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CBack_Logo(const CBack_Logo& rhs);
	virtual ~CBack_Logo() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	virtual _int	Update_GameObject(const _float& fTimeDelta);
	virtual _int	LateUpdate_GameObject(const _float& fTimeDelta);
	virtual HRESULT Render_GameObject();

private:
	virtual HRESULT			Add_Component();
	virtual HRESULT			SetUp_ConstantTable(LPD3DXEFFECT pEffect);

private:
	CBuffer_RcTex*			m_pBufferBGCom = nullptr;

private:
	_int					m_iLoadingProgressValue = 0;

public:
	static CBack_Logo*		Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone(void* pArg = nullptr);
	virtual void			Free();
};

_END