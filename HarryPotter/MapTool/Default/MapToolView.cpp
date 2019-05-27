
// MapToolView.cpp : CMapToolView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "MapTool.h"
#endif

#include "MapToolDoc.h"
#include "MapToolView.h"
#include "ViewManager.h"
#include "Input_Device.h"
#include "Renderer.h"
#include "Component_Manager.h"
#include "Object_Manager.h"
#include "EventManagerTool.h"
#include "EventManager.h"
#include "Layer.h"
#include "DataManager.h"
#include "Buffer_Terrain_Tool.h"


// Object
#include "Camera_Dynamic.h"
#include "StaticObject.h"
#include "Terrain.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMapToolView
HWND g_hWnd;
HWND g_WinhWnd;
IMPLEMENT_DYNCREATE(CMapToolView, CView)

BEGIN_MESSAGE_MAP(CMapToolView, CView)
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_TIMER()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

// CMapToolView ����/�Ҹ�

CMapToolView::CMapToolView()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}

CMapToolView::~CMapToolView()
{
	CDataManager::GetInstance()->DestroyInstance();
	CEventManager::GetInstance()->DestroyInstance();
	CEventManagerTool::GetInstance()->DestroyInstance();
	CViewManager::GetInstance()->DestroyInstance();

	Safe_Release(m_pViewManager);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pManagement);
	Safe_Release(m_pGraphicDevice);

	CManagement::Release_Engine();
}

BOOL CMapToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CMapToolView �׸���

void CMapToolView::OnDraw(CDC* /*pDC*/)
{
	CMapToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
	if (nullptr == m_pGraphicDevice ||
		nullptr == m_pRenderer)
		return;

	m_pGraphicDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DXCOLOR(0.f, 0.f, 1.f, 1.f), 1.f, 0);
	m_pGraphicDevice->BeginScene();

	//���� ������Ʈ�� ���� ����
	if (FAILED(m_pRenderer->Render_RenderGroup()))
		return;

	m_pGraphicDevice->EndScene();
	m_pGraphicDevice->Present(nullptr, nullptr, 0, nullptr);

}


// CMapToolView �μ�

BOOL CMapToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CMapToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CMapToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}

