
// MapToolEZ.h : MapToolEZ ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.


// CMapToolEZApp:
// �� Ŭ������ ������ ���ؼ��� MapToolEZ.cpp�� �����Ͻʽÿ�.
//

class CMapToolEZApp : public CWinAppEx
{
public:
	CMapToolEZApp();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// �����Դϴ�.
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMapToolEZApp theApp;
