#pragma once
#include "afxcmn.h"
#include "EditorView.h"

// CEditorView 뷰입니다.

class CEditorView : public CEditView
{
	DECLARE_DYNCREATE(CEditorView)

protected:
	CEditorView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CEditorView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
};


