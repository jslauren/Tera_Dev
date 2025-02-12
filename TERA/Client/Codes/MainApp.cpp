#include "stdafx.h"
#include "..\Headers\MainApp.h"
#include "Graphic_Device.h"
#include "Input_Device.h"
#include "Scene_Logo.h"
#include "Management.h"
#include "Camera_Dynamic.h"
#include "Camera_Static.h"
#include "Component_Manager.h"
#include "EventManager.h"
#include "Input_Device.h"
#include "Object_Manager.h"
#include "Scene_Loading.h"
#include "Scene_Stage.h"
#include "UI_Loading.h"
#include "FontManager.h"
#include "SoundManager.h"
#include "Player.h"

#include "UI_DamageFont_Manager.h"

_USING(Client)

CMainApp::CMainApp()
	: m_pManagement(CManagement::GetInstance()) // == (m_pManagemnet = CManagement::GetInstance();)
{
	m_pManagement->AddRef();	// 참조 했으니 AddRef 조져준다.
}

HRESULT CMainApp::Ready_MainApp()	// Initialize_MainApp
{
	// 그래픽카드 초기화, 입력장치 초기화, 매니지먼트 초기화.
	if (FAILED(Ready_Default_Setting(CGraphic_Device::TYPE_WINMODE, g_iWinCX, g_iWinCY)))
		return E_FAIL;

	//if (FAILED(Ready_Render_State()))
	//	return E_FAIL;

	// 전역씬에서 사용할 원형컴포넌트들의 생성.
	if (FAILED(Ready_Component_Prototype()))
		return E_FAIL;

	if (FAILED(Ready_GameObject_Prototype()))
		return E_FAIL;

	// 씬 셋팅
	// 어떤 씬을 게임 시작시에 실행 할 지 열거체로 넣어준다.
	if (FAILED(Ready_Scene(SCENE_LOGO)))
		return E_FAIL;

	return NOERROR;
}

_int CMainApp::Update_MainApp(const _float & fTimeDelta)
{
	if (nullptr == m_pManagement)	// 씬 매니저가 GetInstance() 안되었다면,
		return -1;

	// 연결된 키보드, 마우스 장치를 받아오는 구문
	if (FAILED(CInput_Device::GetInstance()->SetUp_Input_State()))
		return -1;

	CKeyManager::GetInstance()->UpdateKey();

	m_fTimeAcc += fTimeDelta;

	return m_pManagement->Update_Management(fTimeDelta);
}

HRESULT CMainApp::Render_MainApp()
{
	///////// FPS 출력 부분 /////////
	{
		++m_iRenderCnt;

		if (m_fTimeAcc >= 1.f)
		{
			wsprintf(m_szFPS, L"%d", m_iRenderCnt);
			//wsprintf(m_szFPS, L"FPS : %d", m_iRenderCnt);
			m_iRenderCnt = 0;
			m_fTimeAcc = 0.f;
		}

		SetWindowText(g_hWnd, m_szGameName);
	}

	if (nullptr == m_pGraphic_Device ||
		nullptr == m_pRenderer)
		return E_FAIL;

	//// MainApp 출력 부분 ////
	{
		m_pGraphic_Device->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DXCOLOR(0.f, 0.f, 0.f, 1.f), 1.f, 0);
		m_pGraphic_Device->BeginScene();

		m_pRenderer->Render_RenderGroup();

		CFontManager::GetInstance()->RenderFont(CFontManager::FONT_NAME, _vec3(10.f, 10.f, 0.f), m_szFPS);

		if (FAILED(m_pManagement->Render_Management()))
			return E_FAIL;

		m_pGraphic_Device->EndScene();
		m_pGraphic_Device->Present(nullptr, nullptr, 0, nullptr);
	}

	return NOERROR;
}

