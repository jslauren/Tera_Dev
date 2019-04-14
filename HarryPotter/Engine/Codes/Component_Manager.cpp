#include "..\Headers\Component_Manager.h"

_IMPLEMENT_SINGLETON(CComponent_Manager)

CComponent_Manager::CComponent_Manager()
	: m_pMapComponent(nullptr)
{
}

HRESULT CComponent_Manager::Reserve_Component_Manager(const _uint & iMaxNumScene)
{
	// Reserve�Լ��� Ready�Լ� ������ �����ϸ� �Ƿ���...?
	if (nullptr != m_pMapComponent)
		return E_FAIL;

	m_pMapComponent = new MAPCOMPONENT[iMaxNumScene];

	m_iMaxNumScene = iMaxNumScene;

	return NOERROR;
}

HRESULT CComponent_Manager::Add_Component_Prototype(const _uint & iSceneIdx, const _tchar * pComponentTag, CComponent * pInComponent)
{
	// ������Ʈ���� �����ϴ°ſ� ����ϴ�. �ƴ� �Ȱ���..?
	// ������Ʈ�� �߰� �� ��, �ش� ������Ʈ�� �̹� �����ϴ��� Ȯ�� ��, ���� ���� ������,
	// �� ������Ʈ �����̳ʿ� �־��ִ� ������ �����Ѵ�.

	if (m_iMaxNumScene <= iSceneIdx ||
		nullptr == m_pMapComponent ||
		nullptr == pInComponent)
		return E_FAIL;

	CComponent*	pComponent = Find_Component(iSceneIdx, pComponentTag);

	// �̹� �����ϴ� ������Ʈ�� �߰��Ϸ� �Ҷ�,
	// �� E_FAIL�� �����ؾ��ұ�..?
	// �Ѵٸ� �޽��� �ڽ��� ����� ���� ������..?
	if (nullptr != pComponent)
		return E_FAIL;

	m_pMapComponent[iSceneIdx].insert(MAPCOMPONENT::value_type(pComponentTag, pInComponent));

	return NOERROR;
}

CComponent * CComponent_Manager::Clone_Component(const _uint & iSceneIdx, const _tchar * pComponentTag)
{
	// ������Ÿ�� ������ ���� ������Ʈ Ŭ�� �Լ�
	if (m_iMaxNumScene <= iSceneIdx ||
		nullptr == m_pMapComponent)
		return nullptr;

	CComponent*	pComponent = Find_Component(iSceneIdx, pComponentTag);

	if (nullptr == pComponent)
		return nullptr;

	return pComponent->Clone();
}

CComponent * CComponent_Manager::Find_Component(const _uint & iSceneIdx, const _tchar * pComponentTag)
{
	if (m_iMaxNumScene <= iSceneIdx ||
		nullptr == m_pMapComponent)
		return nullptr;

	auto	iter = find_if(m_pMapComponent[iSceneIdx].begin(), m_pMapComponent[iSceneIdx].end(), CFinder_Tag(pComponentTag));

	if (iter == m_pMapComponent[iSceneIdx].end())
		return nullptr;

	return iter->second;
}

void CComponent_Manager::Free()
{
	for (size_t i = 0; i < m_iMaxNumScene; i++)
	{
		for (auto& Pair : m_pMapComponent[i])
		{
			Safe_Release(Pair.second);
		}
		m_pMapComponent[i].clear();
	}

	Safe_Delete_Array(m_pMapComponent);
}
