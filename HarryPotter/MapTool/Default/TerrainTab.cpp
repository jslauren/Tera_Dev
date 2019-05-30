// TerrainTab.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include <atlimage.h>
#include "MapTool.h"
#include "TerrainTab.h"
#include "afxdialogex.h"
#include "SceneStatic.h"
#include "Component_Manager.h"
#include "Object_Manager.h"
#include "Layer.h"
#include "Terrain.h"
#include "FileManager.h"
#include "ViewManager.h"
#include "DataManager.h"

// CTerrainTab 대화 상자입니다.

IMPLEMENT_DYNAMIC(CTerrainTab, CDialogEx)

CTerrainTab::CTerrainTab(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TERRAINTAB, pParent)
	, m_fPosX(0)
	, m_fPosY(0)
	, m_fPosZ(0)
	, m_fRotX(0)
	, m_fRotY(0)
	, m_fRotZ(0)
	, m_iNumVtxX(100)
	, m_iNumVtxZ(100)
	, m_fInterval(1)
	, m_fDetail(1)
{
}

CTerrainTab::~CTerrainTab()
{
	// 저장해둔 이미지들을 다 날려준다.
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
	DDX_Text(pDX, IDC_EDIT1, m_iNumVtxX);
	DDV_MinMaxInt(pDX, m_iNumVtxX, 1, 300);
	DDX_Text(pDX, IDC_EDIT2, m_iNumVtxZ);
	DDV_MinMaxInt(pDX, m_iNumVtxZ, 1, 300);
	DDX_Text(pDX, IDC_EDIT3, m_fInterval);
	DDV_MinMaxFloat(pDX, m_fInterval, 1, 20);
	DDX_Text(pDX, IDC_EDIT4, m_fDetail);
	DDV_MinMaxFloat(pDX, m_fDetail, 0, 500);
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
	ON_BN_CLICKED(IDC_BUTTON3, &CTerrainTab::OnBnClicked_Terrain_Save)
	ON_BN_CLICKED(IDC_BUTTON4, &CTerrainTab::OnBnClicked_Terrain_Load)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &CTerrainTab::OnSpin_Vertex_X_Count)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, &CTerrainTab::OnSpin_Vertex_Z_Count)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN3, &CTerrainTab::OnSpin_Vertex_Interval)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN4, &CTerrainTab::OnSpin_Vertex_Detail)
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

	m_pScene = CViewManager::GetInstance()->m_pCurScene;

	Terrain_RenderST_Solild.SetCheck(TRUE);
	Terrain_RenderCood_Axis.SetCheck(TRUE);
	
	CStringW	strNumVtxX;

	Vertex_X_Count_Btn.SetPos(100);
	Vertex_X_Count_Btn.SetRange(1, 300);

	Vertex_Z_Count_Btn.SetPos(100);
	Vertex_Z_Count_Btn.SetRange(1, 300);

	Vertex_Interval_Btn.SetPos(1.f);
	Vertex_Interval_Btn.SetRange(1.f, 20.f);

	Vertex_Detail_Btn.SetPos(1.f);
	Vertex_Detail_Btn.SetRange(1.f, 500.f);

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
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CTerrainTab::OnBnClickedWireFrame()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CLayer* pLayer = CObject_Manager::GetInstance()->FindObjectLayer(SCENE_STATIC, L"Layer_Terrain");
	dynamic_cast<CTerrain*>(pLayer->Get_ObjectList().back())->SetFillMode(true);
	
}


void CTerrainTab::OnBnClickedSolid()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CLayer* pLayer = CObject_Manager::GetInstance()->FindObjectLayer(SCENE_STATIC, L"Layer_Terrain");
	dynamic_cast<CTerrain*>(pLayer->Get_ObjectList().back())->SetFillMode(false);

}

