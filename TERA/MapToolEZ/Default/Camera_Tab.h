#pragma once
#include "afxwin.h"

// CCamera_Tab 대화 상자입니다.

class CScene_Camera;
class CCamera_Tab : public CDialogEx
{
	DECLARE_DYNAMIC(CCamera_Tab)

public:
	CCamera_Tab(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CCamera_Tab();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CAMERATAB };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	CScene_Camera* m_pScene = nullptr;

};
