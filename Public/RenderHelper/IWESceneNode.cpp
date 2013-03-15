#include "iwebase.h"
#include "IWESceneNode.h"

#include "RenderHelper/EngineUtils.h"

#include <OgreSceneNode.h>
#include <OgreSceneManager.h>


IWE_IMPLEMENT_PROP_BEGIN(CIWESceneNode)
  IWE_PROP_LEVEL_BEGIN("Scene Node");
    //IWE_PROP_BOOL_S("Visible", "", m_bTest, [pInst](bool b) { pInst->m_pOgreEnt->setVisible(b); }, false);
    IWE_PROP_VECTOR3_S("Position", m_vecPosition,
      [pInst](float fX, float fY, float fZ) {
        pInst->m_stackMemUndoType.push(SN_MEM_POSITION); pInst->m_stackMemUndoVec3.push(pInst->getPosition()); pInst->saveState();
        pInst->m_vecPosition.Get()[0] = fX; pInst->m_vecPosition.Get()[1] = fY; pInst->m_vecPosition.Get()[2] = fZ;
        pInst->setPosition(pInst->m_vecPosition);
      }, false);
    //IWE_PROP_VECTOR3_GS("Orientation (Degree)", m_vecPosition,
    //  [pInst](int i) -> float {
    //    if (i == 0) return pInst->m_quatOrientation.Get().getPitch();
    //  },
    //  [pInst](float fX, float fY, float fZ) {
    //    pInst->m_stackMemUndoType.push(SN_MEM_ROTATE); pInst->m_stackMemUndoQuat.push(pInst->getOrientation()); pInst->saveState();
    //    pInst->m_quatOrientation.Get().FromAxes(Ogre::Vector3(fX), Ogre::Vector3(fY), Ogre::Vector3(fZ));
    //    pInst->setOrientation(pInst->m_quatOrientation);
    //  }, false);
    IWE_PROP_QUATERNION_S("Orientation (Quat)", m_quatOrientation,
      [pInst](float fW, float fX, float fY, float fZ) {
        pInst->m_stackMemUndoType.push(SN_MEM_ROTATE); pInst->m_stackMemUndoQuat.push(pInst->getOrientation()); pInst->saveState();
        pInst->m_quatOrientation.Get()[0] = fW; pInst->m_quatOrientation.Get()[1] = fX; pInst->m_quatOrientation.Get()[2] = fY; pInst->m_quatOrientation.Get()[3] = fZ;
        pInst->setOrientation(pInst->m_quatOrientation);
      }, false);
    IWE_PROP_VECTOR3_S("Scale", m_vecScale,
      [pInst](float fX, float fY, float fZ) {
        pInst->m_stackMemUndoType.push(SN_MEM_SCALE); pInst->m_stackMemUndoVec3.push(pInst->getScale()); pInst->saveState();
        pInst->m_vecScale.Get()[0] = fX; pInst->m_vecScale.Get()[1] = fY; pInst->m_vecScale.Get()[2] = fZ;
        pInst->setScale(pInst->m_vecScale);
      }, false);
  IWE_PROP_LEVEL_END();
IWE_IMPLEMENT_PROP_END()


CIWESceneNode::CIWESceneNode(Ogre::SceneManager* pSceneMgr, IEditorInteraction* pEditorInteraction)
  : CMementoObject(pEditorInteraction),
    m_pNode(NULL),
    m_bTargetSelected(false),
    m_pSceneMgr(pSceneMgr)
{
  if (!m_pSceneMgr)
    throw std::exception("pSceneMgr is NULL");

  createSceneNode();
}

CIWESceneNode::~CIWESceneNode()
{
  deleteSceneNode();
}

s_SNMemento
CIWESceneNode::generateSNState()
{
  s_SNMemento ret = { getPosition(), getScale(), getOrientation() };
  return ret;
}

void
CIWESceneNode::initialCreation()
{
  s_SNMemento snState = generateSNState();

  m_stackMemUndoType.push(SN_MEM_CREATE);
  m_stackSNMemUndoDelete.push(snState);

  saveState();
}

