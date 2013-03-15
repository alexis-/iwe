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

// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "IntuitiveEditor.h"

#include "MainFrm.h"

#include "UIManager.h"

// CMainFrame

CMainFrame*				CMainFrame::m_pInst = NULL;

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWndEx)
	ON_WM_CREATE()

	// Global help commands
	ON_COMMAND(ID_HELP_FINDER, &CMDIFrameWndEx::OnHelpFinder)
	ON_COMMAND(ID_HELP, &CMDIFrameWndEx::OnHelp)
	ON_COMMAND(ID_CONTEXT_HELP, &CMDIFrameWndEx::OnContextHelp)
	ON_COMMAND(ID_DEFAULT_HELP, &CMDIFrameWndEx::OnHelpFinder)

	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)
	ON_COMMAND(ID_VIEW_CAPTION_BAR, &CMainFrame::OnViewCaptionBar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_CAPTION_BAR, &CMainFrame::OnUpdateViewCaptionBar)
	ON_COMMAND(ID_TOOLS_OPTIONS, &CMainFrame::OnOptions)
	ON_WM_SETTINGCHANGE()
	ON_WM_TIMER()

	ON_UPDATE_COMMAND_UI_RANGE(CUIManager::m_nUIStartingID, CUIManager::m_nUIStartingID + 500, &CMainFrame::OnToolsUIUpdate)
	ON_MESSAGE(WM_OUTPUT_ADD, &CMainFrame::OnOutputAdd)
END_MESSAGE_MAP()



#ifdef X64
void
CMainFrame::OnTimer(UINT_PTR TimerVal)
#else
void
CMainFrame::OnTimer(UINT TimerVal)
#endif // !X64
{
	try
	{
		if (!m_ulTime)
			m_ulTime = GetTickCount();
	
		if (GetTickCount() - m_ulTime > 1000)
		{
			CMFCRibbonStatusBarPane* elem = (CMFCRibbonStatusBarPane*)m_wndStatusBar.GetElement(0);
			if (elem)
			{
				char buffer[10];
				_snprintf(buffer, 10, "FPS: %u", m_ulFrames);
			
				elem->SetText(buffer);
				elem->Redraw();
			}

			m_ulTime = GetTickCount();
			m_ulFrames = 0;
		}

		GetToolMgr()->tick();

		if (m_ulTime + m_ulFrames * iMaxFPSDelay < GetTickCount() && m_ulFrames < iMaxFPS)
		{
			SYNCHRONIZED(syn, &m_critSection)
			{
				GetRenderEngine()->render();
				m_ulFrames++;
			}
		}
	}
	catch (...)
	{
	}
}

// CMainFrame construction/destruction

CMainFrame::CMainFrame()
	: m_ulTime(0),
		m_ulFrames(0),
		m_bInit(false)
{
	// TODO: add member initialization code here
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_VS_2008);
	m_pInst = this;

	//pTerrainHeightTool = CToolManager::getInstance()->getTool("TerrainHeightTool");
}

CMainFrame::~CMainFrame()
{
	//CEntityManager::getInstance()->setClosing();

	//delete CProjectManager::getInstance();
	//delete CTerrainManager::getInstance();
 // delete CVoxelTerrainManager::getInstance();
	//delete CResourceManager::getInstance();
	//delete CBrushManager::getInstance();
 // delete CTextureManager::getInstance();
 // delete CMaterialManager::getInstance();
	//delete CEntityManager::getInstance();
	//delete CToolManager::getInstance();
	//delete CEditorInteraction::getInstance();
	//delete CSaveSystemManager::getInstance();
 // delete COgreAL::getInstance();

	delete GetRenderEngine();

  delete CGUIInterface::getInstance();
}

void
CMainFrame::OnCameraCreated(ICameraController* pController)
{
  pController->eventCameraMoved +=
    FastDelegate1<ICameraController*>(this, &CMainFrame::OnCameraMoved);
}

namespace Ogre
{
  class Vector3 {
  public:
    float x;
    float y;
    float z;
  };
};

