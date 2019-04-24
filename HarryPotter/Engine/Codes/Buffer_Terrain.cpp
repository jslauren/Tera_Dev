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

	m_iVtxSize = sizeof(VTXTEX);
	m_fInterval = 1.f;
	m_iNumVerticesX = 100;
	m_iNumVerticesZ = 100;
	m_iNumVertices = m_iNumVerticesX * m_iNumVerticesZ;
	m_dwVtxFVF = D3DFVF_XYZ | D3DFVF_TEX1;

	// 사각형 개수 * 2 => 삼각형 2개가 사각형이고 폴리곤은 삼각형 기준이기 때문
	m_iNumPolygons = (m_iNumVerticesX - 1) * (m_iNumVerticesZ - 1) * 2;

	m_iIndexSize = sizeof(INDEX16);
	m_Format = D3DFMT_INDEX16;

	// 정점버퍼를 할당한다 + 인덱스버퍼를 할당한다.
	if (FAILED(CVIBuffer::Ready_VIBuffer()))
		return E_FAIL;

	VTXTEX*		pVertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	for (size_t i = 0; i < m_iNumVerticesZ; ++i)
	{
		for (size_t j = 0; j < m_iNumVerticesX; ++j)
		{
			_uint		iIndex = i * m_iNumVerticesX + j;

			pVertices[iIndex].vPosition = _vec3(j * m_fInterval, 0.0f, i * m_fInterval);
			m_pPositions[iIndex] = pVertices[iIndex].vPosition;
			pVertices[iIndex].vTexUV = _vec2(j / (m_iNumVerticesX - 1.f) * 100.f, i / (m_iNumVerticesZ - 1.f) * 100.f);
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
	m_pIB->Unlock();

	return NOERROR;
}

HRESULT CBuffer_Terrain::Ready_VIBuffer(const _tchar * pHeighitMapPath)
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	// 비트맵 이미지를 사용한 맵의 Y값 셋팅 //
	HANDLE		hFile = 0;
	_ulong		dwByte = 0;

	hFile = CreateFile(pHeighitMapPath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	BITMAPFILEHEADER		fh;
	BITMAPINFOHEADER		ih;

	// 비트맵의 파일정보, 이미지 정보 순으로 가져오기 때문에,
	// ReadFile을 두번한다음.
	ReadFile(hFile, &fh, sizeof(BITMAPFILEHEADER), &dwByte, nullptr);
	ReadFile(hFile, &ih, sizeof(BITMAPINFOHEADER), &dwByte, nullptr);

	_ulong*	pPixel = new _ulong[ih.biWidth * ih.biHeight];

	// 마지막인 픽셀정보들을 가져오기 위해 한번 더 ReadFile 해준다.
	ReadFile(hFile, pPixel, sizeof(_ulong) * ih.biWidth * ih.biHeight, &dwByte, nullptr);

	CloseHandle(hFile);

	m_iVtxSize = sizeof(VTXTEX);
	m_fInterval = 1.f;
	m_iNumVerticesX = ih.biWidth;
	m_iNumVerticesZ = ih.biHeight;
	m_iNumVertices = m_iNumVerticesX * m_iNumVerticesZ;

	m_dwVtxFVF = D3DFVF_XYZ | D3DFVF_TEX1;
	m_iNumPolygons = (m_iNumVerticesX - 1) * (m_iNumVerticesZ - 1) * 2;

	m_iIndexSize = sizeof(INDEX16);
	m_Format = D3DFMT_INDEX16;

	// 정점버퍼를 할당한다 + 인덱스버퍼를 할당한다.
	if (FAILED(CVIBuffer::Ready_VIBuffer()))
		return E_FAIL;

	// 밑에 (pPixel[iIndex] & 0x000000ff) 의 이해를 돕기위한 예제
	//	10011100 10011100 10011100 10011100
	//
	//&	00000000 00000000 00000000 11111111
	//
	//	00000000 00000000 00000000 10011100


	VTXTEX*		pVertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	for (size_t i = 0; i < m_iNumVerticesZ; ++i)
	{
		for (size_t j = 0; j < m_iNumVerticesX; ++j)
		{
			_uint		iIndex = i * m_iNumVerticesX + j;

			pVertices[iIndex].vPosition = _vec3(j * m_fInterval, (pPixel[iIndex] & 0x000000ff) / 10.f, i * m_fInterval);
			m_pPositions[iIndex] = pVertices[iIndex].vPosition;
			pVertices[iIndex].vTexUV = _vec2(j / (m_iNumVerticesX - 1.f) * 100.f, i / (m_iNumVerticesZ - 1.f) * 100.f);
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
	m_pIB->Unlock();

	return NOERROR;
}

void CBuffer_Terrain::Render_Buffer(const CTransform* pTransform)
{
	if (nullptr == m_pGraphic_Device)
		return;

	if (nullptr != pTransform)
	{
		_matrix		matTransform = *pTransform->Get_WorldMatrixPointer();

		// 정점의 위치에 행렬을 곱해서 정점의 위치를 변환하자.
		VTXTEX*		pVertices = nullptr;

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

	// 내마우스.y / 타일사이즈.y * 타읽의 가로 갯수 + 내마우스.x / 타일사이즈.x
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
