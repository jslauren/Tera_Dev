#pragma once
#include "afxcmn.h"
#include "ModifyView.h"
#include "Terrain_Tab.h"
#include "Mesh_Tab.h"
#include "Camera_Tab.h"
#include "Effect_Tab.h"
#include "ProtoType_Tab.h"

// CModifyView 폼 뷰입니다.
class CViewManagerTool;
class CModifyView : public CFormView
{
	DECLARE_DYNCREATE(CModifyView)

protected:
	CModifyView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CModifyView();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MODIFYVIEW };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl		m_Tab;
	CTerrain_Tab	m_Tab_Terrain;
	CMesh_Tab		m_Tab_Mesh;
	CCamera_Tab		m_Tab_Camera;
	CEffect_Tab		m_Tab_Effect;
	CProtoType_Tab	m_Tab_Proto;

	CViewManagerTool*	m_pViewManager;

	virtual void OnInitialUpdate();

	afx_msg void OnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);
};


