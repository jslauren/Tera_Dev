#include "stdafx.h"
#include "..\Headers\Terrain.h"
#include "EventManager.h"

CTerrain::CTerrain(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
	, m_pGraphic_Device(pGraphic_Device)
{
}

CTerrain::CTerrain(const CTerrain & rhs)
	: CGameObject(rhs)
	, m_pGraphic_Device(rhs.m_pGraphic_Device)
{
}

// ������ü�� �ʿ��� �����͸� �����Ѵ�.
HRESULT CTerrain::Ready_GameObject_Prototype()
{
	// ����������� ���� ��ü�� ������ �����Ѵ�.
	if (FAILED(CGameObject::Ready_GameObject_Prototype()))
		return E_FAIL;

	return NOERROR;
}

// ���� ������ ����� ��ü�� ȣ���ϴ� �Լ�.
// ������ü �����ܿ��� �߰����� �������ʿ��ϸ� ���⼭ �����ض�.
HRESULT CTerrain::Ready_GameObject(void* pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	Ready_Terrain_Buffer();

	return NOERROR;
}

_int CTerrain::Update_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pTransformCom)
		return -1;

	return _int();
}

_int CTerrain::LateUpdate_GameObject(const _float & fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONEALPHA, this)))
		return -1;

	return _int();
}

HRESULT CTerrain::Render_GameObject()
{
	if (nullptr == m_pTextureCom)
		return E_FAIL;

	m_pTextureCom->SetUp_OnGraphicDev(0);

	SetUp_RenderState();

	// 	m_pBufferCom->Render_Buffer(m_pTransformCom);
	Render_Buffer();

	Release_RenderState();

	return NOERROR;
}

HRESULT CTerrain::Add_Component()
{
	// For.Com_Transform
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	//// For.Com_Buffer
	//if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Buffer_Terrain", L"Com_Buffer", (CComponent**)&m_pBufferCom)))
	//	return E_FAIL;

	// For.Com_Renderer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	// For.Com_Texture
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Texture_Terrain", L"Com_Texture", (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CTerrain::SetUp_RenderState()
{
	if(CEventManagerTool::GetInstance()->m_bIsWireFrame == true)
		CGameObject::Set_RenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	CGameObject::Set_RenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	CGameObject::Set_RenderState(D3DRS_LIGHTING, FALSE);
	CGameObject::Set_SamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	CGameObject::Set_SamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	CGameObject::Set_SamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	return NOERROR;
}

HRESULT CTerrain::Release_RenderState()
{
	CGameObject::Set_SamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_NONE);
	CGameObject::Set_SamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_NONE);
	CGameObject::Set_SamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

	CGameObject::Set_RenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	return NOERROR;
}

HRESULT CTerrain::Ready_Terrain_Buffer()
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
	if (FAILED(Ready_VIBuffer()))
		return E_FAIL;

	VTXNORTEX*		pVertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	for (size_t i = 0; i < m_iNumVerticesZ; ++i)
	{
		for (size_t j = 0; j < m_iNumVerticesX; ++j)
		{
			_uint		iIndex = i * m_iNumVerticesX + j;

			pVertices[iIndex].vPosition = _vec3(j * m_fInterval, 0.0f, i * m_fInterval);
			pVertices[iIndex].vNormal = _vec3(0.f, 0.f, 0.f);
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

	m_pIB->Unlock();

	return NOERROR;
}

HRESULT CTerrain::Ready_Terrain_Buffer(_uint _iNumVtxX, _uint _iNumVtxZ, _float _fInterval, _float _fDetail)
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

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
	if (FAILED(Ready_VIBuffer()))
		return E_FAIL;

	VTXNORTEX*		pVertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	for (size_t i = 0; i < m_iNumVerticesZ; ++i)
	{
		for (size_t j = 0; j < m_iNumVerticesX; ++j)
		{
			_uint		iIndex = i * m_iNumVerticesX + j;

			pVertices[iIndex].vPosition = _vec3(j * m_fInterval, 0.0f, i * m_fInterval);
			pVertices[iIndex].vNormal = _vec3(0.f, 0.f, 0.f);
			m_pPositions[iIndex] = pVertices[iIndex].vPosition;
			pVertices[iIndex].vTexUV = _vec2(j / (m_iNumVerticesX - 1.f) * _fDetail, i / (m_iNumVerticesZ - 1.f) * _fDetail);
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

			{
				_vec3	FaceNormal;

				ComputeNormal(&pVertices[pIndices[iNumPolygons]._1].vPosition, &pVertices[pIndices[iNumPolygons]._2].vPosition, &pVertices[pIndices[iNumPolygons]._3].vPosition, &FaceNormal);
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

				ComputeNormal(&pVertices[pIndices[iNumPolygons]._1].vPosition, &pVertices[pIndices[iNumPolygons]._2].vPosition, &pVertices[pIndices[iNumPolygons]._3].vPosition, &FaceNormal);

				pVertices[pIndices[iNumPolygons]._1].vNormal += FaceNormal;
				pVertices[pIndices[iNumPolygons]._2].vNormal += FaceNormal;
				pVertices[pIndices[iNumPolygons]._3].vNormal += FaceNormal;
			}

			++iNumPolygons;
		}
	}

	memcpy(m_pIndices, pIndices, sizeof(INDEX16) * m_iNumPolygons);

	m_pIB->Unlock();

	return NOERROR;
}

