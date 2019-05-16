// TerrainTab.cpp : 구현 파일입니다.
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
	DDX_Control(pDX, IDC_EDIT5, PositionX);
	DDX_Control(pDX, IDC_EDIT6, PositionY);
	DDX_Control(pDX, IDC_EDIT7, PositionZ);
	DDX_Control(pDX, IDC_EDIT8, RotationX);
	DDX_Control(pDX, IDC_EDIT9, RotationY);
	DDX_Control(pDX, IDC_EDIT10, RotationZ);
	DDX_Control(pDX, IDC_SPIN5, PositionX_Btn);
	DDX_Control(pDX, IDC_SPIN6, PositionY_Btn);
	DDX_Control(pDX, IDC_SPIN7, PositionZ_Btn);
	DDX_Control(pDX, IDC_SPIN8, RotaionX_Btn);
	DDX_Control(pDX, IDC_SPIN9, RotaionY_Btn);
	DDX_Control(pDX, IDC_SPIN10, RotaionZ_Btn);
}


BEGIN_MESSAGE_MAP(CTerrainTab, CDialogEx)
	ON_BN_CLICKED(IDC_RADIO2, &CTerrainTab::OnBnClickedWireFrame)
	ON_BN_CLICKED(IDC_RADIO1, &CTerrainTab::OnBnClickedSolid)
	ON_BN_CLICKED(IDC_BUTTON1, &CTerrainTab::OnBnClickedTerrain_Apply)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN5, &CTerrainTab::OnSpin_Trans_PosX)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN6, &CTerrainTab::OnSpin_Trans_PosY)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN7, &CTerrainTab::OnSpin_Trans_PosZ)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN8, &CTerrainTab::OnSpin_Trans_RotX)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN9, &CTerrainTab::OnSpin_Trans_RotY)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN10, &CTerrainTab::OnSpin_Trans_RotZ)
END_MESSAGE_MAP()


// CTerrainTab 메시지 처리기입니다.


void CTerrainTab::OnOK()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	//CDialogEx::OnOK();

	return;
}


BOOL CTerrainTab::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

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

	PositionX_Btn.SetRange(-1000, 1000);
	PositionX_Btn.SetPos(0);

	PositionY_Btn.SetRange(-1000, 1000);
	PositionY_Btn.SetPos(0);

	PositionZ_Btn.SetRange(-1000, 1000);
	PositionZ_Btn.SetPos(0);

	RotaionX_Btn.SetRange(-360, 360);
	RotaionX_Btn.SetPos(0);

	RotaionY_Btn.SetRange(-360, 360);
	RotaionY_Btn.SetPos(0);

	RotaionZ_Btn.SetRange(-360, 360);
	RotaionZ_Btn.SetPos(0);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CTerrainTab::OnBnClickedWireFrame()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CEventManagerTool::GetInstance()->m_bIsWireFrame = true;
}


void CTerrainTab::OnBnClickedSolid()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CEventManagerTool::GetInstance()->m_bIsWireFrame = false;
}

void CTerrainTab::OnBnClickedTerrain_Apply()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CStringW	strValue;

	Vertex_X_Count.GetWindowTextW(strValue);
	m_iNumVtxX = _ttoi(strValue);

	Vertex_Z_Count.GetWindowTextW(strValue);
	m_iNumVtxZ = _ttoi(strValue);

	Vertex_Interval.GetWindowTextW(strValue);
	m_fInterval = _ttof(strValue);

	Vertex_Detail.GetWindowTextW(strValue);
	m_fDetail = _ttof(strValue);


	CLayer* pLayer = CObject_Manager::GetInstance()->FindObjectLayer(SCENE_STATIC, L"Layer_Terrain");
	dynamic_cast<CTerrain*>(pLayer->Get_ObjectList().back())->Reset_Terrain(m_iNumVtxX, m_iNumVtxZ, m_fInterval, m_fDetail);

}

void CTerrainTab::OnSpin_Trans_PosX(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	CStringW	strValue;

	PositionX.GetWindowTextW(strValue);
	m_iPosX = _ttoi(strValue);

	CLayer* pLayer = CObject_Manager::GetInstance()->FindObjectLayer(SCENE_STATIC, L"Layer_Terrain");
	dynamic_cast<CTerrain*>(pLayer->Get_ObjectList().back())->GetTransformCom()->Set_StateInfo(CTransform::STATE_POSITION, &_vec3((_float)m_iPosX, m_iPosY, m_iPosZ));

}

