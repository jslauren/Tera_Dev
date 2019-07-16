#include "..\Headers\Layer.h"
#include "GameObject.h"

CLayer::CLayer()
{
}

const CComponent * CLayer::Get_Component(const _tchar * pComponentTag, const _uint & iIndex)
{
	if (m_ObjectList.size() <= iIndex)
		return nullptr;

	auto	iter = m_ObjectList.begin();

	for (size_t i = 0; i < iIndex; ++i)
		++iter;

	return (*iter)->Get_Component(pComponentTag);
}

HRESULT CLayer::Ready_Layer()
{
	return NOERROR;
}

HRESULT CLayer::Add_ObjectToLayer(CGameObject * pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	m_ObjectList.push_back(pGameObject);

	return NOERROR;
}

_int CLayer::Update_Layer(const _float & fTimeDelta)
{
	_int		iExitCode = 0;

	for (auto& iter = m_ObjectList.begin(); iter != m_ObjectList.end(); )
	{
		if ((*iter)->Update_GameObject(fTimeDelta) == -1)
		{
			Safe_Release(*iter);
			iter = m_ObjectList.erase(iter);
		}
		else
			++iter;
	}

	return _int(iExitCode);
}

_int CLayer::LateUpdate_Layer(const _float & fTimeDelta)
{
	_int		iExitCode = 0;

	for (auto& iter = m_ObjectList.begin(); iter != m_ObjectList.end(); )
	{
		if ((*iter)->LateUpdate_GameObject(fTimeDelta) == -1)
		{
			Safe_Release(*iter);
			iter = m_ObjectList.erase(iter);
		}
		else
			++iter;
	}

	return _int(iExitCode);
}

CLayer * CLayer::Create()
{
	CLayer* pInstance = new CLayer();

	if (FAILED(pInstance->Ready_Layer()))
	{
		_MSGBOX("CLayer Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLayer::Free()
{
	for (auto& pGameObject : m_ObjectList)
		Safe_Release(pGameObject);

	m_ObjectList.clear();
}

