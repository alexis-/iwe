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

// IntuitiveEditorView.cpp : implementation of the CIntuitiveEditorView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "IntuitiveEditor.h"
#endif


#include "IntuitiveEditorDoc.h"
#include "IntuitiveEditorView.h"

#include "ChildFrm.h"
#include "UIManager.h"


// CIntuitiveEditorView

IMPLEMENT_DYNCREATE(CIntuitiveEditorView, CView)

BEGIN_MESSAGE_MAP(CIntuitiveEditorView, CView)
	ON_WM_CREATE()
	ON_WM_SHOWWINDOW()

	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSEMOVE()

	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_MOUSELEAVE()
END_MESSAGE_MAP()

// CIntuitiveEditorView construction/destruction

CIntuitiveEditorView::CIntuitiveEditorView()
	: m_cOverrideMouseMove(0),
    m_bMouseTracking(false)
{
}

CIntuitiveEditorView::~CIntuitiveEditorView()
{
}

int CIntuitiveEditorView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	return CView::OnCreate(lpCreateStruct);
}

BOOL CIntuitiveEditorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CIntuitiveEditorView drawing

void CIntuitiveEditorView::OnDraw(CDC* /*pDC*/)
{
	CIntuitiveEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
}

// CIntuitiveEditorView diagnostics

#ifdef _DEBUG
void CIntuitiveEditorView::AssertValid() const
{
	CView::AssertValid();
}

void CIntuitiveEditorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CIntuitiveEditorDoc* CIntuitiveEditorView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CIntuitiveEditorDoc)));
	return (CIntuitiveEditorDoc*)m_pDocument;
}
#endif //_DEBUG

void CIntuitiveEditorView::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CView::OnShowWindow(bShow, nStatus);
	IViewport* pViewport = getViewport();

	if (!pViewport)
		return;

	pViewport->showWindow(bShow);
}

void CIntuitiveEditorView::OnInitialUpdate()
{
  CView::OnInitialUpdate();
	VERIFY(m_DropTarget.Register(this));

  GetParent()->SetWindowText("Viewport");
}

