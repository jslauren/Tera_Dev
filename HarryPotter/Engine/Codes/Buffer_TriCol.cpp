#include "..\Headers\Buffer_TriCol.h"

CBuffer_TriCol::CBuffer_TriCol(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer(pGraphic_Device)
{

}

CBuffer_TriCol::CBuffer_TriCol(const CBuffer_TriCol & rhs)
	: CVIBuffer(rhs)
{
}

HRESULT CBuffer_TriCol::Ready_VIBuffer()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_iVtxSize = sizeof(VTXCOL);
	m_iNumVertices = 3;
	m_dwVtxFVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;

	if (FAILED(CVIBuffer::Ready_VIBuffer()))
		return E_FAIL;

	// 1. 공간을 걸어 잠근다.
	// 2. 공간에 접근할 수 있는 포인터를 구해준다.

	/*void*			pVertices = nullptr;

	m_pVB->Lock(0, 0, &pVertices, 0);

	((VTXCOL*)pVertices)[0].vPosition = _vec3();
	((VTXCOL*)pVertices)[0].dwColor = D3DXCOLOR();

	m_pVB->Unlock();*/

	VTXCOL*		pVertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	pVertices[0].vPosition = _vec3(0.f, 1.f, 0.f);
	pVertices[0].dwColor = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);

	pVertices[1].vPosition = _vec3(1.f, -1.f, 0.f);
	pVertices[1].dwColor = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);

	pVertices[2].vPosition = _vec3(-1.f, -1.f, 0.f);
	pVertices[2].dwColor = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);

	m_pVB->Unlock();

	return NOERROR;
}

void CBuffer_TriCol::Render_Buffer()
{
	if (nullptr == m_pGraphic_Device)
		return;

	m_pGraphic_Device->SetStreamSource(0, m_pVB, 0, m_iVtxSize);
	m_pGraphic_Device->SetFVF(m_dwVtxFVF);
	m_pGraphic_Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);



}

CBuffer_TriCol * CBuffer_TriCol::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBuffer_TriCol* pInstance = new CBuffer_TriCol(pGraphic_Device);

	if (FAILED(pInstance->Ready_VIBuffer()))
	{
		_MSGBOX("pGraphic_Device Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CBuffer_TriCol::Clone()
{
	return new CBuffer_TriCol(*this);
}

void CBuffer_TriCol::Free()
{


	CVIBuffer::Free();
}
