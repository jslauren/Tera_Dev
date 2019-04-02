#include "..\Headers\GameObject.h"

CGameObject::CGameObject(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device(pGraphic_Device)
{
	m_pGraphic_Device->AddRef();
}

CGameObject::CGameObject(const CGameObject & rhs)
	: m_pGraphic_Device(rhs.m_pGraphic_Device)
{
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

void CGameObject::Free()
{
	Safe_Release(m_pGraphic_Device);
}
