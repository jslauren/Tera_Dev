#pragma once
#include "VIBuffer.h"

_BEGIN(Engine)

class _DLL_EXPORTS CBuffer_Terrain final : public CVIBuffer
{
private:
	explicit CBuffer_Terrain(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CBuffer_Terrain(const CBuffer_Terrain& rhs);
	virtual ~CBuffer_Terrain() = default;
public:
	virtual	HRESULT	Ready_VIBuffer();
	virtual	void	Render_Buffer(const CTransform* pTransform = nullptr);
private:
	_uint			m_iNumVerticesX = 0;
	_uint			m_iNumVerticesZ = 0;
	_float			m_fInterval = 0.f;
public:
	static	CBuffer_Terrain*	Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual	CComponent*			Clone();
	virtual void				Free();
};

_END