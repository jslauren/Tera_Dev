#include "stdafx.h"
#include "..\Headers\SceneEffect.h"

CSceneEffect::CSceneEffect(LPDIRECT3DDEVICE9 _pGraphic_Device)
	: CScene(_pGraphic_Device)
{
}

HRESULT CSceneEffect::Ready_Scene()
{
	if (FAILED(ReadyGameObject()))
		return E_FAIL;

	return NOERROR;
}

_int CSceneEffect::Update_Scene(const _float & fTimeDelta)
{
	return CScene::Update_Scene(fTimeDelta);
}

_int CSceneEffect::LateUpdate_Scene(const _float & fTimeDelta)
{
	return CScene::LateUpdate_Scene(fTimeDelta);
}

HRESULT CSceneEffect::Render_Scene()
{
	return CScene::Render_Scene();
}

HRESULT CSceneEffect::ReadyGameObject()
{
	return NOERROR;
}

CSceneEffect * CSceneEffect::Create(LPDIRECT3DDEVICE9 _pGraphicDevice)
{
	CSceneEffect* pInstance = new CSceneEffect(_pGraphicDevice);

	if (FAILED(pInstance->Ready_Scene()))
	{
		_MSGBOX("CSceneEffect Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSceneEffect::Free()
{
	CScene::Free();
}
