#include "..\Headers\Component.h"

CComponent::CComponent(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device(pGraphic_Device)
{
	m_pGraphic_Device->AddRef();
}

HRESULT CComponent::Ready_Component()
{
	return NOERROR;
}

void CComponent::Free()
{
	Safe_Release(m_pGraphic_Device);
}
