#include "..\Headers\Scene.h"
#include "GameObject.h"


CScene::CScene(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device(pGraphic_Device)
	, m_pObject_Manager(CObject_Manager::GetInstance())
{
	m_pGraphic_Device->AddRef();
	m_pObject_Manager->AddRef();
}

HRESULT CScene::Ready_Scene()
{
	return NOERROR;
}

_int CScene::Update_Scene(const _float & fTimeDelta)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return _int();
}

_int CScene::LateUpdate_Scene(const _float & fTimeDelta)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return _int();
}

HRESULT CScene::Render_Scene()
{
	return NOERROR;
}

HRESULT CScene::Add_Object_Prototype(const _uint & iSceneIdx, const _tchar * pProtoTag, CGameObject * pGameObject)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Add_Object_Prototype(iSceneIdx, pProtoTag, pGameObject)))
		return E_FAIL;

	return NOERROR;
}

void CScene::Free()
{
	Safe_Release(m_pObject_Manager);
	Safe_Release(m_pGraphic_Device);
}
