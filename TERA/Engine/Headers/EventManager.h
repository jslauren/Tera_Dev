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
	HRESULT Register_Object(const _tchar* _szEventTag, CGameObject* pObject);
	HRESULT Notify_Event(const _tchar* _szEventTag, void* pMsg = nullptr);
	HRESULT Remove_Object(const _tchar* _szEventTag, CGameObject* pObject);
	HRESULT Remove_Event(const _tchar* _szEventTag = nullptr);

private:
	map<const _tchar*, list<CGameObject*>*>		m_pMapEventList;

public:
	virtual void Free();

};

_END