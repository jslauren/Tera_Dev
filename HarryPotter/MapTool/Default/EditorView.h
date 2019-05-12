#pragma once
#include "afxcmn.h"
#include "EditorView.h"
#include "TerrainTab.h"
#include "MeshTab.h"
#include "CameraTab.h"
#include "EffectTab.h"
#include "ProtoTab.h"

// CEditorView �� ���Դϴ�.
class CViewManager;
class CEditorView : public CFormView
{
	DECLARE_DYNCREATE(CEditorView)

protected:
	CEditorView();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CEditorView();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EDITORVIEW };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:
	CTabCtrl		m_Tab;
	CTerrainTab		m_Tab_Terrain;
	CMeshTab		m_Tab_Mesh;
	CCameraTab		m_Tab_Camera;
	CEffectTab		m_Tab_Effect;
	CProtoTab		m_Tab_Proto;

	CViewManager*	m_pViewManager;

public:
	virtual void OnInitialUpdate();

	afx_msg void OnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);

};


