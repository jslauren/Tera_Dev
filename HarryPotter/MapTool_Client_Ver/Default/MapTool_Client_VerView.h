
// MapTool_Client_VerView.h : CMapTool_Client_VerView 클래스의 인터페이스
//

#pragma once


class CMapTool_Client_VerView : public CView
{
protected: // serialization에서만 만들어집니다.
	CMapTool_Client_VerView();
	DECLARE_DYNCREATE(CMapTool_Client_VerView)

// 특성입니다.
public:
	CMapTool_Client_VerDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CMapTool_Client_VerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
};

#ifndef _DEBUG  // MapTool_Client_VerView.cpp의 디버그 버전
inline CMapTool_Client_VerDoc* CMapTool_Client_VerView::GetDocument() const
   { return reinterpret_cast<CMapTool_Client_VerDoc*>(m_pDocument); }
#endif

