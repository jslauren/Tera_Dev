#include "..\Headers\Buffer_Terrain.h"
#include "Transform.h"

CBuffer_Terrain::CBuffer_Terrain(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer(pGraphic_Device)
{
}

CBuffer_Terrain::CBuffer_Terrain(const CBuffer_Terrain & rhs)
	: CVIBuffer(rhs)
	, m_iNumVerticesX(rhs.m_iNumVerticesX)
	, m_iNumVerticesZ(rhs.m_iNumVerticesZ)
	, m_fInterval(rhs.m_fInterval)
{
}

HRESULT CBuffer_Terrain::Ready_VIBuffer()
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

	// 정점버퍼를 할당한다 + 인덱스버퍼를 할당한다.
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
			pVertices[iIndex].vTexUV = _vec2(j / (m_iNumVerticesX - 1.f) * m_fDetail, i / (m_iNumVerticesZ - 1.f) * m_fDetail);
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

	return NOERROR;
}

HRESULT CBuffer_Terrain::Ready_VIBuffer(_int iNumVtxX, _int iNumVtxZ, _float fInterval, _float fDetail)
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_iVtxSize = sizeof(VTXNORTEX);
	m_fInterval = fInterval;
	m_iNumVerticesX = iNumVtxX;
	m_iNumVerticesZ = iNumVtxZ;
	m_iNumVertices = m_iNumVerticesX * m_iNumVerticesZ;

	m_dwVtxFVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;
	m_iNumPolygons = (m_iNumVerticesX - 1) * (m_iNumVerticesZ - 1) * 2;

	m_iIndexSize = sizeof(INDEX16);
	m_Format = D3DFMT_INDEX16;

	// 정점버퍼를 할당한다 + 인덱스버퍼를 할당한다.
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
			pVertices[iIndex].vNormal = _vec3(0.0f, 0.f, 0.f);
			m_pPositions[iIndex] = pVertices[iIndex].vPosition;
			pVertices[iIndex].vTexUV = _vec2(j / (m_iNumVerticesX - 1.f) /** fDetail*/, i / (m_iNumVerticesZ - 1.f) /** fDetail*/);
		}
	}

	INDEX16*	pIndices = nullptr;

	_uint		iNumPolygons = 0;

	_vec3		vDest, vSour, vNormal;

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	for (size_t i = 0; i < m_iNumVerticesZ - 1; i++)
	{
		for (size_t j = 0; j < m_iNumVerticesX - 1; j++)
		{
			_uint		iIndex = i * m_iNumVerticesX + j;

			pIndices[iNumPolygons]._1 = iIndex + m_iNumVerticesX;
			pIndices[iNumPolygons]._2 = iIndex + m_iNumVerticesX + 1;
			pIndices[iNumPolygons]._3 = iIndex + 1;

			vDest = pVertices[pIndices[iNumPolygons]._2].vPosition - pVertices[pIndices[iNumPolygons]._1].vPosition;
			vSour = pVertices[pIndices[iNumPolygons]._3].vPosition - pVertices[pIndices[iNumPolygons]._1].vPosition;
			D3DXVec3Normalize(&vNormal, D3DXVec3Cross(&vNormal, &vDest, &vSour));

			pVertices[pIndices[iNumPolygons]._1].vNormal += vNormal;
			pVertices[pIndices[iNumPolygons]._2].vNormal += vNormal;
			pVertices[pIndices[iNumPolygons]._3].vNormal += vNormal;
			++iNumPolygons;

			pIndices[iNumPolygons]._1 = iIndex + m_iNumVerticesX;
			pIndices[iNumPolygons]._2 = iIndex + 1;
			pIndices[iNumPolygons]._3 = iIndex;

			vDest = pVertices[pIndices[iNumPolygons]._2].vPosition - pVertices[pIndices[iNumPolygons]._1].vPosition;
			vSour = pVertices[pIndices[iNumPolygons]._3].vPosition - pVertices[pIndices[iNumPolygons]._1].vPosition;
			D3DXVec3Normalize(&vNormal, D3DXVec3Cross(&vNormal, &vDest, &vSour));

			pVertices[pIndices[iNumPolygons]._1].vNormal += vNormal;
			pVertices[pIndices[iNumPolygons]._2].vNormal += vNormal;
			pVertices[pIndices[iNumPolygons]._3].vNormal += vNormal;

			++iNumPolygons;
		}
	}
	memcpy(m_pIndices, pIndices, sizeof(INDEX16) * m_iNumPolygons);

	for (size_t i = 0; i < m_iNumVertices; i++)
		D3DXVec3Normalize(&pVertices[i].vNormal, &pVertices[i].vNormal);

	m_pVB->Unlock();
	m_pIB->Unlock();

	return NOERROR;
}

