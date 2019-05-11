
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


// CMapToolEZView

IMPLEMENT_DYNCREATE(CMapToolEZView, CView)

BEGIN_MESSAGE_MAP(CMapToolEZView, CView)
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CMapToolEZView ����/�Ҹ�

CMapToolEZView::CMapToolEZView()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}

CMapToolEZView::~CMapToolEZView()
{
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
