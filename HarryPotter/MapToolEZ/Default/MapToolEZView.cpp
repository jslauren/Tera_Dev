
// MapToolEZView.cpp : CMapToolEZView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "MapToolEZ.h"
#endif

#include "MapToolEZDoc.h"
#include "MapToolEZView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "ViewManagerTool.h"


// CMapToolEZView
HWND g_hWnd;
IMPLEMENT_DYNCREATE(CMapToolEZView, CView)

BEGIN_MESSAGE_MAP(CMapToolEZView, CView)
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_CREATE()
END_MESSAGE_MAP()

// CMapToolEZView ����/�Ҹ�

CMapToolEZView::CMapToolEZView()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	m_pTimer_Manager = CTimer_Manager::GetInstance();
}

CMapToolEZView::~CMapToolEZView()
{
	Safe_Release(m_pTimer_Manager);
	Safe_Release(m_pMainAppTool);
}

BOOL CMapToolEZView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CMapToolEZView �׸���

void CMapToolEZView::OnDraw(CDC* /*pDC*/)
{
	CMapToolEZDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
}


// CMapToolEZView �μ�

BOOL CMapToolEZView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CMapToolEZView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CMapToolEZView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}

HRESULT CMapToolEZView::MainLoop()
{
	_float	fTimeDelta_Default = m_pTimer_Manager->Compute_TimeDelta(L"Timer_Default");

	fTimeAcc += fTimeDelta_Default;

	// For.Rate60
	if (fTimeAcc >= g_fRate_60)
	{
		_float	fTimeDelta_60 = m_pTimer_Manager->Compute_TimeDelta(L"Timer_60");

		if (FAILED(m_pMainAppTool->Update_MainApp(fTimeDelta_60) & 0x80000000))
			return E_FAIL;

		if (FAILED(m_pMainAppTool->Render_MainApp()))
			return E_FAIL;

		fTimeAcc = 0.f;
	}

	return NOERROR;
}


// CMapToolEZView ����

#ifdef _DEBUG
void CMapToolEZView::AssertValid() const
{
	CView::AssertValid();
}

void CMapToolEZView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMapToolEZDoc* CMapToolEZView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMapToolEZDoc)));
	return (CMapToolEZDoc*)m_pDocument;
}
#endif //_DEBUG


// CMapToolEZView �޽��� ó����


void CMapToolEZView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	CViewManagerTool::GetInstance()->m_pMapToolEZView = this;

	m_pViewManager = CViewManagerTool::GetInstance();

	g_hWnd = m_hWnd;

	// m_pMainAppTool ���� ���� //
	m_pMainAppTool = MapTool::CMainAppTool::Create();
	if (nullptr == m_pMainAppTool)
		return;
	//////////////////////////////

	// Timer ���� ���� //
	CTimer_Manager* pTimer_Manager = CTimer_Manager::GetInstance();

	pTimer_Manager->AddRef();

	if (FAILED(pTimer_Manager->Add_Timer(L"Timer_Default")))
		return;
	if (FAILED(pTimer_Manager->Add_Timer(L"Timer_60")))
		return;

	/////////////////////

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
}


int CMapToolEZView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  ���⿡ Ư��ȭ�� �ۼ� �ڵ带 �߰��մϴ�.

	return 0;
}
