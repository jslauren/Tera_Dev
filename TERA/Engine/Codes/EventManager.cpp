#include "..\Headers\EventManager.h"
#include "GameObject.h"

_USING(Engine)

_IMPLEMENT_SINGLETON(CEventManager)

CEventManager::CEventManager()
{
}

HRESULT CEventManager::Register_Object(const _tchar * _szEventTag, CGameObject * pObject)
{
	if (nullptr == pObject ||
		nullptr == _szEventTag)
		return E_FAIL;

	auto	iter = find_if(m_pMapEventList.begin(), m_pMapEventList.end(), CFinder_Tag(_szEventTag));

	if (iter == m_pMapEventList.end())
	{
		list<CGameObject*>* pListGameObject = new list<CGameObject*>;

		if (pListGameObject == nullptr)
			return E_OUTOFMEMORY;

		pListGameObject->push_back(pObject);
		m_pMapEventList.emplace(_szEventTag, pListGameObject);

		return NOERROR;
	}
	else
		iter->second->push_back(pObject);

	return NOERROR;
}

HRESULT CEventManager::Notify_Event(const _tchar * _szEventTag, void* pMsg)
{
	if (nullptr == _szEventTag)
		return NOERROR;

	auto	iter = find_if(m_pMapEventList.begin(), m_pMapEventList.end(), CFinder_Tag(_szEventTag));

	if (iter == m_pMapEventList.end())
		return E_OUTOFMEMORY;

	for (auto Object : *iter->second)
	{
		if (FAILED(Object->OnEvent(_szEventTag, pMsg)))
			return E_FAIL;
	}

	return NOERROR;
}

HRESULT CEventManager::Remove_Object(const _tchar* _szEventTag, CGameObject* pObject)
{
	if (nullptr == pObject ||
		nullptr == _szEventTag)
		return NOERROR;

	auto iterList = find_if(m_pMapEventList.begin(), m_pMapEventList.end(), CFinder_Tag(_szEventTag));

	if (iterList == m_pMapEventList.end())
		return E_OUTOFMEMORY;

	for (auto IterObject = iterList->second->begin(); IterObject != iterList->second->end(); ++IterObject)
	{
		if (pObject == *IterObject)
		{
			iterList->second->erase(IterObject);
			break;
		}
	}

	if (iterList->second->empty())
	{
		iterList->second->clear();
		Safe_Delete(iterList->second);
		m_pMapEventList.erase(iterList);
	}

	return NOERROR;
}

HRESULT CEventManager::Remove_Event(const _tchar* _szEventTag)
{
	if (nullptr == _szEventTag)
	{
		Free();
		return NOERROR;
	}

	auto iter = find_if(m_pMapEventList.begin(), m_pMapEventList.end(), CFinder_Tag(_szEventTag));

	if (iter == m_pMapEventList.end())
		return	E_OUTOFMEMORY;

	Safe_Delete(iter->second);

	m_pMapEventList.erase(iter);

	return NOERROR;
}

void CEventManager::Free()
{
	for (auto & list : m_pMapEventList)
	{
		list.second->clear();
		Safe_Delete(list.second);
	}
	m_pMapEventList.clear();
}
