#pragma once


// CCameraTab ��ȭ �����Դϴ�.

class CCameraTab;
class CCameraTab : public CDialogEx
{
	DECLARE_DYNAMIC(CCameraTab)

public:
	CCameraTab(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CCameraTab();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CAMERATAB };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:
	CCameraTab* m_pScene = nullptr;

};
