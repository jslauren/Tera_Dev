#pragma once
#include "Base.h"

// 1.�� ��ġ�� ������ �����Ѵ�.
// 2.������ ������ �������� ������ �� �ִ� ��ġ ��ü�� ����, �����Ѵ�.

_BEGIN(Engine)

class _DLL_EXPORTS CGraphic_Device final : public CBase
{
	_DECLARE_SINGLETON(CGraphic_Device)
public:
	// ������ ��带 ��������� ���ֱ� ���� ����ü ����.
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