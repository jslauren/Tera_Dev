// TerrainTab.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MapTool.h"
#include "TerrainTab.h"
#include "afxdialogex.h"
#include "SceneTerrain.h"

// CTerrainTab 대화 상자입니다.

IMPLEMENT_DYNAMIC(CTerrainTab, CDialogEx)

CTerrainTab::CTerrainTab(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TERRAINTAB, pParent)
{

}

CTerrainTab::~CTerrainTab()
{
}

void CTerrainTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTerrainTab, CDialogEx)
END_MESSAGE_MAP()


// CTerrainTab 메시지 처리기입니다.