void
CMainFrame::OnCameraMoved(ICameraController* pController)
{		
	CMFCRibbonStatusBarPane* elem = (CMFCRibbonStatusBarPane*)m_wndStatusBar.GetExElement(0);

	if (elem)
	{
    const Ogre::Vector3& camPos = pController->getViewport()->getCamera()->getPosition();
		char buffer[41];
    
		int len = _snprintf(buffer, 41, "(%.1f, %.1f, %.1f)", camPos.x, camPos.y, camPos.z);
		for (int i = len; i < 40; i++)
			buffer[i] = ' ';
    
		buffer[40] = '\0';
			
		elem->SetText(buffer);
		elem->Redraw();
	}
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{ 
  CGUIInterface::getInstance()->Init();

	if (!GetRenderEngine()->init())
		return -1;

  GetRenderEngine()->eventCameraControllerCreated +=
    FastDelegate1<ICameraController*>(this, &CMainFrame::OnCameraCreated);

	if (CMDIFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bNameValid;
	// set the visual manager and style based on persisted value
	OnApplicationLook(theApp.m_nAppLook);

	// Alexis: Decomment this for tabbed viewports
	CMDITabInfo mdiTabParams;
	mdiTabParams.m_style = CMFCTabCtrl::STYLE_3D_ONENOTE; // other styles available...
	mdiTabParams.m_bActiveTabCloseButton = TRUE;      // set to FALSE to place close button at right of tab area
	mdiTabParams.m_bTabIcons = FALSE;    // set to TRUE to enable document icons on MDI taba
	mdiTabParams.m_bAutoColor = TRUE;    // set to FALSE to disable auto-coloring of MDI tabs
	mdiTabParams.m_bDocumentMenu = TRUE; // enable the document menu at the right edge of the tab area
	EnableMDITabbedGroups(TRUE, mdiTabParams);

	m_wndRibbonBar.Create(this);
	m_wndRibbonBar.LoadFromResource(IDR_RIBBON);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	CString strTitlePane1("FPS: 0");
	CString strTitlePane2("(0.0, 0.0, 0.0)                     .");
	//CString strTitlePane3(".                                                    .");
	//bNameValid = strTitlePane1.LoadString(IDS_STATUS_PANE1);
	//ASSERT(bNameValid);
	//bNameValid = strTitlePane2.LoadString(IDS_STATUS_PANE2);
	//ASSERT(bNameValid);
	CMFCRibbonStatusBarPane* pane1 = new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE2, strTitlePane2, TRUE);
	//CMFCRibbonStatusBarPane* pane2 = NEW CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE2 + 1, strTitlePane3, TRUE);

	m_wndStatusBar.AddElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE1, strTitlePane1, TRUE), strTitlePane1);
	m_wndStatusBar.AddExtendedElement(pane1, strTitlePane2);
	//m_wndStatusBar.AddSeparator();
	//m_wndStatusBar.AddExtendedElement(pane2, strTitlePane3);

	// enable Visual Studio 2005 style docking window behavior
	CDockingManager::SetDockingMode(DT_SMART);
	// enable Visual Studio 2005 style docking window auto-hide behavior
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// Create a caption bar:
	//if (!CreateCaptionBar())
	//{
	//	TRACE0("Failed to create caption bar\n");
	//	return -1;      // fail to create
	//}

	// Load menu item image (not placed on any standard toolbars):
	CMFCToolBar::AddToolBarForImageCollection(IDR_MENU_IMAGES, theApp.m_bHiColorIcons ? IDB_MENU_IMAGES_24 : 0);

	// create docking windows
	if (!CreateDockingWindows())
	{
		TRACE0("Failed to create docking windows\n");
		return -1;
	}

  /// Left panels
	m_wndProperties.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndProperties);
	
	//m_wndAssetViewer.EnableDocking(CBRS_ALIGN_ANY);
	//DockPane(&m_wndAssetViewer);
	//m_wndAssetViewer.DockToWindow(&m_wndFileView, CBRS_ALIGN_BOTTOM);

	m_wndClassView.EnableDocking(CBRS_ALIGN_ANY);
  DockPane(&m_wndClassView);
  m_wndClassView.DockToWindow(&m_wndProperties, CBRS_ALIGN_TOP);

	m_wndFileView.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndFileView);
  m_wndFileView.DockToWindow(&m_wndClassView, CBRS_ALIGN_BOTTOM);

  


  /// Bottom panels
	m_wndOutput.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndOutput);
	m_wndOutput.DockToFrameWindow(CBRS_ALIGN_BOTTOM);
  m_wndOutput.SetAutoHideMode(true, CBRS_ALIGN_BOTTOM);


	// Enable enhanced windows management dialog
	//EnableWindowsDialog(ID_WINDOW_MANAGER, ID_WINDOW_MANAGER, TRUE);

	// Switch the order of document name and application name on the window title bar. This
	// improves the usability of the taskbar because the document name is visible with the thumbnail.
	ModifyStyle(0, FWS_PREFIXTITLE);

	//IRenderEngine::getInstance()->init();
	SetTimer(WM_USER + 200, 10, NULL);

  CUIManager::getInstance()->init(&m_wndRibbonBar, &m_wndFileView, &m_wndClassView, m_wndProperties.getPropGrid());

	m_bInit = true;

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

