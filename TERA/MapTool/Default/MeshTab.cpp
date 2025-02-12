// MeshTab.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MapTool.h"
#include "MeshTab.h"
#include "afxdialogex.h"
#include "SceneMesh.h"
#include "SceneStatic.h"
#include "ViewManager.h"
#include "Layer.h"
#include "StaticObject.h"
#include "DataManager.h"
#include "Object_Manager.h"
#include "Terrain.h"

// CMeshTab 대화 상자입니다.

IMPLEMENT_DYNAMIC(CMeshTab, CDialogEx)

CMeshTab::CMeshTab(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MESHTAB, pParent)
	, m_fScalingX(1)
	, m_fScalingY(1)
	, m_fScalingZ(1)
	, m_fRotX(0)
	, m_fRotY(0)
	, m_fRotZ(0)
	, m_fPosX(0)
	, m_fPosY(0)
	, m_fPosZ(0)
{
}

CMeshTab::~CMeshTab()
{
}

void CMeshTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RADIO1, Mesh_RenderST_Solild);
	DDX_Control(pDX, IDC_RADIO3, Mesh_MouseSelType_Object);
	DDX_Control(pDX, IDC_RADIO5, Mesh_ObjectType_Static);
	DDX_Control(pDX, IDC_RADIO7, Mesh_NaviVtxMove_Together);
	DDX_Control(pDX, IDC_TREE1, Tree_Mesh_Object);
	DDX_Control(pDX, IDC_TREE3, Tree_Mesh_StaticObj);
	DDX_Control(pDX, IDC_EDIT5, Obj_X_Scaling);
	DDX_Control(pDX, IDC_EDIT6, Obj_Y_Scaling);
	DDX_Control(pDX, IDC_EDIT7, Obj_Z_Scaling);
	DDX_Control(pDX, IDC_EDIT8, Obj_X_Rotation);
	DDX_Control(pDX, IDC_EDIT9, Obj_Y_Rotation);
	DDX_Control(pDX, IDC_EDIT10, Obj_Z_Rotation);
	DDX_Control(pDX, IDC_EDIT11, Obj_X_Position);
	DDX_Control(pDX, IDC_EDIT12, Obj_Y_Position);
	DDX_Control(pDX, IDC_EDIT13, Obj_Z_Position);
	DDX_Control(pDX, IDC_SPIN5, Obj_X_Scaling_Btn);
	DDX_Control(pDX, IDC_SPIN6, Obj_Y_Scaling_Btn);
	DDX_Control(pDX, IDC_SPIN7, Obj_Z_Scaling_Btn);
	DDX_Control(pDX, IDC_SPIN8, Obj_X_Rotation_Btn);
	DDX_Control(pDX, IDC_SPIN9, Obj_Y_Rotation_Btn);
	DDX_Control(pDX, IDC_SPIN10, Obj_Z_Rotation_Btn);
	DDX_Control(pDX, IDC_SPIN11, Obj_X_Position_Btn);
	DDX_Control(pDX, IDC_SPIN12, Obj_Y_Position_Btn);
	DDX_Control(pDX, IDC_SPIN13, Obj_Z_Position_Btn);
	DDX_Text(pDX, IDC_EDIT5, m_fScalingX);
	DDV_MinMaxDouble(pDX, m_fScalingX, 0.01, 100);
	DDX_Text(pDX, IDC_EDIT6, m_fScalingY);
	DDV_MinMaxDouble(pDX, m_fScalingY, 0.01, 100);
	DDX_Text(pDX, IDC_EDIT7, m_fScalingZ);
	DDV_MinMaxDouble(pDX, m_fScalingZ, 0.01, 100);
	DDX_Text(pDX, IDC_EDIT8, m_fRotX);
	DDV_MinMaxDouble(pDX, m_fRotX, -360, 360);
	DDX_Text(pDX, IDC_EDIT9, m_fRotY);
	DDV_MinMaxDouble(pDX, m_fRotY, -360, 360);
	DDX_Text(pDX, IDC_EDIT10, m_fRotZ);
	DDV_MinMaxDouble(pDX, m_fRotZ, -360, 360);
	DDX_Text(pDX, IDC_EDIT11, m_fPosX);
	DDV_MinMaxDouble(pDX, m_fPosX, -10000, 10000);
	DDX_Text(pDX, IDC_EDIT12, m_fPosY);
	DDV_MinMaxDouble(pDX, m_fPosY, -10000, 10000);
	DDX_Text(pDX, IDC_EDIT13, m_fPosZ);
	DDV_MinMaxDouble(pDX, m_fPosZ, -10000, 10000);
	DDX_Control(pDX, IDC_TREE4, Tree_Mesh_DynamicObj);
	DDX_Control(pDX, IDC_TREE2, Tree_Mesh_Navi);
}

BEGIN_MESSAGE_MAP(CMeshTab, CDialogEx)
	ON_BN_CLICKED(IDC_RADIO1, &CMeshTab::OnBnClickedSolid)
	ON_BN_CLICKED(IDC_RADIO2, &CMeshTab::OnBnClickedWireFrame)
	ON_BN_CLICKED(IDC_RADIO3, &CMeshTab::OnBnClickedObject)
	ON_BN_CLICKED(IDC_RADIO4, &CMeshTab::OnBnClickedNavi_Mesh)
	ON_BN_CLICKED(IDC_RADIO5, &CMeshTab::OnBnClickedStatic)
	ON_BN_CLICKED(IDC_RADIO6, &CMeshTab::OnBnClickedDynamic)
	ON_BN_CLICKED(IDC_RADIO7, &CMeshTab::OnBnClickedTogether)
	ON_BN_CLICKED(IDC_RADIO8, &CMeshTab::OnBnClickedSelected)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CMeshTab::OnTree_Mesh_Object)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE3, &CMeshTab::OnTree_Mesh_StaticObj)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN5, &CMeshTab::OnSpin_Scaling_X)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN6, &CMeshTab::OnSpin_Scaling_Y)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN7, &CMeshTab::OnSpin_Scaling_Z)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN8, &CMeshTab::OnSpin_Rotate_X)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN9, &CMeshTab::OnSpin_Rotate_Y)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN10, &CMeshTab::OnSpin_Rotate_Z)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN11, &CMeshTab::OnSpin_Position_X)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN12, &CMeshTab::OnSpin_Position_Y)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN13, &CMeshTab::OnSpin_Position_Z)
	ON_NOTIFY(NM_CLICK, IDC_TREE1, &CMeshTab::OnNMClickTreeMeshObject)
	ON_NOTIFY(NM_RCLICK, IDC_TREE1, &CMeshTab::OnNMRClickTreeMeshObject)
	ON_NOTIFY(NM_RCLICK, IDC_TREE3, &CMeshTab::OnNMRClickTreeStaticObj)
	ON_BN_CLICKED(IDC_BUTTON1, &CMeshTab::OnBnClicked_StaticObject_Delete)
	ON_NOTIFY(NM_RCLICK, IDC_TREE4, &CMeshTab::OnNMRClickTreeDynamicObj)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE4, &CMeshTab::OnTree_Mesh_DynamicObj)
	ON_BN_CLICKED(IDC_BUTTON8, &CMeshTab::OnBnClicked_DynamicObject_Delete)
	ON_BN_CLICKED(IDC_BUTTON5, &CMeshTab::OnBnClicked_Mesh_Save)
	ON_BN_CLICKED(IDC_BUTTON6, &CMeshTab::OnBnClicked_Mesh_Load)
	ON_BN_CLICKED(IDC_BUTTON9, &CMeshTab::OnBnClicked_Navi_List_Delete)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE2, &CMeshTab::OnTree_Navi_List)
	ON_BN_CLICKED(IDC_BUTTON15, &CMeshTab::OnBnClicked_Navi_Save)
	ON_BN_CLICKED(IDC_BUTTON16, &CMeshTab::OnBnClicked_Navi_Load)
END_MESSAGE_MAP()

