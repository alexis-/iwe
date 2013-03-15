#include "stdafx.h"

#include "IntuitiveEditor.h"
#include "CLIChildFrm.h"


// CCLIChildFrame

IMPLEMENT_DYNCREATE(CCLIChildFrame, CMDIChildWndEx)

BEGIN_MESSAGE_MAP(CCLIChildFrame, CMDIChildWndEx)
	ON_WM_CREATE()
	ON_WM_SIZE()
  ON_WM_SYSCOMMAND()
END_MESSAGE_MAP()

// CCLIChildFrame construction/destruction

CCLIChildFrame::CCLIChildFrame()
{
	// TODO: add member initialization code here
}

CCLIChildFrame::~CCLIChildFrame()
{
}

BOOL CCLIChildFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/, CCreateContext* pContext)
{
	return (CreateView(pContext, AFX_IDW_PANE_FIRST) != NULL);
}

BOOL CCLIChildFrame::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
                         DWORD dwStyle, const RECT& rect,
                         CMDIFrameWnd* pParentWnd, CCreateContext* pContext)
{
  if (CMDIChildWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, pContext))
  {
    CMenu* p = GetSystemMenu(FALSE);

    p->RemoveMenu(SC_CLOSE, MF_BYCOMMAND);

    return TRUE;
  }

  return FALSE;
}

int
CCLIChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	return CMDIChildWndEx::OnCreate(lpCreateStruct);
}

void CCLIChildFrame::OnSize(UINT nType, int cx, int cy)
{
	CMDIChildWndEx::OnSize(nType, cx, cy);
}


BOOL CCLIChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying the CREATESTRUCT cs
	if(!CMDIChildWndEx::PreCreateWindow(cs) )
		return FALSE;

	cs.style &= ~(LONG)FWS_ADDTOTITLE & ~(LONG)WS_MINIMIZEBOX & ~(LONG)WS_MAXIMIZEBOX;

	return TRUE;
}

// CCLIChildFrame diagnostics

#ifdef _DEBUG
void CCLIChildFrame::AssertValid() const
{
	CMDIChildWndEx::AssertValid();
}

void CCLIChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWndEx::Dump(dc);
}
#endif //_DEBUG

// CCLIChildFrame message handlers


void CCLIChildFrame::OnSysCommand(UINT nID, LPARAM lParam)
{
  if(nID == SC_CLOSE)
    return;

  CMDIChildWndEx::OnSysCommand(nID, lParam);
}
