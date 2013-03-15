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

// ChildFrm.cpp : implementation of the CChildFrame class
//

#include "stdafx.h"
#include "IntuitiveEditor.h"
//#include "IntuitiveEditorDoc.h"
//#include "IntuitiveEditorView.h"

#include "ChildFrm.h"


// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWndEx)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWndEx)
	ON_WM_CREATE()
	ON_WM_SIZE()
  ON_WM_SYSCOMMAND()
END_MESSAGE_MAP()

// CChildFrame construction/destruction

CChildFrame::CChildFrame()
	: m_bInitSplitter(false),
		m_pViewport(NULL)
{
	// TODO: add member initialization code here
}

CChildFrame::~CChildFrame()
{
}


BOOL CChildFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/, CCreateContext* pContext)
{	
	//CRect cr;
	//GetWindowRect(&cr );

	//if(!m_wndSplitter.CreateStatic(this, 1, 2))
	//{
	//		TRACE("Failed to create splitter window");
	//		return false;
	//}

	//if (!m_wndSplitter.CreateView(0, 0,
	//	pContext->m_pNewViewClass,
	//	//RUNTIME_CLASS(CIntuitiveEditorView),
	//	CSize(cr.Width()/2, cr.Height()),	// TODO: adjust the minimum pane size
	//	pContext))
	//{
	//		TRACE("Failed to create splitter window");
	//		return false;
	//}
	//
	//if (!m_wndSplitter.CreateView(0, 1,
	//	pContext->m_pNewViewClass,
	//	//RUNTIME_CLASS(CIntuitiveEditorView),
	//	CSize(cr.Width()/2, cr.Height()),	// TODO: adjust the minimum pane size
	//	pContext))
	//{
	//		TRACE("Failed to create splitter window");
	//		return false;
	//}

	//m_bInitSplitter = true; 

	//return true;
	return (CreateView(pContext, AFX_IDW_PANE_FIRST) != NULL);
}

BOOL CChildFrame::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
                         DWORD dwStyle, const RECT& rect,
                         CMDIFrameWnd* pParentWnd, CCreateContext* pContext)
{
  if (CMDIChildWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, pContext))
  {
    CMenu* p = GetSystemMenu(FALSE);

    p->RemoveMenu(SC_CLOSE, MF_BYCOMMAND);

    return TRUE;
  }

  return FALSE;
}

int
CChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIChildWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	RECT rc;
	GetClientRect(&rc);
  CIWEPoint<ushort> size(rc.right, rc.bottom);

	m_pViewport = GetRenderEngine()->createViewport((size_t)GetSafeHwnd(), size, 1);

	//GetRenderEngine()->setupResources();
	//CResourceManager::getInstance()->update();
  GetResourceMgr()->update();

	//ShowWindow(SW_SHOWMAXIMIZED);

	return 0;
}

void CChildFrame::OnSize(UINT nType, int cx, int cy)
{
	CMDIChildWndEx::OnSize(nType, cx, cy);
	
	//CRect rc;
	//GetWindowRect(&rc);
	//

	//if (m_bInitSplitter && nType != SIZE_MINIMIZED)
	//{
	//	m_wndSplitter.SetRowInfo(0, cy, 0 );
	//	m_wndSplitter.SetColumnInfo(0, rc.Width()*0.5, 50);
	//	m_wndSplitter.SetColumnInfo(1, rc.Width()*0.5, 50);

	//	m_wndSplitter.RecalcLayout();
	//}

  if (m_pViewport)
    m_pViewport->resize(cx, cy);
}


BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying the CREATESTRUCT cs
	if(!CMDIChildWndEx::PreCreateWindow(cs) )
		return FALSE;

	cs.style &= ~(LONG)FWS_ADDTOTITLE & ~(LONG)WS_MINIMIZEBOX & ~(LONG)WS_MAXIMIZEBOX;

	return TRUE;
}

// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWndEx::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWndEx::Dump(dc);
}
#endif //_DEBUG

// CChildFrame message handlers


void CChildFrame::OnSysCommand(UINT nID, LPARAM lParam)
{
  if(nID == SC_CLOSE)
    return;

  CMDIChildWndEx::OnSysCommand(nID, lParam);
}