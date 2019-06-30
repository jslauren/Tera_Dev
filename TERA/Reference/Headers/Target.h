#pragma once
#include "Base.h"

_BEGIN(Engine)

class CBuffer_ScreenTex;
class CTarget final : public CBase
{
private:
	explicit CTarget(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CTarget() = default;
public:
	HRESULT Ready_Target(_uint iWidth, _uint iHeight, D3DFORMAT eFormat, D3DXCOLOR Clear_Color);
	HRESULT SetUp_OnGraphicDev(_uint iIndex);
	HRESULT SetUp_OnShader(LPD3DXEFFECT pEffect, const char* pConstantName);
	HRESULT Release_OnGraphicDev(_uint iIndex);
	HRESULT Clear_Target();

#ifdef _DEBUG
	HRESULT Ready_Debug_Buffer(_float fStartX, _float fStartY, _float fSizeX, _float fSizeY);
	HRESULT Render_Debug_Buffer();
#endif

private:
	LPDIRECT3DDEVICE9		m_pGraphic_Device = nullptr;
	LPDIRECT3DTEXTURE9		m_pTarget_Texture = nullptr;
	LPDIRECT3DSURFACE9		m_pTarget_Surface = nullptr;
	LPDIRECT3DSURFACE9		m_pOld_Surface = nullptr;
private:
	D3DXCOLOR				m_Clear_Color;

#ifdef _DEBUG
private: // 렌더타겟에 그려지는 상황을 확인하기위해서.
	CBuffer_ScreenTex*		m_pDebugBuffer = nullptr;
#endif

public:
	static CTarget*	Create(LPDIRECT3DDEVICE9 pGraphic_Device, _uint iWidth, _uint iHeight, D3DFORMAT eFormat, D3DXCOLOR Clear_Color);
	virtual void	Free();
};

_END