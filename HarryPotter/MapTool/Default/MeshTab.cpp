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
	ON_NOTIFY(NM_DBLCLK, IDC_TREE1, &CMeshTab::OnNMDblclkTreeMeshObject)
END_MESSAGE_MAP()

BOOL CMeshTab::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	Mesh_RenderST_Solild.SetCheck(TRUE);
	Mesh_MouseSelType_Object.SetCheck(TRUE);
	Mesh_ObjectType_Static.SetCheck(TRUE);
	Mesh_NaviVtxMove_Together.SetCheck(TRUE);

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

	//CLayer* pLayer = CObject_Manager::GetInstance()->FindObjectLayer(SCENE_STATIC, L"Layer_");
	//dynamic_cast<CStaticObject*>(pLayer->Get_ObjectList().back())->SetState();
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

HRESULT CMeshTab::Add_StaticObject()
{
	m_pScene = CViewManager::GetInstance()->m_pCurScene;

	TCHAR		szFullPath[MAX_PATH] = L"";
	CString		pathSelected = L"";
	HTREEITEM	hParentItem = SelectedItem;

	lstrcpy((LPWSTR)pathSelected.operator LPCWSTR(), L"");

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
			strObjectName = strLayerTag = pathSelected;
			lstrcpy((LPWSTR)strXfileName.operator LPCWSTR(), strLayerTag);
			pathSelected = L"/";
		}

		hParentItem = Tree_Mesh_Object.GetParentItem(hParentItem);
		
		if (hParentItem == NULL)
			break;

		if (pathSelected != L"/")
			pathSelected = _T("/") + pathSelected;
	}

	//ZeroMemory(szFullPath, sizeof(szFullPath));
	StrCpyW(szFullPath, pathSelected);

	PathRemoveExtension((LPWSTR)strLayerTag.operator LPCWSTR());	// 확장자명을 잘라내는 함수

	// Component의 PrototypeTag를 만들어 준다.
	strComponentPrototypeTag = _T("Component_Mesh_Static_") + strLayerTag;

	// LayerTag를 만들어 준다.
	strLayerTag = _T("Layer_") + strLayerTag;


	// Add_Component_Prototype에 해당한다. (원본객체)
z	if (FAILED(dynamic_cast<CSceneStatic*>(m_pScene)->Add_Static_Object_Component_Prototype(strComponentPrototypeTag, szFullPath, strXfileName)))
		return E_FAIL;

	if (FAILED(dynamic_cast<CSceneStatic*>(m_pScene)->Add_Static_Object(strLayerTag)))
		return E_FAIL;

	// Add_Component에 해당한다. (실 사용 객체)
	CLayer* pLayer = CObject_Manager::GetInstance()->FindObjectLayer(SCENE_STATIC, strLayerTag);
	dynamic_cast<CStaticObject*>(pLayer->Get_ObjectList().back())->Add_Component_Tool(strComponentPrototypeTag);


	return NOERROR;
}

void CMeshTab::OnTree_Mesh_Object(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	//TCHAR		 szFullPath[MAX_PATH] = L"";

	// 현재 트리 컨트롤에서 선택된 아이템을 불러온다.
	HTREEITEM	hSelected = pNMTreeView->itemNew.hItem;

	SelectedItem = hSelected;

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

			//if (finder.IsDirectory())
			Tree_Mesh_Object.InsertItem(finder.GetFileName(), hSelected);
		}
		Tree_Mesh_Object.SetItemData(hSelected, 1);
		Tree_Mesh_Object.EnsureVisible(hSelected);
	}
	//else // 똑같은 아이템을 다시 눌렀을 시.
	//{
	//	CString	pathSelected;
	//	HTREEITEM hParentItem = hSelected;
	//}
	UpdateData(FALSE);


	*pResult = 0;
}

void CMeshTab::OnNMDblclkTreeMeshObject(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	HTREEITEM	Root = nullptr;

	if (NULL == Tree_Mesh_StaticObj.GetRootItem())
		Root = Tree_Mesh_StaticObj.InsertItem(TEXT("Static_Mesh"), 0, 0, TVI_ROOT, TVI_LAST);

	if(NOERROR == Add_StaticObject())
		Tree_Mesh_StaticObj.InsertItem(strObjectName, 0, 0, Root, TVI_LAST);

	*pResult = 0;
}

void CMeshTab::OnTree_Mesh_StaticObj(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	


	*pResult = 0;
}
