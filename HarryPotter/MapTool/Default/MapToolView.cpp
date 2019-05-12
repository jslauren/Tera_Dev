
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
#include "Renderer.h"
#include "Component_Manager.h"
#include "Object_Manager.h"
//
#include "SceneTerrain.h"

// Object

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMapToolView
HWND g_hWnd;
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
	CViewManager::GetInstance()->DestroyInstance();
	//Safe_Release(m_pDataManager);
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

	if (FAILED(CGraphic_Device::GetInstance()->Ready_Graphic_Device(g_hWnd, eType, iWinCX, iWinCY, &m_pGraphicDevice)))
		return E_FAIL;

	m_pViewManager->m_pGraphic_Device = m_pGraphicDevice;

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
	CComponent_Manager*	pComponentManager = CComponent_Manager::GetInstance();
	if (nullptr == pComponentManager)
		return E_FAIL;

	if (FAILED(pComponentManager->Add_Component_Prototype(SCENE_STATIC, L"Component_Transform", CTransform::Create(m_pGraphicDevice))))
		return E_FAIL;

	m_pRenderer = CRenderer::Create(m_pGraphicDevice);
	if (FAILED(pComponentManager->Add_Component_Prototype(SCENE_STATIC, L"Component_Renderer", m_pRenderer)))
		return E_FAIL;

	if (FAILED(pComponentManager->Add_Component_Prototype(SCENE_STATIC, L"Component_BufferRctCol", CBuffer_RcCol::Create(m_pGraphicDevice))))
		return E_FAIL;

	if (FAILED(pComponentManager->Add_Component_Prototype(SCENE_STATIC, L"Component_BufferRctTex", CBuffer_RcTex::Create(m_pGraphicDevice))))
		return E_FAIL;

	if (FAILED(pComponentManager->Add_Component_Prototype(SCENE_STATIC, L"Component_BufferCubTex", CBuffer_CubeTex::Create(m_pGraphicDevice))))
		return E_FAIL;

	if (FAILED(pComponentManager->Add_Component_Prototype(SCENE_STATIC, L"Component_Terrain", CBuffer_Terrain::Create(m_pGraphicDevice))))
		return E_FAIL;

	if (FAILED(pComponentManager->Add_Component_Prototype(SCENE_STATIC, L"Component_Terrain_Tex", CBuffer_Terrain::Create(m_pGraphicDevice))))
		return E_FAIL;

	//if (FAILED(pComponentManager->Add_Component_Prototype(TOOL_STATIC, L"Component_Texture", CTexture::Create(m_pGraphicDevice, CTexture::TYPE_GENERAL, L"../Bin/Resources/Textures/Cat.jpg"))))
	//	return E_FAIL;

	//if (FAILED(pComponentManager->Add_Component_Prototype(TOOL_STATIC, L"Component_Texture_Guid", CTexture::Create(m_pGraphicDevice, CTexture::TYPE_GENERAL, L"../Bin/Resources/Textures/GuidLine.png"))))
	//	return E_FAIL;

	return NOERROR;
}

HRESULT CMapToolView::InitObjectPrototype()
{
	//if (FAILED(CObject_Manager::GetInstance()->Add_Object_Prototype(TOOL_STATIC, L"Camera_Tile_45", m_pViewManager->m_pTileCamera = CTileCamera::Create(m_pGraphicDevice))))
	//	return E_FAIL;

	//if (FAILED(CObject_Manager::GetInstance()->AddObject_Prototype(TOOL_STATIC, L"Terrain", CPlaneTerrain::Create(m_pGraphicDevice))))
	//	return E_FAIL;

	//if (FAILED(CObject_Manager::GetInstance()->AddObject_Prototype(TOOL_STATIC, L"Cube_Tile", CTileCube::Create(m_pGraphicDevice))))
	//	return E_FAIL;

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
