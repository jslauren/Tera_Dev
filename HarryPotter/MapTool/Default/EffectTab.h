#pragma once


// CEffectTab 대화 상자입니다.
class CSceneEffect;
class CEffectTab : public CDialogEx
{
	DECLARE_DYNAMIC(CEffectTab)

public:
	CEffectTab(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CEffectTab();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EFFECTTAB };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	CSceneEffect*	m_pScene = nullptr;

};
