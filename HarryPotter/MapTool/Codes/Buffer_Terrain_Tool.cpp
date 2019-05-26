#include "stdafx.h"
#include "..\Headers\Buffer_Terrain_Tool.h"
#include "Transform.h"
#include "DataManager.h"

CBuffer_Terrain_Tool::CBuffer_Terrain_Tool(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer(pGraphic_Device)
{
}

HRESULT CBuffer_Terrain_Tool::Ready_VIBuffer()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_iVtxSize = sizeof(VTXNORTEX);
	m_fInterval = 1.f;
	m_iNumVerticesX = 100;
	m_iNumVerticesZ = 100;

	m_iNumVertices = m_iNumVerticesX * m_iNumVerticesZ;

	m_dwVtxFVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;
	m_iNumPolygons = (m_iNumVerticesX - 1) * (m_iNumVerticesZ - 1) * 2;

	m_iIndexSize = sizeof(INDEX16);
	m_Format = D3DFMT_INDEX16;

	// �������۸� �Ҵ��Ѵ� + �ε������۸� �Ҵ��Ѵ�.
	if (FAILED(CVIBuffer::Ready_VIBuffer()))
		return E_FAIL;

	VTXNORTEX*		pVertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	for (size_t i = 0; i < m_iNumVerticesZ; ++i)
	{
		for (size_t j = 0; j < m_iNumVerticesX; ++j)
		{
			_uint		iIndex = i * m_iNumVerticesX + j;

			pVertices[iIndex].vPosition = _vec3(j * m_fInterval, 0.0f, i * m_fInterval);
			m_pPositions[iIndex] = pVertices[iIndex].vPosition;
			// ���̴��� Detail ���� �����Ҳ��� ���� m_fDetail�� �ʿ� ����.
			pVertices[iIndex].vTexUV = _vec2(j / (m_iNumVerticesX - 1.f)/* * m_fDetail*/, i / (m_iNumVerticesZ - 1.f)/* * m_fDetail*/);
		}
	}

	m_pVB->Unlock();


	INDEX16*	pIndices = nullptr;

	_uint		iNumPolygons = 0;

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	for (size_t i = 0; i < m_iNumVerticesZ - 1; i++)
	{
		for (size_t j = 0; j < m_iNumVerticesX - 1; j++)
		{
			_uint		iIndex = i * m_iNumVerticesX + j;

			pIndices[iNumPolygons]._1 = iIndex + m_iNumVerticesX;
			pIndices[iNumPolygons]._2 = iIndex + m_iNumVerticesX + 1;
			pIndices[iNumPolygons]._3 = iIndex + 1;

			++iNumPolygons;

			pIndices[iNumPolygons]._1 = iIndex + m_iNumVerticesX;
			pIndices[iNumPolygons]._2 = iIndex + 1;
			pIndices[iNumPolygons]._3 = iIndex;
			++iNumPolygons;
		}
	}

	memcpy(m_pIndices, pIndices, sizeof(INDEX16) * m_iNumPolygons);

	m_pIB->Unlock();

	CDataManager::GetInstance()->Set_Picking_Variable(m_Format, m_iNumPolygons, m_pPositions, m_pIndices);

	return NOERROR;
}

