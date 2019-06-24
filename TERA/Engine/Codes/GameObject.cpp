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

_bool CGameObject::Picking(HWND hWnd, CTransform * pTransform, LPD3DXBASEMESH pMesh, _vec3 * pOut, _float* fFinalDist)
{
	// 뷰포트 영역 (윈도우좌표영역) 상의 마우스 위치 = 로컬위치 * 월드행렬 * 뷰행렬 * 투영행렬 / z * 뷰포트 변환

	// 뷰포트 영역 (윈도우좌표영역) 상의 마우스 위치를 구한다.
	POINT	ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(hWnd, &ptMouse);

	D3DVIEWPORT9	ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));

	m_pGraphic_Device->GetViewport(&ViewPort);

	_vec3	vMouse;

	// 투영스페이스 상의 마우스 위치.
	vMouse.x = ptMouse.x / (1300 * 0.5f) - 1.f;
	vMouse.y = ptMouse.y / (881 * -0.5f) + 1.f;
	vMouse.z = 0.f;

	// 뷰스페이스 영역상의 마우스 위치.
	_matrix		matProj;
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, nullptr, &matProj);

	D3DXVec3TransformCoord(&vMouse, &vMouse, &matProj);

	_vec3		vRay, vPivot;

	vPivot = _vec3(0.f, 0.f, 0.f);
	vRay = vMouse - vPivot;

	// 월드스페이스 영역상의 마우스 위치.
	_matrix		matView;
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, nullptr, &matView);

	D3DXVec3TransformCoord(&vPivot, &vPivot, &matView);
	D3DXVec3TransformNormal(&vRay, &vRay, &matView);

	// 메쉬 오브젝트의 로컬스페이스 영역상의 마우스 위치.
	_matrix		matWorld = *pTransform->Get_WorldMatrixPointer();
	_matrix		matInverseWorld = matWorld;
	D3DXMatrixInverse(&matInverseWorld, nullptr, &matInverseWorld);

	D3DXVec3TransformCoord(&vPivot, &vPivot, &matInverseWorld);
	D3DXVec3TransformNormal(&vRay, &vRay, &matInverseWorld);

	D3DXVec3Normalize(&vRay, &vRay);
	
	_ulong			dwFaceIdx;
	LPD3DXBUFFER	pBuffer;
	_ulong			dwCountOfHits;
	_float			fU, fV, fDist;
	BOOL			bIshit = false;

	if (FAILED(D3DXIntersect(pMesh, &vPivot, &vRay, &bIshit, &dwFaceIdx, &fU, &fV, &fDist, &pBuffer, &dwCountOfHits)) || bIshit == false)
		return false;

	LPDIRECT3DVERTEXBUFFER9 pVB;
	LPDIRECT3DINDEXBUFFER9 pIB;
	LPD3DXMESH pClonedMesh;

	pMesh->CloneMeshFVF(pMesh->GetOptions(), D3DFVF_XYZ, m_pGraphic_Device, &pClonedMesh);

	pClonedMesh->GetVertexBuffer(&pVB);
	pClonedMesh->GetIndexBuffer(&pIB);

	_vec3 vPos[3];
	_vec3 vOutTemp;
	WORD* pIndices;
	VTX* pVertices;

	pIB->Lock(0, 0, (void**)&pIndices, 0);
	pVB->Lock(0, 0, (void**)&pVertices, 0);

	for (size_t i = 0; i < 3; ++i)
	{
		DWORD dix = pIndices[dwFaceIdx * 3 + i];
		vPos[i] = pVertices[dix].vPosition;
	}

	if (TRUE == D3DXIntersectTri(&vPos[0], &vPos[1], &vPos[2], &vPivot, &vRay, &fU, &fV, &fDist))
	{
		*fFinalDist = fDist;
		*pOut = *D3DXVec3Normalize(&vRay, &vRay) * fDist + vPivot;
		D3DXVec3TransformCoord(pOut, pOut, &matWorld);
	}

	pVB->Unlock();
	pIB->Unlock();
	Safe_Release(pVB);
	Safe_Release(pIB);
	

	return true;
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

HRESULT CGameObject::Add_Component(const _uint & iSceneIdx, const _tchar * pPrototypeTag, const _tchar* pComponentTag, CComponent** ppOutComponent, void* pArg)
{
	// 객체에 적용 될(또는 사용 할) 컴포넌트 들을 이 함수를 통해 추가 해준다.
	if (nullptr == m_pComponent_Manager)
		return E_FAIL;

	// pComponentTag를 통해 적용을 원하는 컴포넌트를 Clone 해준다.
	CComponent* pComponent = m_pComponent_Manager->Clone_Component(iSceneIdx, pPrototypeTag, pArg);
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
