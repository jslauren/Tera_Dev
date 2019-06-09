#pragma once
#include "afxwin.h"

// CTerrain_Tab 대화 상자입니다.

class CScene_Terrain;
class CTerrain_Tab : public CDialogEx
{
	DECLARE_DYNAMIC(CTerrain_Tab)

public:
	CTerrain_Tab(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CTerrain_Tab();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TERRAINTAB };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	CScene_Terrain* m_pScene = nullptr;

};
