// TerrainTab.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MapTool.h"
#include "TerrainTab.h"
#include "afxdialogex.h"
#include "SceneStatic.h"
#include "Component_Manager.h"
#include "EventManagerTool.h"
#include "Object_Manager.h"
#include "Layer.h"
#include "Terrain.h"

// CTerrainTab ��ȭ �����Դϴ�.

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
	DDX_Control(pDX, IDC_RADIO1, Terrain_RenderST_Solild);
	DDX_Control(pDX, IDC_RADIO3, Terrain_RenderCood_Axis);
	DDX_Control(pDX, IDC_EDIT1, Vertex_X_Count);
	DDX_Control(pDX, IDC_EDIT2, Vertex_Z_Count);
	DDX_Control(pDX, IDC_EDIT3, Vertex_Interval);
	DDX_Control(pDX, IDC_EDIT4, Vertex_Detail);
	DDX_Control(pDX, IDC_SPIN1, Vertex_X_Count_Btn);
	DDX_Control(pDX, IDC_SPIN2, Vertex_Z_Count_Btn);
	DDX_Control(pDX, IDC_SPIN3, Vertex_Interval_Btn);
	DDX_Control(pDX, IDC_SPIN4, Vertex_Detail_Btn);
}


BEGIN_MESSAGE_MAP(CTerrainTab, CDialogEx)
	ON_BN_CLICKED(IDC_RADIO2, &CTerrainTab::OnBnClickedWireFrame)
	ON_BN_CLICKED(IDC_RADIO1, &CTerrainTab::OnBnClickedSolid)
	ON_BN_CLICKED(IDC_BUTTON1, &CTerrainTab::OnBnClickedTerrain_Apply)
END_MESSAGE_MAP()


// CTerrainTab �޽��� ó�����Դϴ�.


void CTerrainTab::OnOK()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	//CDialogEx::OnOK();

	return;
}


BOOL CTerrainTab::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	Terrain_RenderST_Solild.SetCheck(TRUE);
	Terrain_RenderCood_Axis.SetCheck(TRUE);
	

	CStringW	strNumVtxX;

	Vertex_X_Count_Btn.SetRange(0, 360);
	Vertex_X_Count_Btn.SetPos(0);

	Vertex_Z_Count_Btn.SetRange(0, 360);
	Vertex_Z_Count_Btn.SetPos(0);

	Vertex_Interval_Btn.SetRange(0, 360);
	Vertex_Interval_Btn.SetPos(0);

	Vertex_Detail_Btn.SetRange(0, 360);
	Vertex_Detail_Btn.SetPos(0);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CTerrainTab::OnBnClickedWireFrame()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CEventManagerTool::GetInstance()->m_bIsWireFrame = true;
}


void CTerrainTab::OnBnClickedSolid()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CEventManagerTool::GetInstance()->m_bIsWireFrame = false;
}

void CTerrainTab::OnBnClickedTerrain_Apply()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	CLayer* pLayer = CObject_Manager::GetInstance()->FindObjectLayer(SCENE_STATIC, L"Layer_BackGround");

	CStringW	strValue;

	Vertex_X_Count.GetWindowTextW(strValue);
	m_iNumVtxX = _ttoi(strValue);

	Vertex_Z_Count.GetWindowTextW(strValue);
	m_iNumVtxZ = _ttoi(strValue);

	Vertex_Interval.GetWindowTextW(strValue);
	m_fInterval = _ttoi(strValue);

	Vertex_Detail.GetWindowTextW(strValue);
	m_fDetail = _ttoi(strValue);

	dynamic_cast<CTerrain*>(pLayer->Get_ObjectList().back())->Reset_Terrain(m_iNumVtxX, m_iNumVtxZ, m_fInterval, m_fDetail);
}