#pragma once
#include "afxwin.h"

// CEffect_Tab 대화 상자입니다.

class CEffect_Tab : public CDialogEx
{
	DECLARE_DYNAMIC(CEffect_Tab)

public:
	CEffect_Tab(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CEffect_Tab();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EFFECTTAB };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
