#pragma once
#include "stdafx.h"
#include "Shader.h"
#include "Renderer.h"
#include "Management.h"
#include "MainAppTool.h"
//#include "Camera_Dynamic.h"
#include "Object_Manager.h"
#include "ViewManagerTool.h"
#include "Component_Manager.h"

// Scene
#include "Scene_Proto.h"

_USING(MapTool)

CMainAppTool::CMainAppTool()
	: m_pManagement(CManagement::GetInstance())
{
	m_pManagement->AddRef();
}

HRESULT CMainAppTool::Ready_MainApp()
{
	m_pViewManager = CViewManagerTool::GetInstance();

	// �׷���ī�� �ʱ�ȭ, �Է���ġ �ʱ�ȭ, �Ŵ�����Ʈ �ʱ�ȭ.
	if (FAILED(Ready_Default_Setting(CGraphic_Device::TYPE_WINMODE, g_iWinCX, g_iWinCY)))
		return E_FAIL;

	//if (FAILED(Ready_Render_State()))
	//	return E_FAIL;

	// ���������� ����� ����������Ʈ���� ����.
	if (FAILED(Ready_Component_Prototype()))
		return E_FAIL;

	if (FAILED(Ready_GameObject_Prototype()))
		return E_FAIL;

	m_pViewManager->m_pMainAppTool = this;

	// �� ����
	// � ���� ���� ���۽ÿ� ���� �� �� ����ü�� �־��ش�.
	if (FAILED(Ready_Scene(SCENE_PROTO)))
		return E_FAIL;

	return NOERROR;
}

_int CMainAppTool::Update_MainApp(const _float & fTimeDelta)
{
	if (nullptr == m_pManagement)	// �� �Ŵ����� GetInstance() �ȵǾ��ٸ�,
		return -1;

	m_fTimeAcc += fTimeDelta;

	return m_pManagement->Update_Management(fTimeDelta);
}

HRESULT CMainAppTool::Render_MainApp()
{
	///////// FPS ��� �κ� /////////
	{
		++m_iRenderCnt;

		if (m_fTimeAcc >= 1.f)
		{
			wsprintf(m_szFPS, L"FPS : %d", m_iRenderCnt);
			m_iRenderCnt = 0;
			m_fTimeAcc = 0.f;
		}

		SetWindowText(g_hWnd, m_szFPS);
	}

	if (nullptr == m_pGraphic_Device ||
		nullptr == m_pRenderer)
		return E_FAIL;

	//// MainApp ��� �κ� ////
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
	//CWnd *pWnd = CViewManagerTool::GetInstance()->m_pMainFrame;
	//HWND g_hWnd = pWnd->m_hWnd;

	//{
	//	//CMainFrame*		pMainFrm = ((CMainFrame*)AfxGetMainWnd());

	//	CMainFrame*		pMainFrm = CViewManagerTool::GetInstance()->m_pMainFrame;

	//	RECT		rcWindow;
	//	pMainFrm->GetWindowRect(&rcWindow);	 // ������ â �������� ����� ������ �Լ�

	//	SetRect(&rcWindow,	// ������ ũ���� ���ο� ���� ����� ���ο� ��Ʈ�� right, bottom�� ����
	//		0,
	//		0,
	//		rcWindow.right - rcWindow.left,
	//		rcWindow.bottom - rcWindow.top);


	//	RECT	rcMainView;
	//	GetClientRect(g_hWnd, &rcMainView);	// ������ �� â�� ũ�⸦ ������ �Լ�

	//	float	fRowFrm = float(rcWindow.right - rcMainView.right);
	//	float	fColFrm = float(rcWindow.bottom - rcMainView.bottom);

	//	// �� â�� ��ǥ���� 0,0 �������� ����� �� �ְ� â�� ��ġ�� �������ϴ� �Լ�
	//	pMainFrm->SetWindowPos(NULL,
	//		0,
	//		0,
	//		int(g_iWinCX + fRowFrm),
	//		int(g_iWinCY + fColFrm),
	//		SWP_NOZORDER);
	//}

	if (nullptr == m_pManagement)
		return E_FAIL;

	// For.Graphic_Device
	if (FAILED(CGraphic_Device::GetInstance()->Ready_Graphic_Device(g_hWnd, eType, iWinCX, iWinCY, &m_pGraphic_Device)))
		return E_FAIL;

	// For.Scene_Manager Initialize
	// ���� ����ϰ����ϴ� Engine.lib �� �ʱ�ȭ.
	if (FAILED(m_pManagement->Ready_Management(SCENE_END)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CMainAppTool::Ready_Component_Prototype()
{
	CComponent_Manager*	pComponent_Manager = CComponent_Manager::GetInstance();
	if (nullptr == pComponent_Manager)
		return E_FAIL;

	pComponent_Manager->AddRef();

	//// For.Component_Transform
	//if (FAILED(pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Transform", CTransform::Create(m_pGraphic_Device))))
	//	return E_FAIL;

	// For.Component_Renderer
	if (FAILED(pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Renderer", m_pRenderer = CRenderer::Create(m_pGraphic_Device))))
		return E_FAIL;
	m_pRenderer->AddRef();

	////// For.Component_Texture_Default
	////if (FAILED(pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Texture_Default", CTexture::Create(m_pGraphic_Device, CTexture::TYPE_GENERAL, L"../Bin/Resources/Textures/Default.jpg"))))
	////	return E_FAIL;

	// For.Component_Shader_Default
	if (FAILED(pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Shader_Default", CShader::Create(m_pGraphic_Device, L"../Bin/ShaderFiles/Shader_Default.fx"))))
		return E_FAIL;

	Safe_Release(pComponent_Manager);

	return NOERROR;
}

HRESULT CMainAppTool::Ready_GameObject_Prototype()
{
	CObject_Manager*	pObject_Manager = CObject_Manager::GetInstance();
	if (nullptr == pObject_Manager)
		return E_FAIL;
	pObject_Manager->AddRef();

	//// For.GameObject_Camera_Dynamic
	//if (FAILED(pObject_Manager->Add_Object_Prototype(SCENE_STATIC, L"GameObject_Camera_Dynamic", CCamera_Dynamic::Create(m_pGraphic_Device))))
	//	return E_FAIL;

	Safe_Release(pObject_Manager);

	return NOERROR;
}

HRESULT CMainAppTool::Ready_Scene(SCENEID eID)
{
	CViewManagerTool::GetInstance()->SetDevice(m_pGraphic_Device);
	CViewManagerTool::GetInstance()->SetCurScene(eID);

	return NOERROR;
}

CMainAppTool * CMainAppTool::Create()
{
	CMainAppTool* pInstance = new CMainAppTool();

	if (FAILED(pInstance->Ready_MainApp()))
	{
		_MSGBOX("CMainAppTool Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMainAppTool::Free()
{
	CViewManagerTool::GetInstance()->DestroyInstance();
	Safe_Release(m_pViewManager);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pManagement);
	Safe_Release(m_pGraphic_Device);

	if (FAILED(CManagement::Release_Engine()))
		return;
}
