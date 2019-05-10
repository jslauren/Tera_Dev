
// MapTool_Client_VerView.cpp : CMapTool_Client_VerView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
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
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CMapTool_Client_VerView ����/�Ҹ�

CMapTool_Client_VerView::CMapTool_Client_VerView()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}

CMapTool_Client_VerView::~CMapTool_Client_VerView()
{
}

BOOL CMapTool_Client_VerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CMapTool_Client_VerView �׸���

void CMapTool_Client_VerView::OnDraw(CDC* /*pDC*/)
{
	CMapTool_Client_VerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
}


// CMapTool_Client_VerView �μ�

BOOL CMapTool_Client_VerView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CMapTool_Client_VerView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CMapTool_Client_VerView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}


// CMapTool_Client_VerView ����

#ifdef _DEBUG
void CMapTool_Client_VerView::AssertValid() const
{
	CView::AssertValid();
}

void CMapTool_Client_VerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMapTool_Client_VerDoc* CMapTool_Client_VerView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMapTool_Client_VerDoc)));
	return (CMapTool_Client_VerDoc*)m_pDocument;
}
#endif //_DEBUG


// CMapTool_Client_VerView �޽��� ó����


void CMapTool_Client_VerView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	g_ToolhWnd = m_hWnd;

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
}
