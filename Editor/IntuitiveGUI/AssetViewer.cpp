#include "stdafx.h"

//#include <OgreAxisAlignedBox.h>

#include "AssetViewer.h"
#include "Resource.h"
#include "MainFrm.h"
#include "OgreView.h"
//#include "Viewport.h"

//#include "EntityManager.h"


CAssetViewer::CAssetViewer()
	//: m_pDisplayedEnt(NULL)
{
}

CAssetViewer::~CAssetViewer()
{
}

BEGIN_MESSAGE_MAP(CAssetViewer, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

int CAssetViewer::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	const DWORD dwStyle = LBS_NOINTEGRALHEIGHT | WS_CHILD | WS_VISIBLE;
	
	//m_ogreView.Create(dwStyle, rectDummy, this, 1);
	//m_ogreView.createViewport(20, 20);

	return 0;
}

void CAssetViewer::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	//m_ogreView.SetWindowPos (NULL, rectClient.left + 1, rectClient.top + 1, rectClient.Width() - 2, rectClient.Height() - 2, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
}

void
CAssetViewer::OnAssetChanged(const std::string& assetName)
{
	if (!hasCIEnding(assetName, ".mesh"))
		return;

	//if (m_pDisplayedEnt)
		//delete m_pDisplayedEnt;
/*
	m_pDisplayedEnt = CEntityManager::getInstance()->newEnt();
	m_pDisplayedEnt->setModel(assetName);
	m_pDisplayedEnt->setVisibilityFlag(2);
	m_pDisplayedEnt->setPosition(Ogre::Vector3(0.0f, 0.0f, 0.0f));

	Ogre::AxisAlignedBox bbox = m_pDisplayedEnt->getBBox();

	Ogre::Vector3 vecMax = bbox.getMaximum();
	m_ogreView.getViewport()->getCamera()->setPosition(vecMax * Ogre::Vector3(1.0f, 0.3f, 1.0f) * 3.0f);
	m_ogreView.getViewport()->getCamera()->lookAt(Ogre::Vector3(0.0f, vecMax.z * 0.8f, 0.0f));*/
}

void
OnAssetChanged(void* pThis, const std::string& assetName)
{
	CAssetViewer* pAssetViewer = (CAssetViewer*)pThis;

	pAssetViewer->OnAssetChanged(assetName);
}