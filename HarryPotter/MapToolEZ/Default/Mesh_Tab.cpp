// Mesh_Tab.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MapToolEZ.h"
#include "Mesh_Tab.h"
#include "afxdialogex.h"
#include "Scene_Mesh.h"

// CMesh_Tab ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CMesh_Tab, CDialogEx)

CMesh_Tab::CMesh_Tab(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MESHTAB, pParent)
{

}

CMesh_Tab::~CMesh_Tab()
{
}

void CMesh_Tab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMesh_Tab, CDialogEx)
END_MESSAGE_MAP()


// CMesh_Tab �޽��� ó�����Դϴ�.
