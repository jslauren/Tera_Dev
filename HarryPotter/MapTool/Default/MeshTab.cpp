// MeshTab.cpp : ���� �����Դϴ�.
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

// CMeshTab ��ȭ �����Դϴ�.

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
	DDX_Control(pDX, IDC_TREE4, Tree_Mesh_DynamicObj);
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
END_MESSAGE_MAP()

BOOL CMeshTab::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	Mesh_RenderST_Solild.SetCheck(TRUE);
	Mesh_MouseSelType_Object.SetCheck(TRUE);
	Mesh_ObjectType_Static.SetCheck(TRUE);
	Mesh_NaviVtxMove_Together.SetCheck(TRUE);

	//Obj_X_Scaling_Btn.SetRange(0, 360);
	// SetPos�� EditBox�� �ʱ� �ؽ�Ʈ ���� �����ϰ� �ʹٸ�,
	// Spin_Control�� �Ӽ� ���� Auto_Buddy���� Set_Buddy_Integer���� True�� �ٲٰ�,
	// ���ҽ� �� ���̾�α׿��� Ctrl+D�� ������,
	// Edit Box�� Spin Control�� ���ӵ� ���ڸ� ������ �����ϸ� �ȴ�.
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
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

// CMeshTab �޽��� ó�����Դϴ�.
BOOL CMeshTab::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	//// ���� �� ESC Ű �Է� �� �������� ������� ���� �ذ� ���� ////

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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

}


void CMeshTab::OnBnClickedWireFrame()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CMeshTab::OnBnClickedObject()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CMeshTab::OnBnClickedNavi_Mesh()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CMeshTab::OnBnClickedStatic()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	bIsStaticMesh = true;

}


void CMeshTab::OnBnClickedDynamic()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	bIsStaticMesh = false;

}


void CMeshTab::OnBnClickedTogether()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CMeshTab::OnBnClickedSelected()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void CMeshTab::OnBnClicked_StaticObject_Delete()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	// Static_Object Tree���� ���� ���õ� �������� �θ� �������� �������� ����.
	// �̰� ���ִ� ������ ���� Ʈ���� �������� "������Ʈ�� [n]" �̷��� �Ǵµ�,
	// �ڸ��� �����Ƽ� �׳� �θ��� �������� �������� �ϱ� �����̴�.
	HTREEITEM hSelectedParentItem = Tree_Mesh_StaticObj.GetParentItem(SelectedStaticObject);

	// �θ� ������ �׷��ϱ� �� ������Ʈ �̸����ְ� �ε����� �پ����� ���� ������ �ؽ�Ʈ�� �����´�.
	CString ParentItemName = Tree_Mesh_StaticObj.GetItemText(hSelectedParentItem);

	// �̰� ���� ���õ� Static Object�� �ε����� ���°���� �߶���� �����̴�.
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

	_int iItemIdx = _ttoi(strItemIdx);

	if (iLatestItemIdx < iItemIdx)
		iLatestItemIdx = iItemIdx;

	ParentItemName = _T("Layer_") + ParentItemName;

	CLayer* pLayer = CObject_Manager::GetInstance()->FindObjectLayer(SCENE_STATIC, ParentItemName);
	auto& iter = pLayer->Get_ObjectList().begin();

	// Ʈ���� ������ ����.
	Tree_Mesh_StaticObj.DeleteItem(SelectedStaticObject);

	// ������ ++�� Iter ��ġ �ʱ�ȭ.
	iter = pLayer->Get_ObjectList().begin();

	for (; iter != pLayer->Get_ObjectList().end(); )
	{
		if ((*iter)->Get_IdxNum() == iItemIdx)
		{
			Safe_Release(*iter);
			iter = pLayer->Get_ObjectList().erase(iter);

			auto iter = find_if(CDataManager::GetInstance()->m_MapMeshData.begin(), CDataManager::GetInstance()->m_MapMeshData.end(), CFinder_Tag(CDataManager::GetInstance()->m_strObjName));

			iter->second.erase(iter->second.begin() + (iItemIdx-1));

			break;
		}
		else
			++iter;
	}

	Invalidate(false);
}

