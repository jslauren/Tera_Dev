
// MainFrm.h : CMainFrame Ŭ������ �������̽�
//

#pragma once
#include "MainAppTool.h"
#include "Timer_Manager.h"

class CMainFrame : public CFrameWnd
{
	
protected: // serialization������ ��������ϴ�.
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Ư���Դϴ�.
public:

// �۾��Դϴ�.
public:


// �������Դϴ�.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// �����Դϴ�.
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	void				SetStatusBar(int idx, CString _data);
	HRESULT				MainLoop();

protected:  // ��Ʈ�� ������ ���Ե� ����Դϴ�.
	CToolBar			m_wndToolBar;
	CStatusBar			m_wndStatusBar;

private:
	CSplitterWnd			m_MainSplitter;
	MapTool::CMainAppTool*	m_pMainAppTool = nullptr;
	CTimer_Manager*			m_pTimer_Manager = nullptr;

private:
	_float					fTimeAcc = 0.f;

// ������ �޽��� �� �Լ�
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
};


