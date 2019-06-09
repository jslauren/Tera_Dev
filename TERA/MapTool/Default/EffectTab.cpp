// EffectTab.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MapTool.h"
#include "EffectTab.h"
#include "afxdialogex.h"
#include "SceneEffect.h"

// CEffectTab 대화 상자입니다.

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


// CEffectTab 메시지 처리기입니다.
