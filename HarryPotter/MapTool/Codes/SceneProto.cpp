#include "stdafx.h"
#include "SceneProto.h"

CSceneProto::CSceneProto(LPDIRECT3DDEVICE9 _pGraphic_Device)
	: CScene(_pGraphic_Device)
{
}

HRESULT CSceneProto::Ready_Scene()
{
	if (FAILED(ReadyGameObject()))
		return E_FAIL;

	return NOERROR;
}

_int CSceneProto::Update_Scene(const _float & fTimeDelta)
{
	return CScene::Update_Scene(fTimeDelta);
}

_int CSceneProto::LateUpdate_Scene(const _float & fTimeDelta)
{
	return CScene::LateUpdate_Scene(fTimeDelta);
}

HRESULT CSceneProto::Render_Scene()
{
	return CScene::Render_Scene();
}

HRESULT CSceneProto::ReadyGameObject()
{
	return NOERROR;
}

CSceneProto * CSceneProto::Create(LPDIRECT3DDEVICE9 _pGraphicDevice)
{
	CSceneProto* pInstance = new CSceneProto(_pGraphicDevice);

	if (FAILED(pInstance->Ready_Scene()))
	{
		_MSGBOX("CSceneProto Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSceneProto::Free()
{
	CScene::Free();
}
