#pragma once
#include "Component.h"

// 정점을 생성한다.

_BEGIN(Engine)

class CTransform;
class _DLL_EXPORTS CVIBuffer : public CComponent
{
protected:
	explicit CVIBuffer(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CVIBuffer(const CVIBuffer& rhs);
	virtual ~CVIBuffer() = default;
public:
	virtual HRESULT Ready_VIBuffer();
	virtual void	Render_Buffer(const CTransform* pTransform = nullptr);
public:
	virtual _float	Compute_HeightOnBuffer(const CTransform* pTransform);

	// 불?! or 충돌된 위치?!
	_bool			Picking(HWND hWnd, CTransform* pTransform, _vec3* pOut);
protected:
	LPDIRECT3DVERTEXBUFFER9			m_pVB = nullptr;
	_uint							m_iVtxSize = 0;
	_uint							m_iNumVertices = 0;
	_ulong							m_dwVtxFVF = 0;
	_uint							m_iNumPolygons = 0;
protected:
	LPDIRECT3DINDEXBUFFER9			m_pIB = nullptr;
	_uint							m_iIndexSize = 0;
	D3DFORMAT						m_Format = D3DFORMAT(0);
protected:
	_vec3*							m_pPositions = nullptr; // 정점들의 최초 위치만 보관하는 배열.
	void*							m_pIndices = nullptr;
public:
	virtual CComponent* Clone(void* pArg = nullptr) = 0;
	virtual void		Free();
};

_END