HRESULT CMainApp::Ready_Default_Setting(CGraphic_Device::WINMODE eType, const _uint& iWinCX, const _uint& iWinCY)
{
	if (nullptr == m_pManagement)
		return E_FAIL;

	// For.Graphic_Device
	if (FAILED(CGraphic_Device::GetInstance()->Ready_Graphic_Device(g_hWnd, eType, iWinCX, iWinCY, &m_pGraphic_Device)))
		return E_FAIL;

	// For.Input_Device
	if (FAILED(CInput_Device::GetInstance()->Ready_Input_Device(g_hInst, g_hWnd)))
		return E_FAIL;

	// For.FontManager
	if (FAILED(CFontManager::GetInstance()->ReadyFont(m_pGraphic_Device)))
		return E_FAIL;

	// For.SoundManager
	if (FAILED(CSoundManager::GetInstance()->ReadySound()))
		return E_FAIL;

	// For.Scene_Manager Initialize
	// 내가 사용하고자하는 Engine.lib 의 초기화.
	if (FAILED(m_pManagement->Ready_Management(SCENE_END)))
		return E_FAIL;

	return NOERROR;
}

//HRESULT CMainApp::Ready_Render_State()
//{
//	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
//
//	// For.Gara
//	_matrix			matView, matProj;
//
//	D3DXMatrixLookAtLH(&matView, &_vec3(50.0f, 15.f, -15.f), &_vec3(50.f, 0.f, 50.f), &_vec3(0.f, 1.f, 0.f));
//	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &matView);
//
//	D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(90.0f), g_iWinCX / (_float)g_iWinCY, 0.1f, 500.f);
//	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &matProj);
//
//	return NOERROR;
//}

