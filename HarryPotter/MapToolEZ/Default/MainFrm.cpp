
// MainFrm.cpp : CMainFrame 클래스의 구현
//

#include "stdafx.h"
#include "MapToolEZ.h"

#include "MainFrm.h"
#include "ViewManagerTool.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
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
	m_pTimer_Manager = CTimer_Manager::GetInstance();
}

CMainFrame::~CMainFrame()
{
	Safe_Release(m_pTimer_Manager);
	Safe_Release(m_pMainAppTool);
}

void CMainFrame::SetStatusBar(int idx, CString _data)
{
	m_wndStatusBar.SetPaneText(idx, _data);
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// m_pMainAppTool 생성 구문 //
	m_pMainAppTool = MapTool::CMainAppTool::Create();
	if (nullptr == m_pMainAppTool)
		return -1;
	//////////////////////////////

	// Timer 생성 구문 //
	CTimer_Manager* pTimer_Manager = CTimer_Manager::GetInstance();
	
	// 이거 하니까 릭 남음.
	// pTimer_Manager->AddRef();

	if (FAILED(pTimer_Manager->Add_Timer(L"Timer_Default")))
		return FALSE;
	if (FAILED(pTimer_Manager->Add_Timer(L"Timer_60")))
		return FALSE;
	/////////////////////



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

HRESULT CMainFrame::MainLoop()
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

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
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

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	m_MainSplitter.CreateStatic(this, 1, 2);
	m_MainSplitter.CreateView(0, 0, RUNTIME_CLASS(CMapToolEZView), CSize(800, 600), pContext);
	m_MainSplitter.CreateView(0, 1, RUNTIME_CLASS(CModifyView), CSize(400, 600), pContext);

	CViewManagerTool::GetInstance()->m_pMainFrame = this;

	return TRUE;
}

// CMainFrame 진단

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame 메시지 처리기



