#pragma once
#include "afxwin.h"

// CMesh_Tab 대화 상자입니다.

class CScene_Mesh;
class CMesh_Tab : public CDialogEx
{
	DECLARE_DYNAMIC(CMesh_Tab)

public:
	CMesh_Tab(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CMesh_Tab();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MESHTAB };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	CScene_Mesh* m_pScene = nullptr;

};
