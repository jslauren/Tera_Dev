
// MapTool_Client_Ver.h : MapTool_Client_Ver ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.


// CMapTool_Client_VerApp:
// �� Ŭ������ ������ ���ؼ��� MapTool_Client_Ver.cpp�� �����Ͻʽÿ�.
//

class CMapTool_Client_VerApp : public CWinAppEx
{
public:
	CMapTool_Client_VerApp();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// �����Դϴ�.
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnIdle(LONG lCount);
};

extern CMapTool_Client_VerApp theApp;
