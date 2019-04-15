#include "stdafx.h"
#include "..\Headers\MainApp.h"
#include "Graphic_Device.h"
#include "Scene_Logo.h"
#include "Management.h"
#include "Component_Manager.h"

_USING(Client)

CMainApp::CMainApp()
	: m_pManagement(CManagement::GetInstance())	// == (m_pManagemnet = CManagement::GetInstance();)
{
	m_pManagement->AddRef();	// ���� ������ AddRef �����ش�.
}

HRESULT CMainApp::Ready_MainApp()	// Initialize_MainApp
{
	// ��ġ �ʱ�ȭ
	if (FAILED(Ready_Default_Setting(CGraphic_Device::TYPE_WINMODE, g_iWinCX, g_iWinCY)))
		return E_FAIL;

	if (FAILED(Ready_Render_State()))
		return E_FAIL;

	// ���������� ��� �� ����������Ʈ���� ����.
    	if (FAILED(Ready_Component_Prototype()))
		return E_FAIL;

	// �� ����
	// � ���� ���� ���۽ÿ� ���� �� �� ����ü�� �־��ش�.
	if (FAILED(Ready_Scene(SCENE_LOGO)))
		return E_FAIL;

	return NOERROR;
}

_int CMainApp::Update_MainApp(const _float & fTimeDelta)
{
	if (nullptr == m_pManagement)	// �� �Ŵ����� GetInstance() �ȵǾ��ٸ�,
		return -1;

	m_fTimeAcc += fTimeDelta;

	return m_pManagement->Update_Management(fTimeDelta);
}

HRESULT CMainApp::Render_MainApp()
{
	///////// FPS ��� �κ� /////////
	++m_iRenderCnt;

	if (m_fTimeAcc >= 1.f)
	{
		wsprintf(m_szFPS, L"FPS : %d", m_iRenderCnt);
		m_iRenderCnt = 0;
		m_fTimeAcc = 0.f;
	}

	SetWindowText(g_hWnd, m_szFPS);
	//////////////////////////////////

	if (nullptr == m_pGraphic_Device ||
		nullptr == m_pRenderer)
		return E_FAIL;

	//// MainApp ��� �κ� ////
	m_pGraphic_Device->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DXCOLOR(0.f, 0.f, 1.f, 1.f), 1.f, 0);
	m_pGraphic_Device->BeginScene();

	m_pRenderer->Render_RenderGroup();

	if (FAILED(m_pManagement->Render_Management()))	// �� �Ŵ����� ������ ������.
		return E_FAIL;

	m_pGraphic_Device->EndScene();
	m_pGraphic_Device->Present(nullptr, nullptr, 0, nullptr);
	//////////////////////////

	return NOERROR;
}

HRESULT CMainApp::Ready_Default_Setting(CGraphic_Device::WINMODE eType, const _uint& iWinCX, const _uint& iWinCY)
{
	if (nullptr == m_pManagement)
		return E_FAIL;

	// For.Graphic_Device
	if (FAILED(CGraphic_Device::GetInstance()->Ready_Graphic_Device(g_hWnd, eType, iWinCX, iWinCY, &m_pGraphic_Device)))
		return E_FAIL;

	// For.Scene_Manager Initialize
	if (FAILED(m_pManagement->Ready_Management(SCENE_END)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CMainApp::Ready_Render_State()
{
	// For.Turn off Light
	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

	// For.Gara
	_matrix		matView, matProj;

	D3DXMatrixLookAtLH(&matView, &_vec3(0.f, 0.f, -10.f), &_vec3(0.f, 0.f, 0.f), &_vec3(0.f, 1.f, 0.f));
	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &matView);

	D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(90.f), (g_iWinCX / (_float)g_iWinCY), 0.1f, 500.f);
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &matProj);
	
	return NOERROR;
}

HRESULT CMainApp::Ready_Component_Prototype()
{
	CComponent_Manager*	pComponent_Manager = CComponent_Manager::GetInstance();
	if (nullptr == pComponent_Manager)
		return E_FAIL;

	pComponent_Manager->AddRef();

	// For.Component_Transform
	if (FAILED(pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Transform", CTransform::Create(m_pGraphic_Device))))
		return E_FAIL;

	// For.Component_Renderer
	if (FAILED(pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Renderer", m_pRenderer = CRenderer::Create(m_pGraphic_Device))))
		return E_FAIL;
	m_pRenderer->AddRef();

	Safe_Release(pComponent_Manager);

	return NOERROR;
}

HRESULT CMainApp::Ready_Scene(SCENEID eID)
{
	// �̰� Ŭ�󿡼� �ϴ� ������,
	// ������ ���������� ���� �����̰�, �� Ŭ�󸶴� � ���� ����� �� �𸣱⿡,
	// Scene Initialize�� Client�� �����Ѵ�.

	if (nullptr == m_pManagement)
		return E_FAIL;

	CScene*		pScene = nullptr;

	// Ready_MainApp���� ������ ������ Switch�б⸦ ���� �ش� ���� �����Ѵ�.
	switch (eID)
	{
	case SCENE_LOGO:
		pScene = CScene_Logo::Create(m_pGraphic_Device);
		break;
	case SCENE_STAGE:
		// pScene = CScene_Logo::Create(m_pGraphic_Device);
		break;
	}

	if (nullptr == pScene)
		return E_FAIL;

	// (m_pManagement)�� �Ŵ����� ���� ������ ���� ������ �־�� �ϹǷ�,
	// ���� ���� ����ü�� �����ϰ� ������ ���� �� �Ŵ����� ����ȭ
	if (FAILED(m_pManagement->SetUp_CurrentScene(pScene)))
		return E_FAIL;

	Safe_Release(pScene);

	return NOERROR;
}

CMainApp * CMainApp::Create()
{
	// Create �Լ��� �׻� �̷� �������� ����� ����. �� ������ ����.
	CMainApp* pInstance = new CMainApp();

	// Client.cpp ���� pMainApp->Create() ���ָ� �̰� ����Ǵµ�,
	// ��ü�� �����ϸ鼭 �⺻ ������ �� ������ �ֱ� ����,
	// Ready_MainApp() ������ �־��ش�.
	if (FAILED(pInstance->Ready_MainApp()))
	{
		_MSGBOX("CMainApp Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMainApp::Free()
{
	// �� ����� �����Ѵ�.!!!! 
	Safe_Release(m_pRenderer);
	Safe_Release(m_pManagement);
	Safe_Release(m_pGraphic_Device);

	if (FAILED(CManagement::Release_Engine()))
		return;
}
