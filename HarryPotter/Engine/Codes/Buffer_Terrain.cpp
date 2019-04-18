#include "..\Headers\Buffer_Terrain.h"
#include "Transform.h"

CBuffer_Terrain::CBuffer_Terrain(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer(pGraphic_Device)
{
}

CBuffer_Terrain::CBuffer_Terrain(const CBuffer_Terrain & rhs)
	: CVIBuffer(rhs)
{
}

HRESULT CBuffer_Terrain::Ready_VIBuffer()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_iVtxSize = sizeof(VTXTEX);
	m_fInterval = 1.f;
	m_iNumVerticesX = 100;
	m_iNumVerticesZ = 100;
	m_iNumVertices = m_iNumVerticesX * m_iNumVerticesZ;

	m_dwVtxFVF = D3DFVF_XYZ | D3DFVF_TEX1;
	
	// 사각형 개수 * 2 => 삼각형 2개가 사각형이고 폴리곤은 삼각형 기준이기 때문
	m_iNumPolygons = (m_iNumVerticesX - 1) * (m_iNumVerticesZ - 1) * 2;

	m_iIndexSize = sizeof(INDEX16);
	m_Format = D3DFMT_INDEX16;

	if (FAILED(CVIBuffer::Ready_VIBuffer()))
		return E_FAIL;

	VTXTEX*		pVertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	for (size_t i = 0; i < m_iNumVerticesZ; ++i)
	{
		for (size_t j = 0; j < m_iNumVerticesX; ++j)
		{
			_uint	iIndex = i * m_iNumVerticesX + j;

			pVertices[iIndex].vPosition = _vec3(j * m_fInterval, 0.f, i * m_fInterval);
			m_pPositions[iIndex] = pVertices[iIndex].vPosition;
			pVertices[iIndex].vTexUV = _vec2(j / (m_iNumVerticesX - 1.f), i / (m_iNumVerticesZ - 1.f));
		}
	}

	m_pVB->Unlock();


	INDEX16*	pIndices = nullptr;

	_uint		iNumPolygons = 0;

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	for (size_t i = 0; i < m_iNumVerticesZ - 1; i++)
	{
		for (size_t j = 0; j < m_iNumVerticesX -1; j++)
		{
			_uint	iIndex = i * m_iNumVerticesX + j;

			pIndices[iNumPolygons]._1 = iIndex + m_iNumVerticesX;
			pIndices[iNumPolygons]._2 = iIndex + m_iNumVerticesX + 1;
			pIndices[iNumPolygons]._3 = iIndex + 1;
			++iNumPolygons;

			pIndices[iNumPolygons]._1 = iIndex + m_iNumVerticesX;
			pIndices[iNumPolygons]._2 = iIndex + 1;
			pIndices[iNumPolygons]._3 = iIndex;
			++iNumPolygons;
		}
	}
	m_pIB->Unlock();

	return NOERROR;
}

void CBuffer_Terrain::Render_Buffer(const CTransform * pTransform)
{
	if (nullptr == m_pGraphic_Device)
		return;

	if (nullptr != pTransform)
	{
		_matrix		matTransform = *pTransform->Get_WorldMatrixPointer();

		VTXTEX*		pVertices = nullptr;

		m_pVB->Lock(0, 0, (void**)&pVertices, 0);

		for (size_t i = 0; i < m_iNumVertices; i++)
		{
			D3DXVec3TransformCoord(&pVertices[i].vPosition, &m_pPositions[i], &matTransform);
		}
		m_pVB->Unlock();
	}

	m_pGraphic_Device->SetStreamSource(0, m_pVB, 0, m_iVtxSize);
	m_pGraphic_Device->SetFVF(m_dwVtxFVF);
	m_pGraphic_Device->SetIndices(m_pIB);
	m_pGraphic_Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_iNumVertices, 0, m_iNumPolygons);
}

CBuffer_Terrain * CBuffer_Terrain::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBuffer_Terrain* pInstance = new CBuffer_Terrain(pGraphic_Device);

	if (FAILED(pInstance->Ready_VIBuffer()))
	{
		_MSGBOX("CBuffer_Terrain Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CBuffer_Terrain::Clone()
{
	return new CBuffer_Terrain(*this);
}

void CBuffer_Terrain::Free()
{
	CVIBuffer::Free();
}
