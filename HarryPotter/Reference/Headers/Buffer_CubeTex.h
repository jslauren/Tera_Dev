#pragma once
#include "VIBuffer.h"

_BEGIN(Engine)

class _DLL_EXPORTS CBuffer_CubeTex final : public CVIBuffer
{
private:
	explicit CBuffer_CubeTex(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CBuffer_CubeTex(const CBuffer_CubeTex& rhs);
	virtual ~CBuffer_CubeTex() = default;
public:
	virtual HRESULT Ready_VIBuffer();
	virtual void	Render_Buffer(const CTransform* pTransform = nullptr);

public:
	static CBuffer_CubeTex*	Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent*		Clone();
	virtual void			Free();
};

_END