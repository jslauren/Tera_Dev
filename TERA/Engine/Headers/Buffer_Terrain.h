#pragma once
#include "VIBuffer.h"

_BEGIN(Engine)

class CFrustum;
class CQuadTree;
class _DLL_EXPORTS CBuffer_Terrain final : public CVIBuffer
{
private:
	explicit CBuffer_Terrain(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CBuffer_Terrain(const CBuffer_Terrain& rhs);
	virtual ~CBuffer_Terrain() = default;
public:
	virtual HRESULT Ready_VIBuffer();
	virtual HRESULT Ready_VIBuffer(_int iNumVtxX, _int iNumVtxZ, _float fInterval, _float fDetail);
	virtual HRESULT Ready_VIBuffer(const _tchar* pHeighitMapPath);
	virtual void	Render_Buffer(const CTransform* pTransform = nullptr);
	virtual _float	Compute_HeightOnBuffer(const CTransform* pTransform);
public:
	HRESULT			Culling(CTransform* pTransform, CFrustum* pFrustum);
private:
	void			ComputeNormal(_vec3* pVtx0, _vec3* pVtx1, _vec3* pVtx2, _vec3* pOut);
private:
	CQuadTree*		m_pQuadTree = nullptr;
	_uint			m_iNumVerticesX = 0;
	_uint			m_iNumVerticesZ = 0;
	_float			m_fInterval = 1.f;
	_float			m_fDetail = 1.f;
public:
	static CBuffer_Terrain* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	static CBuffer_Terrain* Create(LPDIRECT3DDEVICE9 pGraphic_Device, _int iNumVtxX, _int iNumVtxZ, _float fInterval, _float fDetail);
	static CBuffer_Terrain* Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pHeighitMapPath);

	virtual CComponent* Clone(void* pArg = nullptr);
	virtual void		Free();
};

_END