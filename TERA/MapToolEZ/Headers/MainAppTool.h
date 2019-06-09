#pragma once
#include "Tool_Defines.h"
#include "Base.h"
#include "Graphic_Device.h"

_BEGIN(Engine)
class CRenderer;
class CManagement;
_END

class CViewManagerTool;

_BEGIN(MapTool)

class CMainAppTool : public CBase
{
private:
	explicit CMainAppTool();
	virtual ~CMainAppTool() = default;
public:
	HRESULT Ready_MainApp();
	_int	Update_MainApp(const _float& fTimeDelta);
	HRESULT Render_MainApp();
private:
	LPDIRECT3DDEVICE9	m_pGraphic_Device = nullptr;
	CManagement*		m_pManagement = nullptr;
	CRenderer*			m_pRenderer = nullptr;
	CViewManagerTool*	m_pViewManager = nullptr;
private:
	HRESULT Ready_Default_Setting(CGraphic_Device::WINMODE eType, const _uint& iWinCX, const _uint& iWinCY);
	//	HRESULT Ready_Render_State();
	HRESULT Ready_Component_Prototype();
	HRESULT Ready_GameObject_Prototype();
	HRESULT Ready_Scene(SCENEID eID);
public:
	static CMainAppTool*	Create();	
	virtual void			Free();		

};

_END