HRESULT CBuffer_Terrain::Ready_VIBuffer(const _tchar * pHeighitMapPath)
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

	// 정점버퍼를 할당한다 + 인덱스버퍼를 할당한다.
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

			pVertices[iIndex].vPosition = _vec3(j * m_fInterval, (pPixel[iIndex] & 0x000000ff) / 10.f, i * m_fInterval);
			pVertices[iIndex].vNormal = _vec3(0.0f, 0.f, 0.f);
			m_pPositions[iIndex] = pVertices[iIndex].vPosition;
			pVertices[iIndex].vTexUV = _vec2(j / (m_iNumVerticesX - 1.f), i / (m_iNumVerticesZ - 1.f));
		}
	}

	INDEX16*	pIndices = nullptr;

	_uint		iNumPolygons = 0;

	_vec3		vDest, vSour, vNormal;

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	for (size_t i = 0; i < m_iNumVerticesZ - 1; i++)
	{
		for (size_t j = 0; j < m_iNumVerticesX - 1; j++)
		{
			_uint		iIndex = i * m_iNumVerticesX + j;

			pIndices[iNumPolygons]._1 = iIndex + m_iNumVerticesX;
			pIndices[iNumPolygons]._2 = iIndex + m_iNumVerticesX + 1;
			pIndices[iNumPolygons]._3 = iIndex + 1;

			vDest = pVertices[pIndices[iNumPolygons]._2].vPosition - pVertices[pIndices[iNumPolygons]._1].vPosition;
			vSour = pVertices[pIndices[iNumPolygons]._3].vPosition - pVertices[pIndices[iNumPolygons]._1].vPosition;
			D3DXVec3Normalize(&vNormal, D3DXVec3Cross(&vNormal, &vDest, &vSour));

			pVertices[pIndices[iNumPolygons]._1].vNormal += vNormal;
			pVertices[pIndices[iNumPolygons]._2].vNormal += vNormal;
			pVertices[pIndices[iNumPolygons]._3].vNormal += vNormal;
			++iNumPolygons;

			pIndices[iNumPolygons]._1 = iIndex + m_iNumVerticesX;
			pIndices[iNumPolygons]._2 = iIndex + 1;
			pIndices[iNumPolygons]._3 = iIndex;

			vDest = pVertices[pIndices[iNumPolygons]._2].vPosition - pVertices[pIndices[iNumPolygons]._1].vPosition;
			vSour = pVertices[pIndices[iNumPolygons]._3].vPosition - pVertices[pIndices[iNumPolygons]._1].vPosition;
			D3DXVec3Normalize(&vNormal, D3DXVec3Cross(&vNormal, &vDest, &vSour));

			pVertices[pIndices[iNumPolygons]._1].vNormal += vNormal;
			pVertices[pIndices[iNumPolygons]._2].vNormal += vNormal;
			pVertices[pIndices[iNumPolygons]._3].vNormal += vNormal;

			++iNumPolygons;
		}
	}
	memcpy(m_pIndices, pIndices, sizeof(INDEX16) * m_iNumPolygons);

	for (size_t i = 0; i < m_iNumVertices; i++)
		D3DXVec3Normalize(&pVertices[i].vNormal, &pVertices[i].vNormal);

	Safe_Delete_Array(pPixel);

	m_pVB->Unlock();
	m_pIB->Unlock();

	return NOERROR;
}

