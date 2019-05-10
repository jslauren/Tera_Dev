#include "stdafx.h"
#include "ViewManager.h"
#include "SceneTerrain.h"
#include "SceneProto.h"

_IMPLEMENT_SINGLETON(CViewManager)

CViewManager::CViewManager()
	: m_pManagement(CManagement::GetInstance())
{
}

HRESULT CViewManager::SetCurScene(TOOLID _eID)
{
	if (nullptr == m_pManagement)
		return E_FAIL;

	CScene*		pScene = nullptr;

	switch (_eID)
	{
	case TOOL_TERRAIN:
		pScene = CSceneTerrain::Create(m_pGraphic_Device);
		m_pEditorView->m_Tab_Terrain.m_pScene = (CSceneTerrain*)pScene;
		break;

	case TOOL_PROTO:
		pScene = CSceneProto::Create(m_pGraphic_Device);
		m_pEditorView->m_Tab_Proto.m_pScene = (CSceneProto*)pScene;
		break;
	}

	if (nullptr == pScene)
		return E_FAIL;

	if (FAILED(m_pManagement->SetUp_CurrentScene(pScene, _eID)))
		return E_FAIL;

	m_pCurScene = pScene;
	return NOERROR;
}

void CViewManager::Free()
{
	// Safe_Release(m_pManagement);
}