BOOL CMeshTab::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	Mesh_RenderST_Solild.SetCheck(TRUE);
	Mesh_MouseSelType_Object.SetCheck(TRUE);
	Mesh_ObjectType_Static.SetCheck(TRUE);
	Mesh_NaviVtxMove_Together.SetCheck(TRUE);

	GetDlgItem(IDC_RADIO7)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO8)->EnableWindow(FALSE);

	//Obj_X_Scaling_Btn.SetRange(0, 360);
	// SetPos로 EditBox의 초기 텍스트 값을 지정하고 싶다면,
	// Spin_Control의 속성 값에 Auto_Buddy값과 Set_Buddy_Integer값을 True로 바꾸고,
	// 리소스 뷰 다이얼로그에서 Ctrl+D를 눌러서,
	// Edit Box와 Spin Control이 연속된 숫자를 가지게 셋팅하면 된다.
	Obj_X_Scaling_Btn.SetPos(1);
	Obj_X_Scaling_Btn.SetRange(100, 0);

	Obj_Y_Scaling_Btn.SetPos(1);
	Obj_Y_Scaling_Btn.SetRange(100, 0);

	Obj_Z_Scaling_Btn.SetPos(1);
	Obj_Z_Scaling_Btn.SetRange(100, 0);

	Obj_X_Rotation_Btn.SetPos(0);
	Obj_X_Rotation_Btn.SetRange(360, -360);

	Obj_Y_Rotation_Btn.SetPos(0);
	Obj_Y_Rotation_Btn.SetRange(360, -360);

	Obj_Z_Rotation_Btn.SetPos(0);
	Obj_Z_Rotation_Btn.SetRange(360, -360);

	Obj_X_Position_Btn.SetPos(0);
	Obj_X_Position_Btn.SetRange(10000, -10000);

	Obj_Y_Position_Btn.SetPos(0);
	Obj_Y_Position_Btn.SetRange(10000, -10000);

	Obj_Z_Position_Btn.SetPos(0);
	Obj_Z_Position_Btn.SetRange(10000, -10000);

	InitTreeCtrl_Object();

	D3DXCreateLine(CViewManager::GetInstance()->m_pMainView->m_pGraphicDevice, &m_pLine);

	//m_pGraphic_Device = CViewManager::GetInstance()->m_pMainView->m_pGraphicDevice;
	
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

// CMeshTab 메시지 처리기입니다.
BOOL CMeshTab::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	//// 엔터 및 ESC 키 입력 시 도구상자 사라지는 현상 해결 구문 ////

	//// ESC ////
	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_ESCAPE))
		return TRUE;

	//// Enter ////
	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_RETURN))
		return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CMeshTab::OnBnClickedSolid()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

}


void CMeshTab::OnBnClickedWireFrame()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CMeshTab::OnBnClickedObject()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	bIsNaviMesh = false;

	GetDlgItem(IDC_RADIO7)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO8)->EnableWindow(FALSE);
	
	GetDlgItem(IDC_RADIO5)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO6)->EnableWindow(TRUE);
	GetDlgItem(IDC_SPIN5)->EnableWindow(TRUE);
	GetDlgItem(IDC_SPIN6)->EnableWindow(TRUE);
	GetDlgItem(IDC_SPIN7)->EnableWindow(TRUE);
	GetDlgItem(IDC_SPIN8)->EnableWindow(TRUE);
	GetDlgItem(IDC_SPIN9)->EnableWindow(TRUE);
	GetDlgItem(IDC_SPIN10)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT5)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT6)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT7)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT8)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT9)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT10)->EnableWindow(TRUE);

	UpdateData(FALSE);
}


void CMeshTab::OnBnClickedNavi_Mesh()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	bIsNaviMesh = true;

	GetDlgItem(IDC_RADIO7)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO8)->EnableWindow(TRUE);

	GetDlgItem(IDC_RADIO5)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO6)->EnableWindow(FALSE);
	GetDlgItem(IDC_SPIN5)->EnableWindow(FALSE);
	GetDlgItem(IDC_SPIN6)->EnableWindow(FALSE);
	GetDlgItem(IDC_SPIN7)->EnableWindow(FALSE);
	GetDlgItem(IDC_SPIN8)->EnableWindow(FALSE);
	GetDlgItem(IDC_SPIN9)->EnableWindow(FALSE);
	GetDlgItem(IDC_SPIN10)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT5)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT6)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT7)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT8)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT9)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT10)->EnableWindow(FALSE);
	
	UpdateData(FALSE);
}


void CMeshTab::OnBnClickedStatic()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	bIsStaticMesh = true;

	UpdateData(FALSE);
}


void CMeshTab::OnBnClickedDynamic()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	bIsStaticMesh = false;

	UpdateData(FALSE);

}


void CMeshTab::OnBnClickedTogether()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CMeshTab::OnBnClickedSelected()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CMeshTab::OnBnClicked_StaticObject_Delete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	// Static_Object Tree에서 현재 선택된 아이템의 부모 아이템을 가져오는 구문.
	// 이걸 해주는 이유는 현재 트리에 아이템이 "오브젝트명 [n]" 이렇게 되는데,
	// 자르기 귀찮아서 그냥 부모의 아이템을 가져오려 하기 때문이다.
	HTREEITEM hSelectedParentItem = Tree_Mesh_StaticObj.GetParentItem(SelectedStaticObject);

	// 부모 아이템 그러니까 딱 오브젝트 이름만있고 인덱스가 붙어있지 않은 순수한 텍스트를 가져온다.
	CString ParentItemName = Tree_Mesh_StaticObj.GetItemText(hSelectedParentItem);

	// 이건 현재 선택된 Static Object의 인덱스가 몇번째인지 잘라오는 구문이다.
	CString strItemIdx = Tree_Mesh_StaticObj.GetItemText(SelectedStaticObject);

	strItemIdx.Remove('[');
	strItemIdx.Remove(']');
	strItemIdx.Remove(' ');

	int i = 0;

	for (; i < strItemIdx.GetLength(); ++i)
	{
		if (isdigit(strItemIdx[i]) != 0)
			break;
	}
	strItemIdx.Delete(0, i);

	size_t iItemIdx = _ttoi(strItemIdx);

	if (iLatestItemIdx < iItemIdx)
		iLatestItemIdx = iItemIdx;

	// CDataManager에 m_MapMeshData도 값을 저장하고 있기에, 해당 객체에 접근해서 지워줘야 하는데,
	// 그러려면 objName이 필요하다.
	// 기존에는 CDataManager가 가지고있는 멤버변수에 접근했는데,
	// 이게 로드하면서 값이 다른걸로 바뀌어 있기때문에, 밑에 구문에서 오류를 범할 수 있으므로,
	// 여기서 "Layer_" 를 붙여주기 전에 따로 지역변수로 저장해 놓는다.
	CString strTempObjName;
	strTempObjName = ParentItemName;

	ParentItemName = _T("Layer_") + ParentItemName;

	CLayer* pLayer = CObject_Manager::GetInstance()->FindObjectLayer(SCENE_STATIC, ParentItemName);
	auto& iter = pLayer->Get_ObjectList().begin();

	// 트리에 아이템 제거.
	Tree_Mesh_StaticObj.DeleteItem(SelectedStaticObject);

	for (; iter != pLayer->Get_ObjectList().end(); )
	{
		if ((*iter)->Get_IdxNum() == iItemIdx)
		{
			Safe_Release(*iter);
			iter = pLayer->Get_ObjectList().erase(iter);

			auto& iterFirst = find_if(CDataManager::GetInstance()->m_MapMeshData.begin(), CDataManager::GetInstance()->m_MapMeshData.end(), CFinder_Tag(strTempObjName));

			iterFirst->second.erase(iterFirst->second.begin() + (iItemIdx-1));

			// 내가 지운 객체가 동일한 First Key 값으로 더이상 객체를 가지지 않는다면,
			// 해당 값의 Fisrt 값도 지워준다.
			if (iterFirst->second.size() <= 0)
				CDataManager::GetInstance()->m_MapMeshData.erase(iterFirst);

			break;
		}
		else
			++iter;
	}

	if (iItemIdx == 1)
	{
		Tree_Mesh_StaticObj.DeleteItem(SelectedStaticObject);
		return;
	}

	Invalidate(false);
}

void CMeshTab::OnBnClicked_DynamicObject_Delete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	// Static_Object Tree에서 현재 선택된 아이템의 부모 아이템을 가져오는 구문.
	// 이걸 해주는 이유는 현재 트리에 아이템이 "오브젝트명 [n]" 이렇게 되는데,
	// 자르기 귀찮아서 그냥 부모의 아이템을 가져오려 하기 때문이다.
	HTREEITEM hSelectedParentItem = Tree_Mesh_DynamicObj.GetParentItem(SelectedDynamicObject);

	// 부모 아이템 그러니까 딱 오브젝트 이름만있고 인덱스가 붙어있지 않은 순수한 텍스트를 가져온다.
	CString ParentItemName = Tree_Mesh_DynamicObj.GetItemText(hSelectedParentItem);

	// 이건 현재 선택된 Static Object의 인덱스가 몇번째인지 잘라오는 구문이다.
	CString strItemIdx = Tree_Mesh_DynamicObj.GetItemText(SelectedDynamicObject);

	strItemIdx.Remove('[');
	strItemIdx.Remove(']');
	strItemIdx.Remove(' ');

	int i = 0;

	for (; i < strItemIdx.GetLength(); ++i)
	{
		if (isdigit(strItemIdx[i]) != 0)
			break;
	}
	strItemIdx.Delete(0, i);

	size_t iItemIdx = _ttoi(strItemIdx);

	if (iLatestItemIdx < iItemIdx)
		iLatestItemIdx = iItemIdx;

	CString strTempObjName;
	strTempObjName = ParentItemName;

	ParentItemName = _T("Layer_") + ParentItemName;

	CLayer* pLayer = CObject_Manager::GetInstance()->FindObjectLayer(SCENE_STATIC, ParentItemName);
	auto& iter = pLayer->Get_ObjectList().begin();

	// 트리에 아이템 제거.
	Tree_Mesh_DynamicObj.DeleteItem(SelectedDynamicObject);

	for (; iter != pLayer->Get_ObjectList().end(); )
	{
		if ((*iter)->Get_IdxNum() == iItemIdx)
		{
			Safe_Release(*iter);
			iter = pLayer->Get_ObjectList().erase(iter);

			auto& iterFirst = find_if(CDataManager::GetInstance()->m_MapMeshData.begin(), CDataManager::GetInstance()->m_MapMeshData.end(), CFinder_Tag(strTempObjName));

			iterFirst->second.erase(iterFirst->second.begin() + (iItemIdx - 1));

			// 내가 지운 객체가 동일한 First Key 값으로 더이상 객체를 가지지 않는다면,
			// 해당 값의 Fisrt 값도 지워준다.
			if (iterFirst->second.size() <= 0)
				CDataManager::GetInstance()->m_MapMeshData.erase(iterFirst);

			break;
		}
		else
			++iter;
	}

	if (iItemIdx == 1)
	{
		Tree_Mesh_DynamicObj.DeleteItem(SelectedDynamicObject);
		return;
	}

	//// 위에서 ++한 Iter 위치 초기화.
	//iter = pLayer->Get_ObjectList().begin();

	//for (; iter != pLayer->Get_ObjectList().end(); )
	//{
	//	if ((*iter)->Get_IdxNum() == iItemIdx)
	//	{
	//		Safe_Release(*iter);
	//		iter = pLayer->Get_ObjectList().erase(iter);
	//		break;
	//	}
	//	else
	//		++iter;
	//}

	//// 아이템이 마지막으로 지워졌다면 해당 트리의 부모트리까지 지워준다.
	//if (iItemIdx == 1)
	//{
	//	Tree_Mesh_DynamicObj.DeleteItem(SelectedDynamicObject);
	//	return;
	//}

	Invalidate(false);
}