BOOL CMainFrame::CreateDockingWindows()
{
	BOOL bNameValid;

	// Create asset viewer
	CString strAssetViewerWnd;
	bNameValid = strAssetViewerWnd.LoadString(IDS_ASSETVIEWER_WND);
	ASSERT(bNameValid);

	//if (!m_wndAssetViewer.Create(strAssetViewerWnd, this, CRect(0, 0, 200, 100), TRUE, 308/*ID_VIEW_ASSETVIEWERWND*/, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	//{
	//	TRACE0("Failed to create Asset Viewer window\n");
	//	return FALSE; // failed to create
	//}

	// Create output window
	CString strOutputWnd;
	bNameValid = strOutputWnd.LoadString(IDS_OUTPUT_WND);
	ASSERT(bNameValid);
	if (!m_wndOutput.Create(strOutputWnd, this, CRect(0, 0, 100, 100), TRUE, ID_VIEW_OUTPUTWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create Output window\n");
		return FALSE; // failed to create
	}

	// Create properties window
	CString strPropertiesWnd;
	bNameValid = strPropertiesWnd.LoadString(IDS_PROPERTIES_WND);
	ASSERT(bNameValid);
	if (!m_wndProperties.Create(strPropertiesWnd, this, CRect(0, 0, 250, 250), TRUE, ID_VIEW_PROPERTIESWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create Properties window\n");
		return FALSE; // failed to create
	}

	// Create class view
	CString strClassView;
	bNameValid = strClassView.LoadString(IDS_CLASS_VIEW);
	ASSERT(bNameValid);
	if (!m_wndClassView.Create(strClassView, this, CRect(0, 0, 250, 150), TRUE, ID_VIEW_CLASSVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create Class View window\n");
		return FALSE; // failed to create
	}

	// Create file view
	CString strFileView;
	bNameValid = strFileView.LoadString(IDS_FILE_VIEW);
	ASSERT(bNameValid);
	if (!m_wndFileView.Create(strFileView, this, CRect(0, 0, 250, 200), TRUE, ID_VIEW_FILEVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create File View window\n");
		return FALSE; // failed to create
	}

	SetDockingWindowIcons(theApp.m_bHiColorIcons);
	return TRUE;
}

void CMainFrame::SetDockingWindowIcons(BOOL bHiColorIcons)
{
	HICON hFileViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_FILE_VIEW_HC : IDI_FILE_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndFileView.SetIcon(hFileViewIcon, FALSE);

	HICON hClassViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_CLASS_VIEW_HC : IDI_CLASS_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndClassView.SetIcon(hClassViewIcon, FALSE);

	HICON hOutputBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_OUTPUT_WND_HC : IDI_OUTPUT_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndOutput.SetIcon(hOutputBarIcon, FALSE);

	HICON hPropertiesBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_PROPERTIES_WND_HC : IDI_PROPERTIES_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndProperties.SetIcon(hPropertiesBarIcon, FALSE);

	UpdateMDITabbedBarsIcons();
}

BOOL CMainFrame::CreateCaptionBar()
{
	if (!m_wndCaptionBar.Create(WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, this, ID_VIEW_CAPTION_BAR, -1, TRUE))
	{
		TRACE0("Failed to create caption bar\n");
		return FALSE;
	}

	BOOL bNameValid;

	CString strTemp, strTemp2;
	bNameValid = strTemp.LoadString(IDS_CAPTION_BUTTON);
	ASSERT(bNameValid);
	m_wndCaptionBar.SetButton(strTemp, ID_TOOLS_OPTIONS, CMFCCaptionBar::ALIGN_LEFT, FALSE);
	bNameValid = strTemp.LoadString(IDS_CAPTION_BUTTON_TIP);
	ASSERT(bNameValid);
	m_wndCaptionBar.SetButtonToolTip(strTemp);

	bNameValid = strTemp.LoadString(IDS_CAPTION_TEXT);
	ASSERT(bNameValid);
	m_wndCaptionBar.SetText(strTemp, CMFCCaptionBar::ALIGN_LEFT);

	m_wndCaptionBar.SetBitmap(IDB_INFO, RGB(255, 255, 255), FALSE, CMFCCaptionBar::ALIGN_LEFT);
	bNameValid = strTemp.LoadString(IDS_CAPTION_IMAGE_TIP);
	ASSERT(bNameValid);
	bNameValid = strTemp2.LoadString(IDS_CAPTION_IMAGE_TEXT);
	ASSERT(bNameValid);
	m_wndCaptionBar.SetImageToolTip(strTemp, strTemp2);

	return TRUE;
}

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame message handlers

//void CMainFrame::OnWindowManager()
//{
//	ShowWindowsDialog();
//}

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_VS_2008:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_WINDOWS_7:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(TRUE);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
	}

	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}

void CMainFrame::OnViewCaptionBar()
{
	m_wndCaptionBar.ShowWindow(m_wndCaptionBar.IsVisible() ? SW_HIDE : SW_SHOW);
	RecalcLayout(FALSE);
}

void CMainFrame::OnUpdateViewCaptionBar(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndCaptionBar.IsVisible());
}

void CMainFrame::OnOptions()
{
	CMFCRibbonCustomizeDialog *pOptionsDlg = new CMFCRibbonCustomizeDialog(this, &m_wndRibbonBar);
	ASSERT(pOptionsDlg != NULL);

	pOptionsDlg->DoModal();
	delete pOptionsDlg;
}

void CMainFrame::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CMDIFrameWndEx::OnSettingChange(uFlags, lpszSection);
	m_wndOutput.UpdateFonts();
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL
CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
  //if (CUIManager::getInstance()->OnCmdMsg(nID))
    //return true;
  if (nCode != -1)
    CUIManager::getInstance()->OnCmdMsg(nID);

	return CMDIFrameWndEx::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

CMainFrame*
CMainFrame::getInstance()
{
	return m_pInst;
}

void
CMainFrame::OnToolsUIUpdate(CCmdUI* pCmdUI)
{
  CUIManager::getInstance()->OnCmdUIMsg(pCmdUI);
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//void CMainFrame::OnToolsRotate()
//{
//	GetToolMgr()->selectTool("RotateTool");
//	
//	activateTool(ID_TOOLS_ROTATE);
//}
//
//void CMainFrame::OnToolsScale()
//{
//	GetToolMgr()->selectTool("ScaleTool");
//	
//	activateTool(ID_TOOLS_SCALE);
//}
//
//void CMainFrame::OnToolsMove()
//{
//	GetToolMgr()->selectTool("MoveTool");
//
//	activateTool(ID_TOOLS_MOVE);
//}
//
//void CMainFrame::OnToolsNewterrainNewgroup()
//{
//	GetToolMgr()->selectTool("TerrainCreateTool", (void*)-1);
//	
//	activateTool(ID_TOOLS_NEWTERRAIN);
//}
//
//void
//CMainFrame::addTerrainToGroup(UINT nID)
//{
//	GetToolMgr()->selectTool("TerrainCreateTool", (void*)((UINT)(nID - 53000)));
//
//	activateTool(ID_TOOLS_NEWTERRAIN);
//}
//
//void CMainFrame::OnToolsTerrainpaint()
//{
//	GetToolMgr()->selectTool("TerrainPaintTool");
//
//	activateTool(ID_TOOLS_TERRAINPAINT);
//}
//
//void CMainFrame::OnToolsTerrainProgressiveHeight()
//{
//	GetToolMgr()->selectTool("TerrainProgressiveHeightTool");
//
//	activateTool(ID_TOOLS_TERRAIN_PROGRESSIVE_HEIGHT);
//}
//
//void CMainFrame::OnToolsTerrainheight()
//{
//	GetToolMgr()->selectTool("TerrainHeightTool");
//
//	activateTool(ID_TOOLS_TERRAIN_HEIGHT);
//}
//
//void CMainFrame::OnCameraGridExpand()
//{
//	const std::vector<IViewport*> vecViewports = GetRenderEngine()->getViewports();
//
//  // TODO: Resize grid
//	//for (std::vector<IViewport*>::const_iterator it = vecViewports.begin();
//	//		 it != vecViewports.end();
//	//		 it++)
//	//	(*it)->getViewportGrid()->ResizeGrid(100, 100);
//}
//
//void CMainFrame::OnCameraGridReduce()
//{
//	const std::vector<IViewport*> vecViewports = GetRenderEngine()->getViewports();
//
//  // TODO: Resize grid
//	//for (std::vector<IViewport*>::const_iterator it = vecViewports.begin();
//	//		 it != vecViewports.end();
//	//		 it++)
//	//	(*it)->getViewportGrid()->ResizeGrid(-100, -100);
//}
//
//void CMainFrame::OnToolsNewterrain()
//{
//}
//
//void CMainFrame::OnToolsNewVoxelTerrain()
//{
//	GetToolMgr()->selectTool("VoxelTerrainCreateTool");
//
//	activateTool(ID_TOOLS_NEW_VOXELTERRAIN);
//}
//
//void CMainFrame::OnToolsEditVoxelTerrain()
//{
//	GetToolMgr()->selectTool("VoxelTerrainEditTool");
//
//	activateTool(ID_TOOLS_EDIT_VOXELTERRAIN);
//}
//
//void CMainFrame::OnToolsTerrainBrushes()
//{
//	int iSelected = m_pTerrainHeightBrushGallery->GetSelectedItem();
//
//	CImage brushImage;
//	CImage* pBrushImage = &brushImage;
//	std::string sBrushName;
//
//	CBrushManager::getInstance()->getBrushDatas(iSelected, sBrushName, pBrushImage);
//	//CTerrainManager::getInstance()->setDecalTexture(sBrushName);
//
//	//brushChangeEvent(iSelected, sBrushName, brushImage);
//  // TODO: Change to an event system in BrushManager (in the same way TexManager use)
//
//	//pTerrainHeightTool->setDatas((void*)iSelected);
//}
//
//void CMainFrame::OnToolsTerrainTextures()
//{
//	int iSelected = m_pTerrainHeightTextureGallery->GetSelectedItem();
//
//  CTextureManager::getInstance()->selectTexture(iSelected);
//}
//
//
//void CMainFrame::OnToolsTerrainHeightSlider()
//{
//  int iStrength = m_pTerrainHeightSlider->GetPos();
//  
//  eventTerrainHeightEditStrength(iStrength);
//}
//
//
//void CMainFrame::OnToolsTerrainPaintSlider()
//{
//  int iStrength = m_pTerrainPaintSlider->GetPos();
//
//  eventTerrainPaintEditStrength(iStrength);
//}


afx_msg LRESULT CMainFrame::OnOutputAdd(WPARAM wParam, LPARAM lParam)
{
	char* pText = (char*)lParam;

	m_wndOutput.AppendOutputWindow(pText);
  delete pText;

	return 0;
}

void
CMainFrame::activateMainDoc()
{
  

}