IViewport*
CIntuitiveEditorView::getViewport()
{
	return ((CChildFrame*)GetParent())->getViewport();
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void CIntuitiveEditorView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	IViewport* pViewport = getViewport();

	if (!pViewport)
		return;

  pViewport->onKeyboardInput(IWEKeys::getKey(nChar), true, nRepCnt);

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CIntuitiveEditorView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	IViewport* pViewport = getViewport();

	if (!pViewport)
		return;

	pViewport->onKeyboardInput(IWEKeys::getKey(nChar), false, nRepCnt);

	CView::OnKeyUp(nChar, nRepCnt, nFlags);
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void CIntuitiveEditorView::OnLButtonDown(UINT nFlags, CPoint point)
{
	IViewport* pViewport = getViewport();
	s_inputDatas datas;

	if (!pViewport)
		return;

	datas.m_MouseAction = IPT_MOUSE_DOWN;
  datas.m_Pt = CIWEPoint<int>(point.x, point.y);
  datas.m_MouseButton = IPT_MOUSE_LEFT;

	pViewport->onMouseInput(datas);

	CView::OnLButtonDown(nFlags, point);
}

void CIntuitiveEditorView::OnLButtonUp(UINT nFlags, CPoint point)
{
	IViewport* pViewport = getViewport();
	s_inputDatas datas;

	if (!pViewport)
		return;

	datas.m_MouseAction = IPT_MOUSE_UP;
  datas.m_Pt = CIWEPoint<int>(point.x, point.y);
  datas.m_MouseButton = IPT_MOUSE_LEFT;

	pViewport->onMouseInput(datas);

	CView::OnLButtonUp(nFlags, point);
}

void CIntuitiveEditorView::OnRButtonDown(UINT nFlags, CPoint point)
{
	IViewport* pViewport = getViewport();
	s_inputDatas datas;

	if (!pViewport)
		return;

	datas.m_MouseAction = IPT_MOUSE_DOWN;
  datas.m_Pt = CIWEPoint<int>(point.x, point.y);
  datas.m_MouseButton = IPT_MOUSE_RIGHT;

	pViewport->onMouseInput(datas);

	CView::OnRButtonDown(nFlags, point);
}

void CIntuitiveEditorView::OnRButtonUp(UINT nFlags, CPoint point)
{
	IViewport* pViewport = getViewport();
	s_inputDatas datas;

	if (!pViewport)
		return;

	datas.m_MouseAction = IPT_MOUSE_UP;
  datas.m_Pt = CIWEPoint<int>(point.x, point.y);
  datas.m_MouseButton = IPT_MOUSE_RIGHT;

	pViewport->onMouseInput(datas);

	CView::OnRButtonUp(nFlags, point);
}

void CIntuitiveEditorView::OnMButtonDown(UINT nFlags, CPoint point)
{
	IViewport* pViewport = getViewport();
	s_inputDatas datas;

	if (!pViewport)
		return;

	datas.m_MouseAction = IPT_MOUSE_DOWN;
  datas.m_Pt = CIWEPoint<int>(point.x, point.y);
  datas.m_MouseButton = IPT_MOUSE_MIDDLE;

	pViewport->onMouseInput(datas);

	CView::OnMButtonDown(nFlags, point);
}

void CIntuitiveEditorView::OnMButtonUp(UINT nFlags, CPoint point)
{
	IViewport* pViewport = getViewport();
	s_inputDatas datas;

	if (!pViewport)
		return;

	datas.m_MouseAction = IPT_MOUSE_UP;
  datas.m_Pt = CIWEPoint<int>(point.x, point.y);
  datas.m_MouseButton = IPT_MOUSE_MIDDLE;

	pViewport->onMouseInput(datas);

	CView::OnMButtonUp(nFlags, point);
}

BOOL CIntuitiveEditorView::OnMouseWheel(UINT nFlags, short zDelta, CPoint point)
{
	IViewport* pViewport = getViewport();
	s_inputDatas datas;

	if (!pViewport)
		return CView::OnMouseWheel(nFlags, zDelta, point);

	datas.m_sDelta = zDelta;
  datas.m_Pt = CIWEPoint<int>(point.x, point.y);
  datas.m_MouseButton = IPT_MOUSE_WHEEL;

	pViewport->onMouseInput(datas);

	return CView::OnMouseWheel(nFlags, zDelta, point);
}

void CIntuitiveEditorView::OnMouseMove(UINT nFlags, CPoint point)
{
  if (!m_bMouseTracking)
  {
    TRACKMOUSEEVENT tme;

    tme.cbSize = sizeof(TRACKMOUSEEVENT);
    tme.dwFlags = TME_LEAVE;
    tme.hwndTrack = this->m_hWnd;

    if (::_TrackMouseEvent(&tme))
      m_bMouseTracking = TRUE;
  }

	IViewport* pViewport = getViewport();
	s_inputDatas datas;

	if (!pViewport)
		return;

  datas.m_Pt = CIWEPoint<int>(point.x, point.y);
  datas.m_MouseButton = IPT_MOUSE_MOVE;

	if (m_cOverrideMouseMove == 0)
	{
		m_cOverrideMouseMove = (char)pViewport->onMouseInput(datas);

		if (m_cOverrideMouseMove == 1)
			SetCapture();
	}
	else if (m_cOverrideMouseMove == 1)
		m_cOverrideMouseMove = 2;
	else
	{
		m_cOverrideMouseMove = (char)pViewport->onMouseInput(datas);

		if (m_cOverrideMouseMove == 0)
			ReleaseCapture();
		else
			m_cOverrideMouseMove = 1;
	}

	CView::OnMouseMove(nFlags, point);
}


//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

DROPEFFECT CIntuitiveEditorView::OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	if (pDataObject->IsDataAvailable(CF_TEXT))
	{
		char* pText = (char*)GlobalLock(pDataObject->GetGlobalData(CF_TEXT));

    e_dropEffect ret = UI_DROPEFFECT_NONE;
  
    CUIManager::getInstance()->eventDragEnter(pText, getViewport(), CIWEPoint<int>(point.x, point.y),
      [&ret](e_dropEffect e) { if (e != UI_DROPEFFECT_NONE) ret = e; });

    if (ret != UI_DROPEFFECT_NONE)
      return getDropEffect(ret);
	}
	
	return CView::OnDragEnter(pDataObject, dwKeyState, point);
}


void CIntuitiveEditorView::OnDragLeave()
{
  CUIManager::getInstance()->eventDragLeave(getViewport());

	CView::OnDragLeave();
}


DROPEFFECT CIntuitiveEditorView::OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
  e_dropEffect ret = UI_DROPEFFECT_NONE;
  
  CUIManager::getInstance()->eventDragOver(getViewport(), CIWEPoint<int>(point.x, point.y),
    [&ret](e_dropEffect e) { if (e != UI_DROPEFFECT_NONE) ret = e; });

	return (ret == UI_DROPEFFECT_NONE) ? CView::OnDragOver(pDataObject, dwKeyState, point) : getDropEffect(ret);
}


BOOL CIntuitiveEditorView::OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point)
{
  bool ret = false;
  
  CUIManager::getInstance()->eventDrop(getViewport(), CIWEPoint<int>(point.x, point.y),
    [&ret](bool b) { if (b) ret = true; });

	return (!ret) ? CView::OnDrop(pDataObject, dropEffect, point) : true;
}


void CIntuitiveEditorView::OnMouseLeave()
{
  m_bMouseTracking = false;

	IViewport* pViewport = getViewport();
	pViewport->onMouseLeave();

	CView::OnMouseLeave();
}


LRESULT CIntuitiveEditorView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_MOUSELEAVE)
	{
		IViewport* pViewport = getViewport();
		pViewport->onMouseLeave();
	}

	return CView::WindowProc(message, wParam, lParam);
}