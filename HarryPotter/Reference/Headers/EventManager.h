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
	// �̺�Ʈ�� ����� ��ü�鿡�� �̺�Ʈ�� �˸���.
	HRESULT NotifyEvent(const _tchar* _pSubject, void* pMsg = nullptr);
	// �̺�Ʈ�� ��ü���� ����Ѵ�.
	HRESULT RegisterObject(const _tchar* _pSubject, CGameObject* pObject);
	// �̺�Ʈ�� ��� �� ��ü���� ��� ����Ѵ�.
	HRESULT RemoveObject(const _tchar* _pSubject, CGameObject* pObject);
	// ��� �̺�Ʈ�� �����Ѵ�. ( �� �Ⱦ���.. �ƴ� �Ⱦ��°ɼ���..?)
	HRESULT RemoveAll();
private:
	// �� �̺�Ʈ�� �����ϴ� ��ü ����Ʈ���� �����Ѵ�.
	map<const _tchar*, list<CGameObject*>*>		m_pMapSubject;
public:
	virtual void Free();
};

_END