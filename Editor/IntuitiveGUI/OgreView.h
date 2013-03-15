#ifndef OGRE_VIEW_H
# define OGRE_VIEW_H

# include <afxwin.h>

class CViewport;

class COgreView : public CWnd
{
	DECLARE_DYNAMIC(COgreView)

// Constructors
public:
	COgreView();
	virtual ~COgreView() {};
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
	virtual void OnSize(UINT nType, int cx, int cy);

	bool createViewport(int width, int height);
	
	DECLARE_MESSAGE_MAP()

public:
	IViewport*					getViewport()
	{
		return m_pViewport;
	}

private:
	IViewport*					m_pViewport;
};

#endif // !OGRE_VIEW_H