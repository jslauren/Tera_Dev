
// MapToolView.cpp : CMapToolView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "MapTool.h"
#endif

#include "MapToolDoc.h"
#include "MapToolView.h"
#include "ViewManager.h"
#include "Renderer.h"
#include "Component_Manager.h"
#include "Object_Manager.h"
//
#include "SceneTerrain.h"

// Object

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMapToolView
HWND g_hWnd;
IMPLEMENT_DYNCREATE(CMapToolView, CView)

BEGIN_MESSAGE_MAP(CMapToolView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_TIMER()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

// CMapToolView 생성/소멸

CMapToolView::CMapToolView()
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CMapToolView::~CMapToolView()
{
	CViewManager::GetInstance()->DestroyInstance();
	//Safe_Release(m_pDataManager);
	Safe_Release(m_pViewManager);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pManagement);
	Safe_Release(m_pGraphicDevice);
	CManagement::Release_Engine();
}

BOOL CMapToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CMapToolView 그리기

void CMapToolView::OnDraw(CDC* /*pDC*/)
{
	CMapToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	if (nullptr == m_pGraphicDevice ||
		nullptr == m_pRenderer)
		return;

	m_pGraphicDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DXCOLOR(0.f, 0.f, 1.f, 1.f), 1.f, 0);
	m_pGraphicDevice->BeginScene();

	//렌더 컴포넌트의 렌더 수행
	if (FAILED(m_pRenderer->Render_RenderGroup()))
		return;

	m_pGraphicDevice->EndScene();
	m_pGraphicDevice->Present(nullptr, nullptr, 0, nullptr);

}


// CMapToolView 인쇄

BOOL CMapToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CMapToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CMapToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

HRESULT CMapToolView::InitDefaultSetting(CGraphic_Device::WINMODE eType, const _uint & iWinCX, const _uint & iWinCY)
{
	CMainFrame*		pMainFrm = ((CMainFrame*)AfxGetMainWnd());

	RECT		rcWindow;
	pMainFrm->GetWindowRect(&rcWindow);	 // 윈도우 창 프레임의 사이즈를 얻어오는 함수

	SetRect(&rcWindow,	// 프레임 크기의 가로와 세로 사이즈를 새로운 렉트에 right, bottom에 저장
		0,
		0,
		rcWindow.right - rcWindow.left,
		rcWindow.bottom - rcWindow.top);


	RECT	rcMainView;
	GetClientRect(&rcMainView);	// 순수한 뷰 창의 크기를 얻어오는 함수

	float	fRowFrm = float(rcWindow.right - rcMainView.right);
	float	fColFrm = float(rcWindow.bottom - rcMainView.bottom);

	// 뷰 창의 좌표들을 0,0 기준으로 출력할 수 있게 창의 위치를 재조정하는 함수
	pMainFrm->SetWindowPos(NULL,
		0,
		0,
		int(g_iWinCX + fRowFrm),
		int(g_iWinCY + fColFrm),
		SWP_NOZORDER);

	if (nullptr == m_pManagement)
		return E_FAIL;

	if (FAILED(CGraphic_Device::GetInstance()->Ready_Graphic_Device(g_hWnd, eType, iWinCX, iWinCY, &m_pGraphicDevice)))
		return E_FAIL;

	m_pViewManager->m_pGraphic_Device = m_pGraphicDevice;

	if (FAILED(m_pManagement->Ready_Management(SCENE_END)))
		return E_FAIL;

	return NOERROR;
}

HRESULT CMapToolView::InitRenderState()
{
	m_pGraphicDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pGraphicDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	return NOERROR;
}

