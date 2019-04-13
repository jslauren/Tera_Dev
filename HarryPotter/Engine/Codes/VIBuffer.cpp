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
{
	m_pVB->AddRef();
}

HRESULT CVIBuffer::Ready_VIBuffer()
{
	// 정점을 생성한다
	if (FAILED(m_pGraphic_Device->CreateVertexBuffer(m_iVtxSize * m_iNumVertices, 0, m_dwVtxFVF, D3DPOOL_MANAGED, &m_pVB, nullptr)))
		return E_FAIL;

	return NOERROR;
}

void CVIBuffer::Render_Buffer()
{

}

void CVIBuffer::Free()
{
	Safe_Release(m_pVB);

	CComponent::Free();
}
