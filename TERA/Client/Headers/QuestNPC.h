#pragma once
#include "Unit.h"
#include "Defines.h"
#include "GameObject.h"

_BEGIN(Client)

class CQuestNPC : public CUnit
{
public:
	explicit CQuestNPC(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CQuestNPC(const CQuestNPC& rhs);
	virtual ~CQuestNPC() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	virtual _int	Update_GameObject(const _float& fTimeDelta);
	virtual _int	LateUpdate_GameObject(const _float& fTimeDelta);
	virtual HRESULT Render_GameObject();

public:
	virtual HRESULT Add_Component();
	virtual HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);

private:
	CCollider*		m_pColliderEventCom = nullptr;

public:
	static CQuestNPC*		Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone(void* pArg = nullptr);
	virtual void			Free();
};

_END