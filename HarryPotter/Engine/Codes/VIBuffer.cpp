#include "..\Headers\VIBuffer.h"
#include "Transform.h"

CVIBuffer::CVIBuffer(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
{
}

CVIBuffer::CVIBuffer(const CVIBuffer & rhs)
	: CComponent(rhs)
	, m_pVB(rhs.m_pVB)
	, m_iNumVertices(rhs.m_iNumVertices)
	, m_iVtxSize(rhs.m_iVtxSize)
	, m_dwVtxFVF(rhs.m_dwVtxFVF)
	, m_iNumPolygons(rhs.m_iNumPolygons)
	, m_pIB(rhs.m_pIB)
	, m_iIndexSize(rhs.m_iIndexSize)
	, m_Format(rhs.m_Format)
	, m_pPositions(rhs.m_pPositions)
	, m_pIndices(rhs.m_pIndices)
{
	m_pVB->AddRef();
	m_pIB->AddRef();
}

HRESULT CVIBuffer::Ready_VIBuffer()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	// 정점을 생성한다
	if (FAILED(m_pGraphic_Device->CreateVertexBuffer(m_iVtxSize * m_iNumVertices, 0, m_dwVtxFVF, D3DPOOL_MANAGED, &m_pVB, nullptr)))
		return E_FAIL;

	m_pPositions = new _vec3[m_iNumVertices];
	ZeroMemory(m_pPositions, sizeof(_vec3) * m_iNumVertices);

	// 인덱스들을 생성한다(인덱스버퍼)
	if (FAILED(m_pGraphic_Device->CreateIndexBuffer(m_iIndexSize * m_iNumPolygons, 0, m_Format, D3DPOOL_MANAGED, &m_pIB, nullptr)))
		return E_FAIL;

	if (D3DFMT_INDEX16 == m_Format)
	{
		m_pIndices = new INDEX16[m_iNumPolygons];
		ZeroMemory(m_pIndices, sizeof(INDEX16) * m_iNumPolygons);
	}
	else
	{
		m_pIndices = new INDEX32[m_iNumPolygons];
		ZeroMemory(m_pIndices, sizeof(INDEX32) * m_iNumPolygons);
	}

	return NOERROR;
}

void CVIBuffer::Render_Buffer(const CTransform* pTransform)
{
}

_float CVIBuffer::Compute_HeightOnBuffer(const CTransform * pTransform)
{
	return _float(0.0f);
}

_bool CVIBuffer::Picking(HWND hWnd, CTransform * pTransform, _vec3 * pOut)
{
	//// _vec3 * matrix / w
	//// xyz1 * 1

	//D3DXVec3TransformCoord();

	// 뷰포트 영역 (윈도우좌표영역) 상의 마우스 위치 = 로컬위치 * 월드행렬 * 뷰행렬 * 투영행렬 / z * 뷰포트 변환

	// 뷰포트 영역 (윈도우좌표영역) 상의 마우스 위치를 구한다.
	POINT		ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(hWnd, &ptMouse);

	D3DVIEWPORT9		ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));

	m_pGraphic_Device->GetViewport(&ViewPort);

	_vec3		vMouse;

	// 투영스페이스 상의 마우스 위치.
	vMouse.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
	vMouse.y = ptMouse.y / (ViewPort.Height * -0.5f) + 1.f;
	vMouse.z = 0.f;

	// 뷰스페이스 영역상의 마우스 위치.
	_matrix			matProj;
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, nullptr, &matProj);

	D3DXVec3TransformCoord(&vMouse, &vMouse, &matProj);

	_vec3			vRay, vPivot;

	vPivot = _vec3(0.f, 0.f, 0.f);
	vRay = vMouse - vPivot;

	// 월드스페이스 영역상의 마우스 위치.
	_matrix			matView;
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, nullptr, &matView);

	D3DXVec3TransformCoord(&vPivot, &vPivot, &matView);
	D3DXVec3TransformNormal(&vRay, &vRay, &matView);

	// 지형점정`S 로컬스페이스영역상의 마우스 위치.
	_matrix			matWorld = *pTransform->Get_WorldMatrixPointer();
	D3DXMatrixInverse(&matWorld, nullptr, &matWorld);

	D3DXVec3TransformCoord(&vPivot, &vPivot, &matWorld);
	D3DXVec3TransformNormal(&vRay, &vRay, &matWorld);

	D3DXVec3Normalize(&vRay, &vRay);

	_float		fU, fV, fDist;

	for (size_t i = 0; i < m_iNumPolygons; ++i)
	{
		if (D3DFMT_INDEX16 == m_Format)
		{
			INDEX16* pIndices = (INDEX16*)m_pIndices;

			if (TRUE == D3DXIntersectTri(&m_pPositions[pIndices[i]._1], &m_pPositions[pIndices[i]._2], &m_pPositions[pIndices[i]._3], &vPivot, &vRay, &fU, &fV, &fDist))
			{
				*pOut = *D3DXVec3Normalize(&vRay, &vRay) * fDist + vPivot;

				return true;
			}
		}
	}
	return _bool(false);
}

void CVIBuffer::Free()
{
	if (false == m_isClone)
	{
		Safe_Delete_Array(m_pPositions);
		Safe_Delete_Array(m_pIndices);
	}

	Safe_Release(m_pVB);
	Safe_Release(m_pIB);

	CComponent::Free();
}
