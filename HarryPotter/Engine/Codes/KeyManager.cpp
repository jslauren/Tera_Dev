#include "..\Headers\KeyManager.h"

_IMPLEMENT_SINGLETON(CKeyManager)

// 모든 키 상태를 이전의 상태로 되돌려 놓는 함수.
_int CKeyManager::UpdateKey()
{
	m_dwCurKey = 0;

	if (GetAsyncKeyState(VK_UP) & 0x8000)
		m_dwCurKey |= KEY_UP;
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		m_dwCurKey |= KEY_DOWN;
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		m_dwCurKey |= KEY_LEFT;
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		m_dwCurKey |= KEY_RIGHT;

	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		m_dwCurKey |= KEY_LBUTTON;
	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
		m_dwCurKey |= KEY_RBUTTON;
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
		m_dwCurKey |= KEY_SPACE;
	if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
		m_dwCurKey |= KEY_ESC;

	if (GetAsyncKeyState('W') & 0x8000)
		m_dwCurKey |= KEY_W;
	if (GetAsyncKeyState('S') & 0x8000)
		m_dwCurKey |= KEY_S;
	if (GetAsyncKeyState('A') & 0x8000)
		m_dwCurKey |= KEY_A;
	if (GetAsyncKeyState('D') & 0x8000)
		m_dwCurKey |= KEY_D;

	if (GetAsyncKeyState('1') & 0x8000)
		m_dwCurKey |= KEY_1;
	if (GetAsyncKeyState('2') & 0x8000)
		m_dwCurKey |= KEY_2;
	if (GetAsyncKeyState('3') & 0x8000)
		m_dwCurKey |= KEY_3;
	if (GetAsyncKeyState('4') & 0x8000)
		m_dwCurKey |= KEY_4;

	if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
		m_dwCurKey |= KEY_SHIFT;
	if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
		m_dwCurKey |= KEY_CTRL;
	if (GetAsyncKeyState(VK_TAB) & 0x8000)
		m_dwCurKey |= KEY_TAB;

	if (GetAsyncKeyState('R') & 0x8000)
		m_dwCurKey |= KEY_R;

	return 0;
}

bool CKeyManager::KeyDown(DWORD dwKey)
{
	// 이전에 누른적 없고 현재 눌린 경우 true
	if (!(m_dwKeyDown & dwKey) && (m_dwCurKey & dwKey))
	{
		m_dwKeyDown |= dwKey;
		return true;
	}
	// 이전에 누른적 있고 현재 눌리지 않은 경우 false
	else if ((m_dwKeyDown & dwKey) && !(m_dwCurKey & dwKey))
	{
		m_dwKeyDown ^= dwKey;
		return false;
	}

	return false;
}

bool CKeyManager::KeyUp(DWORD dwKey)
{
	// 이전에 누른적 있고 현재 눌리지 않은 경우 true
	if ((m_dwKeyUp & dwKey) && !(m_dwCurKey & dwKey))
	{
		m_dwKeyUp ^= dwKey;
		return true;
	}
	// 이전에 누른적 없고 현재 눌린 경우 false
	else if (!(m_dwKeyUp & dwKey) && (m_dwCurKey & dwKey))
	{
		m_dwKeyUp |= dwKey;
		return false;
	}

	return false;
}

bool CKeyManager::KeyPressing(DWORD dwKey)
{
	if (m_dwCurKey & dwKey)
		return true;

	return false;
}

void CKeyManager::Free()
{
}
