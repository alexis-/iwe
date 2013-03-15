
#pragma once


class CCLIChildFrame : public CMDIChildWndEx
{
	DECLARE_DYNCREATE(CCLIChildFrame)
public:
	CCLIChildFrame();


// Overrides
public:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
  virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
                      DWORD dwStyle, const RECT& rect,
                      CMDIFrameWnd* pParentWnd, CCreateContext* pContext);
	
	afx_msg virtual int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg virtual void OnSize(UINT nType, int cx, int cy);

// Implementation
public:
	virtual ~CCLIChildFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
};
