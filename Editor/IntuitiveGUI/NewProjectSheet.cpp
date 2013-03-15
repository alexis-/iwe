// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#include "stdafx.h"
#include "resource.h"
#include "NewProjectSheet.h"

//#include "ProjectManager.h"

/////////////////////////////////////////////////////////////////////////////
// CNewProjectSheet

IMPLEMENT_DYNAMIC(CNewProjectSheet, CMFCPropertySheet)

CNewProjectSheet::CNewProjectSheet(CWnd* pParentWnd)
:CMFCPropertySheet(_T("New Project ...	"), pParentWnd)
{
	m_psh.dwFlags &= ~PSH_HASHELP;
	m_psh.dwFlags |= PSH_NOAPPLYNOW;

	//BOOL b32BitIcons = TRUE;

	//if (afxGlobalData.m_nBitsPerPixel < 16)
	//{
	//	b32BitIcons = FALSE;
	//}

	SetLook(CMFCPropertySheet::PropSheetLook_OutlookBar);
	SetIconsList(IDB_NEW_PROJECT_ICONS, 32); //b32BitIcons ? IDB_ICONS32 : IDB_ICONS, 32);

	//CMFCPropertyPage page1(IDD_NEW_PROJECT_MAIN_TAB);
	AddPage(&m_wndMainTab);
	AddPage(&m_wndAssetsTab);

	m_hIcon = AfxGetApp()->LoadIcon(IDB_MAIN);
}

CNewProjectSheet::~CNewProjectSheet()
{
}

BEGIN_MESSAGE_MAP(CNewProjectSheet, CMFCPropertySheet)
	ON_WM_QUERYDRAGICON()
	ON_WM_SYSCOMMAND()
	ON_COMMAND(IDOK, &CNewProjectSheet::OnIdok)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewProjectSheet message handlers

BOOL CNewProjectSheet::OnInitDialog()
{
	BOOL bResult = CMFCPropertySheet::OnInitDialog();


	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE); // Set big icon
	//SetIcon(m_hIcon, FALSE); // Set small icon

	return bResult;
}

void CNewProjectSheet::OnSysCommand(UINT nID, LPARAM lParam)
{
	CMFCPropertySheet::OnSysCommand(nID, lParam);
}


HCURSOR CNewProjectSheet::OnQueryDragIcon()
{
	return(HCURSOR) m_hIcon;
}

void CNewProjectSheet::OnIdok()
{
	CString tmp;

	std::string sProjectName;
	std::string sBaseDir;

	// Project name
	m_wndMainTab.m_projectNameEdit.GetWindowText(tmp);

	{
		CT2CA converter(tmp.GetString());
		sProjectName = converter;
	}

	// Project directory
	m_wndMainTab.m_baseDirBrowse.GetWindowText(tmp);
	
	{
		CT2CA converter(tmp.GetString());
		sBaseDir = converter;
	}


	// Check validity of project settings
	if (sProjectName == "") // || !valid(sBaseDir))
	{
		m_wndMainTab.m_errorEdit.ShowWindow(SW_SHOW);
		m_wndMainTab.m_errorEdit.SetWindowText(_T("Project name cannot be empty !"));

		SetActivePage(0);

		return;
	}

	if (!checkPathExists(tmp.GetString()))
	{
		try
		{
			CreateDirectory(tmp.GetString(), NULL);
		}
		catch (...)
		{
		}
	}

	if (!isDirectory(tmp.GetString()))
	{
		m_wndMainTab.m_errorEdit.ShowWindow(SW_SHOW);
		m_wndMainTab.m_errorEdit.SetWindowText("Invalid project directory !");
		
		SetActivePage(0);

		return;
	}


	// Create a NEW project
	GetProjectMgr()->newProject(sProjectName, sBaseDir);

	// Default button behaviour (close the dialog)
	Default();
}
