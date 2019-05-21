#include "..\Headers\Light_Manager.h"
#include "Light.h"

_IMPLEMENT_SINGLETON(CLight_Manager)

CLight_Manager::CLight_Manager()
{
}

const D3DLIGHT9 * CLight_Manager::Get_LightInfo(const _uint & iIndex) const
{
	auto	iter = m_LightList.begin();

	for (size_t i = 0; i < iIndex; i++)
		++iter;

	if (iter == m_LightList.end())
		return nullptr;

	return (*iter)->Get_LightInfo();
}

HRESULT CLight_Manager::Add_Light(LPDIRECT3DDEVICE9 pGraphic_Device, const D3DLIGHT9 * pLightInfo)
{
	CLight*		pLight = CLight::Create(pGraphic_Device, pLightInfo);
	if (nullptr == pLight)
		return E_FAIL;

	m_LightList.push_back(pLight);

	return NOERROR;
}

void CLight_Manager::Free()
{
	for (auto& pLight : m_LightList)
		Safe_Release(pLight);

	m_LightList.clear();
}
