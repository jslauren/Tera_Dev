// CameraTab.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MapTool.h"
#include "CameraTab.h"
#include "afxdialogex.h"
#include "SceneCamera.h"

// CCameraTab 대화 상자입니다.

IMPLEMENT_DYNAMIC(CCameraTab, CDialogEx)

CCameraTab::CCameraTab(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CAMERATAB, pParent)
{

}

CCameraTab::~CCameraTab()
{
}

void CCameraTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCameraTab, CDialogEx)
END_MESSAGE_MAP()


// CCameraTab 메시지 처리기입니다.
