#include "stdafx.h"

#include <OgreManualObject.h>
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>

#include "ViewportGrid.h"

const int							iSmallGridCount = 5 + 1;

CViewportGrid::CViewportGrid(int									iColumnCount,
														 int									iRowCount,
														 float								fUnit,
														 const std::string&		sName,
														 Ogre::Vector3				vecNormal,
														 uint									viewportFlag)
{
	m_iColumnCount = (iColumnCount / iSmallGridCount) * iSmallGridCount;
	m_iRowCount = (iRowCount / iSmallGridCount) * iSmallGridCount;
	m_fUnit = fUnit;
	m_vecNormal = vecNormal;
	m_fGridBlockUnit = m_fUnit * iSmallGridCount;

	try
	{
    Ogre::SceneManager* pSceneMgr = GetRenderEngine()->getSceneManager();

		m_pGridObject = pSceneMgr->createManualObject(getUniqueName(sName.c_str()));

    // 50 / 51
		m_pGridObject->setRenderQueueGroup(0x60);
		//m_pGridObject->setQueryFlags(0);
		m_pGridObject->setVisibilityFlags(viewportFlag);

		m_pSceneNode = pSceneMgr->getRootSceneNode()->createChildSceneNode(getUniqueName(sName + ".Node"));
		m_pSceneNode->attachObject(m_pGridObject);
		m_pSceneNode->setOrientation(Ogre::Vector3::UNIT_Z.getRotationTo(vecNormal));
		m_pSceneNode->setPosition(Ogre::Vector3::ZERO);

		Render(false);
	}
	catch(...)
	{
		throw std::exception("FATAL ERROR: CViewportGrid could not be instantiated.");
	}
}


Ogre::Vector3 SnapTo(const Ogre::Vector3& value, const Ogre::Vector3& normal, float unitSize)
{
	Ogre::Vector3 vector;

	if (normal == Ogre::Vector3::UNIT_X)
		vector.x = value.x;
	else
		vector.x = ((float) ((Ogre::Math::Floor(value.x / unitSize)) * unitSize));

	if (normal == Ogre::Vector3::UNIT_Y)
		vector.y = value.y;
	else
		vector.y = ((float) ((Ogre::Math::Floor(value.y / unitSize)) * unitSize));

	if (normal == Ogre::Vector3::UNIT_Z)
		vector.z = value.z;
	else
		vector.z = ((float) ((Ogre::Math::Floor(value.z / unitSize)) * unitSize));

	return vector;
}

void
CViewportGrid::OnCameraMoved(const Ogre::Vector3& vecCameraPosition)
{
	m_pSceneNode->setPosition(SnapTo(vecCameraPosition, m_vecNormal, m_fGridBlockUnit) * (1.0f - m_vecNormal));
}

void
CViewportGrid::Render(bool update)
{
	float									columnLength = m_iRowCount * m_fUnit;
	float									rowLength = m_iColumnCount * m_fUnit;
	Ogre::Vector3					vecPos = Ogre::Vector3(columnLength * -0.5f, rowLength * -0.5f, 0.0f);
	Ogre::Vector3					vecStart = Ogre::Vector3::ZERO;
	Ogre::Vector3					vecEnd = Ogre::Vector3::ZERO;
  Ogre::ColourValue			smallGridColor = Ogre::ColourValue(0.5f, 0.5f, 0.5f, 0.25f);
  Ogre::ColourValue			mainGridColor = Ogre::ColourValue(1.0f, 1.0f, 1.0f, 0.3f);

	if (update)
		m_pGridObject->beginUpdate(0);
	else
		m_pGridObject->begin("Editor/Grid", Ogre::RenderOperation::OT_LINE_LIST);

	vecStart.y = 0.0f;
	vecEnd.y = columnLength;

	for (int i = 0; i <= m_iColumnCount; i++)
	{
		vecStart.x = i * m_fUnit;
		vecEnd.x = vecStart.x;
		
		m_pGridObject->position(vecStart + vecPos);
		m_pGridObject->colour((i % iSmallGridCount == 0) ? mainGridColor : smallGridColor);
		m_pGridObject->position(vecEnd + vecPos);
		m_pGridObject->colour((i % iSmallGridCount == 0) ? mainGridColor : smallGridColor);
	}
	
	vecStart.x = 0.0f;
	vecEnd.x = rowLength;

	for (int i = 0; i <= m_iRowCount; i++)
	{
		vecStart.y = i * m_fUnit;
		vecEnd.y = vecStart.y;
		
		m_pGridObject->position(vecStart + vecPos);
		m_pGridObject->colour((i % iSmallGridCount == 0) ? mainGridColor : smallGridColor);
		m_pGridObject->position(vecEnd + vecPos);
		m_pGridObject->colour((i % iSmallGridCount == 0) ? mainGridColor : smallGridColor);
	}

	m_pGridObject->end();
}

void
CViewportGrid::ResizeGrid(int iColumnDelta, int iRowDelta)
{
	int			iColumnCount = m_iColumnCount + iColumnDelta;
	int			iRowCount = m_iRowCount + iRowDelta;
	
	iColumnCount = (iColumnCount / iSmallGridCount) * iSmallGridCount;
	iRowCount = (iRowCount / iSmallGridCount) * iSmallGridCount;

	if (iRowCount > 20 && iColumnCount > 20)
	{
		m_iColumnCount = iColumnCount;
		m_iRowCount = iRowCount;

		Render(true);
	}
}