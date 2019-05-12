
// MapToolEZView.cpp : CMapToolEZView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
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
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_CREATE()
END_MESSAGE_MAP()

// CMapToolEZView 생성/소멸

CMapToolEZView::CMapToolEZView()
{
	// TODO: 여기에 생성 코드를 추가합니다.
	m_pTimer_Manager = CTimer_Manager::GetInstance();
}

CMapToolEZView::~CMapToolEZView()
{
	Safe_Release(m_pTimer_Manager);
	Safe_Release(m_pMainAppTool);
}

BOOL CMapToolEZView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CMapToolEZView 그리기

void CMapToolEZView::OnDraw(CDC* /*pDC*/)
{
	CMapToolEZDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CMapToolEZView 인쇄

BOOL CMapToolEZView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CMapToolEZView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CMapToolEZView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
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


// CMapToolEZView 진단

#ifdef _DEBUG
void CMapToolEZView::AssertValid() const
{
	CView::AssertValid();
}

void CMapToolEZView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMapToolEZDoc* CMapToolEZView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMapToolEZDoc)));
	return (CMapToolEZDoc*)m_pDocument;
}
#endif //_DEBUG


// CMapToolEZView 메시지 처리기


void CMapToolEZView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	CViewManagerTool::GetInstance()->m_pMapToolEZView = this;

	m_pViewManager = CViewManagerTool::GetInstance();

	g_hWnd = m_hWnd;

	// m_pMainAppTool 생성 구문 //
	m_pMainAppTool = MapTool::CMainAppTool::Create();
	if (nullptr == m_pMainAppTool)
		return;
	//////////////////////////////

	// Timer 생성 구문 //
	CTimer_Manager* pTimer_Manager = CTimer_Manager::GetInstance();

	pTimer_Manager->AddRef();

	if (FAILED(pTimer_Manager->Add_Timer(L"Timer_Default")))
		return;
	if (FAILED(pTimer_Manager->Add_Timer(L"Timer_60")))
		return;

	/////////////////////

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
}


int CMapToolEZView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	return 0;
}
