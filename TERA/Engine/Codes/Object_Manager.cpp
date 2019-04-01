#include "..\Headers\Object_Manager.h"
#include "GameObject.h"

_IMPLEMENT_SINGLETON(CObject_Manager)

CObject_Manager::CObject_Manager()
{

}

HRESULT CObject_Manager::Reserve_Object_Manager(const _uint & iMaxNumScene)
{
	if (nullptr != m_pmapPrototype)
		return E_FAIL;

	m_pmapPrototype = new MAPPROTOTYPE[iMaxNumScene];

	m_iMaxNumScene = iMaxNumScene;

	return NOERROR;
}

HRESULT CObject_Manager::Add_Object_Prototype(const _uint & iSceneIdx, const _tchar * pProtoTag, CGameObject * pInGameObject)
{
	if (m_iMaxNumScene <= iSceneIdx ||
		nullptr == m_pmapPrototype ||
		nullptr == pInGameObject)
		return E_FAIL;

	CGameObject*	pGameObject = Find_Object_Prototype(iSceneIdx, pProtoTag);

	if (nullptr != pGameObject)
		return E_FAIL;

	m_pmapPrototype[iSceneIdx].insert(MAPPROTOTYPE::value_type(pProtoTag, pInGameObject));

	return NOERROR;
}

CGameObject * CObject_Manager::Find_Object_Prototype(const _uint & iSceneIdx, const _tchar * pProtoTag)
{
	if (m_iMaxNumScene <= iSceneIdx ||
		nullptr == m_pmapPrototype)
		return nullptr;

	auto	iter = find_if(m_pmapPrototype[iSceneIdx].begin(), m_pmapPrototype[iSceneIdx].end(), [&](MAPPROTOTYPE::value_type Pair)->bool {if (0 == lstrcmp(pProtoTag, Pair.first)) return true; return false; });

	if (iter == m_pmapPrototype[iSceneIdx].end())
		return nullptr;

	return iter->second;
}

void CObject_Manager::Free()
{
	for (size_t i = 0; i < m_iMaxNumScene; i++)
	{
		for (auto& Pair : m_pmapPrototype[i])
			Safe_Release(Pair.second);

		m_pmapPrototype[i].clear();
	}

	Safe_Delete_Array(m_pmapPrototype);
}
