#include "stdafx.h"
#include "..\Headers\SceneCamera.h"

CSceneCamera::CSceneCamera(LPDIRECT3DDEVICE9 _pGraphic_Device)
	: CScene(_pGraphic_Device)
{
}

HRESULT CSceneCamera::Ready_Scene()
{
	if (FAILED(ReadyGameObject()))
		return E_FAIL;

	return NOERROR;
}

_int CSceneCamera::Update_Scene(const _float & fTimeDelta)
{
	return CScene::Update_Scene(fTimeDelta);
}

_int CSceneCamera::LateUpdate_Scene(const _float & fTimeDelta)
{
	return CScene::LateUpdate_Scene(fTimeDelta);
}

HRESULT CSceneCamera::Render_Scene()
{
	return CScene::Render_Scene();
}

HRESULT CSceneCamera::ReadyGameObject()
{
	return NOERROR;
}

CSceneCamera * CSceneCamera::Create(LPDIRECT3DDEVICE9 _pGraphicDevice)
{
	CSceneCamera* pInstance = new CSceneCamera(_pGraphicDevice);

	if (FAILED(pInstance->Ready_Scene()))
	{
		_MSGBOX("CSceneCamera Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSceneCamera::Free()
{
	CScene::Free();
}