// 내가 만든거
//HRESULT CBuffer_Terrain::Ready_VIBuffer(const _tchar * pHeighitMapPath)
//{
//	if (nullptr == m_pGraphic_Device)
//		return E_FAIL;
//
//	HANDLE		hFile = 0;
//	_ulong		dwByte = 0;
//
//	hFile = CreateFile(pHeighitMapPath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
//	if (0 == hFile)
//		return E_FAIL;
//
//	BITMAPFILEHEADER		fh;
//	BITMAPINFOHEADER		ih;
//
//	ReadFile(hFile, &fh, sizeof(BITMAPFILEHEADER), &dwByte, nullptr);
//	ReadFile(hFile, &ih, sizeof(BITMAPINFOHEADER), &dwByte, nullptr);
//
//	_ulong*	pPixel = new _ulong[ih.biWidth * ih.biHeight];
//
//	ReadFile(hFile, pPixel, sizeof(_ulong) * ih.biWidth * ih.biHeight, &dwByte, nullptr);
//
//	CloseHandle(hFile);
//
//	m_iVtxSize = sizeof(VTXNORTEX);
//	m_fInterval = 1.f;
//	m_iNumVerticesX = ih.biWidth;
//	m_iNumVerticesZ = ih.biHeight;
//	m_iNumVertices = m_iNumVerticesX * m_iNumVerticesZ;
//
//	m_dwVtxFVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;
//	m_iNumPolygons = (m_iNumVerticesX - 1) * (m_iNumVerticesZ - 1) * 2;
//
//	m_iIndexSize = sizeof(INDEX16);
//	m_Format = D3DFMT_INDEX16;
//
//	// 정점버퍼를 할당한다 + 인덱스버퍼를 할당한다.
//	if (FAILED(CVIBuffer::Ready_VIBuffer()))
//		return E_FAIL;
//
//	//	10011100 10011100 10011100 10011100
//	//
//	//&	00000000 00000000 00000000 11111111
//	//
//	//	00000000 00000000 00000000 10011100
//
//	VTXNORTEX*		pVertices = nullptr;
//
//	m_pVB->Lock(0, 0, (void**)&pVertices, 0);
//
//	for (size_t i = 0; i < m_iNumVerticesZ; ++i)
//	{
//		for (size_t j = 0; j < m_iNumVerticesX; ++j)
//		{
//			_uint		iIndex = i * m_iNumVerticesX + j;
//
//			_float vHeight = 10.f;
//			pVertices[iIndex].vPosition = _vec3(j * m_fInterval, (pPixel[iIndex] & 0x000000ff) / vHeight, i * m_fInterval);
//			pVertices[iIndex].vNormal = _vec3(0.f, 0.f, 0.f);
//			m_pPositions[iIndex] = pVertices[iIndex].vPosition;
//			pVertices[iIndex].vTexUV = _vec2(j / (m_iNumVerticesX - 1.f) * m_fDetail, i / (m_iNumVerticesZ - 1.f) * m_fDetail);
//		}
//	}
//
//	m_pVB->Unlock();
//
//	Safe_Delete(pPixel);
//
//	INDEX16*	pIndices = nullptr;
//
//	_uint		iNumPolygons = 0;
//
//	m_pIB->Lock(0, 0, (void**)&pIndices, 0);
//
//	for (size_t i = 0; i < m_iNumVerticesZ - 1; i++)
//	{
//		for (size_t j = 0; j < m_iNumVerticesX - 1; j++)
//		{
//			_uint		iIndex = i * m_iNumVerticesX + j;
//
//			pIndices[iNumPolygons]._1 = iIndex + m_iNumVerticesX;
//			pIndices[iNumPolygons]._2 = iIndex + m_iNumVerticesX + 1;
//			pIndices[iNumPolygons]._3 = iIndex + 1;
//
//			{
//				_vec3	FaceNormal;
//				// n 번째 폴리곤의 법선벡터를 구했음.
//				// 0 번째 삼각형의 1 번째 정점, 2 번째 정점, 3 번째 정점을 연산하여 방향벡터를 구한 뒤,
//				// 그 방향벡터를 외적해서 나오는 법선벡터를 FaceNormal에 넣어준다.
//				ComputeNormal(&pVertices[pIndices[iNumPolygons]._1].vPosition, &pVertices[pIndices[iNumPolygons]._2].vPosition, &pVertices[pIndices[iNumPolygons]._3].vPosition, &FaceNormal);
//
//				// 해당 면의 법선벡터를 각 정점들이 가지는 노멀벡터에 넣어주면 되는데,
//				// 이 때, +=으로 누적해서 넣어주면, 정점을 공유하는 면 들의 법선벡터의 합을 자동적으로 구해 넣어주게 된다.
//				// 그리고 밖에서 CGameObject::Set_RenderState(D3DRS_NORMALIZENORMALS, true); 을 해주면,
//				// 평균 값이 적용된다.
//				pVertices[pIndices[iNumPolygons]._1].vNormal += FaceNormal;
//				pVertices[pIndices[iNumPolygons]._2].vNormal += FaceNormal;
//				pVertices[pIndices[iNumPolygons]._3].vNormal += FaceNormal;
//			}
//
//			++iNumPolygons;
//
//			pIndices[iNumPolygons]._1 = iIndex + m_iNumVerticesX;
//			pIndices[iNumPolygons]._2 = iIndex + 1;
//			pIndices[iNumPolygons]._3 = iIndex;
//
//			{
//				_vec3	FaceNormal;
//				// n+1 번째 폴리곤의 법선벡터를 구했음.
//				// 얘는 1 번째 삼각형이다.
//				// 얘를 처리함으로써, 하나의 온전한 삼각형이 아닌 사각형이 완성된다.
//				ComputeNormal(&pVertices[pIndices[iNumPolygons]._1].vPosition, &pVertices[pIndices[iNumPolygons]._2].vPosition, &pVertices[pIndices[iNumPolygons]._3].vPosition, &FaceNormal);
//
//				pVertices[pIndices[iNumPolygons]._1].vNormal += FaceNormal;
//				pVertices[pIndices[iNumPolygons]._2].vNormal += FaceNormal;
//				pVertices[pIndices[iNumPolygons]._3].vNormal += FaceNormal;
//			}
//
//			++iNumPolygons;
//		}
//	}
//	memcpy(m_pIndices, pIndices, sizeof(INDEX16) * m_iNumPolygons);
//
//	// 굳이 여기서 노멀라이즈를 순회하면서 해줄 필요가 없다.
//	// Terrain.cpp에 RenderState 지정 부분에,
//	// CGameObject::Set_RenderState(D3DRS_NORMALIZENORMALS, true);	 같은효과가 된다.
//
//	for (size_t i = 0; i < m_iNumVerticesZ; ++i)
//	{
//		for (size_t j = 0; j < m_iNumVerticesX; ++j)
//		{
//			_uint		iIndex = i * m_iNumVerticesX + j;
//
//			D3DXVec3Normalize(&pVertices[iIndex].vNormal, &pVertices[iIndex].vNormal);
//		}
//	}
//
//	m_pIB->Unlock();
//
//	return NOERROR;
//}

