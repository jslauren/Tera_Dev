#include "..\Headers\VIBuffer.h"

CVIBuffer::CVIBuffer(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(m_pGraphic_Device)
{
}

HRESULT CVIBuffer::Ready_VIBuffer()
{
	// 정점을 생성한다.
	// CreateVertexBuffer 함수의 2번째 인자인 DWORD Usage는,
	// (이 공간을) 무슨 용도로 사용 할 껀데? 라는 뜻으로 이해하면 좋으며, 보통 0이 들어간다.
	// 이 때 0값이 들어가면, 이는 기본 기능에 충실하겠다는 뜻이다.
	// 4번째 인자인 D3DPOOL값은 보통 MANAGED로 쓰며,
	// 빈번한 값 변경이 있을경우 DEFAULT를 사용하는데 만약 DEFAULT로 사용 할 시에는,
	// 해당 하는 것의 서순을 맨 위로 올려줘야 한다.
	if (FAILED(m_pGraphic_Device->CreateVertexBuffer(m_iVtxSize * m_iNumVertices, 0, m_dwVtxFVF, D3DPOOL_MANAGED, &m_pVB, nullptr)))
		return E_FAIL;

	return NOERROR;
}

void CVIBuffer::Free()
{
	CComponent::Free();
}
