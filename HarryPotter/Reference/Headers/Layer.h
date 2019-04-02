#pragma once
#include "Base.h"
// 오브젝트의 컨테이너(List)를 가지고 있는 클래스 CLayer 클래스 되시겠다.
_BEGIN(Engine)

class CGameObject;
class CLayer final : public CBase
{
private:
	explicit CLayer();
	virtual ~CLayer() = default;
public:
	HRESULT Ready_Layer();
public:
	// 해당 컨테이너를 사용하는 함수들을 모두 정의해 놓았다.
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