void CBuffer_Terrain::Render_Buffer(const CTransform* pTransform)
{
	if (nullptr == m_pGraphic_Device)
		return;

	if (nullptr != pTransform)
	{
		_matrix		matTransform = *pTransform->Get_WorldMatrixPointer();

		// 정점의 위치에 행렬을 곱해서 정점의 위치를 변환하자.
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

_float CBuffer_Terrain::Compute_HeightOnBuffer(const CTransform * pTransform)
{
	_vec3		vPosition = *pTransform->Get_StateInfo(CTransform::STATE_POSITION);

	// 내마우스.y / 타일사이즈.y * 타일의 가로 갯수 + 내마우스.x / 타일사이즈.x
	_uint		iIndex = _uint(vPosition.z / m_fInterval) * (m_iNumVerticesX)+_uint(vPosition.x / m_fInterval);

	_uint		iIndices[4] = { iIndex + m_iNumVerticesX, iIndex + m_iNumVerticesX + 1, iIndex + 1, iIndex };

	_float		fRatioX = (vPosition.x - m_pPositions[iIndices[0]].x) / m_fInterval;
	_float		fRatioZ = (m_pPositions[iIndices[0]].z - vPosition.z) / m_fInterval;

	_float		fHeight[4] = { m_pPositions[iIndices[0]].y, m_pPositions[iIndices[1]].y, m_pPositions[iIndices[2]].y, m_pPositions[iIndices[3]].y };

	if (fRatioX > fRatioZ) // 오른쪽 위
		return fHeight[0] + (fHeight[1] - fHeight[0]) * fRatioX + (fHeight[2] - fHeight[1]) * fRatioZ;

	else // 왼쪽 아래
		return fHeight[0] + (fHeight[3] - fHeight[0]) * fRatioZ + (fHeight[2] - fHeight[3]) * fRatioX;

}

void CBuffer_Terrain::ComputeNormal(_vec3* pVtx0, _vec3* pVtx1, _vec3* pVtx2, _vec3* pOut)
{
	_vec3 u = *pVtx1 - *pVtx0;
	_vec3 v = *pVtx2 - *pVtx0;

	D3DXVec3Cross(pOut, &u, &v);
	D3DXVec3Normalize(pOut, pOut);
}

CBuffer_Terrain * CBuffer_Terrain::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBuffer_Terrain* pInstance = new CBuffer_Terrain(pGraphic_Device);

	if (FAILED(pInstance->Ready_VIBuffer()))
	{
		_MSGBOX("CBuffer_Terrain Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CBuffer_Terrain * CBuffer_Terrain::Create(LPDIRECT3DDEVICE9 pGraphic_Device, _int iNumVtxX, _int iNumVtxZ, _float fInterval, _float fDetail)
{
	CBuffer_Terrain* pInstance = new CBuffer_Terrain(pGraphic_Device);

	if (FAILED(pInstance->Ready_VIBuffer(iNumVtxX, iNumVtxZ, fInterval, fDetail)))
	{
		_MSGBOX("CBuffer_Terrain Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CBuffer_Terrain * CBuffer_Terrain::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar * pHeighitMapPath)
{
	CBuffer_Terrain* pInstance = new CBuffer_Terrain(pGraphic_Device);

	if (FAILED(pInstance->Ready_VIBuffer(pHeighitMapPath)))
	{
		_MSGBOX("CBuffer_Terrain Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CBuffer_Terrain::Clone()
{
	return new CBuffer_Terrain(*this);
}

void CBuffer_Terrain::Free()
{
	CVIBuffer::Free();
}