void CMeshTab::OnBnClicked_DynamicObject_Delete()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	// Static_Object Tree���� ���� ���õ� �������� �θ� �������� �������� ����.
	// �̰� ���ִ� ������ ���� Ʈ���� �������� "������Ʈ�� [n]" �̷��� �Ǵµ�,
	// �ڸ��� �����Ƽ� �׳� �θ��� �������� �������� �ϱ� �����̴�.
	HTREEITEM hSelectedParentItem = Tree_Mesh_DynamicObj.GetParentItem(SelectedDynamicObject);

	// �θ� ������ �׷��ϱ� �� ������Ʈ �̸����ְ� �ε����� �پ����� ���� ������ �ؽ�Ʈ�� �����´�.
	CString ParentItemName = Tree_Mesh_DynamicObj.GetItemText(hSelectedParentItem);

	// �̰� ���� ���õ� Static Object�� �ε����� ���°���� �߶���� �����̴�.
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

	_int iItemIdx = _ttoi(strItemIdx);

	if (iLatestItemIdx < iItemIdx)
		iLatestItemIdx = iItemIdx;

	ParentItemName = _T("Layer_") + ParentItemName;

	CLayer* pLayer = CObject_Manager::GetInstance()->FindObjectLayer(SCENE_STATIC, ParentItemName);
	auto& iter = pLayer->Get_ObjectList().begin();

	// Ʈ���� ������ ����.
	Tree_Mesh_DynamicObj.DeleteItem(SelectedDynamicObject);

	// ������ ++�� Iter ��ġ �ʱ�ȭ.
	iter = pLayer->Get_ObjectList().begin();

	for (; iter != pLayer->Get_ObjectList().end(); )
	{
		if ((*iter)->Get_IdxNum() == iItemIdx)
		{
			Safe_Release(*iter);
			iter = pLayer->Get_ObjectList().erase(iter);
			break;
		}
		else
			++iter;
	}

	Invalidate(false);
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
			Root = Tree_Mesh_StaticObj.InsertItem(TEXT("Static_Mesh"), 0, 0, TVI_ROOT, TVI_LAST);
	}
	else
	{
		if (NULL == Tree_Mesh_DynamicObj.GetRootItem())
			Root = Tree_Mesh_DynamicObj.InsertItem(TEXT("Dynamic_Mesh"), 0, 0, TVI_ROOT, TVI_LAST);
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
				hChild1 = Tree_Mesh_StaticObj.InsertItem(strObjectName, 0, 0, Root, TVI_LAST);
				Tree_Mesh_StaticObj.InsertItem(ObjectNameTemp1, 0, 0, hChild1, TVI_LAST);
			}
			else
			{
				hChild1 = Tree_Mesh_DynamicObj.InsertItem(strObjectName, 0, 0, Root, TVI_LAST);
				Tree_Mesh_DynamicObj.InsertItem(ObjectNameTemp1, 0, 0, hChild1, TVI_LAST);
			}

			mapTreeItem.emplace(strObjectName, hChild1);

			//strObjectName = _T("");
			ObjectNameTemp1 = _T("");
			ItemNum = _T("");
		}
		else
		{
			auto iter = find_if(mapTreeItem.begin(), mapTreeItem.end(), CFinder_Tag(strObjectName));

			if (iter != mapTreeItem.end())
			{
				UpdateData(TRUE);

				CString ObjectNameTemp2 = strObjectName;
				CString ItemNum;
				CObject_Manager* pObjectManager = CObject_Manager::GetInstance();

				auto IdxValue = pObjectManager->FindObjectLayer(SCENE_STATIC, strLayerTag)->Get_ObjectList().begin();
				_int iSize = pObjectManager->FindObjectLayer(SCENE_STATIC, strLayerTag)->Get_ObjectList().size();

				// List�� .back() �Լ��� ���ͷ����͸� �����ִ°��� �ƴ϶�, �ش� ������ ��, �� (*iter)�� �����ϱ� ������,
				// back - 1�� ���� -1�� ���ִ°��̴�. �ᱹ �����̶�°���..
				// �ؿ�ó�� ��ȸ�ϸ鼭 ���ƾ�����, ���� �������� ������ ���� �ٷ� ������ ã�� �� �ִ�.

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

				if(bIsStaticMesh == true)
					Tree_Mesh_StaticObj.InsertItem(ObjectNameTemp2, 0, 0, iter->second, TVI_LAST);
				else
					Tree_Mesh_DynamicObj.InsertItem(ObjectNameTemp2, 0, 0, iter->second, TVI_LAST);

				//strLayerTag = _T("");
				ObjectNameTemp2 = _T("");
				ItemNum = _T("");

				UpdateData(FALSE);
			}
		}
	}

	UpdateData(FALSE);

	return NOERROR;
}