void
CIWESceneNode::createSceneNode()
{
  if (!m_pNode)
  {
	  m_pNode = m_pSceneMgr->getRootSceneNode()->createChildSceneNode();

    m_vecPosition = m_pNode->getPosition();
    m_vecScale = m_pNode->getScale();
    m_quatOrientation = m_pNode->getOrientation();
  }
}

void
CIWESceneNode::createSceneNode(const Ogre::Vector3& vecPos, const Ogre::Vector3& vecScale, const Ogre::Quaternion& quatOrient)
{
  if (!m_pNode)
  {
    m_pNode = m_pSceneMgr->getRootSceneNode()->createChildSceneNode(vecPos, quatOrient);
    m_pNode->setScale(vecScale);

    eventRestored();
  }
}

void
CIWESceneNode::deleteSceneNode()
{
  if (m_pNode)
  {
    m_pNode->detachAllObjects();
	  m_pSceneMgr->destroySceneNode(m_pNode);

    m_pNode = NULL;
  }
}

void
CIWESceneNode::destroy()
{
  s_SNMemento snState = generateSNState();

  m_stackMemUndoType.push(SN_MEM_DELETE);
  m_stackSNMemUndoDelete.push(snState);

  saveState();

  eventDestroyed();

  deleteSceneNode();
}

void
CIWESceneNode::attachObject(Ogre::MovableObject* pObj)
{
  if (m_pNode)
    m_pNode->attachObject(pObj);
}

void
CIWESceneNode::detachObject(Ogre::MovableObject* pObj)
{
  if (m_pNode)
    m_pNode->detachObject(pObj);
}

Ogre::SceneNode*
CIWESceneNode::getSceneNode()
{
  return m_pNode;
}

Ogre::Vector3
CIWESceneNode::getPosition() const
{
	if (m_pNode)
		return m_pNode->getPosition();

	printf("CIWESceneNode::getPosition() called but m_pNode is NULL");

	return Ogre::Vector3::ZERO;
}

Ogre::Vector3
CIWESceneNode::getScale() const
{
	if (m_pNode)
		return m_pNode->getScale();

	printf("CIWESceneNode::getScale() called but m_pNode is NULL");

	return Ogre::Vector3::ZERO;
}

Ogre::Quaternion
CIWESceneNode::getOrientation() const
{
	if (m_pNode)
		return m_pNode->getOrientation();

	printf("CIWESceneNode::getOrientation() called but m_pNode is NULL");

	return Ogre::Quaternion::ZERO;
}

void
CIWESceneNode::setPosition(const Ogre::Vector3& vecPos)
{
	if (m_pNode)
  {
		m_pNode->setPosition(vecPos);
    m_vecPosition = vecPos;
  }
	else
		printf("CIWESceneNode::setPosition() called but m_pNode is NULL");
}

void
CIWESceneNode::setScale(const Ogre::Vector3& vecScale)
{
	if (m_pNode)
  {
		m_pNode->setScale(vecScale);
    m_vecScale = vecScale;
  }
	else
		printf("CIWESceneNode::setScale() called but m_pNode is NULL");
}

void
CIWESceneNode::setOrientation(const Ogre::Quaternion& quatRotate)
{
	if (m_pNode)
  {
		m_pNode->setOrientation(quatRotate);
    m_quatOrientation = quatRotate;
  }
	else
		printf("CIWESceneNode::setOrientation() called but m_pNode is NULL");
}