HRESULT CMapToolView::InitDefaultSetting(CGraphic_Device::WINMODE eType, const _uint & iWinCX, const _uint & iWinCY)
{
	CMainFrame*		pMainFrm = ((CMainFrame*)AfxGetMainWnd());

	RECT		rcWindow;
	pMainFrm->GetWindowRect(&rcWindow);	 // ������ â �������� ����� ������ �Լ�

	SetRect(&rcWindow,	// ������ ũ���� ���ο� ���� ����� ���ο� ��Ʈ�� right, bottom�� ����
		0,
		0,
		rcWindow.right - rcWindow.left,
		rcWindow.bottom - rcWindow.top);


	RECT	rcMainView;
	GetClientRect(&rcMainView);	// ������ �� â�� ũ�⸦ ������ �Լ�

	float	fRowFrm = float(rcWindow.right - rcMainView.right);
	float	fColFrm = float(rcWindow.bottom - rcMainView.bottom);

	// �� â�� ��ǥ���� 0,0 �������� ����� �� �ְ� â�� ��ġ�� �������ϴ� �Լ�
	pMainFrm->SetWindowPos(NULL,
		0,
		0,
		int(g_iWinCX + fRowFrm),
		int(g_iWinCY + fColFrm),
		SWP_NOZORDER);

	if (nullptr == m_pManagement)
		return E_FAIL;

	HWND	hWnd;
	hWnd = AfxGetApp()->m_pMainWnd->m_hWnd;

	g_WinhWnd = m_hWnd;

	// For.Graphic_Device
	if (FAILED(CGraphic_Device::GetInstance()->Ready_Graphic_Device(g_WinhWnd, eType, iWinCX, iWinCY, &m_pGraphicDevice)))
		return E_FAIL;

	// For.Input_Device //
	HINSTANCE hInst;
	hInst = AfxGetInstanceHandle();

	if (FAILED(CInput_Device::GetInstance()->Ready_Input_Device(hInst, hWnd)))
		return E_FAIL;
	//////////////////////

	m_pViewManager->m_pGraphic_Device = m_pGraphicDevice;
	
	CEventManagerTool::GetInstance()->m_pGraphic_Device = m_pGraphicDevice;

	if (FAILED(m_pManagement->Ready_Management(SCENE_END)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CMapToolView::InitRenderState()
{
	m_pGraphicDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pGraphicDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	return NOERROR;
}

HRESULT CMapToolView::InitComponentPrototype()
{
	CComponent_Manager*	pComponent_Manager = CComponent_Manager::GetInstance();
	if (nullptr == pComponent_Manager)
		return E_FAIL;

	pComponent_Manager->AddRef();

	// For.Component_Transform
	if (FAILED(pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Transform", CTransform::Create(m_pGraphicDevice))))
		return E_FAIL;

	// For.Component_Renderer
	if (FAILED(pComponent_Manager->Add_Component_Prototype(SCENE_STATIC, L"Component_Renderer", m_pRenderer = CRenderer::Create(m_pGraphicDevice))))
		return E_FAIL;
	m_pRenderer->AddRef();

	Safe_Release(pComponent_Manager);

	return NOERROR;
}

HRESULT CMapToolView::InitObjectPrototype()
{
	CObject_Manager*	pObject_Manager = CObject_Manager::GetInstance();
	if (nullptr == pObject_Manager)
		return E_FAIL;
	
	pObject_Manager->AddRef();

	// For.GameObject_Camera_Dynamic
	if (FAILED(pObject_Manager->Add_Object_Prototype(SCENE_STATIC, L"GameObject_Camera_Dynamic", CCamera_Dynamic::Create(m_pGraphicDevice))))
		return E_FAIL;

	Safe_Release(pObject_Manager);

	return NOERROR;
}


// CMapToolView ����

#ifdef _DEBUG
void CMapToolView::AssertValid() const
{
	CView::AssertValid();
}

void CMapToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMapToolDoc* CMapToolView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMapToolDoc)));
	return (CMapToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CMapToolView �޽��� ó����


void CMapToolView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	m_pViewManager = CViewManager::GetInstance();
//	m_pDataManager = CDataManager::GetInstance();

	g_hWnd = m_hWnd;

	{
		m_pManagement = CManagement::GetInstance();
		m_pManagement->AddRef();
	}

	if (FAILED(InitDefaultSetting(CGraphic_Device::TYPE_WINMODE, g_iWinCX, g_iWinCY)))
		return;

	if (FAILED(InitRenderState()))
		return;

	// ���������� ����� ����������Ʈ���� ����.
	if (FAILED(InitComponentPrototype()))
		return;

	if (FAILED(InitObjectPrototype()))
		return;

	m_pViewManager->m_pMainView = this;

	SetTimer(1, 20, NULL);

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
}


void CMapToolView::OnTimer(UINT_PTR nIDEvent)
{
	m_pManagement->Update_Management(1.f);
	Invalidate(FALSE);

	CView::OnTimer(nIDEvent);
}


void CMapToolView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	
	m_pViewManager->m_vMousePos = _vec3(float(point.x), float(point.y), 0.f);
	
	_tchar mouse[128] = L"";
	wsprintf(mouse, L"Mouse : %d, %d ", point.x, point.y);

	m_pViewManager->m_pMainFrame->SetStatusBar(1, mouse);

	//Invalidate(FALSE);
	CView::OnMouseMove(nFlags, point);
}


void CMapToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	if (FAILED(m_pViewManager->m_pEditorView->m_Tab_Mesh.MakeItemForTree()))
		return;

	_vec3 vPos = { 0.f, 0.f, 0.f };

	CLayer* pTerrainLayer = CObject_Manager::GetInstance()->FindObjectLayer(SCENE_STATIC, L"Layer_Terrain");
	CTerrain* pTerrain = dynamic_cast<CTerrain*>(pTerrainLayer->Get_ObjectList().back());

	CTransform* pTransform = pTerrain->GetTransformCom();

	RECT	rcTemp;
	GetClientRect(&rcTemp);

	pTerrain->Picking(g_WinhWnd, pTransform, &vPos);

	vPos.y = 0.f;

	CLayer* pStaticObjLayer = CObject_Manager::GetInstance()->FindObjectLayer(SCENE_STATIC, m_pViewManager->m_pEditorView->m_Tab_Mesh.strLayerTag);
	dynamic_cast<CStaticObject*>(pStaticObjLayer->Get_ObjectList().back())->SetState(vPos, _vec3(1.f, 1.f, 1.f));


 	CView::OnLButtonDown(nFlags, point);
}


void CMapToolView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	CView::OnRButtonDown(nFlags, point);
}


BOOL CMapToolView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}
