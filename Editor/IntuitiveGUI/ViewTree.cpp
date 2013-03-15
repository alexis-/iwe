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
#include "ViewTree.h"


/////////////////////////////////////////////////////////////////////////////
// CViewTree

CViewTree::CViewTree()
{
}

CViewTree::~CViewTree()
{
}

BEGIN_MESSAGE_MAP(CViewTree, CTreeCtrl)
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, &CViewTree::OnTvnBegindrag)
	ON_WM_LBUTTONDOWN()
  ON_NOTIFY_REFLECT(TVN_DELETEITEM, &CViewTree::OnTvnDeleteitem)
  ON_NOTIFY_REFLECT(TVN_SELCHANGED, &CViewTree::OnTvnSelchanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewTree message handlers

BOOL CViewTree::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	BOOL bRes = CTreeCtrl::OnNotify(wParam, lParam, pResult);

	NMHDR* pNMHDR = (NMHDR*)lParam;
	ASSERT(pNMHDR != NULL);

	if (pNMHDR && pNMHDR->code == TTN_SHOW && GetToolTips() != NULL)
	{
		GetToolTips()->SetWindowPos(&wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOSIZE);
	}

	return bRes;
}


void CViewTree::OnTvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	*pResult = 0;

	//COleDropSource pDropSource;
  COleDataSource pDataSource;

	HTREEITEM hTreeItem = GetSelectedItem();

	if (!hTreeItem)
		return; // Should not happen

	CString itemText = GetItemText(hTreeItem);
	int len = 0;

	for (; itemText[len]; len++)
		;

	HGLOBAL hData = GlobalAlloc(GMEM_MOVEABLE, len + 1);
  char* pChar = (char*)GlobalLock(hData);

  if (pChar)
  {
	  for (int i = 0; i < len; i++)
	  	pChar[i] = itemText[i];

	  pChar[len] = 0;
  }

  GlobalUnlock(hData);

	pDataSource.CacheGlobalData(CF_TEXT, hData);
  pDataSource.DoDragDrop(DROPEFFECT_MOVE | DROPEFFECT_COPY);
}


void CViewTree::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (point != CPoint(-1, -1))
	{
		UINT flags = 0;
		HTREEITEM hTreeItem = HitTest(point, &flags);

		if (hTreeItem != NULL)
		{
			SelectItem(hTreeItem);
			
			std::string itemText = GetItemText(hTreeItem);


		}
	}

	CTreeCtrl::OnLButtonDown(nFlags, point);
}

void CViewTree::OnTvnDeleteitem(NMHDR *pNMHDR, LRESULT *pResult)
{
  LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

  eventItemDeleted(GetSelectedItem());
  // TODO: Add your control notification handler code here
  *pResult = 0;
}


void CViewTree::OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
  LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
  
  eventItemSelected(GetSelectedItem());
  // TODO: Add your control notification handler code here
  *pResult = 0;
}
