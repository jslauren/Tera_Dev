#pragma once
#include "afxwin.h"

// CMesh_Tab ��ȭ �����Դϴ�.

class CScene_Mesh;
class CMesh_Tab : public CDialogEx
{
	DECLARE_DYNAMIC(CMesh_Tab)

public:
	CMesh_Tab(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CMesh_Tab();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MESHTAB };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:
	CScene_Mesh* m_pScene = nullptr;

};
