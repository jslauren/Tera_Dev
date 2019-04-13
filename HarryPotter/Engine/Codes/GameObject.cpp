#include "..\Headers\GameObject.h"

CGameObject::CGameObject(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device(pGraphic_Device)
	, m_pComponent_Manager(CComponent_Manager::GetInstance())
{
	m_pComponent_Manager->AddRef();
	m_pGraphic_Device->AddRef();
}

CGameObject::CGameObject(const CGameObject & rhs)
	: m_pGraphic_Device(rhs.m_pGraphic_Device)
	, m_pComponent_Manager(rhs.m_pComponent_Manager)
{
	m_pComponent_Manager->AddRef();
	m_pGraphic_Device->AddRef();
}

HRESULT CGameObject::Ready_GameObject_Prototype()
{
	return NOERROR;
}

HRESULT CGameObject::Ready_GameObject()
{
	return NOERROR;
}

_int CGameObject::Update_GameObject(const _float & fTimeDelta)
{
	return _int();
}

_int CGameObject::LateUpdate_GameObject(const _float & fTimeDelta)
{
	return _int();
}

HRESULT CGameObject::Render_GameObject()
{
	return NOERROR;
}

HRESULT CGameObject::Add_Component(const _uint & iSceneIdx, const _tchar * pComponentTag, CComponent** ppOutComponent)
{
	if (nullptr == m_pComponent_Manager)
		return E_FAIL;

	CComponent* pComponent = m_pComponent_Manager->Clone_Component(iSceneIdx, pComponentTag);
	if (nullptr == pComponent)
		return E_FAIL;

	*ppOutComponent = pComponent;

	return NOERROR;
}

void CGameObject::Free()
{
	Safe_Release(m_pComponent_Manager);
	Safe_Release(m_pGraphic_Device);
}
