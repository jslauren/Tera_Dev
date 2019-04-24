#include "..\Headers\Input_Device.h"

_IMPLEMENT_SINGLETON(CInput_Device)

CInput_Device::CInput_Device()
{
}

HRESULT CInput_Device::Ready_Input_Device(HINSTANCE hInst, HWND hWnd)
{
	if (FAILED(DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pSDK, nullptr)))
		return E_FAIL;

	if (FAILED(Ready_KeyBoard(hWnd)))
		return E_FAIL;

	if (FAILED(Ready_Mouse(hWnd)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CInput_Device::Investigate_Input_State()
{
	// (=SetUp_Input_State)

	if (nullptr == m_pKeyboard ||
		nullptr == m_pMouse)
		return E_FAIL;

	m_pKeyboard->GetDeviceState(sizeof(m_KeyState), m_KeyState);
	m_pMouse->GetDeviceState(sizeof(m_MouseState), &m_MouseState);

	return NOERROR;
}

HRESULT CInput_Device::Ready_KeyBoard(HWND hWnd)
{
	if (FAILED(m_pSDK->CreateDevice(GUID_SysKeyboard, &m_pKeyboard, nullptr)))
		return E_FAIL;

	if (FAILED(m_pKeyboard->SetDataFormat(&c_dfDIKeyboard)))
		return E_FAIL;

	if (FAILED(m_pKeyboard->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND)))
		return E_FAIL;

	if (FAILED(m_pKeyboard->Acquire()))
		return E_FAIL;

	return NOERROR;
}

HRESULT CInput_Device::Ready_Mouse(HWND hWnd)
{
	if (FAILED(m_pSDK->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr)))
		return E_FAIL;

	if (FAILED(m_pMouse->SetDataFormat(&c_dfDIMouse)))
		return E_FAIL;

	if (FAILED(m_pMouse->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND)))
		return E_FAIL;

	if (FAILED(m_pMouse->Acquire()))
		return E_FAIL;

	return NOERROR;
}

void CInput_Device::Free()
{
	Safe_Release(m_pKeyboard);
	Safe_Release(m_pMouse);
	Safe_Release(m_pSDK);
}
