// Effect_Tab.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MapToolEZ.h"
#include "Effect_Tab.h"
#include "afxdialogex.h"
#include "Scene_Effect.h"


// CEffect_Tab 대화 상자입니다.

IMPLEMENT_DYNAMIC(CEffect_Tab, CDialogEx)

CEffect_Tab::CEffect_Tab(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_EFFECTTAB, pParent)
{

}

CEffect_Tab::~CEffect_Tab()
{
}

void CEffect_Tab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CEffect_Tab, CDialogEx)
END_MESSAGE_MAP()


// CEffect_Tab 메시지 처리기입니다.
