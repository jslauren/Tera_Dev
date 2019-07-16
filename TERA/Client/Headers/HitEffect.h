#pragma once
#include "Effect.h"

_BEGIN(Client)

class CHitEffect final : public CEffect
{
private:
	explicit CHitEffect(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CHitEffect(const CEffect& rhs);
	virtual ~CHitEffect() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	virtual _int	Update_GameObject(const _float& fTimeDelta);
	virtual _int	LateUpdate_GameObject(const _float& fTimeDelta);
	virtual HRESULT Render_GameObject();

private:
	virtual HRESULT Add_Component();
	//virtual HRESULT SetUp_BillBoard();
	//virtual HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);

public:
	static CHitEffect*		Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone(void* pArg = nullptr);
	virtual void			Free();

};

_END