
// MapToolEZView.h : CMapToolEZView Ŭ������ �������̽�
//

#pragma once
#include "MainAppTool.h"
#include "Timer_Manager.h"

class CMapToolEZDoc;
class CMapToolEZView : public CView
{
protected: // serialization������ ��������ϴ�.
	CMapToolEZView();
	DECLARE_DYNCREATE(CMapToolEZView)

// Ư���Դϴ�.
public:
	CMapToolEZDoc* GetDocument() const;

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

public:
	HRESULT				MainLoop();

private:
	MapTool::CMainAppTool*	m_pMainAppTool = nullptr;
	CTimer_Manager*			m_pTimer_Manager = nullptr;
	CViewManagerTool*		m_pViewManager = nullptr;

private:
	_float					fTimeAcc = 0.f;

// �����Դϴ�.
public:
	virtual ~CMapToolEZView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

};

#ifndef _DEBUG  // MapToolEZView.cpp�� ����� ����
inline CMapToolEZDoc* CMapToolEZView::GetDocument() const
   { return reinterpret_cast<CMapToolEZDoc*>(m_pDocument); }
#endif

