#pragma once
#include "Component.h"

_BEGIN(Engine)

class _DLL_EXPORTS CTexture final : public CComponent
{
public:
	enum TEXTURETYPE { TYPE_GENERAL, TYPE_CUBE };
private:
	explicit CTexture(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CTexture(const CTexture& rhs);
	virtual ~CTexture() = default;
public:
	HRESULT Ready_Texture(TEXTURETYPE eType, const _tchar* pFileName, const _uint& iNumTexture);
	HRESULT SetUp_OnGraphicDev(const _uint& iIndex);
private:
	vector<IDirect3DBaseTexture9*>			m_vecTexture;
	typedef vector<IDirect3DBaseTexture9*>	VECTEXTURE;
public:
	static CTexture*	Create(LPDIRECT3DDEVICE9 pGraphic_Device, TEXTURETYPE eType, const _tchar* pFileName, const _uint& iNumTexture = 1);
	virtual CComponent* Clone();
	virtual void		Free();
};

_END