void CTerrainTab::OnBnClickedTerrain_Apply()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CStringW	strValue;

	//Vertex_X_Count.GetWindowTextW(strValue);
	//m_iNumVtxX = _ttoi(strValue);

	//Vertex_Z_Count.GetWindowTextW(strValue);
	//m_iNumVtxZ = _ttoi(strValue);

	Vertex_Interval.GetWindowTextW(strValue);
	m_fInterval = _ttof(strValue);

	Vertex_Detail.GetWindowTextW(strValue);
	m_fDetail = _ttof(strValue);

	CLayer* pLayer = CObject_Manager::GetInstance()->FindObjectLayer(SCENE_STATIC, L"Layer_Terrain");
	dynamic_cast<CTerrain*>(pLayer->Get_ObjectList().back())->Reset_Terrain(m_iNumVtxX, m_iNumVtxZ, m_fInterval, m_fDetail);

	CDataManager::GetInstance()->m_iNumVtxX = m_iNumVtxX;
	CDataManager::GetInstance()->m_iNumVtxZ = m_iNumVtxZ;
	CDataManager::GetInstance()->m_fInterval = m_fInterval;
	CDataManager::GetInstance()->m_fDetail = m_fDetail;

}

void CTerrainTab::OnSpin_Trans_PosX(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

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
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

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
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

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
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

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
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

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
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

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
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	//TCHAR		 szFullPath[MAX_PATH] = L"";

	// 현재 트리 컨트롤에서 선택된 아이템을 불러온다.
	HTREEITEM	hSelected = pNMTreeView->itemNew.hItem;

	// 자세히는 잘 모르겠는데... 처음 선택된 놈만 처리되게 되어있다.
	// 이 때문에 밑에서 else문으로 따로 추가 처리해준다.
	if (m_Tree_Terrain_Texture.GetItemData(hSelected) == 0)
	{
		CString	pathSelected;
		HTREEITEM hParentItem = hSelected;

		// 선택된 아이템의 경로를 만들어주는 구문.
		while (hParentItem != NULL)
		{
			pathSelected = _T("/") + pathSelected;
			pathSelected = m_Tree_Terrain_Texture.GetItemText(hParentItem) + pathSelected;
			hParentItem = m_Tree_Terrain_Texture.GetParentItem(hParentItem);
		}

		// 위에서는 트리컨트롤을위해 맨 마지막에 '/'를 붙여주는데,
		// 파일 경로에서는 맨마지막에 '/'를 붙여주면 Image Load가 실패하여 nullptr값이 들어가기 때문에,
		// 해당 이슈 처리 및 선택한 아이템이 이미지라면 m_mapPngImage에 선택된 아이템 넣어주기,
		// 그리고 넣어준 아이템의 출력과 사이즈 조정까지 한번에 하는 함수를 만들었다.
		ImageProcess(hSelected);

		// -> 여기  L"../Bin/Resources/Textures/Terrain/Grass.tga"

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
			m_Tree_Terrain_Texture.InsertItem(finder.GetFileName(), hSelected);
		}
		m_Tree_Terrain_Texture.SetItemData(hSelected, 1);
		m_Tree_Terrain_Texture.EnsureVisible(hSelected);
	}
	else // 똑같은 아이템을 다시 눌렀을 시.
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
	// 트리 컨트롤로부터 선택된 아이템의(텍스쳐 이미지) 주소를 가져와 가공하기 위한 함수.
	// OnTree_Terrain_Texture 함수와 내용이 일부 비슷하나,
	// pathSelectedTemp = _T("/") + pathSelectedTemp; 구문을 통해, 얻어온 경로의 맨 뒷줄에 '/'를 제거하여 이미지 로드를 성공적으로 처리한다.

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
	strImagName = PathFindFileName(szFullPath);						// 경로를 기준으로 파일의 이름을 얻어오는 함수

	strImagName.TrimRight(L".tga");
	strImagName.TrimRight(L".TGA");
		
	//PathRemoveExtension((LPWSTR)strImagName.operator LPCWSTR());	// 확장자명을 잘라내는 함수

	ZeroMemory(szFullPathForTexture, sizeof(szFullPathForTexture));
	StrCpyW(szFullPathForTexture, szFullPath);
	// 해당 이미지가 있는지 확인 후,
	auto iter = m_mapPngImage.find(strImagName);

	// 당연히 처음이라 없는데, 이 때 m_mapPngImage에 새로이 동적할당한 CImage객체와,
	// 불러온 path에서 경로 쳐내고, 확장자를 쳐낸 순수한 이미지 이름만 넣어준다.
	if (iter == m_mapPngImage.end())
	{
		CImage*		pPngImage = new CImage;
		pPngImage->Load(szFullPath);

		m_mapPngImage.emplace(strImagName, pPngImage);
	}

	// 이 구문부터가 Dialog에 있는 비트맵의 이미지를 변경하기 위한 구문.
	// 위에서 작업을 하고 바로 내려왔기 때문에, 후가공한 이미지 이름을(strImagName) 바로 땡겨와 검사한다.
	auto iterSet = m_mapPngImage.find(strImagName);

	// 위에서 넣었기 때문에 왠만해선 없을수가 없다.
	if (iterSet == m_mapPngImage.end())
		return;

	// 해당 이미지로 바꿔준다.
	m_Terrain_Texture.SetBitmap(*iterSet->second);

	// 이미지 사이즈가 지 멋대로 출력되서,
	// 강제로 조절을 하기위해 함수를 하나 만들었다.
	// 인자로 CImage를 넣어주면 된다.
	// 보통 위에서 처리한 Iter의 second를 바로 넣어준다.
	ImageSizing(iterSet->second);

	_tchar* wsr = szFullPathForTexture;

	CDataManager::GetInstance()->m_pImgPath = wsr;

}