HRESULT CMeshTab::Add_StaticObject()
{
	m_pScene = CViewManager::GetInstance()->m_pCurScene;

	MakeArgVariableForStaticObj();

	// ������Ʈ ������ Ÿ�Ե� ���� �ѹ��� �����ؾ� �ϹǷ�..
	// find_if �Լ��� ����Ͽ� strComponentPrototypeTag�� ���� iter�� �˻��ؼ�,
	auto iter = find_if(mapObj_Com_Prototype.begin(), mapObj_Com_Prototype.end(), CFinder_Tag(strComponentPrototypeTag));

	// �˻��� �ȵǾ��ٸ�, ����������� ���� ���� �����̹Ƿ�,
	if (iter == mapObj_Com_Prototype.end())
	{
		// ���� ���õ� Object�� xFile �̸��� ������ش�.
		wstring strXFileNameTemp;
		strXFileNameTemp = strObjectName;
		strXFileNameTemp += L".X";
		strXfileName = strXFileNameTemp.c_str();

		// Add_Component_Prototype�� �ش��Ѵ�. (������ü)
		if (FAILED(dynamic_cast<CSceneStatic*>(m_pScene)->Add_Static_Object_Component_Prototype(strComponentPrototypeTag, szFullPath, strXfileName)))
			return E_FAIL;

		mapObj_Com_Prototype.emplace(strComponentPrototypeTag, strXfileName);

		strXfileName = _T("");
	}
	
	if (FAILED(dynamic_cast<CSceneStatic*>(m_pScene)->Add_Static_Object(strLayerTag)))
		return E_FAIL;

	CLayer* pLayer = CObject_Manager::GetInstance()->FindObjectLayer(SCENE_STATIC, strLayerTag);

	// ���⿡ ���ϳ� ���� ������Ʈ�� �����Ʈ������, Layer_Tag, Component_Tag �� �־��ְ�,
	// Ŭ�󿡼� Ready�Լ����� Load�Լ� �ҷ����鼭 Load�Լ� �ȿ��� ��� ������ ���� �ٷ� �ε� ���ָ� �ȴ�.

	CString strObjProtoTag = L"GameObject_" + strObjectName;

	CDataManager::GetInstance()->m_bIsStaticMesh = bIsStaticMesh;
	CDataManager::GetInstance()->m_strObjProtoTag = strObjProtoTag;
	CDataManager::GetInstance()->m_strLayerTag = strLayerTag;
	CDataManager::GetInstance()->m_strComProtoTag = strComponentPrototypeTag;

	CString	szFullPathModify = szFullPath;
	//szFullPathModify.TrimRight(L"/");
	
	CDataManager::GetInstance()->m_strFullPath = szFullPathModify;

	CDataManager::GetInstance()->m_strObjName = strObjectName;

	// Add_Component�� �ش��Ѵ�. (�� ��� ��ü)	
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

	// Ȯ���ڸ� �ڸ���.
	strLayerTag.TrimRight(L".X");
	strLayerTag.TrimRight(L".x");

	strObjectName = strLayerTag;

	// Component�� PrototypeTag�� ����� �ش�.
	if(bIsStaticMesh == true)
		strComponentPrototypeTag = _T("Component_Mesh_Static_") + strLayerTag;
	else
		strComponentPrototypeTag = _T("Component_Mesh_Dynamic_") + strLayerTag;

	// LayerTag�� ����� �ش�.
	strLayerTag = _T("Layer_") + strLayerTag;

	return NOERROR;
}

