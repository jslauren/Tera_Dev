#include "..\Headers\Component_Manager.h"

_IMPLEMENT_SINGLETON(CComponent_Manager)

CComponent_Manager::CComponent_Manager()
	: m_pmapComponentPrototype(nullptr)
{
}

HRESULT CComponent_Manager::Reserve_Component_Manager(const _uint & iMaxNumScene)
{
	if (nullptr != m_pmapComponentPrototype)
		return E_FAIL;

	m_pmapComponentPrototype = new MAPCOMPONENT[iMaxNumScene];
	m_iMaxNumScene = iMaxNumScene;

	return NOERROR;
}

HRESULT CComponent_Manager::Add_Component_Prototype(const _uint & iSceneIdx, const _tchar * pComponentTag, CComponent * pInComponent)
{
	if (m_iMaxNumScene <= iSceneIdx ||
		nullptr == m_pmapComponentPrototype ||
		nullptr == pInComponent)
		return E_FAIL;

	CComponent*	pComponent = Find_Component_Prototype(iSceneIdx, pComponentTag);

	if (nullptr != pComponent)
		return E_FAIL;

	_tchar* dynamicArray = new _tchar[lstrlen(pComponentTag) + 1];
	lstrcpy(dynamicArray, pComponentTag);

	m_pmapComponentPrototype[iSceneIdx].insert(MAPCOMPONENT::value_type(dynamicArray, pInComponent));

	return NOERROR;
}

HRESULT CComponent_Manager::Clear_Component_Prototype(const _uint & iSceneIdx)
{
	if (m_iMaxNumScene <= iSceneIdx ||
		nullptr == m_pmapComponentPrototype)
		return E_FAIL;

	for (auto& Pair : m_pmapComponentPrototype[iSceneIdx])
	{
		_tchar* pTempArray = const_cast<_tchar*>(Pair.first);
		Safe_Delete_Array(pTempArray);
		Safe_Release(Pair.second);
	}
	m_pmapComponentPrototype[iSceneIdx].clear();

	return NOERROR;
}

CComponent * CComponent_Manager::Clone_Component(const _uint & iSceneIdx, const _tchar * pComponentTag, void* pArg)
{
	if (m_iMaxNumScene <= iSceneIdx ||
		nullptr == m_pmapComponentPrototype)
		return nullptr;

	CComponent*	pComponent = Find_Component_Prototype(iSceneIdx, pComponentTag);

	if (nullptr == pComponent)
		return nullptr;

	return pComponent->Clone(pArg);
}

CComponent * CComponent_Manager::Find_Component_Prototype(const _uint & iSceneIdx, const _tchar * pComponentTag)
{
	if (m_iMaxNumScene <= iSceneIdx ||
		nullptr == m_pmapComponentPrototype)
		return nullptr;

	auto	iter = find_if(m_pmapComponentPrototype[iSceneIdx].begin(), m_pmapComponentPrototype[iSceneIdx].end(), CFinder_Tag(pComponentTag));

	if (iter == m_pmapComponentPrototype[iSceneIdx].end())
		return nullptr;

	return iter->second;
}

void CComponent_Manager::Free()
{
	for (size_t i = 0; i < m_iMaxNumScene; i++)
	{
		for (auto& Pair : m_pmapComponentPrototype[i])
		{
			_tchar* pTempArray = const_cast<_tchar*>(Pair.first);
			Safe_Delete_Array(pTempArray);
			Safe_Release(Pair.second);
		}
		m_pmapComponentPrototype[i].clear();
	}

	Safe_Delete_Array(m_pmapComponentPrototype);
}
