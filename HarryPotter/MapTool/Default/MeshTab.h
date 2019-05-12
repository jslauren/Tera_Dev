#pragma once
#include "afxcmn.h"
#include "Tool_Defines.h"

// CMeshTab 대화 상자입니다.
class CSceneMesh;
class CMeshTab : public CDialogEx
{
	DECLARE_DYNAMIC(CMeshTab)

public:
	CMeshTab(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CMeshTab();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MESHTAB };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	CSceneMesh*	m_pScene = nullptr;

};
