#include "stdafx.h"
#include "ViewManagerTool.h"
#include "Scene_Proto.h"
#include "MainFrm.h"
#include "ModifyView.h"

_IMPLEMENT_SINGLETON(CViewManagerTool)

CViewManagerTool::CViewManagerTool()
	: m_pManagement(CManagement::GetInstance())
{
}

HRESULT CViewManagerTool::SetCurScene(SCENEID _eID)
{
	if (nullptr == m_pManagement)
		return E_FAIL;

	CScene*		pScene = nullptr;

	switch (_eID)
	{
	case SCENE_TERRAIN:
		//pScene = CScene_Logo::Create(m_pGraphic_Device);
		break;

	case SCENE_MESH:
		break;

	case SCENE_ANIMATION:
		break;

	case SCENE_PROTO:
		pScene = MapTool::CScene_Proto::Create(m_pGraphic_Device);
		//m_pModifyView->m_Tab_Proto.m_pScene = (CScene_Proto*)pScene;
		break;

		//CMainFrame*		pMainFrm = ((CMainFrame*)AfxGetMainWnd());
	}

	if (nullptr == pScene)
		return E_FAIL;

	if (FAILED(m_pManagement->SetUp_CurrentScene(pScene, _eID)))
		return E_FAIL;

	m_pCurScene = pScene;
	return NOERROR;
}

void CViewManagerTool::Free()
{
	// Safe_Release(m_pManagement);
}
