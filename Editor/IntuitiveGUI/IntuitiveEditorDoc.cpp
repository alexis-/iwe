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

// IntuitiveEditorDoc.cpp : implementation of the CIntuitiveEditorDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "IntuitiveEditor.h"
#endif

#include "IntuitiveEditorDoc.h"
//#include "ProjectManager.h"
//#include "SaveSystemManager.h"

#include <propkey.h>
#include <sstream>
// CIntuitiveEditorDoc

IMPLEMENT_DYNCREATE(CIntuitiveEditorDoc, CDocument)

BEGIN_MESSAGE_MAP(CIntuitiveEditorDoc, CDocument)
	ON_COMMAND(ID_FILE_SAVE, &CIntuitiveEditorDoc::OnFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS, &CIntuitiveEditorDoc::OnFileSaveAs)
END_MESSAGE_MAP()


// CIntuitiveEditorDoc construction/destruction

CIntuitiveEditorDoc::CIntuitiveEditorDoc()
{
	// TODO: add one-time construction code here

}

CIntuitiveEditorDoc::~CIntuitiveEditorDoc()
{
}

BOOL CIntuitiveEditorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

  CDocument::SetTitle("Viewport");
	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CIntuitiveEditorDoc serialization

void CIntuitiveEditorDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CIntuitiveEditorDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CIntuitiveEditorDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CIntuitiveEditorDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = NEW CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CIntuitiveEditorDoc diagnostics

#ifdef _DEBUG
void CIntuitiveEditorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CIntuitiveEditorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CIntuitiveEditorDoc commands


void CIntuitiveEditorDoc::OnFileSave()
{
  if (GetProjectMgr()->isProjectLoaded() == false)
		return;

	if (GetProjectMgr()->getProjectFile() != "")
		GetProjectMgr()->saveProject();
	else
		OnFileSaveAs();
}


void CIntuitiveEditorDoc::OnFileSaveAs()
{
	if (GetProjectMgr()->isProjectLoaded() == false)
		return;

	std::vector<std::string> vecFilterExtensions;
	std::stringstream sFilters;
	std::string projectDir = GetProjectMgr()->getProjectDir();
	std::string projectName = GetProjectMgr()->getProjectName();

  // TODO: Get save systems
	//for (std::map<std::string, CSaveSystem*>::iterator it = CSaveSystemManager::getInstance()->begin();
			 //it != CSaveSystemManager::getInstance()->end();
			 //it++)
	//{
		//sFilters << (*it).second->getName().c_str() << " (*." << (*it).second->getExt().c_str() << ")|*." << (*it).second->getExt().c_str() << "|";
		//vecFilterExtensions.push_back((*it).second->getExt());
	//}

	CFileDialog FileDlg(FALSE, NULL, projectName.c_str(),
											OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, sFilters.str().c_str());
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
			sProjectFilePath += '.' + sExt;
		}

		GetProjectMgr()->saveProject(sProjectFile, sProjectFilePath, sExt);
	}

	else
		return;
}


BOOL CIntuitiveEditorDoc::CanCloseFrame(CFrameWnd* pFrame)
{
  // TODO: Add your specialized code here and/or call the base class

  return false; //CDocument::CanCloseFrame(pFrame);
}
