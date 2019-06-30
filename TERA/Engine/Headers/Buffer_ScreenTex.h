#pragma once
#include "VIBuffer.h"

_BEGIN(Engine)

class _DLL_EXPORTS CBuffer_ScreenTex final : public CVIBuffer
{
private:
	explicit CBuffer_ScreenTex(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CBuffer_ScreenTex(const CBuffer_ScreenTex& rhs);
	virtual ~CBuffer_ScreenTex() = default;
public:
	virtual HRESULT Ready_VIBuffer();
	virtual HRESULT Ready_VIBuffer(_float fStartX, _float fStartY, _float fSizeX, _float fSizeY);
	virtual void Render_Buffer(const CTransform* pTransform = nullptr);

public:
	static CBuffer_ScreenTex* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	static CBuffer_ScreenTex* Create(LPDIRECT3DDEVICE9 pGraphic_Device, _float fStartX, _float fStartY, _float fSizeX, _float fSizeY);
	virtual CComponent* Clone(void* pArg = nullptr);
	virtual void Free();
};

_END