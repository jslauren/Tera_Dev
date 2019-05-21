#pragma once
#include "afxcmn.h"
#include "Tool_Defines.h"
#include "afxwin.h"
#include "Scene.h"

// CMeshTab ��ȭ �����Դϴ�.
class CMeshTab : public CDialogEx
{
	DECLARE_DYNAMIC(CMeshTab)

public:
	CMeshTab(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CMeshTab();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MESHTAB };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:
	CScene*	m_pScene = nullptr;

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();

	CButton Mesh_RenderST_Solild;
	CButton Mesh_MouseSelType_Object;
	CButton Mesh_ObjectType_Static;
	CButton Mesh_NaviVtxMove_Together;

	afx_msg void OnBnClickedSolid();
	afx_msg void OnBnClickedWireFrame();
	afx_msg void OnBnClickedObject();
	afx_msg void OnBnClickedNavi_Mesh();
	afx_msg void OnBnClickedStatic();
	afx_msg void OnBnClickedDynamic();
	afx_msg void OnBnClickedTogether();
	afx_msg void OnBnClickedSelected();

	CTreeCtrl Tree_Mesh_Object;

private:
	void InitTreeCtrl_Object();

public:
	afx_msg void OnTree_Mesh_Object(NMHDR *pNMHDR, LRESULT *pResult);
};
