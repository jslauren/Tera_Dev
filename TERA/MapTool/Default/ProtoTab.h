#pragma once
#include "afxwin.h"

// CProtoTab ��ȭ �����Դϴ�.

class CSceneProto;
class CProtoTab : public CDialogEx
{
	DECLARE_DYNAMIC(CProtoTab)

public:
	CProtoTab(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CProtoTab();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROTOTAB };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:
	CSceneProto* m_pScene = nullptr;

};