HRESULT CMapToolView::InitComponentPrototype()
{
	CComponent_Manager*	pComponentManager = CComponent_Manager::GetInstance();
	if (nullptr == pComponentManager)
		return E_FAIL;

	if (FAILED(pComponentManager->Add_Component_Prototype(SCENE_STATIC, L"Component_Transform", CTransform::Create(m_pGraphicDevice))))
		return E_FAIL;

	m_pRenderer = CRenderer::Create(m_pGraphicDevice);
	if (FAILED(pComponentManager->Add_Component_Prototype(SCENE_STATIC, L"Component_Renderer", m_pRenderer)))
		return E_FAIL;

	if (FAILED(pComponentManager->Add_Component_Prototype(SCENE_STATIC, L"Component_BufferRctCol", CBuffer_RcCol::Create(m_pGraphicDevice))))
		return E_FAIL;

	if (FAILED(pComponentManager->Add_Component_Prototype(SCENE_STATIC, L"Component_BufferRctTex", CBuffer_RcTex::Create(m_pGraphicDevice))))
		return E_FAIL;

	if (FAILED(pComponentManager->Add_Component_Prototype(SCENE_STATIC, L"Component_BufferCubTex", CBuffer_CubeTex::Create(m_pGraphicDevice))))
		return E_FAIL;

	if (FAILED(pComponentManager->Add_Component_Prototype(SCENE_STATIC, L"Component_Terrain", CBuffer_Terrain::Create(m_pGraphicDevice))))
		return E_FAIL;

	if (FAILED(pComponentManager->Add_Component_Prototype(SCENE_STATIC, L"Component_Terrain_Tex", CBuffer_Terrain::Create(m_pGraphicDevice))))
		return E_FAIL;

	//if (FAILED(pComponentManager->Add_Component_Prototype(TOOL_STATIC, L"Component_Texture", CTexture::Create(m_pGraphicDevice, CTexture::TYPE_GENERAL, L"../Bin/Resources/Textures/Cat.jpg"))))
	//	return E_FAIL;

	//if (FAILED(pComponentManager->Add_Component_Prototype(TOOL_STATIC, L"Component_Texture_Guid", CTexture::Create(m_pGraphicDevice, CTexture::TYPE_GENERAL, L"../Bin/Resources/Textures/GuidLine.png"))))
	//	return E_FAIL;

	return NOERROR;
}

HRESULT CMapToolView::InitObjectPrototype()
{
	//if (FAILED(CObject_Manager::GetInstance()->Add_Object_Prototype(TOOL_STATIC, L"Camera_Tile_45", m_pViewManager->m_pTileCamera = CTileCamera::Create(m_pGraphicDevice))))
	//	return E_FAIL;

	//if (FAILED(CObject_Manager::GetInstance()->AddObject_Prototype(TOOL_STATIC, L"Terrain", CPlaneTerrain::Create(m_pGraphicDevice))))
	//	return E_FAIL;

	//if (FAILED(CObject_Manager::GetInstance()->AddObject_Prototype(TOOL_STATIC, L"Cube_Tile", CTileCube::Create(m_pGraphicDevice))))
	//	return E_FAIL;

	return NOERROR;
}


// CMapToolView 진단

#ifdef _DEBUG
void CMapToolView::AssertValid() const
{
	CView::AssertValid();
}

void CMapToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMapToolDoc* CMapToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMapToolDoc)));
	return (CMapToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CMapToolView 메시지 처리기


void CMapToolView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	m_pViewManager = CViewManager::GetInstance();
//	m_pDataManager = CDataManager::GetInstance();

	g_hWnd = m_hWnd;

	{
		m_pManagement = CManagement::GetInstance();
		m_pManagement->AddRef();
	}

	if (FAILED(InitDefaultSetting(CGraphic_Device::TYPE_WINMODE, g_iWinCX, g_iWinCY)))
		return;

	if (FAILED(InitRenderState()))
		return;

	// 전역씬에서 사용할 원형컴포넌트들의 생성.
	if (FAILED(InitComponentPrototype()))
		return;

	if (FAILED(InitObjectPrototype()))
		return;

	m_pViewManager->m_pMainView = this;

	SetTimer(1, 20, NULL);

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
}


void CMapToolView::OnTimer(UINT_PTR nIDEvent)
{
	m_pManagement->Update_Management(1.f);
	Invalidate(FALSE);

	CView::OnTimer(nIDEvent);
}


void CMapToolView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	
	m_pViewManager->m_vMousePos = _vec3(float(point.x), float(point.y), 0.f);
	
	_tchar mouse[128] = L"";
	wsprintf(mouse, L"Mouse : %d, %d ", point.x, point.y);

	m_pViewManager->m_pMainFrame->SetStatusBar(1, mouse);

	//Invalidate(FALSE);
	CView::OnMouseMove(nFlags, point);
}


void CMapToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CView::OnLButtonDown(nFlags, point);
}


void CMapToolView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CView::OnRButtonDown(nFlags, point);
}


BOOL CMapToolView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}
