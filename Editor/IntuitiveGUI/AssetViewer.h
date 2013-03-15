#ifndef ASSET_VIEWER_H
# define ASSET_VIEWER_H

# include "OgreView.h"


class CAssetViewer : public CDockablePane
{
// Construction
public:
	CAssetViewer();

// Attributes
protected:
	COgreView						m_ogreView;
	//IEntity*				    m_pDisplayedEnt;

// Implementation
public:
	virtual ~CAssetViewer();
	
	void OnAssetChanged(const std::string& assetName);

protected:
	afx_msg int					OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void				OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()
};

void OnAssetChanged(void* pThis, const std::string& assetName);

#endif // !ASSET_VIEWER_H