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

const CComponent * CGameObject::Get_Component(const _tchar * pComponentTag)
{
	return nullptr;
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

HRESULT CGameObject::Add_Component(const _uint & iSceneIdx, const _tchar* pPrototypeTag, const _tchar * pComponentTag, CComponent** ppOutComponent)
{
	// 객체에 적용 될(또는 사용 할) 컴포넌트 들을 이 함수를 통해 추가 해준다.
	if (nullptr == m_pComponent_Manager)
		return E_FAIL;

	// pComponentTag를 통해 적용을 원하는 컴포넌트를 Clone 해준다.
	CComponent* pComponent = m_pComponent_Manager->Clone_Component(iSceneIdx, pPrototypeTag);
	if (nullptr == pComponent)
		return E_FAIL;

	*ppOutComponent = pComponent;

	m_mapComponents.insert(MAPCOMPONENTS::value_type(pComponentTag, pComponent));
	pComponent->AddRef();

	return NOERROR;
}

void CGameObject::Free()
{
	for (auto& Pair : m_mapComponents)
	{
		Safe_Release(Pair.second);
	}
	m_mapComponents.clear();

	Safe_Release(m_pComponent_Manager);
	Safe_Release(m_pGraphic_Device);
}
