
// MapTool_Client_VerView.cpp : CMapTool_Client_VerView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "MapTool_Client_Ver.h"
#endif

#include "MapTool_Client_VerDoc.h"
#include "MapTool_Client_VerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMapTool_Client_VerView
HWND g_ToolhWnd;
IMPLEMENT_DYNCREATE(CMapTool_Client_VerView, CView)

BEGIN_MESSAGE_MAP(CMapTool_Client_VerView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CMapTool_Client_VerView 생성/소멸

CMapTool_Client_VerView::CMapTool_Client_VerView()
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CMapTool_Client_VerView::~CMapTool_Client_VerView()
{
}

BOOL CMapTool_Client_VerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CMapTool_Client_VerView 그리기

void CMapTool_Client_VerView::OnDraw(CDC* /*pDC*/)
{
	CMapTool_Client_VerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CMapTool_Client_VerView 인쇄

BOOL CMapTool_Client_VerView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CMapTool_Client_VerView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CMapTool_Client_VerView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CMapTool_Client_VerView 진단

#ifdef _DEBUG
void CMapTool_Client_VerView::AssertValid() const
{
	CView::AssertValid();
}

void CMapTool_Client_VerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMapTool_Client_VerDoc* CMapTool_Client_VerView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMapTool_Client_VerDoc)));
	return (CMapTool_Client_VerDoc*)m_pDocument;
}
#endif //_DEBUG


// CMapTool_Client_VerView 메시지 처리기


void CMapTool_Client_VerView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	g_ToolhWnd = m_hWnd;

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
}
