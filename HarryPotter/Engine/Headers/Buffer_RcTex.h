#pragma once
#include "VIBuffer.h"

_BEGIN(Engine)

class _DLL_EXPORTS CBuffer_RcTex final : public CVIBuffer
{
private:
	explicit CBuffer_RcTex(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CBuffer_RcTex(const CBuffer_RcTex& rhs);
	virtual ~CBuffer_RcTex() = default;
public:
	virtual HRESULT Ready_VIBuffer();
	virtual void	Render_Buffer(const CTransform* pTransform = nullptr);
public:
	static	CBuffer_RcTex*	Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual	CComponent*		Clone();
	virtual	void			Free();
};

_END