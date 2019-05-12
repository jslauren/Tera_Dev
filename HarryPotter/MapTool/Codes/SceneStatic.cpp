#include "stdafx.h"
#include "..\Headers\SceneStatic.h"

CSceneStatic::CSceneStatic(LPDIRECT3DDEVICE9 _pGraphic_Device)
	: CScene(_pGraphic_Device)
{
}

HRESULT CSceneStatic::Ready_Scene()
{
	if (FAILED(ReadyGameObject()))
		return E_FAIL;

	return NOERROR;
}

_int CSceneStatic::Update_Scene(const _float & fTimeDelta)
{
	return CScene::Update_Scene(fTimeDelta);
}

_int CSceneStatic::LateUpdate_Scene(const _float & fTimeDelta)
{
	return CScene::LateUpdate_Scene(fTimeDelta);
}

HRESULT CSceneStatic::Render_Scene()
{
	return CScene::Render_Scene();
}

HRESULT CSceneStatic::ReadyGameObject()
{
	return NOERROR;
}

CSceneStatic * CSceneStatic::Create(LPDIRECT3DDEVICE9 _pGraphicDevice)
{
	CSceneStatic* pInstance = new CSceneStatic(_pGraphicDevice);

	if (FAILED(pInstance->Ready_Scene()))
	{
		_MSGBOX("CSceneStatic Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSceneStatic::Free()
{
	CScene::Free();
}
