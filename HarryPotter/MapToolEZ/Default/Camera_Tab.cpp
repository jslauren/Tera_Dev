// Camera_Tab.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MapToolEZ.h"
#include "Camera_Tab.h"
#include "afxdialogex.h"


// CCamera_Tab 대화 상자입니다.

IMPLEMENT_DYNAMIC(CCamera_Tab, CDialogEx)

CCamera_Tab::CCamera_Tab(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CAMERATAB, pParent)
{

}

CCamera_Tab::~CCamera_Tab()
{
}

void CCamera_Tab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCamera_Tab, CDialogEx)
END_MESSAGE_MAP()


// CCamera_Tab 메시지 처리기입니다.
