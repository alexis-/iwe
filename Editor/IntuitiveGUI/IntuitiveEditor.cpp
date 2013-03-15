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

// IntuitiveEditor.cpp : Defines the class behaviors for the application.
//


#ifdef _MANAGED
using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;

#endif

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "IntuitiveEditor.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "IntuitiveEditorDoc.h"
#include "IntuitiveEditorView.h"

#include "CLIChildFrm.h"
#include "CLIDoc.h"
#include "CLIView.h"

#include "NewProjectSheet.h"

#include "Interface.h"
#include "UIManager.h"

#include <sstream>

// CIntuitiveEditorApp

BEGIN_MESSAGE_MAP(CIntuitiveEditorApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CIntuitiveEditorApp::OnAppAbout)
	// Standard file based document commands

	ON_COMMAND(ID_FILE_OPEN, &CIntuitiveEditorApp::OnFileOpen)
	ON_COMMAND(ID_FILE_NEW, &CIntuitiveEditorApp::OnFileNew)
  ON_COMMAND(ID_EDIT_UNDO, &CIntuitiveEditorApp::OnEditUndo)
  ON_COMMAND(ID_EDIT_COPY, &CIntuitiveEditorApp::OnEditCopy)
  ON_COMMAND(ID_EDIT_CUT, &CIntuitiveEditorApp::OnEditCut)
  ON_COMMAND(ID_EDIT_PASTE, &CIntuitiveEditorApp::OnEditPaste)
  ON_COMMAND(ID_EDIT_REDO, &CIntuitiveEditorApp::OnEditRedo)
  ON_COMMAND(ID_EDIT_FIND, &CIntuitiveEditorApp::OnEditFind)
  ON_COMMAND(ID_EDIT_SELECT_ALL, &CIntuitiveEditorApp::OnEditSelectAll)
END_MESSAGE_MAP()


// CIntuitiveEditorApp construction

CIntuitiveEditorApp::CIntuitiveEditorApp()
{
	EnableHtmlHelp();

	m_bHiColorIcons = TRUE;

	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// If the application is built using Common Language Runtime support (/clr):
	//     1) This additional setting is needed for Restart Manager support to work properly.
	//     2) In your project, you must add a reference to System.Windows.Forms in order to build.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: replace application ID string below with unique ID string; recommended
	// format for string is CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("IntuitiveEditor.AppID.NoVersion"));

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance

	// Memory leak stuffs
	//_CrtSetDbgFlag (_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	//_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG );

}

// The one and only CIntuitiveEditorApp object


CIntuitiveEditorApp theApp;


// CIntuitiveEditorApp initialization

BOOL CIntuitiveEditorApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction();

	// AfxInitRichEdit2() is required to use RichEdit control	
	// AfxInitRichEdit2();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Intuitive Solutions"));
	LoadStdProfileSettings(8);  // Load standard INI file options (including MRU)


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views

  //////////
	CMultiDocTemplate* pDocTemplate;

	pDocTemplate = new CMultiDocTemplate(IDR_IntuitiveEditorTYPE,
		RUNTIME_CLASS(CIntuitiveEditorDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CIntuitiveEditorView));

	if (!pDocTemplate)
		return FALSE;

	AddDocTemplate(pDocTemplate);
  m_pMainDoc = pDocTemplate;

  //////////


	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}

	m_pMainWnd = pMainFrame;

	// call DragAcceptFiles only if there's a suffix
	//  In an MDI app, this should occur immediately after setting m_pMainWnd

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	//m_pDocManager

	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// The main window has been initialized, so show and update it
  

 



	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

  //////////

	pDocTemplate = new CMultiDocTemplate(IDR_IntuitiveEditorTYPE,
    RUNTIME_CLASS(CCLIDoc),
    RUNTIME_CLASS(CCLIChildFrame), // custom MDI child frame
    RUNTIME_CLASS(CCLIView));

	if (!pDocTemplate)
		return FALSE;

	AddDocTemplate(pDocTemplate);
  
  
  CUIManager::getInstance()->setCLIViewDoc(pDocTemplate);

  CGUIInterface::getInstance()->eventPostInit();
  
  POSITION docpos = m_pMainDoc->GetFirstDocPosition();
  CDocument* pMainDoc = m_pMainDoc->GetNextDoc(docpos);
  
  CView* pView;
  POSITION pos = pMainDoc->GetFirstViewPosition();
  if (pos != NULL)
  {
    pView = pMainDoc->GetNextView(pos);
    if (pView)
    {
      pView->GetParentFrame()->ActivateFrame();
    }
  }

	return TRUE;
}

