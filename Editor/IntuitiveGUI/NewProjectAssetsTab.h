#pragma once


// CNewProjectAssetsTab dialog

class CNewProjectAssetsTab : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(CNewProjectAssetsTab)

public:
	CNewProjectAssetsTab();
	virtual ~CNewProjectAssetsTab();

// Dialog Data
	enum { IDD = IDD_NEW_PROJECT_ASSETS_TAB };

private:
	CStatic								m_tabLoc;
	CMFCTabCtrl						m_tabCtrl;
	CMFCListCtrl					m_listAsset;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
