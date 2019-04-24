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

	// �θ��� VIBuffer Class�� Ready_VIBuffer�Լ�����,
	// CreateVertexBuffer �Լ��� �����ϴµ�, �� �� �� �Լ��� ���ڰ����� ä���ֱ� ����,
	// ������ �ڽĿ��� �´� ���ڰ����� ������ �ش�.
	// �׷��� �θ��� ���ڰ����� �ڽ��� ���ڰ����� ������ �Ǿ�
	// ���������� CreateVertexBuffer �Լ��� �����ϰ� �ȴ�.
	m_iVtxSize = sizeof(VTXCOL);
	// �ε��� ���۸� ��� �� �����̶� ������ ���� �� 4���̴�.
	m_iNumVertices = 4;
	m_dwVtxFVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;
	m_iNumPolygons = 2;

	m_iIndexSize = sizeof(INDEX16);
	m_Format = D3DFMT_INDEX16;

	// �������۸� �Ҵ��Ѵ� + �ε������۸� �Ҵ��Ѵ�.
	if (FAILED(CVIBuffer::Ready_VIBuffer()))
		return E_FAIL;

	VTXCOL*		pVertices = nullptr;

	// ���� �����忡�� �������� ������ ���� �ϴµ�,
	// �ٸ� �����忡�� �������� ���ϵ��� Lock�� �ɾ��ش�.
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

	//��=========[�ĸ� �߷�����]========��//
	// dx������ ��ü�� �ո� �޸��� �����ϸ�, �ĸ��� �����  �����ִ� ����� �����Ѵ�.
	// ������ �ð� �������� ������ �ո����� �ν��ϴµ�, 
	// �̰� MainApp Class�� Ready_Render_State �Լ��� m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE); ����̴�.
	// ���⼭ D3DCULL_D3DCULL_CCW �� �⺻������ ���� �Ǵµ�,
	// �̴� �ð� �������� ���� �����鸸 ����ϰ� �ݽð� ����鿡 ���ؼ��� ����� ���� �ʰڴ� ��� ���̴�.

	// pVertices[3] - pVertices[2] �ؼ� ������ ���⺤�Ϳ�,
	// pVertices[2] - pVertices[1] �ؼ� ������ ���⺤�͸� �����ϸ�,
	// �츮�� ���� ������� ������ ������ Z������ ������ ������ ���Ͱ��� ����Ǵµ�,
	// �̴� �޼���ǥ�� �������� z���� 1�̸� ����� �����ϰ� �Ǵµ�,
	// ������ �������� �̷��� z���� 1�̵ǰ� ��°� ����.
	// ������ �̷��� ���� D3DCULL_D3DCULL_CCW�� ���� ����� ���� �����Ƿ�,
	// D3DCULL_D3DCULL_NONE���� �ٲپ� �־�� �Ѵ�.
	// ������ ���� D3DCULL_D3DCULL_CW�� �ؾ��� �����

	// �� ������� Ǯ���ش�.

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

		// ������ ��ġ�� ����� ���ؼ� ������ ��ġ�� ��ȯ����.
		VTXCOL*		pVertices = nullptr;

		m_pVB->Lock(0, 0, (void**)&pVertices, 0);

		for (size_t i = 0; i < m_iNumVertices; i++)
		{
			D3DXVec3TransformCoord(&pVertices[i].vPosition, &m_pPositions[i], &matTransform);
		}

		m_pVB->Unlock();
	}

	// ������ ����ִ� ���� //
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
