#include "..\Headers\EventManager.h"
#include "GameObject.h"

_IMPLEMENT_SINGLETON(CEventManager)

CEventManager::CEventManager()
{
}

HRESULT CEventManager::NotifyEvent(const _tchar * _pSubject, void* Msg)
{
	auto	iter = find_if(m_pMapSubject.begin(), m_pMapSubject.end(), CFinder_Tag(_pSubject));

	if (iter == m_pMapSubject.end())
		return E_FAIL;

	list<CGameObject*>* pListGameObject = (iter->second);

	_int iRemoveCode = 0;
	for (auto iter = pListGameObject->begin(); iter != pListGameObject->end();)
	{
		iRemoveCode = (*iter)->OnEvent(_pSubject, Msg);

		if (iRemoveCode < 0)
		{
			Safe_Release(*iter);
			iter = pListGameObject->erase(iter);
			continue;
		}

		++iter;
	}

	return NOERROR;
}

HRESULT CEventManager::RegisterObject(const _tchar * _pSubject, CGameObject * pObject)
{
	if (nullptr == pObject)
		return E_FAIL;

	auto	iter = find_if(m_pMapSubject.begin(), m_pMapSubject.end(), CFinder_Tag(_pSubject));

	list<CGameObject*>* pListGameObject = nullptr;

	if (iter == m_pMapSubject.end())
	{
		pListGameObject = new list<CGameObject*>;
		m_pMapSubject.emplace(_pSubject, pListGameObject);
	}
	else
		pListGameObject = (iter->second);

	pListGameObject->push_back(pObject);
	pObject->AddRef();

	return NOERROR;
}

HRESULT CEventManager::RemoveObject(const _tchar* _pSubject, CGameObject* pObject)
{
	if (nullptr == pObject)
		return E_FAIL;

	auto	iterList = find_if(m_pMapSubject.begin(), m_pMapSubject.end(), CFinder_Tag(_pSubject));

	if (iterList == m_pMapSubject.end())
		return E_FAIL;

	auto iterObject = find_if(iterList->second->begin(), iterList->second->end(),
		[&pObject](CGameObject* pGameObejct)->bool { return pGameObejct == pObject; });

	if (iterObject == iterList->second->end())
		return E_FAIL;

	Safe_Release(*iterObject);
	iterList->second->erase(iterObject);

	return NOERROR;
}

HRESULT CEventManager::RemoveAll()
{
	for (auto& Pair : m_pMapSubject)
	{
		for (auto& pGameObject : (*Pair.second))
			Safe_Release(pGameObject);

		Pair.second->clear();
	}

	m_pMapSubject.clear();

	return NOERROR;
}

void CEventManager::Free()
{
	RemoveAll();
}
