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
#include "ViewManager.h"

// CTerrainTab ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CTerrainTab, CDialogEx)

CTerrainTab::CTerrainTab(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TERRAINTAB, pParent)
	, m_fPosX(0)
	, m_fPosY(0)
	, m_fPosZ(0)
	, m_fRotX(0)
	, m_fRotY(0)
	, m_fRotZ(0)
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
	DDX_Text(pDX, IDC_EDIT5, m_fPosX);
	DDV_MinMaxFloat(pDX, m_fPosX, -10000, 10000);
	DDX_Text(pDX, IDC_EDIT6, m_fPosY);
	DDV_MinMaxFloat(pDX, m_fPosY, -10000, 10000);
	DDX_Text(pDX, IDC_EDIT7, m_fPosZ);
	DDV_MinMaxFloat(pDX, m_fPosZ, -10000, 10000);
	DDX_Text(pDX, IDC_EDIT8, m_fRotX);
	DDV_MinMaxFloat(pDX, m_fRotX, 0, 360);
	DDX_Text(pDX, IDC_EDIT9, m_fRotY);
	DDV_MinMaxFloat(pDX, m_fRotY, 0, 360);
	DDX_Text(pDX, IDC_EDIT10, m_fRotZ);
	DDV_MinMaxFloat(pDX, m_fRotZ, 0, 360);

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

	m_pScene = CViewManager::GetInstance()->m_pCurScene;

	Terrain_RenderST_Solild.SetCheck(TRUE);
	Terrain_RenderCood_Axis.SetCheck(TRUE);
	
	CStringW	strNumVtxX;

	Vertex_X_Count_Btn.SetPos(100.f);
	Vertex_X_Count_Btn.SetRange(0.f, 1000.f);

	Vertex_Z_Count_Btn.SetPos(100.f);
	Vertex_Z_Count_Btn.SetRange(0.f, 1000.f);

	Vertex_Interval_Btn.SetPos(1.f);
	Vertex_Interval_Btn.SetRange(0.f, 1000.f);

	Vertex_Detail_Btn.SetPos(1.f);
	Vertex_Detail_Btn.SetRange(0.f, 100.f);

	PositionX_Btn.SetPos(0.f);
	PositionX_Btn.SetRange(1000.f, -1000.f);

	PositionY_Btn.SetPos(0.f);
	PositionY_Btn.SetRange(1000.f, -1000.f);

	PositionZ_Btn.SetPos(0.f);
	PositionZ_Btn.SetRange(1000.f, -1000.f);

	RotaionX_Btn.SetPos(0.f);
	RotaionX_Btn.SetRange(360.f, 0.f);

	RotaionY_Btn.SetPos(0.f);
	RotaionY_Btn.SetRange(360.f, 0.f);

	RotaionZ_Btn.SetPos(0.f);
	RotaionZ_Btn.SetRange(360.f, 0.f);

	InitTreeCtrl();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CTerrainTab::OnBnClickedWireFrame()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	CLayer* pLayer = CObject_Manager::GetInstance()->FindObjectLayer(SCENE_STATIC, L"Layer_Terrain");
	dynamic_cast<CTerrain*>(pLayer->Get_ObjectList().back())->SetFillMode(true);
	
}


void CTerrainTab::OnBnClickedSolid()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	CLayer* pLayer = CObject_Manager::GetInstance()->FindObjectLayer(SCENE_STATIC, L"Layer_Terrain");
	dynamic_cast<CTerrain*>(pLayer->Get_ObjectList().back())->SetFillMode(false);

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

	CLayer* pLayer = CObject_Manager::GetInstance()->FindObjectLayer(SCENE_STATIC, L"Layer_Terrain");
	dynamic_cast<CTerrain*>(pLayer->Get_ObjectList().back())->GetTransformCom()->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(m_fPosX, m_fPosY, m_fPosZ));

	*pResult = 0;
}

void CTerrainTab::OnSpin_Trans_PosY(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

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

	CLayer* pLayer = CObject_Manager::GetInstance()->FindObjectLayer(SCENE_STATIC, L"Layer_Terrain");
	dynamic_cast<CTerrain*>(pLayer->Get_ObjectList().back())->GetTransformCom()->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(m_fPosX, m_fPosY, m_fPosZ));

	*pResult = 0;
}

void CTerrainTab::OnSpin_Trans_PosZ(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

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

	CLayer* pLayer = CObject_Manager::GetInstance()->FindObjectLayer(SCENE_STATIC, L"Layer_Terrain");
	dynamic_cast<CTerrain*>(pLayer->Get_ObjectList().back())->GetTransformCom()->Set_StateInfo(CTransform::STATE_POSITION, &_vec3(m_fPosX, m_fPosY, m_fPosZ));

	*pResult = 0;
}

void CTerrainTab::OnSpin_Trans_RotX(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

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

	if (m_fRotX <= 1.f)
		m_fRotX = 1.f;

	if (m_fRotX >= 360.f)
		m_fRotX = 360.f;

	UpdateData(FALSE);

	CLayer* pLayer = CObject_Manager::GetInstance()->FindObjectLayer(SCENE_STATIC, L"Layer_Terrain");
	dynamic_cast<CTerrain*>(pLayer->Get_ObjectList().back())->GetTransformCom()->Set_Rotation_YawPitchRoll(D3DXToRadian(m_fRotX), D3DXToRadian(m_fRotY), D3DXToRadian(m_fRotZ));

	*pResult = 0;
}

void CTerrainTab::OnSpin_Trans_RotY(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

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

	if (m_fRotY <= 1.f)
		m_fRotY = 1.f;

	if (m_fRotY >= 360.f)
		m_fRotY = 360.f;

	UpdateData(FALSE);

	CLayer* pLayer = CObject_Manager::GetInstance()->FindObjectLayer(SCENE_STATIC, L"Layer_Terrain");
	dynamic_cast<CTerrain*>(pLayer->Get_ObjectList().back())->GetTransformCom()->Set_Rotation_YawPitchRoll(D3DXToRadian(m_fRotX), D3DXToRadian(m_fRotY), D3DXToRadian(m_fRotZ));

	*pResult = 0;
}

void CTerrainTab::OnSpin_Trans_RotZ(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

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

	if (m_fRotZ <= 1.f)
		m_fRotZ = 1.f;

	if (m_fRotZ >= 360.f)
		m_fRotZ = 360.f;

	UpdateData(FALSE);

	CLayer* pLayer = CObject_Manager::GetInstance()->FindObjectLayer(SCENE_STATIC, L"Layer_Terrain");
	dynamic_cast<CTerrain*>(pLayer->Get_ObjectList().back())->GetTransformCom()->Set_Rotation_YawPitchRoll(D3DXToRadian(m_fRotX), D3DXToRadian(m_fRotY), D3DXToRadian(m_fRotZ));

	*pResult = 0;
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

	//TCHAR		 szFullPath[MAX_PATH] = L"";

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
