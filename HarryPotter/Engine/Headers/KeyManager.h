#pragma once
#include "Base.h"

_BEGIN(Engine)

class _DLL_EXPORTS CKeyManager final : public CBase
{
	_DECLARE_SINGLETON(CKeyManager)

public:
	enum KEY {
		KEY_UP = 0x00000001,
		KEY_DOWN = 0x00000002,
		KEY_LEFT = 0x00000004,
		KEY_RIGHT = 0x00000008,

		KEY_W = 0x00000010,
		KEY_S = 0x00000020,
		KEY_A = 0x00000040,
		KEY_D = 0x00000080,

		KEY_1 = 0x00000100,
		KEY_2 = 0x00000200,
		KEY_3 = 0x00000400,
		KEY_4 = 0x00000800,

		KEY_LBUTTON = 0x00001000,
		KEY_RBUTTON = 0x00002000,
		KEY_SPACE = 0x00004000,
		KEY_ESC = 0x00008000,

		KEY_SHIFT = 0x00010000,
		KEY_CTRL = 0x00020000,
		KEY_TAB = 0x00040000
	};

private:
	explicit CKeyManager() : m_dwCurKey(0), m_dwKeyDown(0), m_dwKeyUp(0) {};
	virtual ~CKeyManager() = default;

public:
	_int UpdateKey();
	bool KeyDown(DWORD dwKey);
	bool KeyUp(DWORD dwKey);
	bool KeyPressing(DWORD dwKey);

private:
	DWORD m_dwCurKey;
	DWORD m_dwKeyDown;
	DWORD m_dwKeyUp;

public:
	virtual void Free();
};

_END