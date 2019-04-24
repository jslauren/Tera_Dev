#pragma once
#include "Component.h"

_BEGIN(Engine)

// ������ �����Ѵ�.

// ������ �Ŵ�¡? �ϴ� �θ� Ŭ�����ν� ������Ʈ ��ü�� �ش�Ǹ�,
// �ڽĵ�� �� ������ ������ �ﰢ��, �簢��, ���� ���� ������ �ִ�.

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
	_vec3*							m_pPositions = nullptr; // �������� ���� ��ġ�� �����ϴ� �迭.

public:
	virtual CComponent* Clone() = 0;
	virtual void Free();
};

_END