#pragma once
#include "VIBuffer.h"

class CBuffer_Terrain_Tool final : public CVIBuffer
{
private:
	explicit CBuffer_Terrain_Tool(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CBuffer_Terrain_Tool() = default;
public:
	virtual HRESULT Ready_VIBuffer();
	virtual HRESULT Ready_VIBuffer(const _tchar* pHeighitMapPath);
	virtual void	Render_Buffer(const CTransform* pTransform = nullptr);
	virtual _float	Compute_HeightOnBuffer(const CTransform* pTransform);
public:
	HRESULT			Reset_Terrain(_uint _iNumVtxX, _uint _iNumVtxZ, _float _fInterval, _float _fDetail);
private:
	void			ComputeNormal(_vec3* pVtx0, _vec3* pVtx1, _vec3* pVtx2, _vec3* pOut);
private:
	_uint			m_iNumVerticesX = 0;
	_uint			m_iNumVerticesZ = 0;
	_float			m_fInterval = 0.f;
	_float			m_fDetail = 1.f;
public:
	static CBuffer_Terrain_Tool* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	static CBuffer_Terrain_Tool* Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pHeighitMapPath);

	virtual CComponent* Clone();
	virtual void		Free();
};

