#pragma once
#include "afxcmn.h"
#include "EditorView.h"

// CEditorView ���Դϴ�.

class CEditorView : public CEditView
{
	DECLARE_DYNCREATE(CEditorView)

protected:
	CEditorView();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
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


