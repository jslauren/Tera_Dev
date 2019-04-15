#include "..\Headers\VIBuffer.h"

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
{
	m_pVB->AddRef();
	m_pIB->AddRef();
}

HRESULT CVIBuffer::Ready_VIBuffer()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	// 정점을 생성한다
	if (FAILED(m_pGraphic_Device->CreateVertexBuffer((m_iVtxSize * m_iNumVertices), 0, m_dwVtxFVF, D3DPOOL_MANAGED, &m_pVB, nullptr)))
		return E_FAIL;

	// 인덱스들을 생성한다(인덱스 버퍼)
	if (FAILED(m_pGraphic_Device->CreateIndexBuffer((m_iIndexSize * m_iNumPolygons), 0, m_Format, D3DPOOL_MANAGED, &m_pIB, nullptr)))
		return E_FAIL;

	return NOERROR;
}

void CVIBuffer::Render_Buffer()
{

}

void CVIBuffer::Free()
{
	Safe_Release(m_pVB);
	Safe_Release(m_pIB);

	CComponent::Free();
}
