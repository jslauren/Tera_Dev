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

HRESULT CInput_Device::SetUp_Input_State()
{
	if (nullptr == m_pKeyboard ||
		nullptr == m_pMouse)
		return E_FAIL;

	m_pKeyboard->GetDeviceState(sizeof(m_KeyState), m_KeyState);

	m_pMouse->GetDeviceState(sizeof(m_MouseState), &m_MouseState);

	return NOERROR;
}

bool CInput_Device::Get_DIKeyDown(BYTE byKeyID)
{
	// 전 프레임의 키보드 키는 눌린 적이 없고, 현재 프레임의 키보드 키는 눌린 적이 있을 때 true, 아니라면 0인 false 반환.
	return (!m_PreKeyState[byKeyID] && m_KeyState[byKeyID]);
}

bool CInput_Device::Get_DIKeyUp(BYTE byKeyID)
{
	// 전 프레임의 키보드 키는 눌린 적이 있고, 현재 프레임의 키보드 키는 눌린 적이 없을 때 true, 아니라면 0인 false 반환.
	return (m_PreKeyState[byKeyID] && !m_KeyState[byKeyID]);
}

bool CInput_Device::Get_DIMouseDown(MOUSEBUTTON eMouseID)
{
	// 전 프레임의 마우스 키는 눌린 적이 없고, 현재 프레임의 마우스 키는 눌린 적이 있을 때 true, 아니라면 0인 false 반환.
	return (!m_PreMouseState.rgbButtons[eMouseID] && m_MouseState.rgbButtons[eMouseID]);
}

bool CInput_Device::Get_DIMouseUp(MOUSEMOVE eMouseID)
{
	// 전 프레임의 마우스 키는 눌린 적이 있고, 현재 프레임의 마우스 키는 눌린 적이 없을 때 true, 아니라면 0인 false 반환.
	return (m_PreMouseState.rgbButtons[eMouseID] && !m_MouseState.rgbButtons[eMouseID]);
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
