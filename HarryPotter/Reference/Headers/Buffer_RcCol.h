#pragma once

#include "VIBuffer.h"

_BEGIN(Engine)

class _DLL_EXPORTS CBuffer_RcCol final : public CVIBuffer
{
private:
	explicit CBuffer_RcCol(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CBuffer_RcCol(const CBuffer_RcCol& rhs);
	virtual ~CBuffer_RcCol() = default;
public:
	virtual HRESULT Ready_VIBuffer();
	virtual void	Render_Buffer(const CTransform* pTransform = nullptr);

public:
	static CBuffer_RcCol*	Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent*		Clone();
	virtual void			Free();
};

_END