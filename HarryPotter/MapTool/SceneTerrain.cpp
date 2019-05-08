#include "stdafx.h"
#include "SceneTerrain.h"
#include "EventManager.h"
#include "DataManager.h"

CSceneTerrain::CSceneTerrain(LPDIRECT3DDEVICE9 _pGraphicDevice)
	: CScene(_pGraphicDevice)
	, m_pViewManager(CViewManager::GetInstance())
	, m_pDataManager(CDataManager::GetInstance())
{
}

//HRESULT CSceneTerrain::ReadyScene()
//{
//	if (FAILED(ReadyGameObject(L"Guid")))
//		return E_FAIL;
//
//	return NOERROR;
//}
//
//_int CSceneTerrain::UpdateScene(const _float & fTimeDelta)
//{
//	int result = CScene::LateUpdate_Scene(fTimeDelta);
//
//	return result;
//}
//
//_int CSceneTerrain::LateUpdateScene(const _float & fTimeDelta)
//{
//	return _int();
//}
//
//HRESULT CSceneTerrain::RenderScene()
//{
//	return CScene::Render_Scene();
//}

void CSceneTerrain::SetMapSize(_int iCountX, _int iCountZ)
{
	CEventManager::GetInstance()->NotifyEvent(L"NewMapSizeApply");
}

HRESULT CSceneTerrain::ReadyGameObject(const _tchar * pLayerTag)
{
	return E_NOTIMPL;
}

CSceneTerrain * CSceneTerrain::Create(LPDIRECT3DDEVICE9 _pGraphicDevice)
{
	CSceneTerrain* pInstance = new CSceneTerrain(_pGraphicDevice);

	if (FAILED(pInstance->Ready_Scene()))
	{
		_MSGBOX("CSceneTerrain Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSceneTerrain::Free()
{
	if (nullptr == m_pObject_Manager ||
		nullptr == m_pComponent_Manager)
		return;

	if (FAILED(m_pObject_Manager->Clear_Object(TOOL_TILE)))
		return;

	CScene::Free();
}