void CMeshTab::OnBnClicked_Navi_List_Delete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CString strItemIdx = Tree_Mesh_Navi.GetItemText(SelectedNaviItem);

	strItemIdx.Remove('[');
	strItemIdx.Remove(']');

	_int iItemIdx = _ttoi(strItemIdx);

	for (auto iter = mapNaviMesh.begin(); iter != mapNaviMesh.end(); )
	{
		if (iter->first == (iItemIdx - 1))
		{
			Tree_Mesh_Navi.DeleteItem(SelectedNaviItem);

			iter->second.clear();
			mapNaviMesh.erase(iter);

			break;
		}
		else
			++iter;
	}

}


void CMeshTab::InitTreeCtrl_Object()
{
	HTREEITEM hItem = Tree_Mesh_Object.InsertItem(_T("../Bin/Resources/Meshes/"));

	CFileFind finder;

	BOOL bWorking = finder.FindFile(_T("..\Bin\Resources\Meshes\\*.*"));

	while (bWorking)
	{
		bWorking = finder.FindNextFileW();

		if (finder.IsDirectory())
			Tree_Mesh_Object.InsertItem(finder.GetFileName(), hItem);
	}

	Tree_Mesh_Object.EnsureVisible(hItem);

}

HRESULT CMeshTab::MakeItemForTree()
{
	HTREEITEM	hChild1 = nullptr;

	UpdateData(TRUE);

	if (bIsStaticMesh == true)
	{
		if (NULL == Tree_Mesh_StaticObj.GetRootItem())
			StaticRoot = Tree_Mesh_StaticObj.InsertItem(TEXT("Static_Mesh"), 0, 0, TVI_ROOT, TVI_LAST);
	}
	else if(bIsStaticMesh == false)
	{
		if (NULL == Tree_Mesh_DynamicObj.GetRootItem())
			DynamicRoot = Tree_Mesh_DynamicObj.InsertItem(TEXT("Dynamic_Mesh"), 0, 0, TVI_ROOT, TVI_LAST);
	}

	if (NOERROR == Add_StaticObject())
	{
		auto iter = find_if(mapTreeItem.begin(), mapTreeItem.end(), CFinder_Tag(strObjectName));

		if (iter == mapTreeItem.end())
		{
			iLatestItemIdx = 0;

			CString ObjectNameTemp1 = strObjectName;
			CString ItemNum;
			CObject_Manager* pObjectManager = CObject_Manager::GetInstance();

			++iLatestItemIdx;

			ItemNum.Format(_T(" [%d]"), iLatestItemIdx);
			pObjectManager->FindObjectLayer(SCENE_STATIC, strLayerTag)->Get_ObjectList().back()->SetIdxNum(iLatestItemIdx);
			
			ObjectNameTemp1 += ItemNum;

			if (bIsStaticMesh == true)
			{
				hChild1 = Tree_Mesh_StaticObj.InsertItem(strObjectName, 0, 0, StaticRoot, TVI_LAST);
				Tree_Mesh_StaticObj.InsertItem(ObjectNameTemp1, 0, 0, hChild1, TVI_LAST);
			}
			else if (bIsStaticMesh == false)
			{
				hChild1 = Tree_Mesh_DynamicObj.InsertItem(strObjectName, 0, 0, DynamicRoot, TVI_LAST);
				Tree_Mesh_DynamicObj.InsertItem(ObjectNameTemp1, 0, 0, hChild1, TVI_LAST);
			}

			mapTreeItem.emplace(strObjectName, hChild1);

			//strObjectName = _T("");
			ObjectNameTemp1 = _T("");
			ItemNum = _T("");
		}
		else
		{
			//auto iter = find_if(mapTreeItem.begin(), mapTreeItem.end(), CFinder_Tag(strObjectName));

			//if (iter != mapTreeItem.end())
			//{
		
			//}
			UpdateData(TRUE);

			CString ObjectNameTemp2 = strObjectName;
			CString ItemNum;
			CObject_Manager* pObjectManager = CObject_Manager::GetInstance();

			auto IdxValue = pObjectManager->FindObjectLayer(SCENE_STATIC, strLayerTag)->Get_ObjectList().begin();
			size_t iSize = pObjectManager->FindObjectLayer(SCENE_STATIC, strLayerTag)->Get_ObjectList().size();

			// List의 .back() 함수는 이터레이터를 던져주는것이 아니라, 해당 이터의 값, 즉 (*iter)를 리턴하기 때문에,
			// back - 1은 값에 -1을 해주는것이다. 결국 뻘짓이라는거지..
			// 밑에처럼 순회하면서 돌아야지만, 가장 마지막에 생성된 놈의 바로 전놈을 찾을 수 있다.

			for (size_t i = 0; i < iSize; ++i)
			{
				if (i == iSize - 2)
					break;
				else
					++IdxValue;
			}

			iSaveItemIdx = (*IdxValue)->Get_IdxNum();

			if (iLatestItemIdx != iSaveItemIdx)
				iLatestItemIdx = iSaveItemIdx;

			//++iSaveItemIdx;
			++iLatestItemIdx;

			ItemNum.Format(_T(" [%d]"), iLatestItemIdx);
			(pObjectManager->FindObjectLayer(SCENE_STATIC, strLayerTag)->Get_ObjectList().back())->SetIdxNum(iLatestItemIdx);

			ObjectNameTemp2 += ItemNum;

			if (bIsStaticMesh == true)
				Tree_Mesh_StaticObj.InsertItem(ObjectNameTemp2, 0, 0, iter->second, TVI_LAST);
			else if (bIsStaticMesh == false)
				Tree_Mesh_DynamicObj.InsertItem(ObjectNameTemp2, 0, 0, iter->second, TVI_LAST);

			//strLayerTag = _T("");
			ObjectNameTemp2 = _T("");
			ItemNum = _T("");

			UpdateData(FALSE);
		}
	}

	UpdateData(FALSE);

	return NOERROR;
}

