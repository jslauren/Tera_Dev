#include "..\Headers\TriCol.h"

CTriCol::CTriCol(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer(pGraphic_Device)
{
}

HRESULT CTriCol::Ready_VIBuffer()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	// �θ� Ŭ������ VIBuffer�� Ready_VIBuffer �Լ��� CreateVertexBuffer�Լ��� �����ϱ� ����,
	// ���ڰ��� �ڽĿ��� ä���ִ� ���̴�.
	m_iVtxSize = sizeof(VTXCOL);
	m_iNumVertices = 3;
	// D3DFVF_XYZ�� ��ġ�� ����, D3DFVF_DIFFUSE�� ���� ������ � ������ �ݻ��ұ�. �� ����.
	m_dwVtxFVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;

	if (FAILED(CVIBuffer::Ready_VIBuffer()))
		return E_FAIL;

	// 1. ������ �ɾ� ��ٴ�.
	// 2. ������ ������ �� �ִ� �����͸� �����ش�.

	//void*	pVertices = nullptr;

	//m_pVB->Lock(0, 0, &pVertices, 0);

	//((VTXCOL*)pVertices)[0].vPosition	= _vec3();
	//((VTXCOL*)pVertices)[0].dwColor		= D3DXCOLOR();

	//m_pVB->Unlock();

	VTXCOL*		pVertices = nullptr;

	// VertexBuffer�� Lock�Լ��� �ᱸ�� �ִ� ������,
	// ���ξ����尡 �������� ���� �ٲ� �����ε�, �ٸ� �����尡 �������� ���ϰ� �ؾ� �ϱ� �����̴�.
	// Lock(), Unlock() �Լ��� �̿��Ͽ� ���� ���� �� ������ Local Space �̴�.
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
