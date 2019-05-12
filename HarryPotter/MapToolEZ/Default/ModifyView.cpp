// ModifyView.cpp : 구현 파일입니다.
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


// CModifyView 진단입니다.

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


// CModifyView 메시지 처리기입니다.


void CModifyView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	m_pViewManager = CViewManagerTool::GetInstance();
	m_pViewManager->m_pModifyView = this;

	m_Tab.InsertItem(0, L"Terrain");
	m_Tab.InsertItem(1, L"Mesh");
	m_Tab.InsertItem(2, L"Camera");
	m_Tab.InsertItem(3, L"Effect");
	m_Tab.InsertItem(4, L"Prototype");

	CRect rect;
	m_Tab.GetWindowRect(&rect);

	m_Tab_Terrain.Create(IDD_TERRAINTAB, &m_Tab);
	m_Tab_Terrain.MoveWindow(0, 25, rect.Width(), rect.Height());
	m_Tab_Terrain.ShowWindow(SW_SHOW);

	m_Tab_Mesh.Create(IDD_MESHTAB, &m_Tab);
	m_Tab_Mesh.MoveWindow(0, 25, rect.Width(), rect.Height());
	m_Tab_Mesh.ShowWindow(SW_HIDE);

	m_Tab_Camera.Create(IDD_CAMERATAB, &m_Tab);
	m_Tab_Camera.MoveWindow(0, 25, rect.Width(), rect.Height());
	m_Tab_Camera.ShowWindow(SW_HIDE);

	m_Tab_Effect.Create(IDD_EFFECTTAB, &m_Tab);
	m_Tab_Effect.MoveWindow(0, 25, rect.Width(), rect.Height());
	m_Tab_Effect.ShowWindow(SW_HIDE);

	m_Tab_Proto.Create(IDD_PROTOTAB, &m_Tab);
	m_Tab_Proto.MoveWindow(0, 25, rect.Width(), rect.Height());
	m_Tab_Proto.ShowWindow(SW_HIDE);

	m_Tab.SetCurSel(0);
	m_pViewManager->SetCurScene(SCENE_TERRAIN);

	SetScrollSizes(MM_TEXT, CSize(0, 0));
}


void CModifyView::OnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	int sel = m_Tab.GetCurSel();

	switch (sel)
	{
	case 0:
		m_Tab_Terrain.ShowWindow(SW_SHOW);
		m_Tab_Mesh.ShowWindow(SW_HIDE);
		m_Tab_Camera.ShowWindow(SW_HIDE);
		m_Tab_Effect.ShowWindow(SW_HIDE);
		m_Tab_Proto.ShowWindow(SW_HIDE);
		m_pViewManager->SetCurScene(SCENE_TERRAIN);
		break;

	case 1:
		m_Tab_Terrain.ShowWindow(SW_HIDE);
		m_Tab_Mesh.ShowWindow(SW_SHOW);
		m_Tab_Camera.ShowWindow(SW_HIDE);
		m_Tab_Effect.ShowWindow(SW_HIDE);
		m_Tab_Proto.ShowWindow(SW_HIDE);
		m_pViewManager->SetCurScene(SCENE_MESH);
		break;

	case 2:
		m_Tab_Terrain.ShowWindow(SW_HIDE);
		m_Tab_Mesh.ShowWindow(SW_HIDE);
		m_Tab_Camera.ShowWindow(SW_SHOW);
		m_Tab_Effect.ShowWindow(SW_HIDE);
		m_Tab_Proto.ShowWindow(SW_HIDE);
		m_pViewManager->SetCurScene(SCENE_CAMERA);
		break;

	case 3:
		m_Tab_Terrain.ShowWindow(SW_HIDE);
		m_Tab_Mesh.ShowWindow(SW_HIDE);
		m_Tab_Camera.ShowWindow(SW_HIDE);
		m_Tab_Effect.ShowWindow(SW_SHOW);
		m_Tab_Proto.ShowWindow(SW_HIDE);
		m_pViewManager->SetCurScene(SCENE_EFFECT);
		break;

	case 4:
		m_Tab_Terrain.ShowWindow(SW_HIDE);
		m_Tab_Mesh.ShowWindow(SW_HIDE);
		m_Tab_Camera.ShowWindow(SW_HIDE);
		m_Tab_Effect.ShowWindow(SW_HIDE);
		m_Tab_Proto.ShowWindow(SW_SHOW);
		m_pViewManager->SetCurScene(SCENE_PROTO);
		break;
	}

	*pResult = 0;
}