HRESULT CMeshTab::Make_Navigation()
{
	HTREEITEM	hRoot = nullptr;

	UpdateData(TRUE);

	// For.Mesh_Picking Variable
	_vec3 vPos;
	_vec3 vPosOut;
	_bool bIsPicked = false;
	_float fFinalDist;
	_float fMinDist = 9999999999.f;
	
	// For.Terrain_Picking Variable
	CLayer* pTerrainLayer = CObject_Manager::GetInstance()->FindObjectLayer(SCENE_STATIC, L"Layer_Terrain");
	CTerrain* pTerrain = dynamic_cast<CTerrain*>(pTerrainLayer->Get_ObjectList().back());
	CTransform* pTerrainTransform = pTerrain->GetTransformCom();
	
	for (auto iter : vecObjLayerTag)
	{
		CLayer* pLayer = CObject_Manager::GetInstance()->FindObjectLayer(SCENE_STATIC, iter);
		
		for (auto Object : pLayer->Get_ObjectList())
		{
			if (true == dynamic_cast<CStaticObject*>(Object)->Picking(g_WinhWnd, dynamic_cast<CStaticObject*>(Object)->GetTransformCom(), dynamic_cast<CStaticObject*>(Object)->GetMeshCom()->Get_BaseMesh(), &vPosOut, &fFinalDist))
			{
				if (fFinalDist <= fMinDist)
				{
					fMinDist = fFinalDist;
					vPos = vPosOut;
				}

				bIsPicked = true;
			}
		}		
	} 

	if (bIsPicked == true)
	{
		_float fCompareLength = 0.f;

		for (auto Pair : mapNaviMesh)
		{
			for (auto PairSecond : Pair.second)
			{
				fCompareLength = D3DXVec3Length(&(vPos - PairSecond));

				if (fNaviDotCompareValue >= fCompareLength)
					vPos = PairSecond;
			}
		}
		
		vecPos.push_back(vPos);

		if (vecPos.size() == 3)
		{
			CString strRoot;
			strRoot.Format(_T("[%d]"), iNaviRootTreeCount++);

			hRoot = Tree_Mesh_Navi.InsertItem(strRoot, 0, 0, TVI_ROOT, TVI_LAST);

			CString strChild;
			strChild.Format(_T("%d"), iNaviChildTreeCount);
			Tree_Mesh_Navi.InsertItem(strChild, 0, 0, hRoot, TVI_LAST);

			++iNaviChildTreeCount;
			strChild.Format(_T("%d"), iNaviChildTreeCount);
			Tree_Mesh_Navi.InsertItem(strChild, 0, 0, hRoot, TVI_LAST);

			++iNaviChildTreeCount;
			strChild.Format(_T("%d"), iNaviChildTreeCount);
			Tree_Mesh_Navi.InsertItem(strChild, 0, 0, hRoot, TVI_LAST);

			iNaviChildTreeCount = 1;

			// 시계 방향으로 네비메쉬를 찍지 않았을 때를 고려한 구문.
			_vec3 vCrossValue;
			D3DXVec3Cross(&vCrossValue, &(vecPos[1] - vecPos[0]), &(vecPos[2] - vecPos[1]));

			if (vCrossValue.y < 0)
			{
				_vec3 vTemp;
				vTemp = vecPos[1];
				vecPos[1] = vecPos[2];
				vecPos[2] = vTemp;
			}

			mapNaviMesh.emplace(iNaviMapCount++, vecPos);
			vecPos.clear();
		}
		return NOERROR;
	}
	
	// 만약 해당하는게 없다면, 해당 위치의 Terrain과 피킹을 한다.
	else if (true == pTerrain->Picking(g_WinhWnd, pTerrainTransform, &vPos))
	{
		_float fCompareLength = 0.f;

		for (auto Pair : mapNaviMesh)
		{
			for (auto PairSecond : Pair.second)
			{
				// 현재 피킹 된 정점의 포지션과,
				// 현재 찍혀있는 정점들이 들어가있는 mapNaviMesh 맵 컨테이너를 순회하며 나온 값 들의 차의 길이 값을
				// 임시 변수를 하나 생성하여 담아준다.
				fCompareLength = D3DXVec3Length(&(vPos - PairSecond));

				// 이 거리 값이 내가 정한 임의의 값보다 작거나 같다면
				// 근처에 위치한 정점이 되므로,
				// 기존에 위치한 정점의 포지션 값을 새롭게 찍으려는 정점의 포지션값에 복사해준다.
				if (fNaviDotCompareValue >= fCompareLength)
					vPos = PairSecond;

				// 이러면 네비메쉬를 찍을 때, 근접한 정점에 피킹을 하면 그 근접한 정점의 포지션으로 새로운 정점이 찍히게 된다.
			}
		}

		vPos.y = 0.f;

		vecPos.push_back(vPos);

		if (vecPos.size() == 3)
		{
			CString strRoot;
			strRoot.Format(_T("[%d]"), iNaviRootTreeCount++);

			hRoot = Tree_Mesh_Navi.InsertItem(strRoot, 0, 0, TVI_ROOT, TVI_LAST);

			CString strChild;
			strChild.Format(_T("%d"), iNaviChildTreeCount);
			Tree_Mesh_Navi.InsertItem(strChild, 0, 0, hRoot, TVI_LAST);

			++iNaviChildTreeCount;
			strChild.Format(_T("%d"), iNaviChildTreeCount);
			Tree_Mesh_Navi.InsertItem(strChild, 0, 0, hRoot, TVI_LAST);

			++iNaviChildTreeCount;
			strChild.Format(_T("%d"), iNaviChildTreeCount);
			Tree_Mesh_Navi.InsertItem(strChild, 0, 0, hRoot, TVI_LAST);

			iNaviChildTreeCount = 1;

			// 시계 방향으로 네비메쉬를 찍지 않았을 때를 고려한 구문.
			_vec3 vCrossValue;
			D3DXVec3Cross(&vCrossValue, &(vecPos[1] - vecPos[0]), &(vecPos[2] - vecPos[1]));

			if (vCrossValue.y < 0)
			{
				_vec3 vTemp;
				vTemp = vecPos[1];
				vecPos[1] = vecPos[2];
				vecPos[2] = vTemp;
			}

			mapNaviMesh.emplace(iNaviMapCount++, vecPos);
			vecPos.clear();
		}
	}
	else
		return E_FAIL;	// 이게 안되면 ㅈ된거쥬?
		
	return NOERROR;
}

