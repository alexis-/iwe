// This MFC Samples source code demonstrates using MFC Microsoft Office Fluent User Interface 
// (the "Fluent UI") and is provided only as referential material to supplement the 
// Microsoft Foundation Classes Reference and related electronic documentation 
// included with the MFC C++ library software.  
// License terms to copy, use or distribute the Fluent UI are available separately.  
// To learn more about our Fluent UI licensing program, please visit 
// http://msdn.microsoft.com/officeui.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// IntuitiveEditorView.cpp : implementation of the CCLIView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "IntuitiveEditor.h"
#endif


#include "CLIDoc.h"
#include "CLIView.h"

#include "CLIChildFrm.h"


gcroot<System::Type^>     CCLIView::m_type;
std::string               CCLIView::m_title = "";

// CCLIView

IMPLEMENT_DYNCREATE(CCLIView, CWinFormsView)

BEGIN_MESSAGE_MAP(CCLIView, CWinFormsView)
	ON_WM_CREATE()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

// CCLIView construction/destruction

CCLIView::CCLIView()
	: CWinFormsView(m_type),
    m_curTitle(m_title)
{
}

CCLIView::~CCLIView()
{
}

int CCLIView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	return CView::OnCreate(lpCreateStruct);
}

BOOL CCLIView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CWinFormsView::PreCreateWindow(cs);
}

// CCLIView drawing

void CCLIView::OnDraw(CDC* /*pDC*/)
{
  CCLIDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
}

// CCLIView diagnostics

#ifdef _DEBUG
void CCLIView::AssertValid() const
{
	CWinFormsView::AssertValid();
}

void CCLIView::Dump(CDumpContext& dc) const
{
	CWinFormsView::Dump(dc);
}

CCLIDoc* CCLIView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCLIDoc)));
	return (CCLIDoc*)m_pDocument;
}
#endif //_DEBUG

void CCLIView::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CWinFormsView::OnShowWindow(bShow, nStatus);
}

void CCLIView::OnInitialUpdate()
{
  
  CView::OnInitialUpdate();
  GetParent()->SetWindowText(m_curTitle.c_str());
}