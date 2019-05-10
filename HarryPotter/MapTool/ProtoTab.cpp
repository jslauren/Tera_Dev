// ProtoTab.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MapTool.h"
#include "ProtoTab.h"
#include "afxdialogex.h"
#include "SceneProto.h"

// CProtoTab 대화 상자입니다.

IMPLEMENT_DYNAMIC(CProtoTab, CDialogEx)

CProtoTab::CProtoTab(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PROTOTAB, pParent)
{

}

CProtoTab::~CProtoTab()
{
}

void CProtoTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CProtoTab, CDialogEx)
END_MESSAGE_MAP()


// CProtoTab 메시지 처리기입니다.
