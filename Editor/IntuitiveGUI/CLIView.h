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

// IntuitiveEditorView.h : interface of the CCLIView class
//

#pragma once

class CCLIDoc;
#include <afxwinforms.h>

class CCLIView : public CWinFormsView
{
protected: // create from serialization only
	CCLIView();
	DECLARE_DYNCREATE(CCLIView)

public:
  static gcroot<System::Type^>    m_type;
  static std::string              m_title;

  std::string m_curTitle;

// Attributes
public:
	CCLIDoc* GetDocument() const;

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();


// Implementation
public:
	virtual ~CCLIView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg virtual int OnCreate(LPCREATESTRUCT lpCreateStruct);

	DECLARE_MESSAGE_MAP()

};

#ifndef _DEBUG  // debug version in IntuitiveEditorView.cpp
inline CCLIDoc* CCLIView::GetDocument() const
   { return reinterpret_cast<CCLIDoc*>(m_pDocument); }
#endif

