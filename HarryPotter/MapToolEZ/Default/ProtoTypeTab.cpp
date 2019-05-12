// ProtoTypeTab.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MapToolEZ.h"
#include "ProtoTypeTab.h"
#include "afxdialogex.h"
#include "Scene_Proto.h"

// CProtoTypeTab 대화 상자입니다.

IMPLEMENT_DYNAMIC(CProtoTypeTab, CDialogEx)

CProtoTypeTab::CProtoTypeTab(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PROTOTAB, pParent)
{

}

CProtoTypeTab::~CProtoTypeTab()
{
}

void CProtoTypeTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CProtoTypeTab, CDialogEx)
END_MESSAGE_MAP()


// CProtoTypeTab 메시지 처리기입니다.
