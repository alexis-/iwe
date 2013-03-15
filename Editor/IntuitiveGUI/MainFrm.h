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

// MainFrm.h : interface of the CMainFrame class
//

#pragma once
#include "FileView.h"
#include "ClassView.h"
#include "OutputWnd.h"
#include "PropertiesWnd.h"
#include "AssetViewer.h"

class CMainFrame;
class ITool;

typedef void (CMainFrame::*f_msgCallback)(UINT);
typedef FastDelegate3<int, const std::string&, const CImage&> fBrushChangeCallback;

class CMainFrame : public CMDIFrameWndEx
{
public:
	static CMainFrame* getInstance();
	
protected: // create from serialization only
	DECLARE_DYNCREATE(CMainFrame)
	
public:
	CMainFrame();

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
  
	void													OnToolsUIUpdate(CCmdUI* pCmdUI);

protected:  // control bar embedded members
	CMFCRibbonBar									m_wndRibbonBar;
	CMFCRibbonApplicationButton		m_MainButton;
	CMFCToolBarImages							m_PanelImages;
	CMFCRibbonStatusBar						m_wndStatusBar;
	CFileView											m_wndFileView;
	CClassView										m_wndClassView;
	COutputWnd										m_wndOutput;
	CPropertiesWnd								m_wndProperties;
	CMFCCaptionBar								m_wndCaptionBar;
	CAssetViewer									m_wndAssetViewer;
	unsigned long									m_ulTime;
	unsigned long									m_ulFrames;
  CDocument*                    m_mainDoc;
	
	bool													m_bInit;

	static CMainFrame*						m_pInst;

	// Hack to deal with ogre being non thread-safe
	CCriticalSection							m_critSection;

public:
  CCriticalSection*             getCriticalSection()
  {
    return &m_critSection;
  }
  

  void                          activateMainDoc();

// Generated message map functions
protected:
	afx_msg int										OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void									OnApplicationLook(UINT id);
	afx_msg void									OnUpdateApplicationLook(CCmdUI* pCmdUI);
	afx_msg void									OnViewCaptionBar();
	afx_msg void									OnUpdateViewCaptionBar(CCmdUI* pCmdUI);
	afx_msg void									OnOptions();
	afx_msg void									OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	//afx_msg void								OnWindowManager();
  
  void                          OnCameraCreated(ICameraController* pController);
  void                          OnCameraMoved(ICameraController* pViewport);

#ifdef X64
	afx_msg void 									OnTimer(UINT_PTR TimerVal);
#else

	afx_msg void 									OnTimer(UINT TimerVal);
#endif // !X64
	DECLARE_MESSAGE_MAP()

	BOOL													CreateDockingWindows();
	void													SetDockingWindowIcons(BOOL bHiColorIcons);
	BOOL													CreateCaptionBar();

protected:
	afx_msg LRESULT OnOutputAdd(WPARAM wParam, LPARAM lParam);
};


