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
	DDV_MinMaxFloat(pDX, m_fScalingX, 0.01, 100);
	DDX_Text(pDX, IDC_EDIT6, m_fScalingY);
	DDV_MinMaxFloat(pDX, m_fScalingY, 0.01, 100);
	DDX_Text(pDX, IDC_EDIT7, m_fScalingZ);
	DDV_MinMaxFloat(pDX, m_fScalingZ, 0.01, 100);
	DDX_Text(pDX, IDC_EDIT8, m_fRotX);
	DDV_MinMaxFloat(pDX, m_fRotX, -360, 360);
	DDX_Text(pDX, IDC_EDIT9, m_fRotY);
	DDV_MinMaxFloat(pDX, m_fRotY, -360, 360);
	DDX_Text(pDX, IDC_EDIT10, m_fRotZ);
	DDV_MinMaxFloat(pDX, m_fRotZ, -360, 360);
	DDX_Text(pDX, IDC_EDIT11, m_fPosX);
	DDV_MinMaxFloat(pDX, m_fPosX, -10000, 10000);
	DDX_Text(pDX, IDC_EDIT12, m_fPosY);
	DDV_MinMaxFloat(pDX, m_fPosY, -10000, 10000);
	DDX_Text(pDX, IDC_EDIT13, m_fPosZ);
	DDV_MinMaxFloat(pDX, m_fPosZ, -10000, 10000);
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
END_MESSAGE_MAP()

BOOL CMeshTab::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	Mesh_RenderST_Solild.SetCheck(TRUE);
	Mesh_MouseSelType_Object.SetCheck(TRUE);
	Mesh_ObjectType_Static.SetCheck(TRUE);
	Mesh_NaviVtxMove_Together.SetCheck(TRUE);

	//Obj_X_Scaling_Btn.SetRange(0, 360);
	// SetPos로 EditBox의 초기 텍스트 값을 지정하고 싶다면,
	// Spin_Control의 속성 값에 Auto_Buddy값과 Set_Buddy_Integer값을 True로 바꾸고,
	// 리소스 뷰 다이얼로그에서 Ctrl+D를 눌러서,
	// Edit Box와 Spin Control이 연속된 숫자를 가지게 셋팅하면 된다.
	Obj_X_Scaling_Btn.SetPos(1.f);
	Obj_X_Scaling_Btn.SetRange(100.f, 0.01f);

	Obj_Y_Scaling_Btn.SetPos(1.f);
	Obj_Y_Scaling_Btn.SetRange(100.f, 0.01f);

	Obj_Z_Scaling_Btn.SetPos(1.f);
	Obj_Z_Scaling_Btn.SetRange(100.f, 0.01f);

	Obj_X_Rotation_Btn.SetPos(0.f);
	Obj_X_Rotation_Btn.SetRange(360.f, -360.f);

	Obj_Y_Rotation_Btn.SetPos(0.f);
	Obj_Y_Rotation_Btn.SetRange(360.f, -360.f);

	Obj_Z_Rotation_Btn.SetPos(0.f);
	Obj_Z_Rotation_Btn.SetRange(360.f, -360.f);

	Obj_X_Position_Btn.SetPos(0.f);
	Obj_X_Position_Btn.SetRange(10000.f, -10000.f);

	Obj_Y_Position_Btn.SetPos(0.f);
	Obj_Y_Position_Btn.SetRange(10000.f, -10000.f);

	Obj_Z_Position_Btn.SetPos(0.f);
	Obj_Z_Position_Btn.SetRange(10000.f, -10000.f);

	InitTreeCtrl_Object();

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
}


void CMeshTab::OnBnClickedNavi_Mesh()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CMeshTab::OnBnClickedStatic()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CMeshTab::OnBnClickedDynamic()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CMeshTab::OnBnClickedTogether()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CMeshTab::OnBnClickedSelected()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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

	if (NULL == Tree_Mesh_StaticObj.GetRootItem())
		Root = Tree_Mesh_StaticObj.InsertItem(TEXT("Static_Mesh"), 0, 0, TVI_ROOT, TVI_LAST);

	if (NOERROR == Add_StaticObject())
	{
		auto iter = find_if(mapTreeItem.begin(), mapTreeItem.end(), CFinder_Tag(strObjectName));

		if (iter == mapTreeItem.end())
		{
			CString ObjectNameTemp1 = strObjectName;
			CString ItemNum;
			int iItemNum = 1;

			ItemNum.Format(_T(" [%d]"), iItemNum);

			ObjectNameTemp1 += ItemNum;

			hChild1 = Tree_Mesh_StaticObj.InsertItem(strObjectName, 0, 0, Root, TVI_LAST);
			Tree_Mesh_StaticObj.InsertItem(ObjectNameTemp1, 0, 0, hChild1, TVI_LAST);

			mapTreeItem.emplace(strObjectName, hChild1);

			strObjectName = _T("");
			ObjectNameTemp1 = _T("");
			ItemNum = _T("");
		}
		else
		{
			auto iter = find_if(mapTreeItem.begin(), mapTreeItem.end(), CFinder_Tag(strObjectName));

			if (iter != mapTreeItem.end())
			{
				CString ObjectNameTemp2 = strObjectName;
				CString ItemNum;
				CObject_Manager* pObjectManager = CObject_Manager::GetInstance();

				int iItemNum = pObjectManager->FindObjectLayer(SCENE_STATIC, strLayerTag)->Get_ObjectList().size();

				ItemNum.Format(_T(" [%d]"), iItemNum);

				ObjectNameTemp2 += ItemNum;

				Tree_Mesh_StaticObj.InsertItem(ObjectNameTemp2, 0, 0, iter->second, TVI_LAST);

				//strLayerTag = _T("");
				ObjectNameTemp2 = _T("");
				ItemNum = _T("");
			}
		}
	}

	UpdateData(FALSE);

	return NOERROR;
}

