// EditorView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MapTool.h"
#include "EditorView.h"


// CEditorView

IMPLEMENT_DYNCREATE(CEditorView, CEditView)

CEditorView::CEditorView()
{

}

CEditorView::~CEditorView()
{
}

BEGIN_MESSAGE_MAP(CEditorView, CEditView)
END_MESSAGE_MAP()


// CEditorView 진단입니다.

#ifdef _DEBUG
void CEditorView::AssertValid() const
{
	CEditView::AssertValid();
}

#ifndef _WIN32_WCE
void CEditorView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}
#endif
#endif //_DEBUG


// CEditorView 메시지 처리기입니다.
