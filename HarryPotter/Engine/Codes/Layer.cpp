#include "..\Headers\Layer.h"
#include "GameObject.h"

CLayer::CLayer()
{
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
	_int	iExitCode = 0;

	for (auto& pGameObject : m_ObjectList)
	{
		if (nullptr != pGameObject)
		{
			iExitCode = pGameObject->Update_GameObject(fTimeDelta);
			if (iExitCode & 0x80000000)
				return iExitCode;
		}
	}
	return _int(iExitCode);
}

_int CLayer::LateUpdate_Layer(const _float & fTimeDelta)
{
	_int	iExitCode = 0;

	for (auto& pGameObject : m_ObjectList)
	{
		if (nullptr != pGameObject)
		{
			iExitCode = pGameObject->LateUpdate_GameObject(fTimeDelta);
			if (iExitCode & 0x80000000)
				return iExitCode;
		}
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