HRESULT CBuffer_Terrain_Tool::Ready_VIBuffer(const _tchar * pHeighitMapPath)
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	HANDLE		hFile = 0;
	_ulong		dwByte = 0;

	hFile = CreateFile(pHeighitMapPath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	BITMAPFILEHEADER		fh;
	BITMAPINFOHEADER		ih;

	ReadFile(hFile, &fh, sizeof(BITMAPFILEHEADER), &dwByte, nullptr);
	ReadFile(hFile, &ih, sizeof(BITMAPINFOHEADER), &dwByte, nullptr);

	_ulong*	pPixel = new _ulong[ih.biWidth * ih.biHeight];

	ReadFile(hFile, pPixel, sizeof(_ulong) * ih.biWidth * ih.biHeight, &dwByte, nullptr);

	CloseHandle(hFile);

	m_iVtxSize = sizeof(VTXNORTEX);
	m_fInterval = 1.f;
	m_iNumVerticesX = ih.biWidth;
	m_iNumVerticesZ = ih.biHeight;
	m_iNumVertices = m_iNumVerticesX * m_iNumVerticesZ;

	m_dwVtxFVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;
	m_iNumPolygons = (m_iNumVerticesX - 1) * (m_iNumVerticesZ - 1) * 2;

	m_iIndexSize = sizeof(INDEX16);
	m_Format = D3DFMT_INDEX16;

	// �������۸� �Ҵ��Ѵ� + �ε������۸� �Ҵ��Ѵ�.
	if (FAILED(CVIBuffer::Ready_VIBuffer()))
		return E_FAIL;

	//	10011100 10011100 10011100 10011100
	//
	//&	00000000 00000000 00000000 11111111
	//
	//	00000000 00000000 00000000 10011100

	VTXNORTEX*		pVertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	for (size_t i = 0; i < m_iNumVerticesZ; ++i)
	{
		for (size_t j = 0; j < m_iNumVerticesX; ++j)
		{
			_uint		iIndex = i * m_iNumVerticesX + j;

			_float vHeight = 10.f;
			pVertices[iIndex].vPosition = _vec3(j * m_fInterval, (pPixel[iIndex] & 0x000000ff) / vHeight, i * m_fInterval);
			pVertices[iIndex].vNormal = _vec3(0.f, 0.f, 0.f);
			m_pPositions[iIndex] = pVertices[iIndex].vPosition;
			pVertices[iIndex].vTexUV = _vec2(j / (m_iNumVerticesX - 1.f)/* * m_fDetail*/, i / (m_iNumVerticesZ - 1.f)/* * m_fDetail*/);
		}
	}

	m_pVB->Unlock();

	Safe_Delete(pPixel);

	INDEX16*	pIndices = nullptr;

	_uint		iNumPolygons = 0;

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	for (size_t i = 0; i < m_iNumVerticesZ - 1; i++)
	{
		for (size_t j = 0; j < m_iNumVerticesX - 1; j++)
		{
			_uint		iIndex = i * m_iNumVerticesX + j;

			pIndices[iNumPolygons]._1 = iIndex + m_iNumVerticesX;
			pIndices[iNumPolygons]._2 = iIndex + m_iNumVerticesX + 1;
			pIndices[iNumPolygons]._3 = iIndex + 1;

			{
				_vec3	FaceNormal;
				// n ��° �������� �������͸� ������.
				// 0 ��° �ﰢ���� 1 ��° ����, 2 ��° ����, 3 ��° ������ �����Ͽ� ���⺤�͸� ���� ��,
				// �� ���⺤�͸� �����ؼ� ������ �������͸� FaceNormal�� �־��ش�.
				ComputeNormal(&pVertices[pIndices[iNumPolygons]._1].vPosition, &pVertices[pIndices[iNumPolygons]._2].vPosition, &pVertices[pIndices[iNumPolygons]._3].vPosition, &FaceNormal);

				// �ش� ���� �������͸� �� �������� ������ ��ֺ��Ϳ� �־��ָ� �Ǵµ�,
				// �� ��, +=���� �����ؼ� �־��ָ�, ������ �����ϴ� �� ���� ���������� ���� �ڵ������� ���� �־��ְ� �ȴ�.
				// �׸��� �ۿ��� CGameObject::Set_RenderState(D3DRS_NORMALIZENORMALS, true); �� ���ָ�,
				// ��� ���� ����ȴ�.
				pVertices[pIndices[iNumPolygons]._1].vNormal += FaceNormal;
				pVertices[pIndices[iNumPolygons]._2].vNormal += FaceNormal;
				pVertices[pIndices[iNumPolygons]._3].vNormal += FaceNormal;
			}

			++iNumPolygons;

			pIndices[iNumPolygons]._1 = iIndex + m_iNumVerticesX;
			pIndices[iNumPolygons]._2 = iIndex + 1;
			pIndices[iNumPolygons]._3 = iIndex;

			{
				_vec3	FaceNormal;
				// n+1 ��° �������� �������͸� ������.
				// ��� 1 ��° �ﰢ���̴�.
				// �긦 ó�������ν�, �ϳ��� ������ �ﰢ���� �ƴ� �簢���� �ϼ��ȴ�.
				ComputeNormal(&pVertices[pIndices[iNumPolygons]._1].vPosition, &pVertices[pIndices[iNumPolygons]._2].vPosition, &pVertices[pIndices[iNumPolygons]._3].vPosition, &FaceNormal);

				pVertices[pIndices[iNumPolygons]._1].vNormal += FaceNormal;
				pVertices[pIndices[iNumPolygons]._2].vNormal += FaceNormal;
				pVertices[pIndices[iNumPolygons]._3].vNormal += FaceNormal;
			}

			++iNumPolygons;
		}
	}
	memcpy(m_pIndices, pIndices, sizeof(INDEX16) * m_iNumPolygons);

	// ���� ���⼭ ��ֶ���� ��ȸ�ϸ鼭 ���� �ʿ䰡 ����.
	// Terrain.cpp�� RenderState ���� �κп�,
	// CGameObject::Set_RenderState(D3DRS_NORMALIZENORMALS, true);	 ����ȿ���� �ȴ�.

	//for (size_t i = 0; i < m_iNumVerticesZ; ++i)
	//{
	//	for (size_t j = 0; j < m_iNumVerticesX; ++j)
	//	{
	//		_uint		iIndex = i * m_iNumVerticesX + j;

	//		D3DXVec3Normalize(&pVertices[iIndex].vNormal, &pVertices[iIndex].vNormal);
	//	}
	//}

	m_pIB->Unlock();

	CDataManager::GetInstance()->Set_Picking_Variable(m_Format, m_iNumPolygons, m_pPositions, m_pIndices);

	return NOERROR;
}

