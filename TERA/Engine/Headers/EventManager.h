#pragma once
#include "Base.h"

_BEGIN(Engine)

class CGameObject;

class _DLL_EXPORTS CEventManager final : public CBase
{
	_DECLARE_SINGLETON(CEventManager)
private:
	explicit CEventManager();
	virtual ~CEventManager() = default;
public:
	// 이벤트에 등록한 객체들에게 이벤트를 알린다.
	HRESULT NotifyEvent(const _tchar* _pSubject, void* pMsg = nullptr);
	// 이벤트에 객체들을 등록한다.
	HRESULT RegisterObject(const _tchar* _pSubject, CGameObject* pObject);
	// 이벤트에 등록 된 객체들을 등록 취소한다.
	HRESULT RemoveObject(const _tchar* _pSubject, CGameObject* pObject);
	// 모든 이벤트를 삭제한다. ( 잘 안쓴다.. 아니 안쓰는걸수도..?)
	HRESULT RemoveAll();
private:
	// 각 이벤트에 대응하는 객체 리스트들을 보관한다.
	map<const _tchar*, list<CGameObject*>*>		m_pMapSubject;
public:
	virtual void Free();
};

_END