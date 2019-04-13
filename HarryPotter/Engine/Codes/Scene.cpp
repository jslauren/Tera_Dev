#include "..\Headers\Scene.h"
#include "GameObject.h"

CScene::CScene(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device(pGraphic_Device)
	, m_pObject_Manager(CObject_Manager::GetInstance())
	, m_pComponent_Manager(CComponent_Manager::GetInstance())
{
	m_pGraphic_Device->AddRef();
	m_pObject_Manager->AddRef();
	m_pComponent_Manager->AddRef();
}

HRESULT CScene::Ready_Scene()
{
	return NOERROR;
}

_int CScene::Update_Scene(const _float & fTimeDelta)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Update_Object_Manager(fTimeDelta);
}

_int CScene::LateUpdate_Scene(const _float & fTimeDelta)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->LateUpdate_Object_Manager(fTimeDelta);
}

HRESULT CScene::Render_Scene()
{
	return NOERROR;
}

HRESULT CScene::Add_Object_Prototype(const _uint & iSceneIdx, const _tchar * pProtoTag, CGameObject * pGameObject)
{
	// 현재 선택된 씬이 Scene_Logo라고 가정했을 때, Scene_Logo Class의 Ready_GameObject_Prototype 함수에서
	// 이 함수를 호출 하게 된다.
	// 여기서 Back_Logo 는 게임 오브젝트에 해당하는데 헷갈리지 말자. 게임 오브젝트다!!
	// 그래서 원본객체를 만들어주는 이 함수를 호출하는 것.
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	// 현재 선택된 씬의 인자로 받은 게임 오브젝트의 원본객체를 추가한다.
	if (FAILED(m_pObject_Manager->Add_Object_Prototype(iSceneIdx, pProtoTag, pGameObject)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CScene::Add_Object(const _uint & iPorotoSceneIdx, const _tchar * pProtoTag, const _uint & iSceneIdx, const _tchar * pLayerTag)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	// 위 Add_Object_Prototype 동등한 기능의 함수이다.
	if (FAILED(m_pObject_Manager->Add_Object(iPorotoSceneIdx, pProtoTag, iSceneIdx, pLayerTag)))
		return E_FAIL;

	return NOERROR;
}

void CScene::Free()
{
	Safe_Release(m_pComponent_Manager);
	Safe_Release(m_pObject_Manager);
	Safe_Release(m_pGraphic_Device);
}
