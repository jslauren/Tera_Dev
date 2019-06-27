#pragma once
#include "Base.h"

// 1.내 장치의 성능을 조사한다.
// 2.조사한 성능을 바탕으로 동작할 수 있는 장치 객체를 생성, 보관한다.

_BEGIN(Engine)

class _DLL_EXPORTS CGraphic_Device final : public CBase
{
	_DECLARE_SINGLETON(CGraphic_Device)
public:
	// 윈도우 모드를 명시적으로 해주기 위한 열거체 선언.
	enum WINMODE { TYPE_FULLMODE, TYPE_WINMODE, TYPE_END };
private:
	explicit CGraphic_Device();
	virtual ~CGraphic_Device() = default;
public:
	HRESULT Ready_Graphic_Device(HWND hWnd, WINMODE eWinMode, const _uint& iWinCX, const _uint& iWinCY, LPDIRECT3DDEVICE9* ppGraphicDev = nullptr);
private:
	LPDIRECT3D9			m_pSDK = nullptr;
	LPDIRECT3DDEVICE9	m_pGraphic_Device = nullptr;
public:
	virtual void Free();
};

_END