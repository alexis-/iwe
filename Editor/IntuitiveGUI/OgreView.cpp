#include "stdafx.h"

#include "OgreView.h"

//#include "Viewport.h"
//#include "IRenderEngine.h"

IMPLEMENT_DYNAMIC(COgreView, CWnd)
	
BEGIN_MESSAGE_MAP(COgreView, CWnd)
	ON_WM_SIZE()
END_MESSAGE_MAP()

COgreView::COgreView()
	: m_pViewport(NULL)
{
}

BOOL
COgreView::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	//CWnd* pWnd = this;
	//bool bOk = pWnd->Create(_T(""), NULL, dwStyle, rect, pParentWnd, nID);
	bool bOk = CWnd::Create(_T("STATIC"), NULL, dwStyle, rect, pParentWnd, nID);

	return bOk;
}

void
COgreView::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	m_pViewport->resize(cx, cy);
}

bool
COgreView::createViewport(int width, int height)
{
	//RECT rc;
	//GetClientRect(&rc);
	//rc.bottom = rc.left = rc.right = rc.top = 1;

  CIWEPoint<ushort> size(width, height);
	
	m_pViewport = GetRenderEngine()->createViewport((size_t)GetSafeHwnd(), size, 2);

	//m_pViewport->getCamera()->setPosition(Ogre::Vector3(-15.0f, 12.0f, 15.0f));
	//m_pViewport->getCamera()->lookAt(Ogre::Vector3(0.0f, 0.0f, 0.0f));

	if (!m_pViewport)
		return false;

	m_pViewport->showWindow(true);

	return true;
}