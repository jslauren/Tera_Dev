#include "..\Headers\VIBuffer.h"

CVIBuffer::CVIBuffer(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(m_pGraphic_Device)
{
}

HRESULT CVIBuffer::Ready_VIBuffer()
{
	// ������ �����Ѵ�.
	// CreateVertexBuffer �Լ��� 2��° ������ DWORD Usage��,
	// (�� ������) ���� �뵵�� ��� �� ����? ��� ������ �����ϸ� ������, ���� 0�� ����.
	// �� �� 0���� ����, �̴� �⺻ ��ɿ� ����ϰڴٴ� ���̴�.
	// 4��° ������ D3DPOOL���� ���� MANAGED�� ����,
	// ����� �� ������ ������� DEFAULT�� ����ϴµ� ���� DEFAULT�� ��� �� �ÿ���,
	// �ش� �ϴ� ���� ������ �� ���� �÷���� �Ѵ�.
	if (FAILED(m_pGraphic_Device->CreateVertexBuffer(m_iVtxSize * m_iNumVertices, 0, m_dwVtxFVF, D3DPOOL_MANAGED, &m_pVB, nullptr)))
		return E_FAIL;

	return NOERROR;
}

void CVIBuffer::Free()
{
	CComponent::Free();
}
