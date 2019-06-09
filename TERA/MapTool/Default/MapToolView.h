
// MapToolView.h : CMapToolView Ŭ������ �������̽�
//

#pragma once
#include "Graphic_Device.h"

_BEGIN(Engine)
class CRenderer;
class CManagement;
class CBase;
_END

class CViewManager;
class CMapToolDoc;
class CMapToolView : public CView
{
protected: // serialization������ ��������ϴ�.
	CMapToolView();
	DECLARE_DYNCREATE(CMapToolView)

// Ư���Դϴ�.
public:
	CMapToolDoc* GetDocument() const;

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

private:
	HRESULT InitDefaultSetting(CGraphic_Device::WINMODE eType, const _uint& iWinCX, const _uint& iWinCY);
	HRESULT InitRenderState();
	HRESULT InitComponentPrototype();
	HRESULT InitObjectPrototype();

public:
	LPDIRECT3DDEVICE9			m_pGraphicDevice = nullptr;

private:
	CManagement*				m_pManagement = nullptr;
	CRenderer*					m_pRenderer = nullptr;
	CViewManager*				m_pViewManager = nullptr;
	//CDataManager*				m_pDataManager = nullptr;

private:
	_int	m_iPairIndex=0;

// �����Դϴ�.
public:
	virtual ~CMapToolView();
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
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};

#ifndef _DEBUG  // MapToolView.cpp�� ����� ����
inline CMapToolDoc* CMapToolView::GetDocument() const
   { return reinterpret_cast<CMapToolDoc*>(m_pDocument); }
#endif

