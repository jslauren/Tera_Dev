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
	// 객체들을 모아놓은 오브젝트 리스트 컨테이너.
	// (실사용 객체, 원본객체) 둘 중에 하나가 아닌 그 둘을 모두 가지고 있는
	// 오브젝트 클래스를 리스트안에 넣어 가지고 있다.
	list<CGameObject*>			m_ObjectList;
	typedef list<CGameObject*>	OBJECTLIST;
public:
	static CLayer* Create();
	virtual void Free();
};

_END