void CMeshTab::Render_Navigation()
{
	for (auto& Pair : mapNaviMesh)
	{
		_vec3		vPoint[4];

		vPoint[0] = Pair.second[0];
		vPoint[1] = Pair.second[1];
		vPoint[2] = Pair.second[2];
		vPoint[3] = Pair.second[0];

		_matrix			matView, matProj;

		CViewManager::GetInstance()->m_pMainView->m_pGraphicDevice->GetTransform(D3DTS_VIEW, &matView);
		CViewManager::GetInstance()->m_pMainView->m_pGraphicDevice->GetTransform(D3DTS_PROJECTION, &matProj);

		for (size_t i = 0; i < 4; ++i)
		{
			D3DXVec3TransformCoord(&vPoint[i], &vPoint[i], &matView);

			if (vPoint[i].z <= 0)
				vPoint[i].z = 0;

			D3DXVec3TransformCoord(&vPoint[i], &vPoint[i], &matProj);
		}
		_matrix		matIdentity;

		m_pLine->SetWidth(5.f);
		m_pLine->DrawTransform(vPoint, 4, D3DXMatrixIdentity(&matIdentity), D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
	}
}

HRESULT CMeshTab::Add_StaticObject()
{
	m_pScene = CViewManager::GetInstance()->m_pCurScene;

	MakeArgVariableForStaticObj();

	// 컴포넌트 프로토 타입도 최초 한번만 생성해야 하므로..
	// find_if 함수를 사용하여 strComponentPrototypeTag를 통해 iter를 검색해서,
	auto iter = find_if(mapObj_Com_Prototype.begin(), mapObj_Com_Prototype.end(), CFinder_Tag(strComponentPrototypeTag));

	// 검색이 안되었다면, 만들어진적이 없는 최초 생성이므로,
	if (iter == mapObj_Com_Prototype.end())
	{
		// 현재 선택된 Object의 xFile 이름을 만들어준다.
		wstring strXFileNameTemp;
		strXFileNameTemp = strObjectName;
		strXFileNameTemp += L".X";
		strXfileName = strXFileNameTemp.c_str();

		// Add_Component_Prototype에 해당한다. (원본객체)
		if (FAILED(dynamic_cast<CSceneStatic*>(m_pScene)->Add_Static_Object_Component_Prototype(strComponentPrototypeTag, szFullPath, strXfileName)))
			return E_FAIL;

		mapObj_Com_Prototype.emplace(strComponentPrototypeTag, strXfileName);

		strXfileName = _T("");

		vecObjLayerTag.push_back(strLayerTag);
	}
	
	if (FAILED(dynamic_cast<CSceneStatic*>(m_pScene)->Add_Static_Object(strLayerTag)))
		return E_FAIL;

	CString strObjProtoTag = L"GameObject_" + strObjectName;

	CDataManager::GetInstance()->m_bIsStaticMesh = bIsStaticMesh;
	CDataManager::GetInstance()->m_strObjProtoTag = strObjProtoTag;
	CDataManager::GetInstance()->m_strLayerTag = strLayerTag;
	CDataManager::GetInstance()->m_strComProtoTag = strComponentPrototypeTag;

	CString	szFullPathModify = szFullPath;
	//szFullPathModify.TrimRight(L"/");
	
	CDataManager::GetInstance()->m_strFullPath = szFullPathModify;
	CDataManager::GetInstance()->m_strObjName = strObjectName;
	
	CLayer* pLayer = CObject_Manager::GetInstance()->FindObjectLayer(SCENE_STATIC, strLayerTag);

	// Add_Component에 해당한다. (실 사용 객체)	
	dynamic_cast<CStaticObject*>(pLayer->Get_ObjectList().back())->Add_Component_Tool(strComponentPrototypeTag);
	strComponentPrototypeTag = _T("");

	return NOERROR;
}

HRESULT CMeshTab::MakeArgVariableForStaticObj()
{
	//TCHAR		szFullPath[MAX_PATH] = L"";
	CString		pathSelected = L"";
	HTREEITEM	hParentItem = SelectedObjectItem;

	if (nullptr == SelectedObjectItem)
		return E_FAIL;

	while (TRUE)
	{
		if (pathSelected != L"")
		{
			if (Tree_Mesh_Object.GetItemText(hParentItem).Right(1) == "/")
			{
				pathSelected.Delete(0, 1);
				pathSelected = Tree_Mesh_Object.GetItemText(hParentItem) + pathSelected;
			}
			else
				pathSelected = Tree_Mesh_Object.GetItemText(hParentItem) + pathSelected;
		}
		else
		{
			pathSelected = Tree_Mesh_Object.GetItemText(hParentItem);
			strLayerTag = pathSelected;

			strXfileName = L"";
			strXfileName += strLayerTag;
			pathSelected = L"/";
		}

		hParentItem = Tree_Mesh_Object.GetParentItem(hParentItem);

		if (hParentItem == NULL)
			break;

		if (pathSelected != L"/")
			pathSelected = _T("/") + pathSelected;
	}

	StrCpyW(szFullPath, pathSelected);

	// 확장자명 자르기.
	strLayerTag.TrimRight(L".X");
	strLayerTag.TrimRight(L".x");

	strObjectName = strLayerTag;

	// Component의 PrototypeTag를 만들어 준다.
	if(bIsStaticMesh == true)
		strComponentPrototypeTag = _T("Component_Mesh_Static_") + strLayerTag;
	else
		strComponentPrototypeTag = _T("Component_Mesh_Dynamic_") + strLayerTag;

	// LayerTag를 만들어 준다.
	strLayerTag = _T("Layer_") + strLayerTag;

	return NOERROR;
}

void CMeshTab::OnTree_Mesh_Object(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	// 현재 트리 컨트롤에서 선택된 아이템을 불러온다.
	HTREEITEM	hSelected = pNMTreeView->itemNew.hItem;

	SelectedObjectItem = hSelected;

	// 자세히는 잘 모르겠는데... 처음 선택된 놈만 처리되게 되어있다.
	// 이 때문에 밑에서 else문으로 따로 추가 처리해준다.
	if (Tree_Mesh_Object.GetItemData(hSelected) == 0)
	{
		CString	pathSelected;
		HTREEITEM hParentItem = hSelected;

		// 선택된 아이템의 경로를 만들어주는 구문.
		while (hParentItem != NULL)
		{
			pathSelected = _T("/") + pathSelected;
			pathSelected = Tree_Mesh_Object.GetItemText(hParentItem) + pathSelected;
			hParentItem = Tree_Mesh_Object.GetParentItem(hParentItem);
		}

		// 밑으로는 계속 트리컨트롤을 그려내기 위한 구문이다.
		pathSelected = pathSelected + _T("*.*");

		CFileFind finder;
		BOOL bWorking = finder.FindFile(pathSelected);

		while (bWorking)
		{
			bWorking = finder.FindNextFileW();

			if (finder.IsDots())
				continue;

			Tree_Mesh_Object.InsertItem(finder.GetFileName(), hSelected);
		}
		Tree_Mesh_Object.SetItemData(hSelected, 1);
		Tree_Mesh_Object.EnsureVisible(hSelected);
	}

	UpdateData(FALSE);

	*pResult = 0;
}

void CMeshTab::OnNMRClickTreeMeshObject(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	//UpdateData(TRUE);

 	MakeItemForTree();

	// UpdateData(FALSE);
	
	*pResult = 0;
}

void CMeshTab::OnNMClickTreeMeshObject(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	MakeArgVariableForStaticObj();

	*pResult = 0;
}

void CMeshTab::OnTree_Mesh_StaticObj(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	// 현재 트리 컨트롤에서 선택된 아이템을 불러온다.
	HTREEITEM	hSelected = pNMTreeView->itemNew.hItem;

	SelectedStaticObject = hSelected;

	*pResult = 0;
}

void CMeshTab::OnTree_Mesh_DynamicObj(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	// 현재 트리 컨트롤에서 선택된 아이템을 불러온다.
	HTREEITEM	hSelected = pNMTreeView->itemNew.hItem;

	SelectedDynamicObject = hSelected;

	*pResult = 0;
}

void CMeshTab::OnTree_Navi_List(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	// 현재 트리 컨트롤에서 선택된 아이템을 불러온다.
	HTREEITEM	hSelected = pNMTreeView->itemNew.hItem;

	SelectedNaviItem = hSelected;

	*pResult = 0;
}

void CMeshTab::OnNMRClickTreeStaticObj(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	bDblClkTreeStaticObj = true;

	// Static_Object Tree에서 현재 선택된 아이템의 부모 아이템을 가져오는 구문.
	// 이걸 해주는 이유는 현재 트리에 아이템이 "오브젝트명 [n]" 이렇게 되는데,
	// 자르기 귀찮아서 그냥 부모의 아이템을 가져오려 하기 때문이다.
	HTREEITEM hSelectedParentItem = Tree_Mesh_StaticObj.GetParentItem(SelectedStaticObject);

	// 부모 아이템 그러니까 딱 오브젝트 이름만있고 인덱스가 붙어있지 않은 순수한 텍스트를 가져온다.
	CString ParentItemName = Tree_Mesh_StaticObj.GetItemText(hSelectedParentItem);

	// 이건 현재 선택된 Static Object의 인덱스가 몇번째인지 잘라오는 구문이다.
 	CString strItemIdx = Tree_Mesh_StaticObj.GetItemText(SelectedStaticObject);

	strItemIdx.Remove('[');
	strItemIdx.Remove(']');
	strItemIdx.Remove(' ');

	int i = 0;

	for (; i < strItemIdx.GetLength(); ++i)
	{
		if (isdigit(strItemIdx[i]) != 0)
			break;
	}
	strItemIdx.Delete(0, i);

	size_t iItemIdx = _ttoi(strItemIdx);

	if (iLatestItemIdx < iItemIdx)
		iLatestItemIdx = iItemIdx;

	iCurrentSelcetedIndex = iItemIdx;
	strCurrentSelectedObjectName = ParentItemName;

	ParentItemName = _T("Layer_") + ParentItemName;

	CLayer* pLayer = CObject_Manager::GetInstance()->FindObjectLayer(SCENE_STATIC, ParentItemName);
	list<CGameObject*> ObjList = pLayer->Get_ObjectList();

	_int iObjListIdx = 0;

	auto iter = ObjList.begin();

	for (size_t i = 0; i < iItemIdx; ++i)
	{
		if (i == (iItemIdx - 1))
		{
			pSelectedObj = *iter;
			break;
		}
		else
			++iter;
	}

	UpdateData(TRUE);

	_vec3	vRight = *dynamic_cast<CStaticObject*>(pSelectedObj)->GetTransformCom()->Get_StateInfo(CTransform::STATE_RIGHT);
	_vec3	vUp = *dynamic_cast<CStaticObject*>(pSelectedObj)->GetTransformCom()->Get_StateInfo(CTransform::STATE_UP);
	_vec3	vLook = *dynamic_cast<CStaticObject*>(pSelectedObj)->GetTransformCom()->Get_StateInfo(CTransform::STATE_LOOK);

	m_fScalingX = D3DXVec3Length(&vRight);
	m_fScalingY = D3DXVec3Length(&vUp);
	m_fScalingZ = D3DXVec3Length(&vLook);

	_vec3 vRotRadValue = *dynamic_cast<CStaticObject*>(pSelectedObj)->GetTransformCom()->Get_RotRadValue();

	m_fRotX = D3DXToDegree(vRotRadValue.x);
	m_fRotY = D3DXToDegree(vRotRadValue.y);
	m_fRotZ = D3DXToDegree(vRotRadValue.z);

	_vec3 vPosition = *dynamic_cast<CStaticObject*>(pSelectedObj)->GetTransformCom()->Get_StateInfo(CTransform::STATE_POSITION);

	m_fPosX = vPosition.x;
	m_fPosY = vPosition.y;
	m_fPosZ = vPosition.z;

	UpdateData(FALSE);

	*pResult = 0;
}

void CMeshTab::OnNMRClickTreeDynamicObj(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	bDblClkTreeDynamicObj = true;

	// Static_Object Tree에서 현재 선택된 아이템의 부모 아이템을 가져오는 구문.
	// 이걸 해주는 이유는 현재 트리에 아이템이 "오브젝트명 [n]" 이렇게 되는데,
	// 자르기 귀찮아서 그냥 부모의 아이템을 가져오려 하기 때문이다.
	HTREEITEM hSelectedParentItem = Tree_Mesh_DynamicObj.GetParentItem(SelectedDynamicObject);

	// 부모 아이템 그러니까 딱 오브젝트 이름만있고 인덱스가 붙어있지 않은 순수한 텍스트를 가져온다.
	CString ParentItemName = Tree_Mesh_DynamicObj.GetItemText(hSelectedParentItem);

	// 이건 현재 선택된 Static Object의 인덱스가 몇번째인지 잘라오는 구문이다.
	CString strItemIdx = Tree_Mesh_DynamicObj.GetItemText(SelectedDynamicObject);

	strItemIdx.Remove('[');
	strItemIdx.Remove(']');
	strItemIdx.Remove(' ');

	int i = 0;

	for (; i < strItemIdx.GetLength(); ++i)
	{
		if (isdigit(strItemIdx[i]) != 0)
			break;
	}
	strItemIdx.Delete(0, i);

	size_t iItemIdx = _ttoi(strItemIdx);

	iCurrentSelcetedIndex = iItemIdx;
	strCurrentSelectedObjectName = ParentItemName;

	if (iLatestItemIdx < iItemIdx)
		iLatestItemIdx = iItemIdx;

	ParentItemName = _T("Layer_") + ParentItemName;

	CLayer* pLayer = CObject_Manager::GetInstance()->FindObjectLayer(SCENE_STATIC, ParentItemName);
	list<CGameObject*> ObjList = pLayer->Get_ObjectList();

	_int iObjListIdx = 0;
	auto iter = ObjList.begin();

	for (size_t i = 0; i < iItemIdx; ++i)
	{
		if (i == (iItemIdx - 1))
		{
			pSelectedObj = *iter;
			break;
		}
		else
			++iter;
	}
	
	UpdateData(TRUE);

	_vec3	vRight = *dynamic_cast<CStaticObject*>(pSelectedObj)->GetTransformCom()->Get_StateInfo(CTransform::STATE_RIGHT);
	_vec3	vUp = *dynamic_cast<CStaticObject*>(pSelectedObj)->GetTransformCom()->Get_StateInfo(CTransform::STATE_UP);
	_vec3	vLook = *dynamic_cast<CStaticObject*>(pSelectedObj)->GetTransformCom()->Get_StateInfo(CTransform::STATE_LOOK);

	m_fScalingX = D3DXVec3Length(&vRight);
	m_fScalingY = D3DXVec3Length(&vUp);
	m_fScalingZ = D3DXVec3Length(&vLook);

	_vec3 vRotRadValue = *dynamic_cast<CStaticObject*>(pSelectedObj)->GetTransformCom()->Get_RotRadValue();

	m_fRotX = D3DXToDegree(vRotRadValue.x);
	m_fRotY = D3DXToDegree(vRotRadValue.y);
	m_fRotZ = D3DXToDegree(vRotRadValue.z);

	_vec3 vPosition = *dynamic_cast<CStaticObject*>(pSelectedObj)->GetTransformCom()->Get_StateInfo(CTransform::STATE_POSITION);

	m_fPosX = vPosition.x;
	m_fPosY = vPosition.y;
	m_fPosZ = vPosition.z;

	UpdateData(FALSE);

	*pResult = 0;
}

void CMeshTab::OnSpin_Scaling_X(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if (bDblClkTreeStaticObj == true
		|| bDblClkTreeDynamicObj == true)
	{
		UpdateData(TRUE);

		if (pNMUpDown->iDelta < 0)
		{
			if (GetKeyState('Z') & 0x8000)
				m_fScalingX += 1.f;
			else
				m_fScalingX += 0.01f;
		}
		else
		{
			if (GetKeyState('Z') & 0x8000)
				m_fScalingX -= 1.f;
			else
				m_fScalingX -= 0.01f;
		}

		if (m_fScalingX <= 0.01f)
			m_fScalingX = 0.01f;

		UpdateData(FALSE);

		dynamic_cast<CStaticObject*>(pSelectedObj)->GetTransformCom()->Set_Scaling(m_fScalingX, m_fScalingY, m_fScalingZ);

		auto iter = find_if(CDataManager::GetInstance()->m_MapMeshData.begin(), CDataManager::GetInstance()->m_MapMeshData.end(), CFinder_Tag(strCurrentSelectedObjectName));
		
		//iter->second[iCurrentSelcetedIndex-1].matWorld.m[0][0] = m_fScalingX;
		
		D3DXMatrixIdentity(&iter->second[iCurrentSelcetedIndex - 1].matWorld);
		iter->second[iCurrentSelcetedIndex - 1].matWorld = *dynamic_cast<CStaticObject*>(pSelectedObj)->GetTransformCom()->Get_WorldMatrixPointer();

	}

	*pResult = 0;
}


void CMeshTab::OnSpin_Scaling_Y(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if (bDblClkTreeStaticObj == true
		|| bDblClkTreeDynamicObj == true)
	{
		UpdateData(TRUE);

		if (pNMUpDown->iDelta < 0)
		{
			if (GetKeyState('Z') & 0x8000)
				m_fScalingY += 1.f;
			else
				m_fScalingY += 0.01f;
		}
		else
		{
			if (GetKeyState('Z') & 0x8000)
				m_fScalingY -= 1.f;
			else
				m_fScalingY -= 0.01f;
		}

		if (m_fScalingY <= 0.01f)
			m_fScalingY = 0.01f;

		UpdateData(FALSE);

		dynamic_cast<CStaticObject*>(pSelectedObj)->GetTransformCom()->Set_Scaling(m_fScalingX, m_fScalingY, m_fScalingZ);

		auto iter = find_if(CDataManager::GetInstance()->m_MapMeshData.begin(), CDataManager::GetInstance()->m_MapMeshData.end(), CFinder_Tag(strCurrentSelectedObjectName));

		D3DXMatrixIdentity(&iter->second[iCurrentSelcetedIndex - 1].matWorld);
		iter->second[iCurrentSelcetedIndex - 1].matWorld = *dynamic_cast<CStaticObject*>(pSelectedObj)->GetTransformCom()->Get_WorldMatrixPointer();
	}

	*pResult = 0;
}


void CMeshTab::OnSpin_Scaling_Z(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if (bDblClkTreeStaticObj == true
		|| bDblClkTreeDynamicObj == true)
	{
		UpdateData(TRUE);

		if (pNMUpDown->iDelta < 0)
		{
			if (GetKeyState('Z') & 0x8000)
				m_fScalingZ += 1.f;
			else
				m_fScalingZ += 0.01f;
		}
		else
		{
			if (GetKeyState('Z') & 0x8000)
				m_fScalingZ -= 1.f;
			else
				m_fScalingZ -= 0.01f;
		}

		if (m_fScalingZ <= 0.01f)
			m_fScalingZ = 0.01f;

		UpdateData(FALSE);

		dynamic_cast<CStaticObject*>(pSelectedObj)->GetTransformCom()->Set_Scaling(m_fScalingX, m_fScalingY, m_fScalingZ);

		auto iter = find_if(CDataManager::GetInstance()->m_MapMeshData.begin(), CDataManager::GetInstance()->m_MapMeshData.end(), CFinder_Tag(strCurrentSelectedObjectName));

		D3DXMatrixIdentity(&iter->second[iCurrentSelcetedIndex - 1].matWorld);
		iter->second[iCurrentSelcetedIndex - 1].matWorld = *dynamic_cast<CStaticObject*>(pSelectedObj)->GetTransformCom()->Get_WorldMatrixPointer();
	}
	
	*pResult = 0;
}


void CMeshTab::OnSpin_Rotate_X(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if (bDblClkTreeStaticObj == true
		|| bDblClkTreeDynamicObj == true)
	{
		UpdateData(TRUE);

		if (pNMUpDown->iDelta < 0)
		{
			if (GetKeyState('Z') & 0x8000)
				m_fRotX += 10.f;
			else
				m_fRotX += 0.1f;
		}
		else
		{
			if (GetKeyState('Z') & 0x8000)
				m_fRotX -= 10.f;
			else
				m_fRotX -= 0.1f;
		}

		if (m_fRotX <= -360.f)
			m_fRotX = -360.f;

		if (m_fRotX >= 360.f)
			m_fRotX = 360.f;

	

		dynamic_cast<CStaticObject*>(pSelectedObj)->GetTransformCom()->Set_Rotation_YawPitchRoll(D3DXToRadian(m_fRotX), D3DXToRadian(m_fRotY), D3DXToRadian(m_fRotZ));
		
		auto iter = find_if(CDataManager::GetInstance()->m_MapMeshData.begin(), CDataManager::GetInstance()->m_MapMeshData.end(), CFinder_Tag(strCurrentSelectedObjectName));

		D3DXMatrixIdentity(&iter->second[iCurrentSelcetedIndex - 1].matWorld);
		iter->second[iCurrentSelcetedIndex - 1].matWorld = *dynamic_cast<CStaticObject*>(pSelectedObj)->GetTransformCom()->Get_WorldMatrixPointer();

		UpdateData(FALSE);
	}

	*pResult = 0;
}


void CMeshTab::OnSpin_Rotate_Y(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if (bDblClkTreeStaticObj == true
		|| bDblClkTreeDynamicObj == true)
	{
		UpdateData(TRUE);

		if (pNMUpDown->iDelta < 0)
		{
			if (GetKeyState('Z') & 0x8000)
				m_fRotY += 10.f;
			else
				m_fRotY += 0.1f;
		}
		else
		{
			if (GetKeyState('Z') & 0x8000)
				m_fRotY -= 10.f;
			else
				m_fRotY -= 0.1f;
		}

		if (m_fRotY <= -360.f)
			m_fRotY = -360.f;

		if (m_fRotY >= 360.f)
			m_fRotY = 360.f;


		dynamic_cast<CStaticObject*>(pSelectedObj)->GetTransformCom()->Set_Rotation_YawPitchRoll(D3DXToRadian(m_fRotX), D3DXToRadian(m_fRotY), D3DXToRadian(m_fRotZ));

		auto iter = find_if(CDataManager::GetInstance()->m_MapMeshData.begin(), CDataManager::GetInstance()->m_MapMeshData.end(), CFinder_Tag(strCurrentSelectedObjectName));

		D3DXMatrixIdentity(&iter->second[iCurrentSelcetedIndex - 1].matWorld);
		iter->second[iCurrentSelcetedIndex - 1].matWorld = *dynamic_cast<CStaticObject*>(pSelectedObj)->GetTransformCom()->Get_WorldMatrixPointer();

		UpdateData(FALSE);
	}

	*pResult = 0;
}


void CMeshTab::OnSpin_Rotate_Z(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if (bDblClkTreeStaticObj == true
		|| bDblClkTreeDynamicObj == true)
	{
		UpdateData(TRUE);

		if (pNMUpDown->iDelta < 0)
		{
			if (GetKeyState('Z') & 0x8000)
				m_fRotZ += 10.f;
			else
				m_fRotZ += 0.1f;
		}
		else
		{
			if (GetKeyState('Z') & 0x8000)
				m_fRotZ -= 10.f;
			else
				m_fRotZ -= 0.1f;
		}

		if (m_fRotZ <= -360.f)
			m_fRotZ = -360.f;

		if (m_fRotZ >= 360.f)
			m_fRotZ = 360.f;

		dynamic_cast<CStaticObject*>(pSelectedObj)->GetTransformCom()->Set_Rotation_YawPitchRoll(D3DXToRadian(m_fRotX), D3DXToRadian(m_fRotY), D3DXToRadian(m_fRotZ));

		auto iter = find_if(CDataManager::GetInstance()->m_MapMeshData.begin(), CDataManager::GetInstance()->m_MapMeshData.end(), CFinder_Tag(strCurrentSelectedObjectName));

		D3DXMatrixIdentity(&iter->second[iCurrentSelcetedIndex - 1].matWorld);
		iter->second[iCurrentSelcetedIndex - 1].matWorld = *dynamic_cast<CStaticObject*>(pSelectedObj)->GetTransformCom()->Get_WorldMatrixPointer();

		UpdateData(FALSE);

	}

	*pResult = 0;
}


void CMeshTab::OnSpin_Position_X(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if (bDblClkTreeStaticObj == true
		|| bDblClkTreeDynamicObj == true)
	{
		UpdateData(TRUE);

		if (pNMUpDown->iDelta < 0)
		{
			if (GetKeyState('Z') & 0x8000)
				m_fPosX += 1.f;
			else
				m_fPosX += 0.1f;
		}
		else
		{
			if (GetKeyState('Z') & 0x8000)
				m_fPosX -= 1.f;
			else
				m_fPosX -= 0.1f;
		}

		if (m_fPosX <= -10000.f)
			m_fPosX = -10000.f;

		if (m_fPosX >= 10000.f)
			m_fPosX = 10000.f;

		UpdateData(FALSE);

		dynamic_cast<CStaticObject*>(pSelectedObj)->GetTransformCom()->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(m_fPosX, m_fPosY, m_fPosZ));

		auto iter = find_if(CDataManager::GetInstance()->m_MapMeshData.begin(), CDataManager::GetInstance()->m_MapMeshData.end(), CFinder_Tag(strCurrentSelectedObjectName));

		D3DXMatrixIdentity(&iter->second[iCurrentSelcetedIndex - 1].matWorld);
		iter->second[iCurrentSelcetedIndex - 1].matWorld = *dynamic_cast<CStaticObject*>(pSelectedObj)->GetTransformCom()->Get_WorldMatrixPointer();

	}

	*pResult = 0;
}

