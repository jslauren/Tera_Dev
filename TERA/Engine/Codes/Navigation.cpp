#include "..\Headers\Navigation.h"
#include "Cell.h"

CNavigation::CNavigation(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
{

}

CNavigation::CNavigation(const CNavigation & rhs)
	: CComponent(rhs)
	, m_vecCell(rhs.m_vecCell)
{
	for (auto& pCell : m_vecCell)
		Safe_AddRef(pCell);
}

void CNavigation::SetUp_CurrentIndexLoop(_vec3 vPos)
{
	CCell::EDGE		eEdge;

	for (size_t i = 0; i < m_vecCell.size(); ++i)
	{
		if (true == m_vecCell[i]->isIn(&vPos, &eEdge))
		{
			m_iCurrentIndex = i;
			return;
		}
	}
}

HRESULT CNavigation::Ready_Component_Prototype(const _tchar* pFilePath)
{
	HANDLE hFile = CreateFile(pFilePath,
		GENERIC_READ,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	DWORD		dwByte;
	_vec3		vPoint[3];

	if (0 == hFile)
		return E_FAIL;

	while (true)
	{
		ReadFile(hFile, vPoint, sizeof(_vec3) * 3, &dwByte, nullptr);

		if (0 == dwByte)
			break;

		CCell*	pCell = CCell::Create(m_pGraphic_Device, &vPoint[0], &vPoint[1], &vPoint[2], m_vecCell.size());
		if (nullptr == pCell)
			break;

		m_vecCell.push_back(pCell);
	}

	CloseHandle(hFile);

	if (SetUp_Neighbor())
		return E_FAIL;

	return NOERROR;
}

HRESULT CNavigation::Ready_Component(void * pArg)
{
	m_iCurrentIndex = *((_uint*)pArg);

	return NOERROR;
}

void CNavigation::Render_Navigation()
{
	for (auto& pCell : m_vecCell)
		pCell->Render_Cell();

}

_bool CNavigation::Move_OnNavigation(const _vec3 * pPosition, const _vec3 * pDir, _float fTimeDelta, _uint* pOutIndex)
{
	if (0 > m_iCurrentIndex ||
		m_vecCell.size() <= m_iCurrentIndex)
		return false;

	_vec3		vLook;

	_vec3		vPosition = *pPosition + *D3DXVec3Normalize(&vLook, pDir) * fTimeDelta;

	CCell::EDGE			eEdge;

	if (true == m_vecCell[m_iCurrentIndex]->isIn(&vPosition, &eEdge))
	{
		*pOutIndex = m_iCurrentIndex;
		return true;
	}
	else
	{
		const CCell* pNeighbor = nullptr;

		if (pNeighbor = m_vecCell[m_iCurrentIndex]->Get_Neighbor(CCell::NEIGHBOR(eEdge)))
		{
			*pOutIndex = pNeighbor->Get_Index();
			return true;
		}

		else
		{
			*pOutIndex = m_iCurrentIndex;
			return false;
		}
	}

	return _bool();
}

HRESULT CNavigation::SetUp_Neighbor()
{
	_uint iSize = m_vecCell.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		for (size_t j = 0; j < iSize; ++j)
		{
			if (m_vecCell[i] == m_vecCell[j])
				continue;

			if (true == m_vecCell[i]->isNeighbor(m_vecCell[i]->Get_Point(CCell::POINT_A)
				, m_vecCell[i]->Get_Point(CCell::POINT_B), m_vecCell[j]))
			{
				m_vecCell[i]->Set_Neighbor(CCell::NEIGHBOR_AB, m_vecCell[j]);
			}

			if (true == m_vecCell[i]->isNeighbor(m_vecCell[i]->Get_Point(CCell::POINT_B)
				, m_vecCell[i]->Get_Point(CCell::POINT_C), m_vecCell[j]))
			{
				m_vecCell[i]->Set_Neighbor(CCell::NEIGHBOR_BC, m_vecCell[j]);
			}

			if (true == m_vecCell[i]->isNeighbor(m_vecCell[i]->Get_Point(CCell::POINT_C)
				, m_vecCell[i]->Get_Point(CCell::POINT_A), m_vecCell[j]))
			{
				m_vecCell[i]->Set_Neighbor(CCell::NEIGHBOR_CA, m_vecCell[j]);
			}
		}
	}
	return NOERROR;
}

_float CNavigation::Compute_HeightOnNavi(_vec3 * _pPlayerPos)
{
	_vec3	vPoint[3];

	vPoint[0] = *m_vecCell[m_iCurrentIndex]->Get_Point(CCell::POINT_A);
	vPoint[1] = *m_vecCell[m_iCurrentIndex]->Get_Point(CCell::POINT_B);
	vPoint[2] = *m_vecCell[m_iCurrentIndex]->Get_Point(CCell::POINT_C);

	_float	fU, fV, fDist;

	_vec3 pPlayerPos = *_pPlayerPos;

	pPlayerPos.y = 100;

	if(true == D3DXIntersectTri(&vPoint[0], &vPoint[1], &vPoint[2], &pPlayerPos, &_vec3(0.f, -1.f, 0.f), &fU, &fV, &fDist))
		fDist = 100 - fDist;

	return _float(fDist);
}

CNavigation * CNavigation::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar * pFilePath)
{
	CNavigation* pInstance = new CNavigation(pGraphic_Device);

	if (FAILED(pInstance->Ready_Component_Prototype(pFilePath)))
	{
		_MSGBOX("CNavigation Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CNavigation::Clone(void * pArg)
{
	CNavigation* pInstance = new CNavigation(*this);

	if (FAILED(pInstance->Ready_Component(pArg)))
	{
		_MSGBOX("CNavigation Cloned Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CNavigation::Free()
{
	if (false == m_isClone)
	{
		for (auto& pCell : m_vecCell)
		{
			pCell->Clear_Neighbor();
		}
	}

	for (auto& pCell : m_vecCell)
		Safe_Release(pCell);

	m_vecCell.clear();

	CComponent::Free();
}
