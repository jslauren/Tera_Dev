#pragma once


// CEffectTab ��ȭ �����Դϴ�.
class CSceneEffect;
class CEffectTab : public CDialogEx
{
	DECLARE_DYNAMIC(CEffectTab)

public:
	CEffectTab(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CEffectTab();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EFFECTTAB };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:
	CSceneEffect*	m_pScene = nullptr;

};
