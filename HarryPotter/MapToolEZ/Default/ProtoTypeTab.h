#pragma once
#include "afxwin.h"

// CProtoTypeTab ��ȭ �����Դϴ�.

class CScene_Proto;
class CProtoTypeTab : public CDialogEx
{
	DECLARE_DYNAMIC(CProtoTypeTab)

public:
	CProtoTypeTab(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CProtoTypeTab();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_IDD_PROTOTAB };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:
	CScene_Proto* m_pScene = nullptr;
};
