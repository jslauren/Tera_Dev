#pragma once
#include "Unit.h"
#include "Defines.h"
#include "GameObject.h"

_BEGIN(Client)

class CArkus final : public CUnit
{
public:
	explicit CArkus(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CArkus(const CArkus& rhs);
	virtual ~CArkus() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	virtual _int	Update_GameObject(const _float& fTimeDelta);
	virtual _int	LateUpdate_GameObject(const _float& fTimeDelta);
	virtual HRESULT Render_GameObject();

public:
	HRESULT			Add_Component();
	HRESULT			SetUp_ConstantTable(LPD3DXEFFECT pEffect);
	HRESULT			SetUp_HeightOnTerrain(_uint iIndex);

//protected:
//	Engine::OBJECTMESHDATA tObjectMeshData;

public:
	static CArkus*			Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone(void* pArg = nullptr);
	virtual void			Free();

};

_END