#include "stdafx.h"
#include "..\Headers\Scene_Logo.h"
#include "Scene_Stage.h"
#include "Management.h"
#include "Back_Logo.h"
#include "UI_Loading.h"
#include "Loading.h"

_USING(Client)

CScene_Logo::CScene_Logo(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CScene(pGraphic_Device)
{
}

HRESULT CScene_Logo::Ready_Scene()
{
	// 로고씬에서 사용할 원형컴포넌트들의 생성.
	if (FAILED(Ready_Component_Prototype()))
		return E_FAIL;

	// 로고씬에서 사용할 객체들의 원형을 준비해놓는다.
	if (FAILED(Ready_GameObject_Prototype()))
		return E_FAIL;

	// For.Layer_BackGround
	if (FAILED(Ready_Layer_BackGround(L"Layer_BackGround")))
		return E_FAIL;

	m_pLoading = CLoading::Create(m_pGraphic_Device, SCENE_STAGE);
	if (nullptr == m_pLoading)
		return E_FAIL;

	// 로딩 정도를 보내주기 위한 구문
	dynamic_cast<CUI_Loading*>(CObject_Manager::GetInstance()->Get_Object(SCENE_LOGO, L"Layer_BackGround"))->Set_LodingClass(m_pLoading);
	
	return NOERROR;
}

_int CScene_Logo::Update_Scene(const _float & fTimeDelta)
{
	return _int(CScene::Update_Scene(fTimeDelta));
}

_int CScene_Logo::LateUpdate_Scene(const _float & fTimeDelta)
{
	CManagement*	pManagement = CManagement::GetInstance();
	if (nullptr == pManagement)
		return -1;

	pManagement->AddRef();

	if (GetKeyState(VK_SPACE) & 0x8000)
	{
		if (100 == m_pLoading->Get_Complete())
		{
			if (FAILED(pManagement->SetUp_CurrentScene(CScene_Stage::Create(m_pGraphic_Device), SCENE_STAGE)))
			{
				Safe_Release(pManagement);
				return -1;
			}

			Safe_Release(pManagement);
			return 0;
		}
	}

	Safe_Release(pManagement);

	return _int(CScene::LateUpdate_Scene(fTimeDelta));
}

HRESULT CScene_Logo::Render_Scene()
{
	//_tchar		szComplete[MAX_PATH];

	//wsprintf(szComplete, L"%d", m_pLoading->Get_Complete());

	//SetWindowText(g_hWnd, szComplete);

	return CScene::Render_Scene();
}

HRESULT CScene_Logo::Ready_Component_Prototype()
{
	if (nullptr == m_pComponent_Manager)
		return E_FAIL;

	return NOERROR;
}

HRESULT CScene_Logo::Ready_GameObject_Prototype()
{
	if (FAILED(Add_Object_Prototype(SCENE_LOGO, L"GameObject_Loading", CUI_Loading::Create(m_pGraphic_Device))))
		return E_FAIL;

	return NOERROR;
}

HRESULT CScene_Logo::Ready_Layer_BackGround(const _tchar* pLayerTag)
{
	if (FAILED(Add_Object(SCENE_LOGO, L"GameObject_Loading", SCENE_LOGO, pLayerTag)))
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
	if (nullptr == m_pObject_Manager ||
		nullptr == m_pComponent_Manager)
		return;

	if (FAILED(m_pObject_Manager->Clear_Object(SCENE_LOGO)))
		return;

	if (FAILED(m_pObject_Manager->Clear_Prototype(SCENE_LOGO)))
		return;

	if (FAILED(m_pComponent_Manager->Clear_Component_Prototype(SCENE_LOGO)))
		return;

	Safe_Release(m_pLoading);

	CScene::Free();
}


