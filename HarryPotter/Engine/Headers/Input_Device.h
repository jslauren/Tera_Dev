#pragma once
#include "Base.h"

_BEGIN(Engine)

class _DLL_EXPORTS CInput_Device final : public CBase
{
	_DECLARE_SINGLETON(CInput_Device)
public:
	enum MOUSEBUTTON { DIM_LBUTTON, DIM_RBUTTON, DIM_WHEELBUTTON, DIM_XBUTTON };
	enum MOUSEMOVE { DIMM_X, DIMM_Y, DIMM_WHEEL };
private:
	explicit CInput_Device();
	virtual ~CInput_Device() = default;
public:	// Getter
	_byte GetDIKeyState(_ubyte byKey) {	return m_KeyState[byKey]; }
	_byte GetDIMouseState(MOUSEBUTTON eMouseBT) { return m_MouseState.rgbButtons[eMouseBT]; }
	_long GetDIMouseMove(MOUSEMOVE eMouseMove) { return *((_long*)&m_MouseState + eMouseMove); }
public:
	HRESULT Ready_Input_Device(HINSTANCE hInst, HWND hWnd);
	HRESULT SetUp_Input_State();
private:
	LPDIRECTINPUT8				m_pSDK = nullptr;
private:
	LPDIRECTINPUTDEVICE8		m_pKeyboard = nullptr;
	_byte						m_KeyState[256];
private:
	LPDIRECTINPUTDEVICE8		m_pMouse = nullptr;
	DIMOUSESTATE				m_MouseState;
private:
	HRESULT Ready_KeyBoard(HWND hWnd);
	HRESULT Ready_Mouse(HWND hWnd);
public:
	virtual void Free();

};

_END