void CTerrainTab::OnSpin_Trans_PosY(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	CStringW	strValue;

	PositionY.GetWindowTextW(strValue);
	m_iPosY = _ttoi(strValue);

	CLayer* pLayer = CObject_Manager::GetInstance()->FindObjectLayer(SCENE_STATIC, L"Layer_Terrain");
	dynamic_cast<CTerrain*>(pLayer->Get_ObjectList().back())->GetTransformCom()->Set_StateInfo(CTransform::STATE_POSITION, &_vec3((_float)m_iPosX, m_iPosY, m_iPosZ));

}

void CTerrainTab::OnSpin_Trans_PosZ(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	CStringW	strValue;

	PositionZ.GetWindowTextW(strValue);
	m_iPosZ = _ttoi(strValue);

	CLayer* pLayer = CObject_Manager::GetInstance()->FindObjectLayer(SCENE_STATIC, L"Layer_Terrain");
	dynamic_cast<CTerrain*>(pLayer->Get_ObjectList().back())->GetTransformCom()->Set_StateInfo(CTransform::STATE_POSITION, &_vec3((_float)m_iPosX, m_iPosY, m_iPosZ));

}

void CTerrainTab::OnSpin_Trans_RotX(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	CStringW	strValue;

	RotationX.GetWindowTextW(strValue);
	m_fRotX = _ttof(strValue) + 1;

	CLayer* pLayer = CObject_Manager::GetInstance()->FindObjectLayer(SCENE_STATIC, L"Layer_Terrain");
	dynamic_cast<CTerrain*>(pLayer->Get_ObjectList().back())->GetTransformCom()->Rotation_Axis(_vec3(1.f, 0.f, 0.f), D3DXToRadian(m_fRotX), 0.1f);
	dynamic_cast<CTerrain*>(pLayer->Get_ObjectList().back())->GetTransformCom()->Set_Angle_Axis(_vec3(1.f, 0.f, 0.f), D3DXToRadian(m_fRotX));

}

void CTerrainTab::OnSpin_Trans_RotY(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	CStringW	strValue;

	RotationY.GetWindowTextW(strValue);
	m_fRotY = _ttoi(strValue) + 1;

	Rotation_Axis(_vec3(0.f, 1.f, 0.f), D3DXToRadian(m_fRotY));
}

void CTerrainTab::OnSpin_Trans_RotZ(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	CStringW	strValue;

	RotationZ.GetWindowTextW(strValue);
	m_fRotZ = _ttoi(strValue) + 1;

	Rotation_Axis(_vec3(0.f, 0.f, 1.f), D3DXToRadian(m_fRotZ));
}

void CTerrainTab::Rotation_Axis(const _vec3 & vAxis, const _float & fRadianPerSec)
{
	CLayer* pLayer = CObject_Manager::GetInstance()->FindObjectLayer(SCENE_STATIC, L"Layer_Terrain");

	_matrix matWorld = *(dynamic_cast<CTerrain*>(pLayer->Get_ObjectList().back())->GetTransformCom()->Get_WorldMatrixPointer());

	D3DXMatrixRotationAxis(&matWorld, &vAxis, fRadianPerSec);

	_vec3	vRight(1.f, 0.f, 0.f), vUp(0.f, 1.f, 0.f), vLook(0.f, 0.f, 1.f);

	vRight *= D3DXVec3Length((_vec3*)&matWorld.m[Engine::CTransform::STATE_RIGHT][0]);
	D3DXVec3TransformNormal(&vRight, &vRight, &matWorld);

	vUp *= D3DXVec3Length((_vec3*)&matWorld.m[Engine::CTransform::STATE_UP][0]);
	D3DXVec3TransformNormal(&vUp, &vUp, &matWorld);

	vLook *= D3DXVec3Length((_vec3*)&matWorld.m[Engine::CTransform::STATE_LOOK][0]);
	D3DXVec3TransformNormal(&vLook, &vLook, &matWorld);

	memcpy(&matWorld.m[0][0], &vRight, sizeof(_vec3));
	memcpy(&matWorld.m[1][0], &vUp, sizeof(_vec3));
	memcpy(&matWorld.m[2][0], &vLook, sizeof(_vec3));

	


	dynamic_cast<CTerrain*>(pLayer->Get_ObjectList().back())->GetTransformCom()->Set_StateInfo(Engine::CTransform::STATE_RIGHT, &vRight);
	dynamic_cast<CTerrain*>(pLayer->Get_ObjectList().back())->GetTransformCom()->Set_StateInfo(Engine::CTransform::STATE_UP, &vUp);
	dynamic_cast<CTerrain*>(pLayer->Get_ObjectList().back())->GetTransformCom()->Set_StateInfo(Engine::CTransform::STATE_LOOK, &vLook);

}
