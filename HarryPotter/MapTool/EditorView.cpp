// EditorView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MapTool.h"
#include "EditorView.h"


// CEditorView

IMPLEMENT_DYNCREATE(CEditorView, CFormView)

#include "ViewManager.h"

CEditorView::CEditorView()
	: CFormView(IDD_EDITORVIEW)
{

}

CEditorView::~CEditorView()
{
}

void CEditorView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_Tab);
}

BEGIN_MESSAGE_MAP(CEditorView, CFormView)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CEditorView::OnSelchangeTab)
END_MESSAGE_MAP()


// CEditorView 진단입니다.

#ifdef _DEBUG
void CEditorView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CEditorView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CEditorView 메시지 처리기입니다.


void CEditorView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	m_pViewManager = CViewManager::GetInstance();
	m_pViewManager->m_pEditorView = this;

	m_Tab.InsertItem(0, L"Terrain");
	m_Tab.InsertItem(1, L"Prototype");

	CRect rect;
	m_Tab.GetWindowRect(&rect);

	m_Tab_Terrain.Create(IDD_TERRAINTAB, &m_Tab);
	m_Tab_Terrain.MoveWindow(0, 25, rect.Width(), rect.Height());
	m_Tab_Terrain.ShowWindow(SW_SHOW);


	m_Tab_Proto.Create(IDD_PROTOTAB, &m_Tab);
	m_Tab_Proto.MoveWindow(0, 25, rect.Width(), rect.Height());
	m_Tab_Proto.ShowWindow(SW_HIDE);

	m_Tab.SetCurSel(0);
	m_pViewManager->SetCurScene(TOOL_TERRAIN);

	SetScrollSizes(MM_TEXT, CSize(0, 0));
}


void CEditorView::OnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	int sel = m_Tab.GetCurSel();

	switch (sel)
	{
	case 0:
		m_Tab_Terrain.ShowWindow(SW_SHOW);
		m_Tab_Proto.ShowWindow(SW_HIDE);
		m_pViewManager->SetCurScene(TOOL_TERRAIN);
		break;

	case 1:
		m_Tab_Terrain.ShowWindow(SW_HIDE);
		m_Tab_Proto.ShowWindow(SW_SHOW);
		m_pViewManager->SetCurScene(TOOL_PROTO);
		break;
	}

	*pResult = 0;
}
