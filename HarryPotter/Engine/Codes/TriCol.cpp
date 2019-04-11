#include "..\Headers\TriCol.h"

CTriCol::CTriCol(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer(pGraphic_Device)
{
}

HRESULT CTriCol::Ready_VIBuffer()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	// 부모 클래스인 VIBuffer의 Ready_VIBuffer 함수에 CreateVertexBuffer함수를 실행하기 위해,
	// 인자값을 자식에서 채워주는 중이다.
	m_iVtxSize = sizeof(VTXCOL);
	m_iNumVertices = 3;
	// D3DFVF_XYZ는 위치를 뜻함, D3DFVF_DIFFUSE는 빛이 들어오면 어떤 색으로 반사할까. 를 뜻함.
	m_dwVtxFVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;

	if (FAILED(CVIBuffer::Ready_VIBuffer()))
		return E_FAIL;

	// 1. 공간을 걸어 잠근다.
	// 2. 공간에 접근할 수 있는 포인터를 구해준다.

	//void*	pVertices = nullptr;

	//m_pVB->Lock(0, 0, &pVertices, 0);

	//((VTXCOL*)pVertices)[0].vPosition	= _vec3();
	//((VTXCOL*)pVertices)[0].dwColor		= D3DXCOLOR();

	//m_pVB->Unlock();

	VTXCOL*		pVertices = nullptr;

	// VertexBuffer를 Lock함수로 잠구어 주는 이유는,
	// 메인쓰레드가 이제부터 값을 바꿀 예정인데, 다른 쓰레드가 접근하지 못하게 해야 하기 때문이다.
	// Lock(), Unlock() 함수를 이용하여 점을 찍은 이 공간은 Local Space 이다.
	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	pVertices[0].vPosition	= _vec3(0.f, 1.f, 0.f);
	pVertices[0].dwColor = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);

	pVertices[1].vPosition = _vec3(1.f, -1.f, 0.f);
	pVertices[1].dwColor = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);

	pVertices[2].vPosition = _vec3(-1.f, -1.f, 0.f);
	pVertices[2].dwColor = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);

	m_pVB->Unlock();

	return NOERROR;
}

CTriCol * CTriCol::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	return nullptr;
}

void CTriCol::Free()
{
}
