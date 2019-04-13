#include "stdafx.h"
#include "..\Headers\Scene_Logo.h"

#include "Back_Logo.h"

_USING(Client)

CScene_Logo::CScene_Logo(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CScene(pGraphic_Device)
{
}

HRESULT CScene_Logo::Ready_Scene()
{
	// �ΰ������ ����� ����������Ʈ���� ����.
	if (FAILED(Ready_Component_Prototype()))
		return E_FAIL;

	// �ΰ������ ����� ��ü���� ������ �غ��س��´�.
	if (FAILED(Ready_GameObject_Prototype()))
		return E_FAIL;

	// For.Layer_BackGround
	if (FAILED(Ready_Layer_BackGround(L"Layer_BackGround")))
		return E_FAIL;


	return NOERROR;
}

_int CScene_Logo::Update_Scene(const _float & fTimeDelta)
{

	return _int(CScene::Update_Scene(fTimeDelta));
}

_int CScene_Logo::LateUpdate_Scene(const _float & fTimeDelta)
{
	return _int(CScene::LateUpdate_Scene(fTimeDelta));
}

HRESULT CScene_Logo::Render_Scene()
{

	return CScene::Render_Scene();
}

HRESULT CScene_Logo::Ready_Component_Prototype()
{
	if (nullptr == m_pComponent_Manager)
		return E_FAIL;

	// For.Component_Buffer_TriCol
	if (FAILED(m_pComponent_Manager->Add_Component_Prototype(SCENE_LOGO, L"Component_Buffer_TriCol", CBuffer_TriCol::Create(m_pGraphic_Device))))
		return E_FAIL;

	return NOERROR;
}

HRESULT CScene_Logo::Ready_GameObject_Prototype()
{
	// For.GameObject_BackLogo
	// Engine Project�� Scene Class�� Add_Object_Prototype�Լ� ȣ��
	if (FAILED(Add_Object_Prototype(SCENE_LOGO, L"GameObject_BackLogo", CBack_Logo::Create(m_pGraphic_Device))))
		return E_FAIL;

	return NOERROR;
}

HRESULT CScene_Logo::Ready_Layer_BackGround(const _tchar* pLayerTag)
{
	// For.Back_Logo
	if (FAILED(Add_Object(SCENE_LOGO, L"GameObject_BackLogo", SCENE_LOGO, pLayerTag)))
		return E_FAIL;

	return NOERROR;
}

CScene_Logo * CScene_Logo::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CScene_Logo* pInstance = new CScene_Logo(pGraphic_Device);

	if (FAILED(pInstance->Ready_Scene()))
	{
		_MSGBOX("CScene_Logo Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CScene_Logo::Free()
{
	CScene::Free();
}


