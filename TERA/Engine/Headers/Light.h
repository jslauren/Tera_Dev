#pragma once
#include "Base.h"

_BEGIN(Engine)

class CBuffer_ScreenTex;
class CLight final : public CBase
{
private:
	explicit CLight(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLight() = default;
public:
	const D3DLIGHT9* Get_LightInfo() const { return &m_LightInfo; }
public:
	HRESULT Ready_Light(const D3DLIGHT9* pLightInfo);
	HRESULT Render_Light(LPD3DXEFFECT pEffect);
private:
	LPDIRECT3DDEVICE9	m_pGraphic_Device = nullptr;
	CBuffer_ScreenTex*	m_pBuffer = nullptr;
	D3DLIGHT9			m_LightInfo;
public:
	static CLight*	Create(LPDIRECT3DDEVICE9 pGraphic_Device, const D3DLIGHT9* pLightInfo);
	virtual void	Free();
};

_END