void CMeshTab::OnSpin_Position_Y(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if (bDblClkTreeStaticObj == true
		|| bDblClkTreeDynamicObj == true)
	{
		UpdateData(TRUE);

		if (pNMUpDown->iDelta < 0)
		{
			if (GetKeyState('Z') & 0x8000)
				m_fPosY += 1.f;
			else
				m_fPosY += 0.1f;
		}
		else
		{
			if (GetKeyState('Z') & 0x8000)
				m_fPosY -= 1.f;
			else
				m_fPosY -= 0.1f;
		}

		if (m_fPosY <= -10000.f)
			m_fPosY = -10000.f;

		if (m_fPosY >= 10000.f)
			m_fPosY = 10000.f;

		UpdateData(FALSE);

		dynamic_cast<CStaticObject*>(pSelectedObj)->GetTransformCom()->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(m_fPosX, m_fPosY, m_fPosZ));

		auto iter = find_if(CDataManager::GetInstance()->m_MapMeshData.begin(), CDataManager::GetInstance()->m_MapMeshData.end(), CFinder_Tag(strCurrentSelectedObjectName));

		D3DXMatrixIdentity(&iter->second[iCurrentSelcetedIndex - 1].matWorld);
		iter->second[iCurrentSelcetedIndex - 1].matWorld = *dynamic_cast<CStaticObject*>(pSelectedObj)->GetTransformCom()->Get_WorldMatrixPointer();

	}

	*pResult = 0;
}


