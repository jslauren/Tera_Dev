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
	// ���� ������ ��� �� ������Ʈ���� Update��,
	// Scene Class�� Update_Scene�Լ����� ������Ʈ �Ŵ����� ���Ͽ� �����Ѵ�.
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	// �� �� ������Ʈ���� �翬�� �� ��� ������Ʈ(��ü) ���̴�.
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
	// ���� ������Ʈ(��ü)�� �߰����ִ� �Լ�.

	// ���� ���õ� ���� Scene_Logo��� �������� ��, Scene_Logo Class�� Ready_GameObject_Prototype �Լ�����
	// �� �Լ��� ȣ�� �ϰ� �ȴ�.
	// ���⼭ Back_Logo �� ���� ������Ʈ�� �ش��ϴµ� �򰥸��� ����. ���� ������Ʈ��!!
	// �׷��� ������ü�� ������ִ� �� �Լ��� ȣ���ϴ� ��.
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	// ���� ���õ� ���� ���ڷ� ���� ���� ������Ʈ�� ������ü�� �߰��Ѵ�.
	if (FAILED(m_pObject_Manager->Add_Object_Prototype(iSceneIdx, pProtoTag, pGameObject)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CScene::Add_Object(const _uint & iPorotoSceneIdx, const _tchar * pProtoTag, const _uint & iSceneIdx, const _tchar * pLayerTag, void* pArg)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	// �� Add_Object_Prototype ������ ����� �Լ��̴�.
	if (FAILED(m_pObject_Manager->Add_Object(iPorotoSceneIdx, pProtoTag, iSceneIdx, pLayerTag, pArg)))
		return E_FAIL;

	return NOERROR;
}

void CScene::Free()
{
	Safe_Release(m_pComponent_Manager);
	Safe_Release(m_pObject_Manager);
	Safe_Release(m_pGraphic_Device);
}
