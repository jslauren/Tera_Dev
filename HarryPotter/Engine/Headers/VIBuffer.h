#pragma once

#include "Component.h"

_BEGIN(Engine)

// 정점을 생성한다.

class _DLL_EXPORTS CVIBuffer : public CComponent
{
protected:
	explicit CVIBuffer(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CVIBuffer() = default;
public:
	virtual HRESULT Ready_VIBuffer();
protected:
	LPDIRECT3DVERTEXBUFFER9		m_pVB = nullptr;
	_uint						m_iVtxSize = 0;
	_uint						m_iNumVertices = 0;
	_ulong						m_dwVtxFVF = 0;
public:
	virtual void Free();
};

_END