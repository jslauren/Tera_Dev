#include "stdafx.h"
#include "..\Headers\SceneMesh.h"

CSceneMesh::CSceneMesh(LPDIRECT3DDEVICE9 _pGraphic_Device)
	: CScene(_pGraphic_Device)
{
}

HRESULT CSceneMesh::Ready_Scene()
{
	if (FAILED(ReadyGameObject()))
		return E_FAIL;

	return NOERROR;
}

_int CSceneMesh::Update_Scene(const _float & fTimeDelta)
{
	return CScene::Update_Scene(fTimeDelta);
}

_int CSceneMesh::LateUpdate_Scene(const _float & fTimeDelta)
{
	return CScene::LateUpdate_Scene(fTimeDelta);
}

HRESULT CSceneMesh::Render_Scene()
{
	return CScene::Render_Scene();
}

HRESULT CSceneMesh::ReadyGameObject()
{
	return NOERROR;
}

CSceneMesh * CSceneMesh::Create(LPDIRECT3DDEVICE9 _pGraphicDevice)
{
	CSceneMesh* pInstance = new CSceneMesh(_pGraphicDevice);

	if (FAILED(pInstance->Ready_Scene()))
	{
		_MSGBOX("CSceneMesh Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSceneMesh::Free()
{
	CScene::Free();
}
