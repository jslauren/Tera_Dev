#pragma once


// CCameraTab 대화 상자입니다.

class CCameraTab;
class CCameraTab : public CDialogEx
{
	DECLARE_DYNAMIC(CCameraTab)

public:
	CCameraTab(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CCameraTab();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CAMERATAB };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	CCameraTab* m_pScene = nullptr;

};
