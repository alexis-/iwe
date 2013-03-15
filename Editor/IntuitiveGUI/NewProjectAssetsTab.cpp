// NewProjectAssetsTab.cpp : implementation file
//

#include "stdafx.h"
#include "IntuitiveEditor.h"
#include "NewProjectAssetsTab.h"
#include "afxdialogex.h"


// CNewProjectAssetsTab dialog

IMPLEMENT_DYNAMIC(CNewProjectAssetsTab, CMFCPropertyPage)

CNewProjectAssetsTab::CNewProjectAssetsTab()
	: CMFCPropertyPage(CNewProjectAssetsTab::IDD)
{

}

CNewProjectAssetsTab::~CNewProjectAssetsTab()
{
}

void CNewProjectAssetsTab::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ASSETS_TAB, m_tabLoc);
}


BEGIN_MESSAGE_MAP(CNewProjectAssetsTab, CMFCPropertyPage)
END_MESSAGE_MAP()


// CNewProjectAssetsTab message handlers


BOOL CNewProjectAssetsTab::OnInitDialog()
{
	CMFCPropertyPage::OnInitDialog();

	RECT tabPos;
	m_tabLoc.GetWindowRect(&tabPos);
	ScreenToClient(&tabPos);

	m_tabCtrl.Create(CMFCTabCtrl::STYLE_3D_ONENOTE, tabPos, this, 83240, CMFCBaseTabCtrl::LOCATION_TOP, false);

	m_listAsset.Create(WS_CHILD | WS_VISIBLE, CRect (0, 0, 0, 0), &m_tabCtrl, 83241);

	m_tabCtrl.AddTab(&m_listAsset, _T("Project Assets"));
	m_tabCtrl.EnableAutoColor(TRUE);


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
