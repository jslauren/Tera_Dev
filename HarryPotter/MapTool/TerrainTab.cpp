// TerrainTab.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MapTool.h"
#include "TerrainTab.h"
#include "afxdialogex.h"


// CTerrainTab 대화 상자입니다.

IMPLEMENT_DYNAMIC(CTerrainTab, CDialog)

CTerrainTab::CTerrainTab(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_TERRAINTAB, pParent)
{

}

CTerrainTab::~CTerrainTab()
{
}

void CTerrainTab::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTerrainTab, CDialog)
END_MESSAGE_MAP()


// CTerrainTab 메시지 처리기입니다.
