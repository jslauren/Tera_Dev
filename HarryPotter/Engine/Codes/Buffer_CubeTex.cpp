#include "..\Headers\Buffer_CubeTex.h"
#include "Transform.h"

CBuffer_CubeTex::CBuffer_CubeTex(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer(pGraphic_Device)
{
}

CBuffer_CubeTex::CBuffer_CubeTex(const CBuffer_CubeTex & rhs)
	: CVIBuffer(rhs)
{
}

HRESULT CBuffer_CubeTex::Ready_VIBuffer()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_iVtxSize = sizeof(VTXCUBETEX);
	m_iNumVertices = 8;

	m_dwVtxFVF = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);
	m_iNumPolygons = 12;

	m_iIndexSize = sizeof(INDEX16);
	m_Format = D3DFMT_INDEX16;

	// 정점버퍼를 할당한다 + 인덱스버퍼를 할당한다.
	if (FAILED(CVIBuffer::Ready_VIBuffer()))
		return E_FAIL;

	VTXCUBETEX*		pVertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	pVertices[0].vPosition = _vec3(-0.5f, 0.5f, -0.5f);
	m_pPositions[0] = pVertices[0].vPosition;
	D3DXVec3Normalize(&pVertices[0].vTexUV, &pVertices[0].vPosition);

	pVertices[1].vPosition = _vec3(0.5f, 0.5f, -0.5f);
	m_pPositions[1] = pVertices[1].vPosition;
	D3DXVec3Normalize(&pVertices[1].vTexUV, &pVertices[1].vPosition);

	pVertices[2].vPosition = _vec3(0.5f, -0.5f, -0.5f);
	m_pPositions[2] = pVertices[2].vPosition;
	D3DXVec3Normalize(&pVertices[2].vTexUV, &pVertices[2].vPosition);

	pVertices[3].vPosition = _vec3(-0.5f, -0.5f, -0.5f);
	m_pPositions[3] = pVertices[3].vPosition;
	D3DXVec3Normalize(&pVertices[3].vTexUV, &pVertices[3].vPosition);

	pVertices[4].vPosition = _vec3(-0.5f, 0.5f, 0.5f);
	m_pPositions[4] = pVertices[4].vPosition;
	D3DXVec3Normalize(&pVertices[4].vTexUV, &pVertices[4].vPosition);

	pVertices[5].vPosition = _vec3(0.5f, 0.5f, 0.5f);
	m_pPositions[5] = pVertices[5].vPosition;
	D3DXVec3Normalize(&pVertices[5].vTexUV, &pVertices[5].vPosition);

	pVertices[6].vPosition = _vec3(0.5f, -0.5f, 0.5f);
	m_pPositions[6] = pVertices[6].vPosition;
	D3DXVec3Normalize(&pVertices[6].vTexUV, &pVertices[6].vPosition);

	pVertices[7].vPosition = _vec3(-0.5f, -0.5f, 0.5f);
	m_pPositions[7] = pVertices[7].vPosition;
	D3DXVec3Normalize(&pVertices[7].vTexUV, &pVertices[7].vPosition);
	m_pVB->Unlock();


	INDEX16*	pIndices = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	// +x
	pIndices[0]._1 = 1; pIndices[0]._2 = 5; pIndices[0]._3 = 6;
	pIndices[1]._1 = 1; pIndices[1]._2 = 6; pIndices[1]._3 = 2;

	// -x
	pIndices[2]._1 = 4; pIndices[2]._2 = 0; pIndices[2]._3 = 3;
	pIndices[3]._1 = 4; pIndices[3]._2 = 3; pIndices[3]._3 = 7;

	// +y
	pIndices[4]._1 = 4; pIndices[4]._2 = 5; pIndices[4]._3 = 1;
	pIndices[5]._1 = 4; pIndices[5]._2 = 1; pIndices[5]._3 = 0;

	// -y
	pIndices[6]._1 = 3; pIndices[6]._2 = 2; pIndices[6]._3 = 6;
	pIndices[7]._1 = 3; pIndices[7]._2 = 6; pIndices[7]._3 = 7;

	// +z
	pIndices[8]._1 = 5; pIndices[8]._2 = 4; pIndices[8]._3 = 7;
	pIndices[9]._1 = 5; pIndices[9]._2 = 7; pIndices[9]._3 = 6;

	// -z
	pIndices[10]._1 = 0; pIndices[10]._2 = 1; pIndices[10]._3 = 2;
	pIndices[11]._1 = 0; pIndices[11]._2 = 2; pIndices[11]._3 = 3;

	m_pIB->Unlock();

	return NOERROR;
}

void CBuffer_CubeTex::Render_Buffer(const CTransform* pTransform)
{
	if (nullptr == m_pGraphic_Device)
		return;

	if (nullptr != pTransform)
	{
		_matrix		matTransform = *pTransform->Get_WorldMatrixPointer();

		// 정점의 위치에 행렬을 곱해서 정점의 위치를 변환하자.
		VTXCUBETEX*		pVertices = nullptr;

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

CBuffer_CubeTex * CBuffer_CubeTex::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBuffer_CubeTex* pInstance = new CBuffer_CubeTex(pGraphic_Device);

	if (FAILED(pInstance->Ready_VIBuffer()))
	{
		_MSGBOX("CBuffer_CubeTex Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CBuffer_CubeTex::Clone(void* pArg)
{
	return new CBuffer_CubeTex(*this);
}

void CBuffer_CubeTex::Free()
{
	CVIBuffer::Free();
}