void
CIWESceneNode::undo()
{
  switch (m_stackMemUndoType.top())
  {
    case SN_MEM_POSITION:
    {
      Ogre::Vector3 vecPos = m_stackMemUndoVec3.top();

      m_stackMemUndoVec3.pop();
      m_stackMemRedoType.push(m_stackMemUndoType.top());
      m_stackMemRedoVec3.push(getPosition());
    
      setPosition(vecPos);
      break;
    }
    case SN_MEM_SCALE:
    {
      Ogre::Vector3 vecScale = m_stackMemUndoVec3.top();

      m_stackMemUndoVec3.pop();
      m_stackMemRedoType.push(m_stackMemUndoType.top());
      m_stackMemRedoVec3.push(getScale());
    
      setScale(vecScale);
      break;
    }
    case SN_MEM_ROTATE:
    {
      Ogre::Quaternion quatOrient = m_stackMemUndoQuat.top();

      m_stackMemUndoQuat.pop();
      m_stackMemRedoType.push(m_stackMemUndoType.top());
      m_stackMemRedoQuat.push(getOrientation());

      setOrientation(quatOrient);
      break;
    }
    case SN_MEM_DELETE:
    {
      s_SNMemento snState = m_stackSNMemUndoDelete.top();
      
      m_stackSNMemUndoDelete.pop();
      m_stackMemRedoType.push(m_stackMemUndoType.top());
      m_stackSNMemRedoDelete.push(snState);

      createSceneNode(snState.m_vecPos, snState.m_vecScale, snState.m_quatOrient);
      break;
    }
    case SN_MEM_CREATE:
    {
      s_SNMemento snState = m_stackSNMemUndoDelete.top();
      
      m_stackSNMemUndoDelete.pop();
      m_stackMemRedoType.push(m_stackMemUndoType.top());
      m_stackSNMemRedoDelete.push(snState);

      deleteSceneNode();
      break;
    }
  }

  m_stackMemUndoType.pop();
}

void
CIWESceneNode::redo()
{
  switch (m_stackMemRedoType.top())
  {
    case SN_MEM_POSITION:
    {
      Ogre::Vector3 vecPos = m_stackMemRedoVec3.top();

      m_stackMemRedoVec3.pop();
      m_stackMemUndoType.push(m_stackMemRedoType.top());
      m_stackMemUndoVec3.push(getPosition());
    
      setPosition(vecPos);

      break;
    }
    case SN_MEM_SCALE:
    {
      Ogre::Vector3 vecScale = m_stackMemRedoVec3.top();

      m_stackMemRedoVec3.pop();
      m_stackMemUndoType.push(m_stackMemRedoType.top());
      m_stackMemUndoVec3.push(getScale());
    
      setScale(vecScale);
      break;
    }
    case SN_MEM_ROTATE:
    {
      Ogre::Quaternion quatOrient = m_stackMemRedoQuat.top();

      m_stackMemRedoQuat.pop();
      m_stackMemUndoType.push(m_stackMemRedoType.top());
      m_stackMemUndoQuat.push(getOrientation());

      setOrientation(quatOrient);
      break;
    }
    case SN_MEM_DELETE:
    {
      s_SNMemento snState = m_stackSNMemRedoDelete.top();
      
      m_stackSNMemRedoDelete.pop();
      m_stackMemUndoType.push(m_stackMemRedoType.top());
      m_stackSNMemUndoDelete.push(snState);

      deleteSceneNode();
      break;
    }
    case SN_MEM_CREATE:
    {
      s_SNMemento snState = m_stackSNMemRedoDelete.top();
      
      m_stackSNMemRedoDelete.pop();
      m_stackMemUndoType.push(m_stackMemRedoType.top());
      m_stackSNMemUndoDelete.push(snState);
      
      createSceneNode(snState.m_vecPos, snState.m_vecScale, snState.m_quatOrient);
      break;
    }
  }

  m_stackMemRedoType.pop();
}

void
CIWESceneNode::forgetLast()
{
}

void
CIWESceneNode::forgetFirst()
{
  switch (m_stackMemRedoType.top())
  {
    case SN_MEM_POSITION:
    case SN_MEM_SCALE:
    {
      m_stackMemRedoVec3.pop();
      break;
    }
    case SN_MEM_ROTATE:
    {
      m_stackMemRedoQuat.pop();
      break;
    }
    case SN_MEM_CREATE:
    case SN_MEM_DELETE:
    {
      m_stackSNMemRedoDelete.pop();
      break;
    }
  }

  m_stackMemRedoType.pop();
}