#pragma once
#include "afxwin.h"

// CProtoTab 대화 상자입니다.

class CSceneProto;
class CProtoTab : public CDialogEx
{
	DECLARE_DYNAMIC(CProtoTab)

public:
	CProtoTab(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CProtoTab();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROTOTAB };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	CSceneProto* m_pScene = nullptr;

};
