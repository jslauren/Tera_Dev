#pragma once
#include "afxwin.h"

// CCamera_Tab ��ȭ �����Դϴ�.

class CScene_Camera;
class CCamera_Tab : public CDialogEx
{
	DECLARE_DYNAMIC(CCamera_Tab)

public:
	CCamera_Tab(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CCamera_Tab();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CAMERATAB };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:
	CScene_Camera* m_pScene = nullptr;

};
