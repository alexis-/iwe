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

// IntuitiveEditorView.h : interface of the CIntuitiveEditorView class
//

#pragma once

class IBaseEntity;

class CIntuitiveEditorView : public CView
{
protected: // create from serialization only
	CIntuitiveEditorView();
	DECLARE_DYNCREATE(CIntuitiveEditorView)

// Attributes
public:
	CIntuitiveEditorDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();

	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
protected:

// Implementation
public:
	virtual ~CIntuitiveEditorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	IViewport*		getViewport();

// Generated message map functions
protected:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg virtual int OnCreate(LPCREATESTRUCT lpCreateStruct);
	
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);

	DECLARE_MESSAGE_MAP()

private:
	char							m_cOverrideMouseMove;
  bool              m_bMouseTracking;
	COleDropTarget		m_DropTarget;

public:
	virtual DROPEFFECT OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual void OnDragLeave();
	virtual DROPEFFECT OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual BOOL OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);
	afx_msg void OnMouseLeave();
};

#ifndef _DEBUG  // debug version in IntuitiveEditorView.cpp
inline CIntuitiveEditorDoc* CIntuitiveEditorView::GetDocument() const
   { return reinterpret_cast<CIntuitiveEditorDoc*>(m_pDocument); }
#endif

