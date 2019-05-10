#pragma once
#include "stdafx.h"
#include "..\Headers\MainAppTool.h"
#include "Graphic_Device.h"
#include "Input_Device.h"
#include "Object_Manager.h"

#include "Management.h"
//#include "Camera_Dynamic.h"
#include "Component_Manager.h"

_USING(Maptool)

CMainAppTool::CMainAppTool()
	: m_pManagement(CManagement::GetInstance())
{
	m_pManagement->AddRef();
}

HRESULT CMainAppTool::Ready_MainApp()
{
	// 그래픽카드 초기화, 입력장치 초기화, 매니지먼트 초기화.
	if (FAILED(Ready_Default_Setting(CGraphic_Device::TYPE_WINMODE, g_iToolWinCX, g_iToolWinCY)))
		return E_FAIL;


	// 씬 셋팅
	// 어떤 씬을 게임 시작시에 실행 할 지 열거체로 넣어준다.
	if (FAILED(Ready_Scene(TOOL_SCENE_TERRAIN)))
		return E_FAIL;

}

_int CMainAppTool::Update_MainApp(const _float & fTimeDelta)
{
	if (nullptr == m_pManagement)	// 씬 매니저가 GetInstance() 안되었다면,
		return -1;

	// 연결된 키보드, 마우스 장치를 받아오는 구문
	if (FAILED(CInput_Device::GetInstance()->SetUp_Input_State()))
		return -1;

	m_fTimeAcc += fTimeDelta;

	return m_pManagement->Update_Management(fTimeDelta);
}

HRESULT CMainAppTool::Render_MainApp()
{
	///////// FPS 출력 부분 /////////
	{
		++m_iRenderCnt;

		if (m_fTimeAcc >= 1.f)
		{
			wsprintf(m_szFPS, L"FPS : %d", m_iRenderCnt);
			m_iRenderCnt = 0;
			m_fTimeAcc = 0.f;
		}

		SetWindowText(g_ToolhWnd, m_szFPS);
	}

	if (nullptr == m_pGraphic_Device ||
		nullptr == m_pRenderer)
		return E_FAIL;

	//// MainApp 출력 부분 ////
	{
		m_pGraphic_Device->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DXCOLOR(0.f, 0.f, 1.f, 1.f), 1.f, 0);
		m_pGraphic_Device->BeginScene();

		m_pRenderer->Render_RenderGroup();

		if (FAILED(m_pManagement->Render_Management()))
			return E_FAIL;

		m_pGraphic_Device->EndScene();
		m_pGraphic_Device->Present(nullptr, nullptr, 0, nullptr);
	}

	return NOERROR;
}

