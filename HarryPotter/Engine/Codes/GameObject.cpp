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
	CComponent*	pComponent = Find_Component(pComponentTag);

	if (nullptr == pComponent)
		return nullptr;

	return pComponent;
}

_matrix CGameObject::Get_Transform(_D3DTRANSFORMSTATETYPE eType) const
{
	_matrix			matTransform;

	m_pGraphic_Device->GetTransform(eType, &matTransform);

	return matTransform;
}

void CGameObject::Set_SamplerState(_ulong dwSampler, D3DSAMPLERSTATETYPE SamplerState, _ulong dwValue)
{
	if (nullptr == m_pGraphic_Device)
		return;

	m_pGraphic_Device->SetSamplerState(dwSampler, SamplerState, dwValue);
}

void CGameObject::Set_RenderState(D3DRENDERSTATETYPE eType, _ulong dwValue)
{
	if (nullptr == m_pGraphic_Device)
		return;

	m_pGraphic_Device->SetRenderState(eType, dwValue);
}

void CGameObject::Set_Material(const D3DMATERIAL9 & Material)
{
	m_pGraphic_Device->SetMaterial(&Material);
}

HRESULT CGameObject::Ready_GameObject_Prototype()
{
	return NOERROR;
}

HRESULT CGameObject::Ready_GameObject(void* pArg)
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

_int CGameObject::OnEvent(const _tchar * _pSubject, void * _pMsg)
{
	return _int();
}

HRESULT CGameObject::Add_Component(const _uint & iSceneIdx, const _tchar * pPrototypeTag, const _tchar* pComponentTag, CComponent** ppOutComponent)
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

HRESULT CGameObject::Compute_ViewZ(CTransform * pTransform)
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	_matrix			matView;

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &matView);

	D3DXMatrixInverse(&matView, nullptr, &matView);

	_vec3		vDir = (*pTransform->Get_StateInfo(CTransform::STATE_POSITION) - *(_vec3*)&matView.m[3][0]);

	m_fViewZ = D3DXVec3Length(&vDir);

	return NOERROR;
}

const bool & CGameObject::GetDelete()
{
	return m_bDelete;
}

CComponent * CGameObject::Find_Component(const _tchar * pComponentTag)
{
	auto	iter = find_if(m_mapComponents.begin(), m_mapComponents.end(), CFinder_Tag(pComponentTag));

	if (iter == m_mapComponents.end())
		return nullptr;

	return iter->second;
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
