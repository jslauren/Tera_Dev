// CameraTab.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MapTool.h"
#include "CameraTab.h"
#include "afxdialogex.h"
#include "SceneCamera.h"

// CCameraTab ��ȭ �����Դϴ�.

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


// CCameraTab �޽��� ó�����Դϴ�.