void CTerrainTab::ImageSizing(CImage* pImage)
{
	// 트리컨트롤에서 불러온 아이템의 경로가 만약 이미지가 아니고 폴더(?) 일 수 있다.
	// 이러면 크기 조절할 때 오류가 생기는데, 이 아이템이 폴더가 아니고 이미지라면?
	// 이라는 조건처리를 해준다.
	if (pImage->IsNull() != true)
	{
		// Image Size Adjust//
		// 이미지 크기를 조정해준다 
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

void CTerrainTab::OnNMDblclkTreeTerrainTexture(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CLayer* pLayer = CObject_Manager::GetInstance()->FindObjectLayer(SCENE_STATIC, L"Layer_Terrain");
	dynamic_cast<CTerrain*>(pLayer->Get_ObjectList().back())->Reset_Texture(szFullPathForTexture);

	*pResult = 0;
}

void CTerrainTab::OnBnClicked_Terrain_Save()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	TCHAR	szDirPath[MAX_PATH];
	CString strName = L"";
	GetCurrentDirectory(MAX_PATH, szDirPath);

	CFileDialog		Dlg(FALSE,	// false인 경우 save, true인 경우 load
		L"Map",	// 파일의 확장자명	
		L"*.Map", // 창에 최초로 띄워줄 파일이름 문자열
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, // 중복 파일이 있을 경우 경고창 띄워주기
		L"*.Map", // 저장 시 지원하는 파일 형식
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

	CDataManager::GetInstance()->Set_Terrain_Data();

	for (auto& iter : CDataManager::GetInstance()->m_MapTerrainData)
	{
		WriteFile(hFile, &iter.second->iNumVtxX, sizeof(iter.second->iNumVtxX), &dwByte, NULL);
		WriteFile(hFile, &iter.second->iNumVtxZ, sizeof(iter.second->iNumVtxZ), &dwByte, NULL);
		WriteFile(hFile, &iter.second->fInterval, sizeof(iter.second->fInterval), &dwByte, NULL);
		WriteFile(hFile, &iter.second->fDetail, sizeof(iter.second->fDetail), &dwByte, NULL);
		
		_tchar imgPathTemp[MAX_PATH] = L"";

		lstrcpy(imgPathTemp, iter.second->pImgPath);

		_int iPathLength = lstrlen(imgPathTemp);

		WriteFile(hFile, &iPathLength, sizeof(_int), &dwByte, NULL);
		WriteFile(hFile, imgPathTemp, sizeof(_tchar) * iPathLength, &dwByte, NULL);
	}

	CloseHandle(hFile);
 }

void CTerrainTab::OnBnClicked_Terrain_Load()
{
	// ※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※ //
	// 저장할 때, 숫자로 안하고 글자로 해서 터레인 카메라가 움직여 버리면 
	// 이상하게 저장이 된다. 참고하고 이상하면 최대한 그렇게 만들어 보자.
	// ※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※ //

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	TCHAR	szDirPath[MAX_PATH];
	CString strName = L"";
	GetCurrentDirectory(MAX_PATH, szDirPath);

	CFileDialog		Dlg(TRUE,	// false인 경우 save, true인 경우 load
		L"Map",	// 파일의 확장자명	
		L"*.Map", // 창에 최초로 띄워줄 파일이름 문자열
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, // 중복 파일이 있을 경우 경고창 띄워주기
		L"*.Map", // 저장 시 지원하는 파일 형식
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

	int iMapFirstValue = 0;
	_tchar	imgPath[MAX_PATH] = L"";

	while (true)
	{
		TERRAINDATA*	pTerrainData = new TERRAINDATA;
		_int	iNumVtxX = 0, iNumVtxZ = 0;
		_float	fInterval = 0.f, fDetail = 0.f;
		_int	iImgPathLength = 0;
		_tchar	imgPathTemp[MAX_PATH] = L"";
		
		ReadFile(hFile, &iNumVtxX, sizeof(_int), &dwByte, NULL);
		ReadFile(hFile, &iNumVtxZ, sizeof(_int), &dwByte, NULL);
		ReadFile(hFile, &fInterval, sizeof(_float), &dwByte, NULL);
		ReadFile(hFile, &fDetail, sizeof(_float), &dwByte, NULL);
		ReadFile(hFile, &iImgPathLength, sizeof(_int), &dwByte, NULL);
		ReadFile(hFile, imgPathTemp, sizeof(_tchar) * iImgPathLength, &dwByte, NULL);
		
		pTerrainData->iNumVtxX = iNumVtxX;
		pTerrainData->iNumVtxZ = iNumVtxX;
		pTerrainData->fInterval = fInterval;
		pTerrainData->fDetail = fDetail;
		pTerrainData->pImgPath = imgPathTemp;

		if (0 == dwByte)
		{
			Safe_Delete(pTerrainData);
			break;
		}

		CDataManager::GetInstance()->m_iNumVtxX = pTerrainData->iNumVtxX;
		CDataManager::GetInstance()->m_iNumVtxZ = pTerrainData->iNumVtxZ;
		CDataManager::GetInstance()->m_fInterval = pTerrainData->fInterval;
		CDataManager::GetInstance()->m_fDetail = pTerrainData->fDetail;
		CDataManager::GetInstance()->m_pImgPath = imgPathTemp;

		lstrcpy(imgPath, imgPathTemp);

	//	CDataManager::GetInstance()->m_MapTerrainData.emplace(iMapFirstValue++, pTerrainData);		
	}

	CloseHandle(hFile);

	// Load 해온 정보를 가지고 새로운 터레인을 생성하여 셋팅해준다.
	CLayer* pLayer = CObject_Manager::GetInstance()->FindObjectLayer(SCENE_STATIC, L"Layer_Terrain");
	dynamic_cast<CTerrain*>(pLayer->Get_ObjectList().back())->Reset_Terrain(CDataManager::GetInstance()->m_iNumVtxX, CDataManager::GetInstance()->m_iNumVtxZ, CDataManager::GetInstance()->m_fInterval, CDataManager::GetInstance()->m_fDetail);

	// Load 해온 이미지 정보를 가지고 새로운 이미지로 터레인을 셋팅해준다.
	CString strImgName = PathFindFileName(imgPath);
	
	strImgName.TrimRight(L".tga");
	strImgName.TrimRight(L".TGA");

	ZeroMemory(szFullPathForTexture, sizeof(szFullPathForTexture));
	StrCpyW(szFullPathForTexture, imgPath);

	auto iter = m_mapPngImage.find(strImgName);

	if (iter == m_mapPngImage.end())
	{
		CImage*		pPngImage = new CImage;
		pPngImage->Load(imgPath);

		m_mapPngImage.emplace(strImgName, pPngImage);
	}
	
	auto iterSet = m_mapPngImage.find(strImgName);

	if (iterSet == m_mapPngImage.end())
		return;

	m_Terrain_Texture.SetBitmap(*iterSet->second);
	ImageSizing(iterSet->second);

	_tchar* wsr = szFullPathForTexture;
	CDataManager::GetInstance()->m_pImgPath = wsr;

	dynamic_cast<CTerrain*>(pLayer->Get_ObjectList().back())->Reset_Texture(szFullPathForTexture);

	//////////////////////////////////////////////////////////////////////////

	m_iNumVtxX = CDataManager::GetInstance()->m_iNumVtxX;
	m_iNumVtxZ = CDataManager::GetInstance()->m_iNumVtxZ;
	m_fInterval = CDataManager::GetInstance()->m_fInterval;
	m_fDetail = CDataManager::GetInstance()->m_fDetail;

	Invalidate(false);

	UpdateData(FALSE);

}


void CTerrainTab::OnSpin_Vertex_X_Count(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	if (pNMUpDown->iDelta < 0)
	{
		if (GetKeyState('Z') & 0x8000)
			m_iNumVtxX += 10;
		else
			m_iNumVtxX += 1;
	}
	else
	{
		if (GetKeyState('Z') & 0x8000)
			m_iNumVtxX -= 10;
		else
			m_iNumVtxX -= 1;
	}

	if (m_iNumVtxX <= 0)
		m_iNumVtxX = 1;

	if (m_iNumVtxX >= 300)
		m_iNumVtxX = 300;

	UpdateData(FALSE);

	*pResult = 0;
}


void CTerrainTab::OnSpin_Vertex_Z_Count(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	if (pNMUpDown->iDelta < 0)
	{
		if (GetKeyState('Z') & 0x8000)
			m_iNumVtxZ += 10;
		else
			m_iNumVtxZ += 1;
	}
	else
	{
		if (GetKeyState('Z') & 0x8000)
			m_iNumVtxZ -= 10;
		else
			m_iNumVtxZ -= 1;
	}

	if (m_iNumVtxZ <= 0)
		m_iNumVtxZ = 1;

	if (m_iNumVtxZ >= 300)
		m_iNumVtxZ = 300;

	UpdateData(FALSE);

	*pResult = 0;
}


void CTerrainTab::OnSpin_Vertex_Interval(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	if (pNMUpDown->iDelta < 0)
	{
		if (GetKeyState('Z') & 0x8000)
			m_fInterval += 1;
		else
			m_fInterval += 1;
	}
	else
	{
		if (GetKeyState('Z') & 0x8000)
			m_fInterval -= 1;
		else
			m_fInterval -= 1;
	}

	if (m_fInterval <= 0)
		m_fInterval = 1;

	if (m_fInterval >= 20)
		m_fInterval = 20;

	UpdateData(FALSE);

	*pResult = 0;
}


void CTerrainTab::OnSpin_Vertex_Detail(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	if (pNMUpDown->iDelta < 0)
	{
		if (GetKeyState('Z') & 0x8000)
			m_fDetail += 1;
		else
			m_fDetail += 1;
	}
	else
	{
		if (GetKeyState('Z') & 0x8000)
			m_fDetail -= 1;
		else
			m_fDetail -= 1;
	}

	if (m_fDetail <= 0)
		m_fDetail = 1;

	if (m_fDetail >= 500)
		m_fDetail = 500;

	UpdateData(FALSE);

	*pResult = 0;
}
