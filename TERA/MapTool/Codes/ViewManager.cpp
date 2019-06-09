#include "stdafx.h"
#include "ViewManager.h"
#include "SceneStatic.h"
#include "SceneTerrain.h"
#include "SceneMesh.h"
#include "SceneCamera.h"
#include "SceneEffect.h"
#include "SceneProto.h"

_IMPLEMENT_SINGLETON(CViewManager)

CViewManager::CViewManager()
	: m_pManagement(CManagement::GetInstance())
{
}

HRESULT CViewManager::SetCurScene(SCENEID _eID)
{
	if (nullptr == m_pManagement)
		return E_FAIL;

	CScene*		pScene = nullptr;

	switch (_eID)
	{
	case SCENE_STATIC:
		pScene = CSceneStatic::Create(m_pGraphic_Device);
		//m_pEditorView->m_Tab_Terrain.m_pScene = (CSceneStatic*)pScene;
		break;

	case SCENE_TERRAIN:
		break;

	case SCENE_MESH:
		break;

	case SCENE_CAMERA:
		break;

	case SCENE_EFFECT:
		break;

	case SCENE_PROTO:
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
