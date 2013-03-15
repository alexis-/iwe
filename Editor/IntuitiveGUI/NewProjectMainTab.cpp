// NewProjectMainTab.cpp : implementation file
//

#include "stdafx.h"
#include "IntuitiveEditor.h"
#include "NewProjectMainTab.h"
#include "afxdialogex.h"
#include "resource.h"

// CNewProjectMainTab dialog

IMPLEMENT_DYNAMIC(CNewProjectMainTab, CMFCPropertyPage)

CNewProjectMainTab::CNewProjectMainTab()
	: CMFCPropertyPage(CNewProjectMainTab::IDD)
{

}

CNewProjectMainTab::~CNewProjectMainTab()
{
}

void CNewProjectMainTab::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MAIN_TAB_BASEDIR, m_baseDirBrowse);
	DDX_Control(pDX, IDC_MAIN_TAB_PROJECT_NAME, m_projectNameEdit);
	DDX_Control(pDX, IDC_MAIN_TAB_ERROR_TEXT, m_errorEdit);
}


BEGIN_MESSAGE_MAP(CNewProjectMainTab, CMFCPropertyPage)
END_MESSAGE_MAP()


// CNewProjectMainTab message handlers


BOOL CNewProjectMainTab::OnInitDialog()
{
	CMFCPropertyPage::OnInitDialog();

	m_baseDirBrowse.EnableFolderBrowseButton();
	m_errorEdit.SetTextColor(RGB(175, 0, 0));
	m_errorEdit.SetBkColor(RGB(255, 255, 255));

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
