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

#include "stdafx.h"
#include "mainfrm.h"
#include "FileView.h"
#include "Resource.h"
#include "IntuitiveEditor.h"


/////////////////////////////////////////////////////////////////////////////
// CFileView

CFileView::CFileView()
	: m_bIsInit(false)
{
}

CFileView::~CFileView()
{
}

BEGIN_MESSAGE_MAP(CFileView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
#if 0
	ON_COMMAND(ID_MATERIAL_EDIT, &CFileView::OnMaterialEdit)
	ON_COMMAND(ID_MATERIAL_RELOAD, &CFileView::OnMaterialReload)
	ON_COMMAND(ID_MATERIAL_RELOADUPDATE, &CFileView::OnMaterialReloadupdate)
	ON_COMMAND(ID_MATERIAL_UPDATE, &CFileView::OnMaterialUpdate)
#endif // !0
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceBar message handlers

int CFileView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create view:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS;

	if (!m_wndFileView.Create(dwViewStyle, rectDummy, this, 4))
	{
		TRACE0("Failed to create file view\n");
		return -1;      // fail to create
	}

	// Load view images:
	m_FileViewImages.Create(IDB_FILE_VIEW, 16, 0, RGB(255, 0, 255));
	m_wndFileView.SetImageList(&m_FileViewImages, TVSIL_NORMAL);

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_EXPLORER);
	m_wndToolBar.LoadToolBar(IDR_EXPLORER, 0, 0, TRUE /* Is locked */);

	OnChangeVisualStyle();

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	m_wndToolBar.SetOwner(this);

	// All commands will be routed via this control , not via the parent frame:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	// Fill in some static tree view data (dummy code, nothing magic here)
	//FillFileView();
	AdjustLayout();


	//CResourceManager::getInstance()->setFileView(this);
  GetProjectMgr()->eventProjectLoaded += FastDelegate1<const std::string&>(this, &CFileView::FillFileView);
  GetProjectMgr()->eventProjectClosed += FastDelegate0<>(this, &CFileView::ClearFileView);

	return 0;
}

void CFileView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CFileView::FillFileView(const std::string& sProjectName)
{
	m_hRoot = m_wndFileView.InsertItem(sProjectName.c_str(), 3, 3);
	m_wndFileView.SetItemState(m_hRoot, TVIS_BOLD, TVIS_BOLD);

  eventTreeLoaded();
  
	m_wndFileView.Expand(m_hRoot, TVE_EXPAND);
	m_wndFileView.RedrawWindow();

	m_bIsInit = true;
}

void CFileView::ClearFileView()
{
  eventTreeUnloaded();

	m_wndFileView.DeleteAllItems();
	m_wndFileView.RedrawWindow();
}

void CFileView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CTreeCtrl* pWndTree = (CTreeCtrl*) &m_wndFileView;
	ASSERT_VALID(pWndTree);

	if (pWnd != pWndTree)
	{
		CDockablePane::OnContextMenu(pWnd, point);
		return;
	}

	if (point != CPoint(-1, -1))
	{
		// Select clicked item:
		CPoint ptTree = point;
		pWndTree->ScreenToClient(&ptTree);

		UINT flags = 0;
		HTREEITEM hTreeItem = pWndTree->HitTest(ptTree, &flags);
		if (hTreeItem != NULL)
		{
			pWndTree->SelectItem(hTreeItem);

      // TODO: move this to a dynamic menu
			//if (pWndTree->GetParentItem(hTreeItem) == m_hMaterialItems) // It's a material file
			//{
			//	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_MATERIAL_MENU, point.x, point.y, this, TRUE);
			//}
		}
	}

	pWndTree->SetFocus();
	//theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EXPLORER, point.x, point.y, this, TRUE);
}

void CFileView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndFileView.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}

void CFileView::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rectTree;
	m_wndFileView.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void CFileView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	m_wndFileView.SetFocus();
}

