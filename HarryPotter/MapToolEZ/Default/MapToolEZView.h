
// MapToolEZView.h : CMapToolEZView Ŭ������ �������̽�
//

#pragma once


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
};

#ifndef _DEBUG  // MapToolEZView.cpp�� ����� ����
inline CMapToolEZDoc* CMapToolEZView::GetDocument() const
   { return reinterpret_cast<CMapToolEZDoc*>(m_pDocument); }
#endif