HRESULT CMeshTab::Add_StaticObject()
{
	m_pScene = CViewManager::GetInstance()->m_pCurScene;

	if (FAILED(MakeArgVariableForStaticObj()))
		return E_FAIL;	

	// 컴포넌트 프로토 타입도 최초 한번만 생성해야 하므로..
	// find_if 함수를 사용하여 strComponentPrototypeTag를 통해 iter를 검색해서,
	auto iter = find_if(mapStaticObj_Com_Prototype.begin(), mapStaticObj_Com_Prototype.end(), CFinder_Tag(strComponentPrototypeTag));

	// 검색이 안되었다면, 만들어진적이 없는 최초 생성이므로,
	if (iter == mapStaticObj_Com_Prototype.end())
	{
		// 생성한다.
		// Add_Component_Prototype에 해당한다. (원본객체)
		if (FAILED(dynamic_cast<CSceneStatic*>(m_pScene)->Add_Static_Object_Component_Prototype(strComponentPrototypeTag, szFullPath, strXfileName)))
			return E_FAIL;

		mapStaticObj_Com_Prototype.emplace(strComponentPrototypeTag, strXfileName);

		strXfileName = _T("");
	}
	
	if (FAILED(dynamic_cast<CSceneStatic*>(m_pScene)->Add_Static_Object(strLayerTag)))
		return E_FAIL;

	// Add_Component에 해당한다. (실 사용 객체)
	CLayer* pLayer = CObject_Manager::GetInstance()->FindObjectLayer(SCENE_STATIC, strLayerTag);
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
	strComponentPrototypeTag = _T("Component_Mesh_Static_") + strLayerTag;

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
	strItemIdx.TrimRight(L"]");

	_int	  iItemIdx = _ttoi(strItemIdx.Right(1));

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

	if (bDblClkTreeStaticObj == true)
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
	}

	*pResult = 0;
}


void CMeshTab::OnSpin_Scaling_Y(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if (bDblClkTreeStaticObj == true)
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
	}

	*pResult = 0;
}


void CMeshTab::OnSpin_Scaling_Z(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if (bDblClkTreeStaticObj == true)
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
	}
	
	*pResult = 0;
}


void CMeshTab::OnSpin_Rotate_X(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if (bDblClkTreeStaticObj == true)
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

		UpdateData(FALSE);

		dynamic_cast<CStaticObject*>(pSelectedObj)->GetTransformCom()->Set_Rotation_YawPitchRoll(D3DXToRadian(m_fRotX), D3DXToRadian(m_fRotY), D3DXToRadian(m_fRotZ));
	}

	*pResult = 0;
}


void CMeshTab::OnSpin_Rotate_Y(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if (bDblClkTreeStaticObj == true)
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

		UpdateData(FALSE);

		dynamic_cast<CStaticObject*>(pSelectedObj)->GetTransformCom()->Set_Rotation_YawPitchRoll(D3DXToRadian(m_fRotX), D3DXToRadian(m_fRotY), D3DXToRadian(m_fRotZ));
	}

	*pResult = 0;
}


void CMeshTab::OnSpin_Rotate_Z(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if (bDblClkTreeStaticObj == true)
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

		UpdateData(FALSE);

		dynamic_cast<CStaticObject*>(pSelectedObj)->GetTransformCom()->Set_Rotation_YawPitchRoll(D3DXToRadian(m_fRotX), D3DXToRadian(m_fRotY), D3DXToRadian(m_fRotZ));
	}

	*pResult = 0;
}


void CMeshTab::OnSpin_Position_X(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if (bDblClkTreeStaticObj == true)
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
	}

	*pResult = 0;
}

void CMeshTab::OnSpin_Position_Y(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if (bDblClkTreeStaticObj == true)
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
	}

	*pResult = 0;
}


void CMeshTab::OnSpin_Position_Z(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if (bDblClkTreeStaticObj == true)
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
	}

	*pResult = 0;
}
