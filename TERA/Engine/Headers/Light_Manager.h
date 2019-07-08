#pragma once
#include "Base.h"

_BEGIN(Engine)

class CLight;
class _DLL_EXPORTS CLight_Manager final : public CBase
{
	_DECLARE_SINGLETON(CLight_Manager)
private:
	explicit CLight_Manager();
	virtual ~CLight_Manager() = default;
public:
	const D3DLIGHT9* Get_LightInfo(const _uint& iIndex) const;
public:
	HRESULT Add_Light(LPDIRECT3DDEVICE9 pGraphic_Device, const D3DLIGHT9* pLightInfo);
	HRESULT Render_LightAcc(LPD3DXEFFECT pEffect);
	HRESULT	DeleteLightAll();
private:
	list<CLight*>	m_LightList;
public:
	virtual void Free();

};

_END