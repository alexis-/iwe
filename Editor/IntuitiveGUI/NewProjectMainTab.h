#pragma once

#include "ColorStatic.h"
// CNewProjectMainTab dialog

class CNewProjectMainTab : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(CNewProjectMainTab)

public:
	CNewProjectMainTab();
	virtual ~CNewProjectMainTab();

// Dialog Data
	enum { IDD = IDD_NEW_PROJECT_MAIN_TAB };

public:
	CMFCEditBrowseCtrl			m_baseDirBrowse;
	CEdit										m_projectNameEdit;
	CColorStatic						m_errorEdit;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