HRESULT CTerrain::Ready_VIBuffer()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	// ������ �����Ѵ�
	if (FAILED(m_pGraphic_Device->CreateVertexBuffer(m_iVtxSize * m_iNumVertices, 0, m_dwVtxFVF, D3DPOOL_MANAGED, &m_pVB, nullptr)))
		return E_FAIL;

	m_pPositions = new _vec3[m_iNumVertices];
	ZeroMemory(m_pPositions, sizeof(_vec3) * m_iNumVertices);

	// �ε������� �����Ѵ�(�ε�������)
	if (FAILED(m_pGraphic_Device->CreateIndexBuffer(m_iIndexSize * m_iNumPolygons, 0, m_Format, D3DPOOL_MANAGED, &m_pIB, nullptr)))
		return E_FAIL;

	if (D3DFMT_INDEX16 == m_Format)
	{
		m_pIndices = new INDEX16[m_iNumPolygons];
		ZeroMemory(m_pIndices, sizeof(INDEX16) * m_iNumPolygons);
	}
	else
	{
		m_pIndices = new INDEX32[m_iNumPolygons];
		ZeroMemory(m_pIndices, sizeof(INDEX32) * m_iNumPolygons);
	}

	return NOERROR;
}

void CTerrain::Render_Buffer()
{
	if (nullptr == m_pGraphic_Device)
		return;

	if (nullptr != m_pTransformCom)
	{
		_matrix		matTransform = *m_pTransformCom->Get_WorldMatrixPointer();

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
void CTerrain::Reset_Terrain(_uint _iNumVtxX, _uint _iNumVtxZ, _float _fInterval, _float _fDetail)
{
	if (false == m_isClone)
	{
		Safe_Delete_Array(m_pPositions);
		Safe_Delete_Array(m_pIndices);
	}

	Safe_Release(m_pVB);
	Safe_Release(m_pIB);

	Ready_Terrain_Buffer(_iNumVtxX, _iNumVtxZ, _fInterval, _fDetail);

}
void CTerrain::ComputeNormal(_vec3 * pVtx0, _vec3 * pVtx1, _vec3 * pVtx2, _vec3 * pOut)
{
	_vec3 u = *pVtx1 - *pVtx0;
	_vec3 v = *pVtx2 - *pVtx0;

	D3DXVec3Cross(pOut, &u, &v);
	D3DXVec3Normalize(pOut, pOut);
}

void CTerrain::DrawAxis()
{
	//D3DXVECTOR3 p[3];
	//p[0].x = 0; p[0].y = 0; p[0].z = 0.0f;
	//p[1].x = 0.1; p[1].y = 1; p[1].z = 0.0f;
	//p[2].x = 2; p[2].y = 0; p[2].z = 0.0f;

	//ID3DXLine *Line;
	//D3DXCreateLine(m_pGraphic_Device, &Line);
	//Line->SetWidth(2.f);
	//Line->SetAntialias(true);
	//Line->Begin();
	//Line->DrawTransform(p, 3, m_pTransformCom->Get_WorldMatrixPointer(), D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
	////Line->Draw( p, 3, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f) );    // �̷������ε� �׸��� ����
	//Line->End();
	//Line->Release();
}

// ������ü�� �����Ѵ�.
CTerrain * CTerrain::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTerrain* pInstance = new CTerrain(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		_MSGBOX("CTerrain Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CTerrain::Clone(void* pArg)
{
	CTerrain* pInstance = new CTerrain(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		_MSGBOX("CTerrain Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTerrain::Free()
{
	if (false == m_isClone)
	{
		Safe_Delete_Array(m_pPositions);
		Safe_Delete_Array(m_pIndices);
	}

	Safe_Release(m_pVB);
	Safe_Release(m_pIB);

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	//Safe_Release(m_pBufferCom);

	CGameObject::Free();
}
