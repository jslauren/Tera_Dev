#pragma once
#include "afxwin.h"

// CEffect_Tab ��ȭ �����Դϴ�.

class CScene_Effect;
class CEffect_Tab : public CDialogEx
{
	DECLARE_DYNAMIC(CEffect_Tab)

public:
	CEffect_Tab(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CEffect_Tab();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EFFECTTAB };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:
	CScene_Effect* m_pScene = nullptr;
};
