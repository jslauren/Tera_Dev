
// MapTool_Client_VerView.h : CMapTool_Client_VerView Ŭ������ �������̽�
//

#pragma once


class CMapTool_Client_VerView : public CView
{
protected: // serialization������ ��������ϴ�.
	CMapTool_Client_VerView();
	DECLARE_DYNCREATE(CMapTool_Client_VerView)

// Ư���Դϴ�.
public:
	CMapTool_Client_VerDoc* GetDocument() const;

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

// �����Դϴ�.
public:
	virtual ~CMapTool_Client_VerView();
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
};

#ifndef _DEBUG  // MapTool_Client_VerView.cpp�� ����� ����
inline CMapTool_Client_VerDoc* CMapTool_Client_VerView::GetDocument() const
   { return reinterpret_cast<CMapTool_Client_VerDoc*>(m_pDocument); }
#endif

