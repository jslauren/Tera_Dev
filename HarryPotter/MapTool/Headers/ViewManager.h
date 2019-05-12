#pragma once
#include "Base.h"
#include "MainFrm.h"
#include "MapToolView.h"
#include "EditorView.h"
#include "Management.h"
#include "Graphic_Device.h"

class CViewManager final : public CBase
{
	_DECLARE_SINGLETON(CViewManager)

private:
	explicit CViewManager();
	virtual ~CViewManager() = default;

public:
	HRESULT	SetCurScene(SCENEID _eID);

public:
	LPDIRECT3DDEVICE9	m_pGraphic_Device = nullptr;

public:
	CMainFrame*			m_pMainFrame = nullptr;
	CMapToolView*		m_pMainView = nullptr;
	CEditorView*		m_pEditorView = nullptr;

	CScene*				m_pCurScene = nullptr;

public:
	_vec3				m_vMousePos = _vec3(g_iWinCX * 0.5f, g_iWinCY * 0.5f, 0);

private:
	CManagement*		m_pManagement = nullptr;

public:
	virtual void		Free();

};

