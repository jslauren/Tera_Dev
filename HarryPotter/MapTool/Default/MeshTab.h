#pragma once
#include "afxcmn.h"
#include "Tool_Defines.h"

// CMeshTab ��ȭ �����Դϴ�.
class CSceneMesh;
class CMeshTab : public CDialogEx
{
	DECLARE_DYNAMIC(CMeshTab)

public:
	CMeshTab(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CMeshTab();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MESHTAB };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:
	CSceneMesh*	m_pScene = nullptr;

};
