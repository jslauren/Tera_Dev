// TerrainTab.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MapTool.h"
#include "TerrainTab.h"
#include "afxdialogex.h"


// CTerrainTab ��ȭ �����Դϴ�.

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


// CTerrainTab �޽��� ó�����Դϴ�.
