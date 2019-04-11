#pragma once

#include "VIBuffer.h"

_BEGIN(Engine)

class _DLL_EXPORTS CTriCol final : public CVIBuffer
{
private:
	explicit CTriCol(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CTriCol() = default;
public:
	virtual HRESULT	Ready_VIBuffer();
public:
	static CTriCol*	Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void	Free();
};

_END