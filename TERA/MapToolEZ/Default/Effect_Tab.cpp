// Effect_Tab.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MapToolEZ.h"
#include "Effect_Tab.h"
#include "afxdialogex.h"
#include "Scene_Effect.h"


// CEffect_Tab ��ȭ �����Դϴ�.

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


// CEffect_Tab �޽��� ó�����Դϴ�.
