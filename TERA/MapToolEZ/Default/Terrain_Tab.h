#pragma once
#include "afxwin.h"

// CTerrain_Tab ��ȭ �����Դϴ�.

class CScene_Terrain;
class CTerrain_Tab : public CDialogEx
{
	DECLARE_DYNAMIC(CTerrain_Tab)

public:
	CTerrain_Tab(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CTerrain_Tab();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TERRAINTAB };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:
	CScene_Terrain* m_pScene = nullptr;

};
