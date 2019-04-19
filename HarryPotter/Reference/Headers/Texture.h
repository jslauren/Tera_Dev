#pragma once
#include "Component.h"

_BEGIN(Engine)

class _DLL_EXPORTS CTexture final : public CComponent
{
private:
	explicit CTexture(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CTexture(const CTexture& rhs);
	virtual ~CTexture() = default;
public:
	HRESULT Ready_Texture(const _tchar* pFileName, const _uint& iNumTexture);
	HRESULT SetUp_OnGraphicDev(const _uint& iIndex);
private:
	vector<LPDIRECT3DTEXTURE9>			m_vecTexture;
	typedef vector<LPDIRECT3DTEXTURE9>	VECTEXTURE;
public:
	static CTexture*	Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pFileName, const _uint& iNumTexture = 1);
	virtual CComponent* Clone();
	virtual void		Free();
};

_END