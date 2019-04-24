#include "..\Headers\Buffer_RcCol.h"
#include "Transform.h"

CBuffer_RcCol::CBuffer_RcCol(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer(pGraphic_Device)
{
}

CBuffer_RcCol::CBuffer_RcCol(const CBuffer_RcCol & rhs)
	: CVIBuffer(rhs)
{
}

HRESULT CBuffer_RcCol::Ready_VIBuffer()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	// 부모인 VIBuffer Class의 Ready_VIBuffer함수에서,
	// CreateVertexBuffer 함수를 실행하는데, 이 때 이 함수의 인자값들을 채워주기 위해,
	// 각각의 자식에서 맞는 인자값들을 셋팅해 준다.
	// 그러면 부모의 인자값으로 자식의 인자값들으 셋팅이 되어
	// 정상적으로 CreateVertexBuffer 함수를 실행하게 된다.
	m_iVtxSize = sizeof(VTXCOL);
	// 인덱스 버퍼를 사용 할 예정이라 정점의 수는 총 4개이다.
	m_iNumVertices = 4;
	m_dwVtxFVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;
	m_iNumPolygons = 2;

	m_iIndexSize = sizeof(INDEX16);
	m_Format = D3DFMT_INDEX16;

	// 정점버퍼를 할당한다 + 인덱스버퍼를 할당한다.
	if (FAILED(CVIBuffer::Ready_VIBuffer()))
		return E_FAIL;

	VTXCOL*		pVertices = nullptr;

	// 메인 쓰레드에서 이제부터 정점을 찍어야 하는데,
	// 다른 쓰레드에서 접근하지 못하도록 Lock을 걸어준다.
	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	pVertices[0].vPosition = _vec3(0.5f, 0.5f, 0.f);
	m_pPositions[0] = pVertices[0].vPosition;
	pVertices[0].dwColor = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);

	pVertices[1].vPosition = _vec3(-0.5f, 0.5f, 0.f);
	m_pPositions[1] = pVertices[1].vPosition;
	pVertices[1].dwColor = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);

	pVertices[2].vPosition = _vec3(-0.5f, -0.5f, 0.f);
	m_pPositions[2] = pVertices[2].vPosition;
	pVertices[2].dwColor = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);

	pVertices[3].vPosition = _vec3(0.5f, -0.5f, 0.f);
	m_pPositions[3] = pVertices[3].vPosition;
	pVertices[3].dwColor = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);

	//※=========[후면 추려내기]========※//
	// dx에서는 객체의 앞면 뒷면이 존재하며, 후면은 출력을  안해주는 기능이 존재한다.
	// 정점을 시계 방향으로 찍으면 앞면으로 인식하는데, 
	// 이게 MainApp Class의 Ready_Render_State 함수에 m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE); 기능이다.
	// 여기서 D3DCULL_D3DCULL_CCW 이 기본적으로 적용 되는데,
	// 이는 시계 방향으로 찍힌 정점들만 출력하고 반시계 방향들에 대해서는 출력을 하지 않겠다 라는 뜻이다.

	// pVertices[3] - pVertices[2] 해서 나오는 방향벡터와,
	// pVertices[2] - pVertices[1] 해서 나오는 방향벡터를 외적하면,
	// 우리가 보는 모니터의 맞은편 방향을 Z축으로 가지는 수직인 벡터값이 도출되는데,
	// 이는 왼손좌표계 기준으로 z값이 1이며 양수로 존재하게 되는데,
	// 정점을 찍을때는 이렇게 z값이 1이되게 찍는게 좋다.
	// 하지만 이러면 위의 D3DCULL_D3DCULL_CCW로 인해 출력이 되지 않으므로,
	// D3DCULL_D3DCULL_NONE으로 바꾸어 주어야 한다.
	// 하지만 나는 D3DCULL_D3DCULL_CW로 해야지 데헷★

	// 다 찍었으니 풀어준다.

	m_pVB->Unlock();

	INDEX16*	pIndices = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	pIndices[0]._1 = 0;
	pIndices[0]._2 = 1;
	pIndices[0]._3 = 2;

	pIndices[1]._1 = 0;
	pIndices[1]._2 = 2;
	pIndices[1]._3 = 3;

	m_pIB->Unlock();

	return NOERROR;
}

void CBuffer_RcCol::Render_Buffer(const CTransform* pTransform)
{
	if (nullptr == m_pGraphic_Device)
		return;

	if (nullptr != pTransform)
	{
		_matrix		matTransform = *pTransform->Get_WorldMatrixPointer();

		// 정점의 위치에 행렬을 곱해서 정점의 위치를 변환하자.
		VTXCOL*		pVertices = nullptr;

		m_pVB->Lock(0, 0, (void**)&pVertices, 0);

		for (size_t i = 0; i < m_iNumVertices; i++)
		{
			D3DXVec3TransformCoord(&pVertices[i].vPosition, &m_pPositions[i], &matTransform);
		}

		m_pVB->Unlock();
	}

	// 정점을 찍어주는 구문 //
	m_pGraphic_Device->SetStreamSource(0, m_pVB, 0, m_iVtxSize);
	m_pGraphic_Device->SetFVF(m_dwVtxFVF);
	m_pGraphic_Device->SetIndices(m_pIB);
	m_pGraphic_Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_iNumVertices, 0, m_iNumPolygons);
	//////////////////////////

}

CBuffer_RcCol * CBuffer_RcCol::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBuffer_RcCol* pInstance = new CBuffer_RcCol(pGraphic_Device);

	if (FAILED(pInstance->Ready_VIBuffer()))
	{
		_MSGBOX("CBuffer_RcCol Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CBuffer_RcCol::Clone()
{
	return new CBuffer_RcCol(*this);
}

void CBuffer_RcCol::Free()
{
	CVIBuffer::Free();
}