void CMeshTab::OnTree_Mesh_Object(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	UpdateData(TRUE);

	// ���� Ʈ�� ��Ʈ�ѿ��� ���õ� �������� �ҷ��´�.
	HTREEITEM	hSelected = pNMTreeView->itemNew.hItem;

	SelectedObjectItem = hSelected;

	// �ڼ����� �� �𸣰ڴµ�... ó�� ���õ� �� ó���ǰ� �Ǿ��ִ�.
	// �� ������ �ؿ��� else������ ���� �߰� ó�����ش�.
	if (Tree_Mesh_Object.GetItemData(hSelected) == 0)
	{
		CString	pathSelected;
		HTREEITEM hParentItem = hSelected;

		// ���õ� �������� ��θ� ������ִ� ����.
		while (hParentItem != NULL)
		{
			pathSelected = _T("/") + pathSelected;
			pathSelected = Tree_Mesh_Object.GetItemText(hParentItem) + pathSelected;
			hParentItem = Tree_Mesh_Object.GetParentItem(hParentItem);
		}

		// �����δ� ��� Ʈ����Ʈ���� �׷����� ���� �����̴�.
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	//UpdateData(TRUE);

 	MakeItemForTree();

	// UpdateData(FALSE);
	
	*pResult = 0;
}

void CMeshTab::OnNMClickTreeMeshObject(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	MakeArgVariableForStaticObj();

	*pResult = 0;
}

void CMeshTab::OnTree_Mesh_StaticObj(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	
	// ���� Ʈ�� ��Ʈ�ѿ��� ���õ� �������� �ҷ��´�.
	HTREEITEM	hSelected = pNMTreeView->itemNew.hItem;

	SelectedStaticObject = hSelected;

	*pResult = 0;
}

void CMeshTab::OnTree_Mesh_DynamicObj(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	// ���� Ʈ�� ��Ʈ�ѿ��� ���õ� �������� �ҷ��´�.
	HTREEITEM	hSelected = pNMTreeView->itemNew.hItem;

	SelectedDynamicObject = hSelected;

	*pResult = 0;
}

void CMeshTab::OnNMRClickTreeStaticObj(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	bDblClkTreeStaticObj = true;

	// Static_Object Tree���� ���� ���õ� �������� �θ� �������� �������� ����.
	// �̰� ���ִ� ������ ���� Ʈ���� �������� "������Ʈ�� [n]" �̷��� �Ǵµ�,
	// �ڸ��� �����Ƽ� �׳� �θ��� �������� �������� �ϱ� �����̴�.
	HTREEITEM hSelectedParentItem = Tree_Mesh_StaticObj.GetParentItem(SelectedStaticObject);

	// �θ� ������ �׷��ϱ� �� ������Ʈ �̸����ְ� �ε����� �پ����� ���� ������ �ؽ�Ʈ�� �����´�.
	CString ParentItemName = Tree_Mesh_StaticObj.GetItemText(hSelectedParentItem);

	// �̰� ���� ���õ� Static Object�� �ε����� ���°���� �߶���� �����̴�.
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

	_int iItemIdx = _ttoi(strItemIdx);

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

void CMeshTab::OnNMRClickTreeDynamicObj(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	bDblClkTreeDynamicObj = true;

	// Static_Object Tree���� ���� ���õ� �������� �θ� �������� �������� ����.
	// �̰� ���ִ� ������ ���� Ʈ���� �������� "������Ʈ�� [n]" �̷��� �Ǵµ�,
	// �ڸ��� �����Ƽ� �׳� �θ��� �������� �������� �ϱ� �����̴�.
	HTREEITEM hSelectedParentItem = Tree_Mesh_DynamicObj.GetParentItem(SelectedDynamicObject);

	// �θ� ������ �׷��ϱ� �� ������Ʈ �̸����ְ� �ε����� �پ����� ���� ������ �ؽ�Ʈ�� �����´�.
	CString ParentItemName = Tree_Mesh_DynamicObj.GetItemText(hSelectedParentItem);

	// �̰� ���� ���õ� Static Object�� �ε����� ���°���� �߶���� �����̴�.
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

	_int iItemIdx = _ttoi(strItemIdx);

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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

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
	}

	*pResult = 0;
}


void CMeshTab::OnSpin_Scaling_Y(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

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
	}

	*pResult = 0;
}


