#pragma once
#include "Component.h"

_BEGIN(Engine)

// 정점을 생성한다.

// 정점을 매니징? 하는 부모 클래스로써 컴포넌트 객체에 해당되며,
// 자식들로 색 정보를 가지는 삼각형, 사각형, 지형 등을 가지고 있다.

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
	virtual _float Compute_HeightOnBuffer(const CTransform* pTransform);
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

public:
	virtual CComponent* Clone() = 0;
	virtual void Free();
};

_END