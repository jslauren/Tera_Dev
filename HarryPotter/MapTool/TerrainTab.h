#pragma once


// CTerrainTab ��ȭ �����Դϴ�.
#include "afxcmn.h"
#include "Tool_Defines.h"

class CTerrainTab : public CDialog
{
	DECLARE_DYNAMIC(CTerrainTab)

public:
	CTerrainTab(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CTerrainTab();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TERRAINTAB };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};