int CIntuitiveEditorApp::ExitInstance()
{
	//TODO: handle additional resources you may have added
	AfxOleTerm(FALSE);

	return CWinAppEx::ExitInstance();
}

// CIntuitiveEditorApp message handlers


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnEditPaste();
//	afx_msg void OnButton4();
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
//	ON_COMMAND(ID_EDIT_PASTE, &CAboutDlg::OnEditPaste)
//	ON_COMMAND(ID_BUTTON4, &CAboutDlg::OnButton4)
END_MESSAGE_MAP()

// App command to run the dialog
void CIntuitiveEditorApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CIntuitiveEditorApp customization load/save methods

void CIntuitiveEditorApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
	bNameValid = strName.LoadString(IDS_EXPLORER);
	ASSERT(bNameValid);
	//GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EXPLORER);
	GetContextMenuManager()->AddMenu(strName, IDR_MATERIAL_MENU);
}

void CIntuitiveEditorApp::LoadCustomState()
{
}

void CIntuitiveEditorApp::SaveCustomState()
{
}

// CIntuitiveEditorApp message handlers




CDocument* CIntuitiveEditorApp::OpenDocumentFile(LPCTSTR lpszFileName)
{
	// TODO: Add your specialized code here and/or call the base class

	//return CWinAppEx::OpenDocumentFile(lpszFileName);
	return NULL;
}



void CIntuitiveEditorApp::OnFileNew()
{
	static bool bOpened = false;

	if (!bOpened)
	{
		CWinAppEx::OnFileNew();
		bOpened = true;
	}
	else
	{
		// TODO: Check if a project is open and unsaved
		CNewProjectSheet frame;

		frame.DoModal();
	}
}

void CIntuitiveEditorApp::OnFileOpen()
{
	std::vector<std::string> vecFilterExtensions;
	std::stringstream sFilters;
	std::string projectDir = GetProjectMgr()->getProjectDir();
	std::string projectName = GetProjectMgr()->getProjectName();

  // TODO : Get save systems
	//for (std::map<std::string, CSaveSystem*>::iterator it = CSaveSystemManager::getInstance()->begin();
			 //it != CSaveSystemManager::getInstance()->end();
			 //it++)
	//{
		//sFilters << (*it).second->getName().c_str() << " (*." << (*it).second->getExt().c_str() << ")|*." << (*it).second->getExt().c_str() << "|";
		//vecFilterExtensions.push_back((*it).second->getExt());
	//}

	CFileDialog FileDlg(FALSE, NULL, projectName.c_str(), OFN_HIDEREADONLY
											| OFN_FILEMUSTEXIST, sFilters.str().c_str());
	FileDlg.m_ofn.lpstrInitialDir = projectDir.c_str();

	if(FileDlg.DoModal() == IDOK )
	{
		std::string sProjectFile = "";
		std::string sProjectFilePath = "";
		std::string sExt = vecFilterExtensions[FileDlg.m_ofn.nFilterIndex - 1];

		CString str = FileDlg.GetFileTitle();

		{
			CT2CA converter(str.GetString());
			sProjectFile = converter;
			sProjectFile += '.' + sExt;
		}

		str = FileDlg.GetPathName();

		{
			CT2CA converter(str.GetString());
			sProjectFilePath = converter;
			//sProjectFilePath += '.' + sExt;
		}

		GetProjectMgr()->loadProject(sProjectFilePath, sExt);
	}

	else
		return;
}


void CIntuitiveEditorApp::OnEditUndo()
{
  GetEditorInteraction()->onUndo();
}


void CIntuitiveEditorApp::OnEditRedo()
{
  GetEditorInteraction()->onRedo();
}


void CIntuitiveEditorApp::OnEditCopy()
{
  GetEditorInteraction()->onCopy();
}


void CIntuitiveEditorApp::OnEditCut()
{
  GetEditorInteraction()->onCut();
}


void CIntuitiveEditorApp::OnEditPaste()
{
  GetEditorInteraction()->onPaste();
}


void CIntuitiveEditorApp::OnEditFind()
{
  GetEditorInteraction()->onFind();
}


void CIntuitiveEditorApp::OnEditSelectAll()
{
  GetEditorInteraction()->onSelectAll();
}