void CFileView::OnChangeVisualStyle()
{
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_EXPLORER_24 : IDR_EXPLORER, 0, 0, TRUE /* Locked */);

	m_FileViewImages.DeleteImageList();

	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_FILE_VIEW_24 : IDB_FILE_VIEW;

	CBitmap bmp;
	if (!bmp.LoadBitmap(uiBmpId))
	{
		TRACE(_T("Can't load bitmap: %x\n"), uiBmpId);
		ASSERT(FALSE);
		return;
	}

	BITMAP bmpObj;
	bmp.GetBitmap(&bmpObj);

	UINT nFlags = ILC_MASK;

	nFlags |= (theApp.m_bHiColorIcons) ? ILC_COLOR24 : ILC_COLOR4;

	m_FileViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	m_FileViewImages.Add(&bmp, RGB(255, 0, 255));

	m_wndFileView.SetImageList(&m_FileViewImages, TVSIL_NORMAL);
}

#if 0

static const char* getCurrentItemText(CViewTree* pTree)
{
	if (!pTree || !pTree->GetSelectedItem())
		return NULL;

	static std::string sText;

	sText = pTree->GetItemText(pTree->GetSelectedItem());
	//static char pText[256];

	//for (int i = 0; i < csText.GetLength(); i++)
	//	pText[i] = (char)csText[i];

	//pText[csText.GetLength()] = 0;

  return sText.c_str();
}

static const char* getCurrentItemChar(CViewTree* pTree)
{
	if (!pTree || !pTree->GetSelectedItem())
		return NULL;

	CString csText = pTree->GetItemText(pTree->GetSelectedItem());
	static char pText[256];

	for (int i = 0; i < csText.GetLength(); i++)
		pText[i] = (char)csText[i];

	pText[csText.GetLength()] = 0;

	return pText;
}

static const char* getPath(std::string dllName)
{
	//CString dllName = AfxGetApp()->m_pszExeName; // Get the "MyExe" portion of "MyExe.exe". Or, "MyDll" if RunDll32 is used.

	dllName += ".exe"; // Now has "MyExe.exe" (or "MyDll.dll").

  HMODULE hmod = GetModuleHandle(dllName.c_str());

  static char buff[MAX_PATH + 1];

	DWORD pathLen = ::GetModuleFileName(hmod, buff, MAX_PATH); // hmod of zero gets the main EXE

	int i = pathLen - 1;
	for (; i >= 0; i--)
		if (buff[i] == '\\')
			break;

	if (i > 0)
		buff[i + 1] = '\0';

	return buff;
}

void runYaose(HWND hwnd, const char* pArgs, const char* pPath)
{
	std::string appPath;
	std::string args;

	appPath += pPath;
	appPath += "..\\..\\Yaose\\yaose.exe";

	args += pPath;
	args += pArgs;

	//args.replace('/', '\\');

  for (int i = 0; i < args.length(); i++)
    if (args[i] == '/')
      args[i] = '\\';

  ShellExecute(hwnd, "open", appPath.c_str(), args.c_str(), NULL, SW_SHOW);
}

void reloadMaterial(const char* pMat)
{
	std::string sGroup;
	std::string sPath;

  // TODO: Actually make CMaterialManager track materials
	CMaterialManager::getInstance()->getMaterialDatas(pMat, sPath, sGroup);
  GetResourceMgr()->ReloadMaterial(pMat, sGroup, pMat, true);
}

void CFileView::OnMaterialEdit()
{
	std::string sText = getCurrentItemText(&m_wndFileView);
	const char* pMat = getCurrentItemChar(&m_wndFileView);
	HWND hwnd = GetSafeHwnd();
	
	std::string sGroup;
	std::string sPath;

	CMaterialManager::getInstance()->getMaterialDatas(pMat, sPath, sGroup);

	std::string matPath(sPath.c_str());
	matPath += "\\";
	matPath += sText;

	runYaose(hwnd, matPath.c_str(), getPath(AfxGetApp()->m_pszAppName));
}

void CFileView::OnMaterialReload()
{
	const char* pMat = getCurrentItemChar(&m_wndFileView);

	reloadMaterial(pMat);
}


void CFileView::OnMaterialReloadupdate()
{
	const char* pMat = getCurrentItemChar(&m_wndFileView);

	reloadMaterial(pMat);
	
	GetResourceMgr()->updateOnEveryRenderable();
}


void CFileView::OnMaterialUpdate()
{
	GetResourceMgr()->updateOnEveryRenderable();
}

#endif // !0