#pragma once
#include "afxwin.h"

// CProtoTypeTab 대화 상자입니다.

class CScene_Proto;
class CProtoTypeTab : public CDialogEx
{
	DECLARE_DYNAMIC(CProtoTypeTab)

public:
	CProtoTypeTab(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CProtoTypeTab();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_IDD_PROTOTAB };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	CScene_Proto* m_pScene = nullptr;
};
