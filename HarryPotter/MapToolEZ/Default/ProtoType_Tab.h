#pragma once
#include "afxwin.h"

// CProtoType_Tab ��ȭ �����Դϴ�.

class CScene_Proto;
class CProtoType_Tab : public CDialogEx
{
	DECLARE_DYNAMIC(CProtoType_Tab)

public:
	CProtoType_Tab(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CProtoType_Tab();

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