void CMeshTab::OnSpin_Position_Z(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if (bDblClkTreeStaticObj == true
		|| bDblClkTreeDynamicObj == true)
	{
		UpdateData(TRUE);

		if (pNMUpDown->iDelta < 0)
		{
			if (GetKeyState('Z') & 0x8000)
				m_fPosZ += 1.f;
			else
				m_fPosZ += 0.1f;
		}
		else
		{
			if (GetKeyState('Z') & 0x8000)
				m_fPosZ -= 1.f;
			else
				m_fPosZ -= 0.1f;
		}

		if (m_fPosZ <= -10000.f)
			m_fPosZ = -10000.f;

		if (m_fPosZ >= 10000.f)
			m_fPosZ = 10000.f;

		UpdateData(FALSE);

		dynamic_cast<CStaticObject*>(pSelectedObj)->GetTransformCom()->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(m_fPosX, m_fPosY, m_fPosZ));

		auto iter = find_if(CDataManager::GetInstance()->m_MapMeshData.begin(), CDataManager::GetInstance()->m_MapMeshData.end(), CFinder_Tag(strCurrentSelectedObjectName));

		D3DXMatrixIdentity(&iter->second[iCurrentSelcetedIndex - 1].matWorld);
		iter->second[iCurrentSelcetedIndex - 1].matWorld = *dynamic_cast<CStaticObject*>(pSelectedObj)->GetTransformCom()->Get_WorldMatrixPointer();

	}

	*pResult = 0;
}


void CMeshTab::OnBnClicked_Mesh_Save()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	TCHAR	szDirPath[MAX_PATH];
	CString strName = L"";
	GetCurrentDirectory(MAX_PATH, szDirPath);

	CFileDialog		Dlg(FALSE,	// false인 경우 save, true인 경우 load
		L"MeshDat",	// 파일의 확장자명	
		L"*.MeshDat", // 창에 최초로 띄워줄 파일이름 문자열
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, // 중복 파일이 있을 경우 경고창 띄워주기
		L"*.MeshDat", // 저장 시 지원하는 파일 형식
		this);

	if (Dlg.DoModal() == IDOK)
	{
		strName = Dlg.GetPathName();
		SetCurrentDirectory(szDirPath);
	}
	else return;

	HANDLE	hFile = CreateFile(strName,
		GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	DWORD		dwByte;

	for (auto& Pair : CDataManager::GetInstance()->m_MapMeshData)
	{
		for (auto& iter : Pair.second)
		{
			WriteFile(hFile, &iter.bIsStaticMesh, sizeof(iter.bIsStaticMesh), &dwByte, NULL);
			WriteFile(hFile, &iter.matWorld, sizeof(iter.matWorld), &dwByte, NULL);

			_tchar szTextInfo[MAX_PATH] = L"";
			lstrcpy(szTextInfo, iter.strObjProtoTag.c_str());
			_int iTextLength = lstrlen(szTextInfo) + 1;

			WriteFile(hFile, &iTextLength, sizeof(_int), &dwByte, NULL);
			WriteFile(hFile, szTextInfo, sizeof(_tchar) * iTextLength, &dwByte, NULL);

			lstrcpy(szTextInfo, iter.strLayerTag.c_str());
			iTextLength = lstrlen(szTextInfo) + 1;

			WriteFile(hFile, &iTextLength, sizeof(_int), &dwByte, NULL);
			WriteFile(hFile, szTextInfo, sizeof(_tchar) * iTextLength, &dwByte, NULL);

			lstrcpy(szTextInfo, iter.strComProtoTag.c_str());
			iTextLength = lstrlen(szTextInfo) + 1;

			WriteFile(hFile, &iTextLength, sizeof(_int), &dwByte, NULL);
			WriteFile(hFile, szTextInfo, sizeof(_tchar) * iTextLength, &dwByte, NULL);

			//(iter.strFullPath).erase(0, 34);
			//iter.strFullPath = L"../Bin/Resources/Meshes/StaticMesh/TerrainObject" + iter.strFullPath;;

			lstrcpy(szTextInfo, iter.strFullPath.c_str());
			iTextLength = lstrlen(szTextInfo) + 1;

			WriteFile(hFile, &iTextLength, sizeof(_int), &dwByte, NULL);
			WriteFile(hFile, szTextInfo, sizeof(_tchar) * iTextLength, &dwByte, NULL);

			int i = 0;
		}
	}

	CloseHandle(hFile);
}


