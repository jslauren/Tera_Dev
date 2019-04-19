#include "..\Headers\Component_Manager.h"

_IMPLEMENT_SINGLETON(CComponent_Manager)

CComponent_Manager::CComponent_Manager()
	: m_pmapComponent(nullptr)
{
}

HRESULT CComponent_Manager::Reserve_Component_Manager(const _uint & iMaxNumScene)
{
	// Reserve함수는 Ready함수 정도로 이해하면 되려나...?
	if (nullptr != m_pmapComponent)
		return E_FAIL;

	m_pmapComponent = new MAPCOMPONENT[iMaxNumScene];

	m_iMaxNumScene = iMaxNumScene;

	return NOERROR;
}

HRESULT CComponent_Manager::Add_Component_Prototype(const _uint & iSceneIdx, const _tchar * pComponentTag, CComponent * pInComponent)
{
	// 오브젝트들을 관리하는거와 비슷하다. 아니 똑같나..?
	// 컴포넌트를 추가 할 때, 해당 컴포넌트가 이미 존재하는지 확인 후, 존재 하지 않으면,
	// 맵 컴포넌트 컨테이너에 넣어주는 식으로 관리한다.

	if (m_iMaxNumScene <= iSceneIdx ||
		nullptr == m_pmapComponent ||
		nullptr == pInComponent)
		return E_FAIL;

	CComponent*	pComponent = Find_Component(iSceneIdx, pComponentTag);

	// 이미 존재하는 컴포넌트를 추가하려 할때,
	// 꼭 E_FAIL을 리턴해야할까..?
	// 한다면 메시지 박스를 해줘야 되지 않을까..?
	if (nullptr != pComponent)
		return E_FAIL;

	m_pmapComponent[iSceneIdx].insert(MAPCOMPONENT::value_type(pComponentTag, pInComponent));

	return NOERROR;
}

HRESULT CComponent_Manager::Clear_Component_Prototype(const _uint & iSceneIdx)
{
	if (m_iMaxNumScene <= iSceneIdx ||
		nullptr == m_pmapComponent)
		return E_FAIL;

	for (auto& Pair : m_pmapComponent[iSceneIdx])
	{
		Safe_Release(Pair.second);
	}
	m_pmapComponent[iSceneIdx].clear();


	return NOERROR;
}

CComponent * CComponent_Manager::Clone_Component(const _uint & iSceneIdx, const _tchar * pComponentTag)
{
	// 프로토타입 패턴을 위한 컴포넌트 클론 함수
	if (m_iMaxNumScene <= iSceneIdx ||
		nullptr == m_pmapComponent)
		return nullptr;

	CComponent*	pComponent = Find_Component(iSceneIdx, pComponentTag);

	if (nullptr == pComponent)
		return nullptr;

	return pComponent->Clone();
}


CComponent * CComponent_Manager::Find_Component(const _uint & iSceneIdx, const _tchar * pComponentTag)
{
	if (m_iMaxNumScene <= iSceneIdx ||
		nullptr == m_pmapComponent)
		return nullptr;

	auto	iter = find_if(m_pmapComponent[iSceneIdx].begin(), m_pmapComponent[iSceneIdx].end(), CFinder_Tag(pComponentTag));

	if (iter == m_pmapComponent[iSceneIdx].end())
		return nullptr;

	return iter->second;
}

void CComponent_Manager::Free()
{
	for (size_t i = 0; i < m_iMaxNumScene; i++)
	{
		for (auto& Pair : m_pmapComponent[i])
		{
			Safe_Release(Pair.second);
		}
		m_pmapComponent[i].clear();
	}

	Safe_Delete_Array(m_pmapComponent);
}
