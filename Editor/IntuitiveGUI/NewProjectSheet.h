// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#pragma once

#include "NewProjectMainTab.h"
#include "NewProjectAssetsTab.h"

/////////////////////////////////////////////////////////////////////////////
// CNewProjectSheet

class CNewProjectSheet : public CMFCPropertySheet
{
	DECLARE_DYNAMIC(CNewProjectSheet)

// Construction
public:
	CNewProjectSheet(CWnd* pParentWnd = NULL);

// Attributes
private:
	CNewProjectMainTab		m_wndMainTab;
	CNewProjectAssetsTab	m_wndAssetsTab;
	HICON m_hIcon;

// Overrides
public:
	virtual BOOL OnInitDialog();

// Implementation
public:
	virtual ~CNewProjectSheet();

protected:
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg HCURSOR OnQueryDragIcon();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnIdok();
};

