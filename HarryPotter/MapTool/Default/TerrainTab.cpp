// TerrainTab.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include <atlimage.h>
#include "MapTool.h"
#include "TerrainTab.h"
#include "afxdialogex.h"
#include "SceneStatic.h"
#include "Component_Manager.h"
#include "EventManagerTool.h"
#include "Object_Manager.h"
#include "Layer.h"
#include "Terrain.h"
#include "FileManager.h"

// CTerrainTab ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CTerrainTab, CDialogEx)

CTerrainTab::CTerrainTab(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TERRAINTAB, pParent)
{
}

CTerrainTab::~CTerrainTab()
{
	// �����ص� �̹������� �� �����ش�.
	for (auto iter = m_mapPngImage.begin(); iter != m_mapPngImage.end(); ++iter)
	{
		iter->second->Destroy();
		Safe_Delete(iter->second);
	}
	m_mapPngImage.clear();
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
	DDX_Control(pDX, IDC_TREE1, m_Tree_Terrain_Texture);
	DDX_Control(pDX, IDC_PICTURE1, m_Terrain_Texture);
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
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CTerrainTab::OnTree_Terrain_Texture)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE1, &CTerrainTab::OnNMDblclkTreeTerrainTexture)
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

	PositionX_Btn.SetRange(-1000, 1000);
	PositionX_Btn.SetPos(0);

	PositionY_Btn.SetRange(-1000, 1000);
	PositionY_Btn.SetPos(0);

	PositionZ_Btn.SetRange(-1000, 1000);
	PositionZ_Btn.SetPos(0);

	RotaionX_Btn.SetRange(-0, 360);
	RotaionX_Btn.SetPos(0);

	RotaionY_Btn.SetRange(-0, 360);
	RotaionY_Btn.SetPos(0);

	RotaionZ_Btn.SetRange(-0, 360);
	RotaionZ_Btn.SetPos(0);

	InitTreeCtrl();

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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;

	CStringW	strValue;

	PositionX.GetWindowTextW(strValue);
	m_iPosX = _ttoi(strValue);

	CLayer* pLayer = CObject_Manager::GetInstance()->FindObjectLayer(SCENE_STATIC, L"Layer_Terrain");
	dynamic_cast<CTerrain*>(pLayer->Get_ObjectList().back())->GetTransformCom()->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(m_iPosX, m_iPosY, m_iPosZ));

}

void CTerrainTab::OnSpin_Trans_PosY(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;

	CStringW	strValue;

	PositionY.GetWindowTextW(strValue);
	m_iPosY = _ttoi(strValue);

	CLayer* pLayer = CObject_Manager::GetInstance()->FindObjectLayer(SCENE_STATIC, L"Layer_Terrain");
	dynamic_cast<CTerrain*>(pLayer->Get_ObjectList().back())->GetTransformCom()->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(m_iPosX, m_iPosY, m_iPosZ));

}

void CTerrainTab::OnSpin_Trans_PosZ(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;

	CStringW	strValue;

	PositionZ.GetWindowTextW(strValue);
	m_iPosZ = _ttoi(strValue);

	CLayer* pLayer = CObject_Manager::GetInstance()->FindObjectLayer(SCENE_STATIC, L"Layer_Terrain");
	dynamic_cast<CTerrain*>(pLayer->Get_ObjectList().back())->GetTransformCom()->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(m_iPosX, m_iPosY, m_iPosZ));

}

void CTerrainTab::OnSpin_Trans_RotX(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;

	CStringW	strValue;

	RotationY.GetWindowTextW(strValue);
	m_fRotY = _ttoi(strValue) + 1;

	Rotation_Axis(_vec3(0.f, 1.f, 0.f), D3DXToRadian(m_fRotY));
}

