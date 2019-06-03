#include "..\Headers\Cell.h"

CCell::CCell(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device(pGraphic_Device)
	, m_pLine(nullptr)
{
	Safe_AddRef(m_pGraphic_Device);
	ZeroMemory(m_pNeighbor, sizeof(CCell*) * NEIGHBOR_END);
}

HRESULT CCell::Ready_Cell(const _vec3 * pPointA, const _vec3 * pPointB, const _vec3 * pPointC, _uint iIndex)
{
	m_iIndex = iIndex;

	m_vPoint[POINT_A] = *pPointA;
	m_vPoint[POINT_B] = *pPointB;
	m_vPoint[POINT_C] = *pPointC;

	m_vEdge[EDGE_AB] = m_vPoint[POINT_B] - m_vPoint[POINT_A];
	m_vEdge[EDGE_BC] = m_vPoint[POINT_C] - m_vPoint[POINT_B];
	m_vEdge[EDGE_CA] = m_vPoint[POINT_A] - m_vPoint[POINT_C];

	if (FAILED(D3DXCreateLine(m_pGraphic_Device, &m_pLine)))
		return E_FAIL;

	return NOERROR;
}

void CCell::Render_Cell()
{
	_vec3		vPoint[4];

	vPoint[0] = m_vPoint[POINT_A];
	vPoint[1] = m_vPoint[POINT_B];
	vPoint[2] = m_vPoint[POINT_C];
	vPoint[3] = m_vPoint[POINT_A];

	_matrix			matView, matProj;

	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &matProj);

	m_pLine->SetWidth(2.f);

	for (size_t i = 0; i < 4; ++i)
	{
		D3DXVec3TransformCoord(&vPoint[i], &vPoint[i], &matView);

		if (vPoint[i].z <= 0)
			vPoint[i].z = 0;
		
		D3DXVec3TransformCoord(&vPoint[i], &vPoint[i], &matProj);
		
	}

	_matrix		matIdentity;

	m_pLine->DrawTransform(vPoint, 4, D3DXMatrixIdentity(&matIdentity), D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
}

_bool CCell::isIn(const _vec3 * pPosition, EDGE* pOutEdge)
{
	for (size_t i = 0; i < EDGE_END; ++i)
	{
		_vec3	vNormal = _vec3(m_vEdge[i].z * -1.f, 0.0f, m_vEdge[i].x);
		_vec3	vAxis = *pPosition - m_vPoint[i];

		if (0 < D3DXVec3Dot(D3DXVec3Normalize(&vNormal, &vNormal), D3DXVec3Normalize(&vAxis, &vAxis)))
		{
			*pOutEdge = EDGE(i);
			return false;
		}

	}
	return true;
}

_bool CCell::isNeighbor(const _vec3 * pSourPoint, const _vec3 * pDestPoint, CCell * pCell)
{
	if (*pSourPoint == pCell->m_vPoint[POINT_A])
	{
		if (*pDestPoint == pCell->m_vPoint[POINT_B])
			return true;
		if (*pDestPoint == pCell->m_vPoint[POINT_C])
			return true;
	}

	if (*pSourPoint == pCell->m_vPoint[POINT_B])
	{
		if (*pDestPoint == pCell->m_vPoint[POINT_A])
			return true;
		if (*pDestPoint == pCell->m_vPoint[POINT_C])
			return true;
	}

	if (*pSourPoint == pCell->m_vPoint[POINT_C])
	{
		if (*pDestPoint == pCell->m_vPoint[POINT_A])
			return true;
		if (*pDestPoint == pCell->m_vPoint[POINT_B])
			return true;
	}

	return _bool(false);
}

CCell * CCell::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _vec3 * pPointA, const _vec3 * pPointB, const _vec3 * pPointC, _uint iIndex)
{
	CCell* pInstance = new CCell(pGraphic_Device);

	if (FAILED(pInstance->Ready_Cell(pPointA, pPointB, pPointC, iIndex)))
	{
		_MSGBOX("CCell Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}


void CCell::Free()
{
	Safe_Release(m_pLine);
	Safe_Release(m_pGraphic_Device);
}
