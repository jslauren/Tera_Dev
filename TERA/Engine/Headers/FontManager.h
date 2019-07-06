#pragma once
#include "Base.h"

_BEGIN(Engine)

class _DLL_EXPORTS CFontManager final : public CBase
{
	_DECLARE_SINGLETON(CFontManager)
public:
	enum FONT_ID { FONT_NAME, FONT_POINT_VALUE, FONT_SCRIPT, FONT_END };
private:
	explicit CFontManager();
	virtual ~CFontManager() = default;

public:
	HRESULT	ReadyFont(LPDIRECT3DDEVICE9 pGraphic_Device);
	void	RenderFont(FONT_ID eFontID, _vec3 vPosition, _tchar* pFontContents, D3DCOLORVALUE dwColor = { 1.f, 1.f, 1.f, 1.f });

private:
	LPDIRECT3DDEVICE9	m_pGraphic_Device = nullptr;
	LPD3DXSPRITE		m_pSprite = nullptr;
	LPD3DXFONT			m_pFont[FONT_END];

public:
	virtual void Free();

};

_END