void CMeshTab::OnSpin_Scaling_Z(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

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
	}
	
	*pResult = 0;
}


void CMeshTab::OnSpin_Rotate_X(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

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

		UpdateData(FALSE);

		dynamic_cast<CStaticObject*>(pSelectedObj)->GetTransformCom()->Set_Rotation_YawPitchRoll(D3DXToRadian(m_fRotX), D3DXToRadian(m_fRotY), D3DXToRadian(m_fRotZ));
	}

	*pResult = 0;
}


void CMeshTab::OnSpin_Rotate_Y(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

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

		UpdateData(FALSE);

		dynamic_cast<CStaticObject*>(pSelectedObj)->GetTransformCom()->Set_Rotation_YawPitchRoll(D3DXToRadian(m_fRotX), D3DXToRadian(m_fRotY), D3DXToRadian(m_fRotZ));
	}

	*pResult = 0;
}


void CMeshTab::OnSpin_Rotate_Z(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

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

		UpdateData(FALSE);

		dynamic_cast<CStaticObject*>(pSelectedObj)->GetTransformCom()->Set_Rotation_YawPitchRoll(D3DXToRadian(m_fRotX), D3DXToRadian(m_fRotY), D3DXToRadian(m_fRotZ));
	}

	*pResult = 0;
}


void CMeshTab::OnSpin_Position_X(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

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
	}

	*pResult = 0;
}

void CMeshTab::OnSpin_Position_Y(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

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
	}

	*pResult = 0;
}


void CMeshTab::OnSpin_Position_Z(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

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
	}

	*pResult = 0;
}


void CMeshTab::OnBnClicked_Mesh_Save()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	TCHAR	szDirPath[MAX_PATH];
	CString strName = L"";
	GetCurrentDirectory(MAX_PATH, szDirPath);

	CFileDialog		Dlg(FALSE,	// false�� ��� save, true�� ��� load
		L"MeshDat",	// ������ Ȯ���ڸ�	
		L"*.MeshDat", // â�� ���ʷ� ����� �����̸� ���ڿ�
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, // �ߺ� ������ ���� ��� ���â ����ֱ�
		L"*.MeshDat", // ���� �� �����ϴ� ���� ����
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);

	TCHAR	szDirPath[MAX_PATH];
	CString strName = L"";
	GetCurrentDirectory(MAX_PATH, szDirPath);

	CFileDialog		Dlg(TRUE,	// false�� ��� save, true�� ��� load
		L"MeshDat",	// ������ Ȯ���ڸ�	
		L"*.MeshDat", // â�� ���ʷ� ����� �����̸� ���ڿ�
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, // �ߺ� ������ ���� ��� ���â ����ֱ�
		L"*.MeshDat", // ���� �� �����ϴ� ���� ����
		this);

	CDataManager::GetInstance()->Free();

	// �����̰� ������ ��
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
		dynamic_cast<CStaticObject*>(pStaticObjLayer->Get_ObjectList().back())->SetState(*(_vec3*)&tObjMeshData.matWorld.m[3], _vec3(1.f, 1.f, 1.f));

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