void CMeshTab::OnBnClicked_Mesh_Load()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	TCHAR	szDirPath[MAX_PATH];
	CString strName = L"";
	GetCurrentDirectory(MAX_PATH, szDirPath);

	CFileDialog		Dlg(TRUE,	// false인 경우 save, true인 경우 load
		L"MeshDat",	// 파일의 확장자명	
		L"*.MeshDat", // 창에 최초로 띄워줄 파일이름 문자열
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, // 중복 파일이 있을 경우 경고창 띄워주기
		L"*.MeshDat", // 저장 시 지원하는 파일 형식
		this);

	CDataManager::GetInstance()->Free();

	// 혜현이가 도와줌 ㅎ
	if (Dlg.DoModal() == IDOK)
	{
		strName = Dlg.GetPathName();
		SetCurrentDirectory(szDirPath);
	}

	HANDLE	hFile = CreateFile(Dlg.GetPathName(),
		GENERIC_READ,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	DWORD		dwByte;

	Tree_Mesh_StaticObj.DeleteAllItems();
	Tree_Mesh_DynamicObj.DeleteAllItems();

	while (true)
	{
		CString		ObjectName;

		_int	iTextLength = 0;
		_tchar	szTextTemp[MAX_PATH] = L"";

		OBJECTMESHDATA tObjMeshData;

		ReadFile(hFile, &tObjMeshData.bIsStaticMesh, sizeof(_bool), &dwByte, NULL);
		ReadFile(hFile, &tObjMeshData.matWorld, sizeof(_matrix), &dwByte, NULL);

		ReadFile(hFile, &iTextLength, sizeof(_int), &dwByte, NULL);
		ReadFile(hFile, szTextTemp, sizeof(_tchar) * iTextLength, &dwByte, NULL);

		tObjMeshData.strObjProtoTag = szTextTemp;

		ReadFile(hFile, &iTextLength, sizeof(_int), &dwByte, NULL);
		ReadFile(hFile, szTextTemp, sizeof(_tchar) * iTextLength, &dwByte, NULL);

		tObjMeshData.strLayerTag = szTextTemp;
		ObjectName = tObjMeshData.strLayerTag.c_str();

		ObjectName.TrimLeft(L"Layer_");

		strObjectName = ObjectName;

		ReadFile(hFile, &iTextLength, sizeof(_int), &dwByte, NULL);
		ReadFile(hFile, szTextTemp, sizeof(_tchar) * iTextLength, &dwByte, NULL);

		tObjMeshData.strComProtoTag = szTextTemp;

		ReadFile(hFile, &iTextLength, sizeof(_int), &dwByte, NULL);
		ReadFile(hFile, szTextTemp, sizeof(_tchar) * iTextLength, &dwByte, NULL);

		tObjMeshData.strFullPath = szTextTemp;

		if (0 == dwByte)
			break;

		bIsStaticMesh = tObjMeshData.bIsStaticMesh;
		strObjectName = ObjectName;
		strLayerTag = tObjMeshData.strLayerTag.c_str();
		strComponentPrototypeTag = tObjMeshData.strComProtoTag.c_str();
		lstrcpy(szFullPath, tObjMeshData.strFullPath.c_str());

		MakeItemForTree();

		CLayer* pStaticObjLayer = CObject_Manager::GetInstance()->FindObjectLayer(SCENE_STATIC, strLayerTag);
	//	dynamic_cast<CStaticObject*>(pStaticObjLayer->Get_ObjectList().back())->SetState(*(_vec3*)&tObjMeshData.matWorld.m[3], _vec3(tObjMeshData.matWorld.m[0][0], tObjMeshData.matWorld.m[1][1], tObjMeshData.matWorld.m[2][2]));

		dynamic_cast<CStaticObject*>(pStaticObjLayer->Get_ObjectList().back())->SetWorldMatrix(tObjMeshData.matWorld);

		vector<OBJECTMESHDATA> vObjMeshData;

		auto iter = find_if(CDataManager::GetInstance()->m_MapMeshData.begin(), CDataManager::GetInstance()->m_MapMeshData.end(), CFinder_Tag(strObjectName));
		
		if (iter == CDataManager::GetInstance()->m_MapMeshData.end())
		{
			vObjMeshData.push_back(tObjMeshData);
			CDataManager::GetInstance()->m_MapMeshData.emplace(ObjectName, vObjMeshData);
		}
		else
			(*iter).second.push_back(tObjMeshData);
	}

	CloseHandle(hFile);

	Invalidate(false);

	UpdateData(FALSE);
}


void CMeshTab::OnBnClicked_Navi_Save()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	TCHAR	szDirPath[MAX_PATH];
	CString strName = L"";
	GetCurrentDirectory(MAX_PATH, szDirPath);

	CFileDialog		Dlg(FALSE,	// false인 경우 save, true인 경우 load
		L"NaviDat",	// 파일의 확장자명	
		L"*.NaviDat", // 창에 최초로 띄워줄 파일이름 문자열
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, // 중복 파일이 있을 경우 경고창 띄워주기
		L"*.NaviDat", // 저장 시 지원하는 파일 형식
		this);

	if (Dlg.DoModal() == IDOK)
	{
		strName = Dlg.GetPathName();
		SetCurrentDirectory(szDirPath);
	}
	else return;

	HANDLE	hFile = CreateFile(strName,
		GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	DWORD		dwByte;

	for (auto& Pair : mapNaviMesh)
	{
		for (auto& iter : Pair.second)
		{
			//WriteFile(hFile, iter, sizeof(_vec3) * 3, &dwByte, nullptr);
			WriteFile(hFile, &iter, sizeof(_vec3), &dwByte, nullptr);
		}
	}

	CloseHandle(hFile);
	
}

void CMeshTab::OnBnClicked_Navi_Load()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	TCHAR	szDirPath[MAX_PATH];
	CString strName = L"";
	GetCurrentDirectory(MAX_PATH, szDirPath);

	CFileDialog		Dlg(TRUE,	// false인 경우 save, true인 경우 load
		L"NaviDat",	// 파일의 확장자명	
		L"*.NaviDat", // 창에 최초로 띄워줄 파일이름 문자열
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, // 중복 파일이 있을 경우 경고창 띄워주기
		L"*.NaviDat", // 저장 시 지원하는 파일 형식
		this);

	mapNaviMesh.clear();

	// 혜현이가 도와줌 ㅎ
	if (Dlg.DoModal() == IDOK)
	{
		strName = Dlg.GetPathName();
		SetCurrentDirectory(szDirPath);
	}

	HANDLE	hFile = CreateFile(Dlg.GetPathName(),
		GENERIC_READ,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	DWORD		dwByte;

	_vec3		vPoint[3];

	HTREEITEM	hRoot = nullptr;

	while (true)
	{
		ReadFile(hFile, vPoint, sizeof(_vec3) * 3, &dwByte, nullptr);

		if (0 == dwByte)
			break;

		vecPos.push_back(vPoint[0]);
		vecPos.push_back(vPoint[1]);
		vecPos.push_back(vPoint[2]);

		CString strRoot;
		strRoot.Format(_T("[%d]"), iNaviRootTreeCount++);

		hRoot = Tree_Mesh_Navi.InsertItem(strRoot, 0, 0, TVI_ROOT, TVI_LAST);

		CString strChild;
		strChild.Format(_T("%d"), iNaviChildTreeCount);
		Tree_Mesh_Navi.InsertItem(strChild, 0, 0, hRoot, TVI_LAST);

		++iNaviChildTreeCount;
		strChild.Format(_T("%d"), iNaviChildTreeCount);
		Tree_Mesh_Navi.InsertItem(strChild, 0, 0, hRoot, TVI_LAST);

		++iNaviChildTreeCount;
		strChild.Format(_T("%d"), iNaviChildTreeCount);
		Tree_Mesh_Navi.InsertItem(strChild, 0, 0, hRoot, TVI_LAST);

		iNaviChildTreeCount = 1;

		mapNaviMesh.emplace(iNaviMapCount++, vecPos);
		vecPos.clear();
	}

	CloseHandle(hFile);
}
