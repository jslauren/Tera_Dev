#pragma once
#include "afxcmn.h"
#include "Tool_Defines.h"
#include "afxwin.h"
#include "EventManagerTool.h"

// CTerrainTab ��ȭ �����Դϴ�.
class CSceneTerrain;
class CTerrainTab : public CDialogEx
{
	DECLARE_DYNAMIC(CTerrainTab)

public:
	CTerrainTab(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CTerrainTab();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TERRAINTAB };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:
	CSceneTerrain*	m_pScene = nullptr;

	virtual void OnOK();
	virtual BOOL OnInitDialog();

	CButton Terrain_RenderST_Solild;
	CButton Terrain_RenderCood_Axis;

	afx_msg void OnBnClickedWireFrame();
	afx_msg void OnBnClickedSolid();

	CEdit Vertex_X_Count;
	CEdit Vertex_Z_Count;
	CEdit Vertex_Interval;
	CEdit Vertex_Detail;

	CSpinButtonCtrl Vertex_X_Count_Btn;
	CSpinButtonCtrl Vertex_Z_Count_Btn;
	CSpinButtonCtrl Vertex_Interval_Btn;
	CSpinButtonCtrl Vertex_Detail_Btn;

private:
	// Terrain_Vertex_Variable
	_int	m_iNumVtxX = 0;
	_int	m_iNumVtxZ = 0;
	_float	m_fInterval = 0;
	_float	m_fDetail = 0;

	// Terrain_Transform_Variable
	_int	m_iPosX = 0;
	_int	m_iPosY = 0;
	_int	m_iPosZ = 0;
	_float	m_fRotX = 0.f;
	_float	m_fRotY = 0.f;
	_float	m_fRotZ = 0.f;

public:
	afx_msg void OnBnClickedTerrain_Apply();

};
