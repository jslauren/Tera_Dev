#pragma once
#include "Component.h"

class CTexture_Tool final : public CComponent
{
public:
	enum TEXTURETYPE { TYPE_GENERAL, TYPE_CUBE };
private:
	explicit CTexture_Tool(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CTexture_Tool() = default;
public:
	HRESULT Ready_Texture(TEXTURETYPE eType, const _tchar* pFileName, const _uint& iNumTexture);
	HRESULT SetUp_OnGraphicDev(const _uint& iIndex);
	HRESULT SetUp_OnShader(LPD3DXEFFECT pEffect, const char* pConstantName, const _uint& iIndex = 0);
public:
	HRESULT	Reset_Texture(TEXTURETYPE eType, const _tchar * pFileName, const _uint & iNumTexture);
private:
	vector<IDirect3DBaseTexture9*>			m_vecTexture;
	typedef vector<IDirect3DBaseTexture9*>	VECTEXTURE;
public:
	static CTexture_Tool*	Create(LPDIRECT3DDEVICE9 pGraphic_Device, TEXTURETYPE eType, const _tchar* pFileName, const _uint& iNumTexture = 1);
	virtual CComponent*		Clone();
	virtual void			Free();
};