HRESULT CMainAppTool::Ready_Default_Setting(CGraphic_Device::WINMODE eType, const _uint & iWinCX, const _uint & iWinCY)
{
	if (nullptr == m_pManagement)
		return E_FAIL;

	// For.Graphic_Device
	if (FAILED(CGraphic_Device::GetInstance()->Ready_Graphic_Device(g_ToolhWnd, eType, iWinCX, iWinCY, &m_pGraphic_Device)))
		return E_FAIL;

	// For.Scene_Manager Initialize
	// 내가 사용하고자하는 Engine.lib 의 초기화.
	if (FAILED(m_pManagement->Ready_Management(TOOL_SCENE_END)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CMainAppTool::Ready_Component_Prototype()
{
	CComponent_Manager*	pComponent_Manager = CComponent_Manager::GetInstance();
	if (nullptr == pComponent_Manager)
		return E_FAIL;

	pComponent_Manager->AddRef();

	// For.Component_Transform
	if (FAILED(pComponent_Manager->Add_Component_Prototype(TOOL_SCENE_STATIC, L"Component_Transform", CTransform::Create(m_pGraphic_Device))))
		return E_FAIL;

	// For.Component_Renderer
	if (FAILED(pComponent_Manager->Add_Component_Prototype(TOOL_SCENE_STATIC, L"Component_Renderer", m_pRenderer = CRenderer::Create(m_pGraphic_Device))))
		return E_FAIL;
	m_pRenderer->AddRef();

	// For.Component_Buffer_RcTex
	if (FAILED(pComponent_Manager->Add_Component_Prototype(TOOL_SCENE_STATIC, L"Component_Buffer_RcTex", CBuffer_RcTex::Create(m_pGraphic_Device))))
		return E_FAIL;

	// For.Component_Buffer_UI
	if (FAILED(pComponent_Manager->Add_Component_Prototype(TOOL_SCENE_STATIC, L"Component_Buffer_UI", CBuffer_ScreenTex::Create(m_pGraphic_Device))))
		return E_FAIL;

	//// For.Component_Texture_Default
	//if (FAILED(pComponent_Manager->Add_Component_Prototype(TOOL_SCENE_STATIC, L"Component_Texture_Default", CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, L"../Bin/Resources/Textures/Default.jpg"))))
	//	return E_FAIL;

	// For.Component_Shader_Default
	if (FAILED(pComponent_Manager->Add_Component_Prototype(TOOL_SCENE_STATIC, L"Component_Shader_Default", CShader::Create(m_pGraphic_Device, L"../Bin/ShaderFiles/Shader_Default.fx"))))
		return E_FAIL;

	Safe_Release(pComponent_Manager);
}

HRESULT CMainAppTool::Ready_GameObject_Prototype()
{
	//CObject_Manager*	pObject_Manager = CObject_Manager::GetInstance();
	//if (nullptr == pObject_Manager)
	//	return E_FAIL;
	//pObject_Manager->AddRef();

	//// For.GameObject_Camera_Dynamic
	//if (FAILED(pObject_Manager->Add_Object_Prototype(TOOL_SCENE_STATIC, L"GameObject_Camera_Dynamic", Engine::CCamera_Dynamic::Create(m_pGraphic_Device))))
	//	return E_FAIL;

	//Safe_Release(pObject_Manager);

	return NOERROR;
}

HRESULT CMainAppTool::Ready_Scene(TOOLSCENEID eID)
{
	if (nullptr == m_pManagement)
		return E_FAIL;

	//CScene*		pScene = nullptr;

	//// Ready_MainApp에서 선택한 씬으로 Switch분기를 나눠 해당 씬을 생성한다.

	//switch (eID)
	//{
	//case TOOL_SCENE_MESH:
	//	//pScene = CScene_Logo::Create(m_pGraphic_Device);
	//	break;
	//case TOOL_SCENE_CAMERA:
	//	// pScene = CScene_Logo::Create(m_pGraphic_Device);
	//	break;
	//case TOOL_SCENE_EFFECT:
	//	// pScene = CScene_Logo::Create(m_pGraphic_Device);
	//	break;
	//}

	//if (nullptr == pScene)
	//	return E_FAIL;

	//// (m_pManagement)씬 매니저는 현재 생성된 씬을 가지고 있어야 하므로,
	//// 현재 내가 열거체로 선택하고 생성한 씬을 씬 매니저와 동기화
	//if (FAILED(m_pManagement->SetUp_CurrentScene(pScene, TOOL_SCENE_TERRAIN)))
	//	return E_FAIL;

	return NOERROR;
}

CMainAppTool * CMainAppTool::Create()
{
	// Create 함수는 항상 이런 패턴으로 만들어 진다. 잘 숙지해 두자.
	CMainAppTool* pInstance = new CMainAppTool();

	// Client.cpp 에서 pMainApp->Create() 해주면 이게 실행되는데,
	// 객체를 생성하면서 기본 값들을 다 셋팅해 주기 위해,
	// Ready_MainApp() 구문을 넣어준다.
	if (FAILED(pInstance->Ready_MainApp()))
	{
		_MSGBOX("CMainAppTool Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMainAppTool::Free()
{
	// 내 멤버를 정리한다.!!!! 
	Safe_Release(m_pRenderer);
	Safe_Release(m_pManagement);
	Safe_Release(m_pGraphic_Device);

	if (FAILED(CManagement::Release_Engine()))
		return;
}
