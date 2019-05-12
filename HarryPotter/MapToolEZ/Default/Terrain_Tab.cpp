// Terrain_Tab.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MapToolEZ.h"
#include "Terrain_Tab.h"
#include "afxdialogex.h"


// CTerrain_Tab 대화 상자입니다.

IMPLEMENT_DYNAMIC(CTerrain_Tab, CDialogEx)

CTerrain_Tab::CTerrain_Tab(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TERRAINTAB, pParent)
{

}

CTerrain_Tab::~CTerrain_Tab()
{
}

void CTerrain_Tab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTerrain_Tab, CDialogEx)
END_MESSAGE_MAP()


// CTerrain_Tab 메시지 처리기입니다.
