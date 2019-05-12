
// MainFrm.cpp : CMainFrame 클래스의 구현
//

#include "stdafx.h"
#include "MapTool.h"

#include "MainFrm.h"
#include "ViewManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CSplitterWnd)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CSplitterWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	IDS_INDEX,
	IDS_MOUSEPT
};

// CMainFrame 생성/소멸

CMainFrame::CMainFrame()
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
}

CMainFrame::~CMainFrame()
{
}

void CMainFrame::SetStatusBar(int idx, CString _data)
{
	m_wndStatusBar.SetPaneText(idx, _data);
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CSplitterWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("상태 표시줄을 만들지 못했습니다.\n");
		return -1;      // 만들지 못했습니다.
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));
	
	m_wndStatusBar.SetPaneInfo(0, IDS_MOUSEPT, SBPS_NORMAL, 200);
	m_wndStatusBar.SetPaneInfo(1, IDS_INDEX, SBPS_NORMAL, 200);

	m_wndStatusBar.SetPaneText(1, L"MousePos");
	
	return 0;
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	m_MainSplitter.CreateStatic(this, 1, 2);
	m_MainSplitter.CreateView(0, 0, RUNTIME_CLASS(CMapToolView), CSize(800, 600), pContext);
	m_MainSplitter.CreateView(0, 1, RUNTIME_CLASS(CEditorView), CSize(400, 600), pContext);

	CViewManager::GetInstance()->m_pMainFrame = this;

	return TRUE;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CSplitterWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	cs.style &= ~(WS_THICKFRAME | WS_MAXIMIZE);
	//cs.cx = 1600;
	//cs.cy = 900;
	cs.cx = 1200;
	cs.cy = 600;

	return TRUE;
}

// CMainFrame 진단

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CSplitterWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CSplitterWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 메시지 처리기