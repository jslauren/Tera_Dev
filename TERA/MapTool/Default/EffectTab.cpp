// EffectTab.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MapTool.h"
#include "EffectTab.h"
#include "afxdialogex.h"
#include "SceneEffect.h"

// CEffectTab ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CEffectTab, CDialogEx)

CEffectTab::CEffectTab(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_EFFECTTAB, pParent)
{

}

CEffectTab::~CEffectTab()
{
}

void CEffectTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CEffectTab, CDialogEx)
END_MESSAGE_MAP()


// CEffectTab �޽��� ó�����Դϴ�.
