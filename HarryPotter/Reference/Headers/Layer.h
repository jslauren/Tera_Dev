#pragma once
#include "Base.h"

_BEGIN(Engine)

class CComponent;
class CGameObject;
class CLayer final : public CBase
{
private:
	explicit CLayer();
	virtual ~CLayer() = default;
public:
	list<CGameObject*>&	Get_ObjectList() { return m_ObjectList; }
	const CComponent*	Get_Component(const _tchar* pComponentTag, const _uint& iIndex);
public:
	HRESULT Ready_Layer();
public:
	HRESULT Add_ObjectToLayer(CGameObject* pGameObject);
	_int	Update_Layer(const _float& fTimeDelta);
	_int	LateUpdate_Layer(const _float& fTimeDelta);
private:
	list<CGameObject*>			m_ObjectList;
	typedef list<CGameObject*>	OBJECTLIST;
public:
	static CLayer*	Create();
	virtual void	Free();
};

_END