void CTerrainTab::OnSpin_Trans_RotZ(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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

void CTerrainTab::InitTreeCtrl()
{

	HTREEITEM hItem = m_Tree_Terrain_Texture.InsertItem(_T("../Bin/Resources/Textures/Terrain"));
	//HTREEITEM hItem = m_Tree_Terrain_Texture.InsertItem(_T("../Bin/Resources/Textures/Terrain/"));

	CFileFind finder;

	BOOL bWorking = finder.FindFile(_T("..\Bin\Resources\Textures\Terrain\\*.*"));
	//BOOL bWorking = finder.FindFile(_T("../Bin/Resources/Textures/Terrain/*.*"));

	while (bWorking)
	{
		bWorking = finder.FindNextFileW();

		if (finder.IsDirectory())
			m_Tree_Terrain_Texture.InsertItem(finder.GetFileName(), hItem);
	}

	m_Tree_Terrain_Texture.EnsureVisible(hItem);
}

void CTerrainTab::OnTree_Terrain_Texture(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	UpdateData(TRUE);

	TCHAR		 szFullPath[MAX_PATH] = L"";

	// ���� Ʈ�� ��Ʈ�ѿ��� ���õ� �������� �ҷ��´�.
	HTREEITEM	hSelected = pNMTreeView->itemNew.hItem;

	// �ڼ����� �� �𸣰ڴµ�... ó�� ���õ� �� ó���ǰ� �Ǿ��ִ�.
	// �� ������ �ؿ��� else������ ���� �߰� ó�����ش�.
	if (m_Tree_Terrain_Texture.GetItemData(hSelected) == 0)
	{
		CString	pathSelected;
		HTREEITEM hParentItem = hSelected;

		// ���õ� �������� ��θ� ������ִ� ����.
		while (hParentItem != NULL)
		{
			pathSelected = _T("/") + pathSelected;
			pathSelected = m_Tree_Terrain_Texture.GetItemText(hParentItem) + pathSelected;
			hParentItem = m_Tree_Terrain_Texture.GetParentItem(hParentItem);
		}

		// �������� Ʈ����Ʈ�������� �� �������� '/'�� �ٿ��ִµ�,
		// ���� ��ο����� �Ǹ������� '/'�� �ٿ��ָ� Image Load�� �����Ͽ� nullptr���� ���� ������,
		// �ش� �̽� ó�� �� ������ �������� �̹������ m_mapPngImage�� ���õ� ������ �־��ֱ�,
		// �׸��� �־��� �������� ��°� ������ �������� �ѹ��� �ϴ� �Լ��� �������.
		ImageProcess(hSelected);

		// �����δ� ��� Ʈ����Ʈ���� �׷����� ���� �����̴�.
		pathSelected = pathSelected + _T("*.*");

		CFileFind finder;
		BOOL bWorking = finder.FindFile(pathSelected);

		while (bWorking)
		{
			bWorking = finder.FindNextFileW();
			
			if (finder.IsDots())
				continue;

			//if (finder.IsDirectory())
			m_Tree_Terrain_Texture.InsertItem(finder.GetFileName(), hSelected);
		}
		m_Tree_Terrain_Texture.SetItemData(hSelected, 1);
		m_Tree_Terrain_Texture.EnsureVisible(hSelected);
	}
	else // �Ȱ��� �������� �ٽ� ������ ��.
	{
		CString	pathSelected;
		HTREEITEM hParentItem = hSelected;

		ImageProcess(hParentItem);
	}
	UpdateData(FALSE);

	*pResult = 0;

}

void CTerrainTab::ImageProcess(HTREEITEM _hSelected)
{
	// Ʈ�� ��Ʈ�ѷκ��� ���õ� ��������(�ؽ��� �̹���) �ּҸ� ������ �����ϱ� ���� �Լ�.
	// OnTree_Terrain_Texture �Լ��� ������ �Ϻ� ����ϳ�,
	// pathSelectedTemp = _T("/") + pathSelectedTemp; ������ ����, ���� ����� �� ���ٿ� '/'�� �����Ͽ� �̹��� �ε带 ���������� ó���Ѵ�.

	TCHAR		szFullPath[MAX_PATH] = L"";
	CString		pathSelected;
	HTREEITEM	hParentItem = _hSelected;

	while (hParentItem != NULL)
	{
		if (pathSelected != L"")
			pathSelected = _T("/") + pathSelected;
		pathSelected = m_Tree_Terrain_Texture.GetItemText(hParentItem) + pathSelected;
		hParentItem = m_Tree_Terrain_Texture.GetParentItem(hParentItem);
	}

	lstrcatW(szFullPath, pathSelected);
	strImagName = PathFindFileName(szFullPath);						// ��θ� �������� ������ �̸��� ������ �Լ�
	PathRemoveExtension((LPWSTR)strImagName.operator LPCWSTR());	// Ȯ���ڸ��� �߶󳻴� �Լ�

	ZeroMemory(szFullPathForTexture, sizeof(szFullPathForTexture));
	StrCpyW(szFullPathForTexture, szFullPath);
	// �ش� �̹����� �ִ��� Ȯ�� ��,
	auto iter = m_mapPngImage.find(strImagName);

	// �翬�� ó���̶� ���µ�, �� �� m_mapPngImage�� ������ �����Ҵ��� CImage��ü��,
	// �ҷ��� path���� ��� �ĳ���, Ȯ���ڸ� �ĳ� ������ �̹��� �̸��� �־��ش�.
	if (iter == m_mapPngImage.end())
	{
		CImage*		pPngImage = new CImage;
		pPngImage->Load(szFullPath);

		m_mapPngImage.emplace(strImagName, pPngImage);
	}

	// �� �������Ͱ� Dialog�� �ִ� ��Ʈ���� �̹����� �����ϱ� ���� ����.
	// ������ �۾��� �ϰ� �ٷ� �����Ա� ������, �İ����� �̹��� �̸���(strImagName) �ٷ� ���ܿ� �˻��Ѵ�.
	auto iterSet = m_mapPngImage.find(strImagName);

	// ������ �־��� ������ �ظ��ؼ� �������� ����.
	if (iterSet == m_mapPngImage.end())
		return;

	// �ش� �̹����� �ٲ��ش�.
	m_Terrain_Texture.SetBitmap(*iterSet->second);

	// �̹��� ����� �� �ڴ�� ��µǼ�,
	// ������ ������ �ϱ����� �Լ��� �ϳ� �������.
	// ���ڷ� CImage�� �־��ָ� �ȴ�.
	// ���� ������ ó���� Iter�� second�� �ٷ� �־��ش�.
	ImageSizing(iterSet->second);

}

void CTerrainTab::ImageSizing(CImage* pImage)
{
	// Ʈ����Ʈ�ѿ��� �ҷ��� �������� ��ΰ� ���� �̹����� �ƴϰ� ����(?) �� �� �ִ�.
	// �̷��� ũ�� ������ �� ������ ����µ�, �� �������� ������ �ƴϰ� �̹������?
	// �̶�� ����ó���� ���ش�.
	if (pImage->IsNull() != true)
	{
		// Image Size Adjust//
		// �̹��� ũ�⸦ �������ش� 
		CDC* screen = m_Terrain_Texture.GetDC();
		CDC pMdc;
		pMdc.CreateCompatibleDC(screen);
		CBitmap bmp;

		int   iSizeX = 170, iSizeY = 170;
		bmp.CreateCompatibleBitmap(screen, iSizeX, iSizeY);

		CBitmap* pob = pMdc.SelectObject(&bmp);
		pMdc.SetStretchBltMode(COLORONCOLOR);
		pImage->StretchBlt(pMdc.m_hDC, 0, 0, iSizeX, iSizeY, 0, 0, pImage->GetWidth(), pImage->GetHeight(), SRCCOPY);
		pMdc.SelectObject(pob);

		m_Terrain_Texture.SetBitmap((HBITMAP)bmp.Detach());

		ReleaseDC(screen);
		DeleteDC(pMdc);
	}
}

BOOL CTerrainTab::PreTranslateMessage(MSG* pMsg)
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


void CTerrainTab::OnNMDblclkTreeTerrainTexture(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	CLayer* pLayer = CObject_Manager::GetInstance()->FindObjectLayer(SCENE_STATIC, L"Layer_Terrain");
	dynamic_cast<CTerrain*>(pLayer->Get_ObjectList().back())->Reset_Texture(szFullPathForTexture);

	*pResult = 0;
}
