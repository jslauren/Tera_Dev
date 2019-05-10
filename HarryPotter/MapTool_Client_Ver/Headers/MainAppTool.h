#pragma once
#include "../Default/Tool_Defines.h"
#include "Base.h"
#include "Graphic_Device.h"

_BEGIN(Engine)
class CManagement;
class CRenderer;
_END

_BEGIN(Maptool)
class CMainAppTool
{
public:
	explicit CMainAppTool();
	virtual ~CMainAppTool() = default;

public:
	HRESULT Ready_MainApp(); // �� ������ �ʱ�ȭ ������ �����ϳ�.
	_int	Update_MainApp(const _float& fTimeDelta);
	HRESULT Render_MainApp();
private:
	LPDIRECT3DDEVICE9	m_pGraphic_Device = nullptr;
private:
	CManagement*		m_pManagement = nullptr;
	CRenderer*			m_pRenderer = nullptr;
private: // For.FPS
	_uint	m_iRenderCnt = 0;
	_float	m_fTimeAcc = 0.f;
	_tchar	m_szFPS[128] = L"";
private:
	HRESULT Ready_Default_Setting(CGraphic_Device::WINMODE eType, const _uint& iWinCX, const _uint& iWinCY);
	//	HRESULT Ready_Render_State();
	HRESULT Ready_Component_Prototype();
	HRESULT Ready_GameObject_Prototype();
	HRESULT Ready_Scene(TOOLSCENEID eID);
public:
	static CMainAppTool*	Create();	// �������õ� �Լ�.
	virtual void			Free();		// �Ҹ���õ� �Լ�.
};

_END