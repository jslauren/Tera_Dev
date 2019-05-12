// ProtoType_Tab.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MapToolEZ.h"
#include "ProtoType_Tab.h"
#include "afxdialogex.h"
#include "Scene_Proto.h"

// CProtoType_Tab 대화 상자입니다.

IMPLEMENT_DYNAMIC(CProtoType_Tab, CDialogEx)

CProtoType_Tab::CProtoType_Tab(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PROTOTAB, pParent)
{

}

CProtoType_Tab::~CProtoType_Tab()
{
}

void CProtoType_Tab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CProtoType_Tab, CDialogEx)
END_MESSAGE_MAP()


// CProtoType_Tab 메시지 처리기입니다.
