#include "..\Headers\Graphic_Device.h"

_IMPLEMENT_SINGLETON(CGraphic_Device)

CGraphic_Device::CGraphic_Device()
{
}

HRESULT CGraphic_Device::Ready_Graphic_Device(HWND hWnd, WINMODE eWinMode, const _uint & iWinCX, const _uint & iWinCY, LPDIRECT3DDEVICE9* ppGraphicDev)
{
	m_pSDK = Direct3DCreate9(D3D_SDK_VERSION); // 자체적인 컴객체 할당함수

	D3DCAPS9		DeviceCaps;
	ZeroMemory(&DeviceCaps, sizeof(D3DCAPS9));

	if (FAILED(m_pSDK->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &DeviceCaps)))
	{
		//MessageBox(NULL, L"장치 생성실패", L"System Error", NULL);
		return E_FAIL;
	}

	DWORD		bf;

	if (DeviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		bf = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		bf = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	bf |= D3DCREATE_MULTITHREADED;

	D3DPRESENT_PARAMETERS		d3dpp;
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));

	d3dpp.BackBufferWidth = iWinCX;
	d3dpp.BackBufferHeight = iWinCY;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount = 1;

	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;

	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hWnd;

	d3dpp.Windowed = eWinMode;	 // 창 모드냐 전체화면 모드냐		

	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;

	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	if (FAILED(m_pSDK->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, bf, &d3dpp, &m_pGraphic_Device)))
		return E_FAIL;

	if (nullptr != ppGraphicDev)
	{
		*ppGraphicDev = m_pGraphic_Device;
		m_pGraphic_Device->AddRef();
	}

	return NOERROR;
}

void CGraphic_Device::Free()
{
	if (0 != Safe_Release(m_pGraphic_Device))
		_MSGBOX("m_pGraphic_Device Release Failed");

	if (0 != Safe_Release(m_pSDK))
		_MSGBOX("m_pSDK Release Failed");
}
