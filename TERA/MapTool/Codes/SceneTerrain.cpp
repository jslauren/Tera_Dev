#include "stdafx.h"
#include "SceneTerrain.h"

CSceneTerrain::CSceneTerrain(LPDIRECT3DDEVICE9 _pGraphic_Device)
	: CScene(_pGraphic_Device)
{
}

HRESULT CSceneTerrain::Ready_Scene()
{
	if (FAILED(ReadyGameObject()))
		return E_FAIL;

	return NOERROR;
}

_int CSceneTerrain::Update_Scene(const _float & fTimeDelta)
{
	return CScene::Update_Scene(fTimeDelta);
}

_int CSceneTerrain::LateUpdate_Scene(const _float & fTimeDelta)
{
	return CScene::LateUpdate_Scene(fTimeDelta);
}

HRESULT CSceneTerrain::Render_Scene()
{
	return CScene::Render_Scene();
}

HRESULT CSceneTerrain::ReadyGameObject()
{
	return NOERROR;
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
	CScene::Free();
}
