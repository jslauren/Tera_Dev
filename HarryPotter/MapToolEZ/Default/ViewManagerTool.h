#pragma once
#include "Base.h"
#include "MainFrm.h"
#include "MapToolEZView.h"
#include "ModifyView.h"
#include "Management.h"
#include "Graphic_Device.h"

class CViewManagerTool final : public CBase
{
	_DECLARE_SINGLETON(CViewManagerTool)

private:
	explicit CViewManagerTool();
	virtual ~CViewManagerTool() = default;

public:
	HRESULT	SetCurScene(SCENEID _eID);

public:	// Setter
	void	SetDevice(LPDIRECT3DDEVICE9 _pGraphic_Device) { m_pGraphic_Device = _pGraphic_Device; }

public:
	LPDIRECT3DDEVICE9	m_pGraphic_Device = nullptr;

public:
	CMainFrame*			m_pMainFrame = nullptr;
	CMapToolEZView*		m_pMapToolEZView = nullptr;
	CModifyView*		m_pModifyView = nullptr;

	CScene*				m_pCurScene = nullptr;

public:
	_vec3				m_vMousePos = _vec3(g_iWinCX * 0.5f, g_iWinCY * 0.5f, 0);

private:
	CManagement*		m_pManagement = nullptr;

public:
	virtual void		Free();
	
};
