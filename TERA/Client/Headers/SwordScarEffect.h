#pragma once
#include "Effect.h"

_BEGIN(Client)

class CSwordScarEffect final : public CEffect
{
private:
	explicit CSwordScarEffect(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CSwordScarEffect(const CSwordScarEffect& rhs);
	virtual ~CSwordScarEffect() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	virtual _int	Update_GameObject(const _float& fTimeDelta);
	virtual _int	LateUpdate_GameObject(const _float& fTimeDelta);
	virtual HRESULT Render_GameObject();

private:
	virtual HRESULT Add_Component();
	virtual HRESULT	SetUp_ConstantTable(LPD3DXEFFECT pEffect);

private:
	_float			m_fScalingY = 0.f;
	_float			m_fAlphaValue = 0.f;
	_vec4			m_vColliderPos = { 0.f, 0.f, 0.f, 0.f };

public:
	static CSwordScarEffect*	Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*		Clone(void* pArg = nullptr);
	virtual void				Free();

};

_END