HRESULT CMainApp::Ready_Component_Prototype()
{
	CComponent_Manager*	pComponent_Manager = CComponent_Manager::GetInstance();
	if (nullptr == pComponent_Manager)
		return E_FAIL;

	pComponent_Manager->AddRef();

	// 로딩에 필요한 텍스쳐를 그리는데 필요한 일반 쉐이더
	// 쉐이더는 왠만하면 가장 상단에 위치하는게 좋다.
	// For.Component_Shader_Default
	if (FAILED(pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Shader_UI", CShader::Create(m_pGraphic_Device, L"../Bin/ShaderFiles/Shader_UI.fx"))))
		return E_FAIL;

	// For.Component_Transform
	if (FAILED(pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Transform", CTransform::Create(m_pGraphic_Device))))
		return E_FAIL;

	// For.Component_Renderer
	if (FAILED(pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Renderer", m_pRenderer = CRenderer::Create(m_pGraphic_Device))))
		return E_FAIL;
	m_pRenderer->AddRef();

	// [ 로딩 화면 리소스 ] //
	// 로딩화면을 채울 텍스쳐를 위한 RcTex 로딩.
	// For.Component_Buffer_RcTex
	if (FAILED(pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Buffer_Background", CBuffer_RcTex::Create(m_pGraphic_Device))))
		return E_FAIL;

	// 로딩 텍스쳐 지정
	// For.Component_Texture_Default
	if (FAILED(pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_Loading_Main", CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, L"../Bin/Resources/Textures/Tera_Main.tga"))))
		return E_FAIL;

	// For.Component_Texture_Loading_Sub
	if (FAILED(pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_Loading_Sub", CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, L"../Bin/Resources/Textures/UI/Loading/LoadingTexture/Loading_%d.tga", 10))))
		return E_FAIL;

	// 로딩 씬의 로딩 게이지를 위한 버퍼
	// For.Component_Buffer_UI_Loading_Frame
	if (FAILED(pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Buffer_UI_Bar_Frame", CBuffer_RcTex::Create(m_pGraphic_Device))))
		return E_FAIL;

	// For.Component_Buffer_UI_Loading_Filter
	if (FAILED(pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Buffer_UI_Loading_Filter", CBuffer_RcTex::Create(m_pGraphic_Device))))
		return E_FAIL;

	// For.Component_Buffer_UI_Loading_Gauge
	if (FAILED(pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Buffer_UI_Loading_Gauge", CBuffer_RcTex::Create(m_pGraphic_Device))))
		return E_FAIL;

	// For.Component_Buffer_UI_Loading
	if (FAILED(pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Buffer_UI_Loading", CBuffer_RcTex::Create(m_pGraphic_Device))))
		return E_FAIL;

	// 로딩 바 텍스쳐
	// For.Component_Texture_Bar_Frame
	if (FAILED(pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_Bar_Frame", CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, L"../Bin/Resources/Textures/UI/Loading/Bar_Frame.png"))))
		return E_FAIL;

	// For.Component_Texture_Bar_Filter
	if (FAILED(pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_Bar_Filter", CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, L"../Bin/Resources/Textures/UI/Loading/Bar_Filter.png"))))
		return E_FAIL;

	// For.Component_Texture_Bar_Gauge
	if (FAILED(pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_Bar_Gauge", CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, L"../Bin/Resources/Textures/UI/Loading/Bar_Gauge.png"))))
		return E_FAIL;
	
	Safe_Release(pComponent_Manager);

	return NOERROR;
}

HRESULT CMainApp::Ready_GameObject_Prototype()
{
	CObject_Manager*	pObject_Manager = CObject_Manager::GetInstance();
	if (nullptr == pObject_Manager)
		return E_FAIL;

	pObject_Manager->AddRef();

	if (FAILED(pObject_Manager->Add_Object_Prototype(SCENE_STATIC, L"GameObject_Loading", CUI_Loading::Create(m_pGraphic_Device))))
		return E_FAIL;
		
	Safe_Release(pObject_Manager);

	return NOERROR;
}

HRESULT CMainApp::Ready_Scene(SCENEID eID)
{
	// 이걸 클라에서 하는 이유는,
	// 엔진은 범용적으로 쓰기 위함이고, 각 클라마다 어떤 씬을 사용할 지 모르기에,
	// Scene Initialize를 Client에 생성한다.

	if (nullptr == m_pManagement)
		return E_FAIL;

	CScene*		pScene = nullptr;

	// Ready_MainApp에서 선택한 씬으로 Switch분기를 나눠 해당 씬을 생성한다.

	switch (eID)
	{
	case SCENE_STATIC:
		break;
	case SCENE_LOGO:
		pScene = CScene_Logo::Create(m_pGraphic_Device);
		break;
	case SCENE_STAGE:
		pScene = CScene_Stage::Create(m_pGraphic_Device);
		break;
	case SCENE_DRAGON:
		break;
	case SCENE_BOSS:
		break;
	//case SCENE_LOADING:
	//	pScene = CScene_Loading::Create(m_pGraphic_Device, eID);
	//	break;
	}

	if (nullptr == pScene)
		return E_FAIL;

	// (m_pManagement)씬 매니저는 현재 생성된 씬을 가지고 있어야 하므로,
	// 현재 내가 열거체로 선택하고 생성한 씬을 씬 매니저와 동기화
	if (FAILED(m_pManagement->SetUp_CurrentScene(pScene, eID)))
		return E_FAIL;

	return NOERROR;
}

CMainApp * CMainApp::Create()
{
	// Create 함수는 항상 이런 패턴으로 만들어 진다. 잘 숙지해 두자.
	CMainApp* pInstance = new CMainApp();

	// Client.cpp 에서 pMainApp->Create() 해주면 이게 실행되는데,
	// 객체를 생성하면서 기본 값들을 다 셋팅해 주기 위해,
	// Ready_MainApp() 구문을 넣어준다.
	if (FAILED(pInstance->Ready_MainApp()))
	{
		_MSGBOX("CMainApp Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMainApp::Free()
{
	// 내 멤버를 정리한다.!!!! 
	Safe_Release(m_pRenderer);
	Safe_Release(m_pManagement);
	Safe_Release(m_pGraphic_Device);

	CSoundManager::GetInstance()->DestroyInstance();
	CUI_DamageFont_Manager::GetInstance()->DestroyInstance();

 	if (FAILED(CManagement::Release_Engine()))
		return;
}
