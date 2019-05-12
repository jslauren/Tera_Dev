// ModifyView.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MapToolEZ.h"
#include "ModifyView.h"


// CModifyView

IMPLEMENT_DYNCREATE(CModifyView, CFormView)

#include "ViewManagerTool.h"

CModifyView::CModifyView()
	: CFormView(IDD_MODIFYVIEW)
{

}

CModifyView::~CModifyView()
{
}

void CModifyView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_Tab);
}

BEGIN_MESSAGE_MAP(CModifyView, CFormView)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CModifyView::OnSelchangeTab)
END_MESSAGE_MAP()


// CModifyView �����Դϴ�.

#ifdef _DEBUG
void CModifyView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CModifyView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CModifyView �޽��� ó�����Դϴ�.


void CModifyView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	m_pViewManager = CViewManagerTool::GetInstance();
	m_pViewManager->m_pModifyView = this;

	m_Tab.InsertItem(1, L"Prototype");
//	m_Tab.InsertItem(1, L"Prototype");

	CRect rect;
	m_Tab.GetWindowRect(&rect);

	m_Tab_Proto.Create(IDD_PROTOTAB, &m_Tab);
	m_Tab_Proto.MoveWindow(0, 25, rect.Width(), rect.Height());
	m_Tab_Proto.ShowWindow(SW_SHOW);


	//m_Tab_Proto.Create(IDD_PROTOTAB, &m_Tab);
	//m_Tab_Proto.MoveWindow(0, 25, rect.Width(), rect.Height());
	//m_Tab_Proto.ShowWindow(SW_HIDE);

	m_Tab.SetCurSel(1);
	m_pViewManager->SetCurScene(SCENE_PROTO);

	SetScrollSizes(MM_TEXT, CSize(0, 0));
}


void CModifyView::OnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;
}