void CBuffer_Terrain_Tool::Render_Buffer(const CTransform* pTransform)
{
	if (nullptr == m_pGraphic_Device)
		return;

	if (nullptr != pTransform)
	{
		_matrix		matTransform = *pTransform->Get_WorldMatrixPointer();

		// ������ ��ġ�� ����� ���ؼ� ������ ��ġ�� ��ȯ����.
		VTXNORTEX*		pVertices = nullptr;

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

_float CBuffer_Terrain_Tool::Compute_HeightOnBuffer(const CTransform * pTransform)
{
	_vec3		vPosition = *pTransform->Get_StateInfo(CTransform::STATE_POSITION);

	// �����콺.y / Ÿ�ϻ�����.y * Ÿ���� ���� ���� + �����콺.x / Ÿ�ϻ�����.x
	_uint		iIndex = _uint(vPosition.z / m_fInterval) * (m_iNumVerticesX)+_uint(vPosition.x / m_fInterval);

	_uint		iIndices[4] = { iIndex + m_iNumVerticesX, iIndex + m_iNumVerticesX + 1, iIndex + 1, iIndex };

	_float		fRatioX = (vPosition.x - m_pPositions[iIndices[0]].x) / m_fInterval;
	_float		fRatioZ = (m_pPositions[iIndices[0]].z - vPosition.z) / m_fInterval;

	_float		fHeight[4] = { m_pPositions[iIndices[0]].y, m_pPositions[iIndices[1]].y, m_pPositions[iIndices[2]].y, m_pPositions[iIndices[3]].y };

	if (fRatioX > fRatioZ) // ������ ��
		return fHeight[0] + (fHeight[1] - fHeight[0]) * fRatioX + (fHeight[2] - fHeight[1]) * fRatioZ;

	else // ���� �Ʒ�
		return fHeight[0] + (fHeight[3] - fHeight[0]) * fRatioZ + (fHeight[2] - fHeight[3]) * fRatioX;

}

HRESULT CBuffer_Terrain_Tool::Reset_Terrain(_uint _iNumVtxX, _uint _iNumVtxZ, _float _fInterval/*, _float _fDetail*/)
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	// ����? �ϸ� ����...
	Safe_Delete_Array(m_pPositions);
	Safe_Delete_Array(m_pIndices);

	Safe_Release(m_pVB);
	Safe_Release(m_pIB);

	//Free();

	m_iVtxSize = sizeof(VTXNORTEX);
	m_fInterval = _fInterval;
	m_iNumVerticesX = _iNumVtxX;
	m_iNumVerticesZ = _iNumVtxZ;

	m_iNumVertices = m_iNumVerticesX * m_iNumVerticesZ;

	m_dwVtxFVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;
	m_iNumPolygons = (m_iNumVerticesX - 1) * (m_iNumVerticesZ - 1) * 2;

	m_iIndexSize = sizeof(INDEX16);
	m_Format = D3DFMT_INDEX16;

	// �������۸� �Ҵ��Ѵ� + �ε������۸� �Ҵ��Ѵ�.
	if (FAILED(CVIBuffer::Ready_VIBuffer()))
		return E_FAIL;

	VTXNORTEX*		pVertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	for (size_t i = 0; i < m_iNumVerticesZ; ++i)
	{
		for (size_t j = 0; j < m_iNumVerticesX; ++j)
		{
			_uint		iIndex = i * m_iNumVerticesX + j;

			pVertices[iIndex].vPosition = _vec3(j * m_fInterval, 0.0f, i * m_fInterval);
			m_pPositions[iIndex] = pVertices[iIndex].vPosition;
			pVertices[iIndex].vTexUV = _vec2(j / (m_iNumVerticesX - 1.f)/* * _fDetail*/, i / (m_iNumVerticesZ - 1.f)/* * _fDetail*/);
		}
	}

	m_pVB->Unlock();


	INDEX16*	pIndices = nullptr;

	_uint		iNumPolygons = 0;

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	for (size_t i = 0; i < m_iNumVerticesZ - 1; i++)
	{
		for (size_t j = 0; j < m_iNumVerticesX - 1; j++)
		{
			_uint		iIndex = i * m_iNumVerticesX + j;

			pIndices[iNumPolygons]._1 = iIndex + m_iNumVerticesX;
			pIndices[iNumPolygons]._2 = iIndex + m_iNumVerticesX + 1;
			pIndices[iNumPolygons]._3 = iIndex + 1;

			++iNumPolygons;

			pIndices[iNumPolygons]._1 = iIndex + m_iNumVerticesX;
			pIndices[iNumPolygons]._2 = iIndex + 1;
			pIndices[iNumPolygons]._3 = iIndex;
			++iNumPolygons;
		}
	}

	memcpy(m_pIndices, pIndices, sizeof(INDEX16) * m_iNumPolygons);

	m_pIB->Unlock();

	CDataManager::GetInstance()->Set_Picking_Variable(m_Format, m_iNumPolygons, m_pPositions, m_pIndices);

	return NOERROR;
}

void CBuffer_Terrain_Tool::ComputeNormal(_vec3* pVtx0, _vec3* pVtx1, _vec3* pVtx2, _vec3* pOut)
{
	_vec3 u = *pVtx1 - *pVtx0;
	_vec3 v = *pVtx2 - *pVtx0;

	D3DXVec3Cross(pOut, &u, &v);
	D3DXVec3Normalize(pOut, pOut);
}

CBuffer_Terrain_Tool * CBuffer_Terrain_Tool::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBuffer_Terrain_Tool* pInstance = new CBuffer_Terrain_Tool(pGraphic_Device);

	if (FAILED(pInstance->Ready_VIBuffer()))
	{
		_MSGBOX("CBuffer_Terrain_Tool Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CBuffer_Terrain_Tool * CBuffer_Terrain_Tool::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar * pHeighitMapPath)
{
	CBuffer_Terrain_Tool* pInstance = new CBuffer_Terrain_Tool(pGraphic_Device);

	if (FAILED(pInstance->Ready_VIBuffer(pHeighitMapPath)))
	{
		_MSGBOX("CBuffer_Terrain_Tool Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CBuffer_Terrain_Tool::Clone()
{
	AddRef();

	return this;
}

void CBuffer_Terrain_Tool::Free()
{
	CVIBuffer::Free();
}
