#include "iwebase.h"
#include "IWEMovableObject.h"

#include <OgreSceneManager.h>

IWE_IMPLEMENT_PROP_BEGIN(CIWEMovableObject)
  IWE_PROP_HOLDER_OBJ(m_pNode);
IWE_IMPLEMENT_PROP_END()


CIWEMovableObject::CIWEMovableObject(Ogre::MovableObject* pObj, Ogre::SceneManager* pSceneMgr, IEditorInteraction* pEditorInteraction)
  : CMementoObject(pEditorInteraction),
    m_pSceneMgr(pSceneMgr),
    m_pMovObj(pObj),
    m_pNode(NULL),
    m_bTargetSelected(false)
{
  if (!m_pSceneMgr)
    throw std::exception("pSceneMgr is NULL");
  
  m_pOriNode = m_pNode = new CIWESceneNode(m_pSceneMgr, m_pEditorInteraction);
  m_pNode->attachObject(m_pMovObj);
  
  m_pNode->eventDestroyed +=
    FastDelegate0<>(this, &CIWEMovableObject::destroy);
  m_pNode->eventRestored +=
    FastDelegate0<>(this, &CIWEMovableObject::restore);
}

CIWEMovableObject::~CIWEMovableObject()
{
  if (m_pOriNode)
    delete m_pOriNode;

  m_pOriNode = m_pNode = NULL;
}

void
CIWEMovableObject::initialCreation()
{
  //m_stackMOMemUndoType.push(MO_MEM_CREATE);
  m_pNode->initialCreation();

  //saveState();
}

void
CIWEMovableObject::restore()
{
  if (m_pNode && m_pMovObj)
    m_pNode->attachObject(m_pMovObj);
}

void
CIWEMovableObject::destroy()
{
  setTargetSelected(false);

  eventTargetDeleted(this);

  if (m_pNode && m_pMovObj)
    m_pNode->detachObject(m_pMovObj);
}

void
CIWEMovableObject::deleteTarget()
{
  if (m_pNode)
    m_pNode->destroy();
}

void
CIWEMovableObject::setTargetSelected(bool bSelected)
{
  m_bTargetSelected = bSelected;
  
  if (bSelected)
    eventTargetSelected(this);
  else
    eventTargetUnselected(this);
}

bool
CIWEMovableObject::isTargetSelected() const
{
  return m_bTargetSelected;
}

Ogre::Vector3
CIWEMovableObject::getTargetPosition() const
{
	if (m_pNode)
		return m_pNode->getPosition();

	printf("CIWEMovableObject::getTargetPosition() called but m_pNode is NULL");

	return Ogre::Vector3::ZERO;
}

Ogre::Vector3
CIWEMovableObject::getTargetScale() const
{
	if (m_pNode)
		return m_pNode->getScale();

	printf("CIWEMovableObject::getTargetScale() called but m_pNode is NULL");

	return Ogre::Vector3::ZERO;
}

Ogre::Quaternion
CIWEMovableObject::getTargetOrientation() const
{
	if (m_pNode)
		return m_pNode->getOrientation();

	printf("CIWEMovableObject::getTargetOrientation() called but m_pNode is NULL");

	return Ogre::Quaternion::ZERO;
}

void
CIWEMovableObject::setTargetPosition(const Ogre::Vector3& vecPos)
{
	if (m_pNode)
  {
		m_pNode->setPosition(vecPos);
    setPosition(vecPos);

    eventTargetMoved(this);
  }
	else
		printf("CIWEMovableObject::setTargetPosition() called but m_pNode is NULL");
}

void
CIWEMovableObject::setTargetScale(const Ogre::Vector3& vecScale)
{
	if (m_pNode)
  {
		m_pNode->setScale(vecScale);
    setScale(vecScale);

    eventTargetScaled(this);
  }
	else
		printf("CIWEMovableObject::setTargetScale() called but m_pNode is NULL");
}

void
CIWEMovableObject::setTargetOrientation(const Ogre::Quaternion& quatRotate)
{
	if (m_pNode)
  {
		m_pNode->setOrientation(quatRotate);
    setOrientation(quatRotate);

    eventTargetRotated(this);
  }
	else
		printf("CIWEMovableObject::setTargetOrientation() called but m_pNode is NULL");
}

void
CIWEMovableObject::undo()
{
#if 0
  switch (m_stackMOMemUndoType.top())
  {
  }
  
  m_stackMOMemRedoType.push(m_stackMOMemUndoType.top());
  m_stackMOMemUndoType.pop();
#endif // !0
}

void
CIWEMovableObject::redo()
{
#if 0
  switch (m_stackMOMemRedoType.top())
  {
    case MO_MEM_DELETE:
    case MO_MEM_CREATE:
    {
      break;
    }
  }
  
  m_stackMOMemUndoType.push(m_stackMOMemRedoType.top());
  m_stackMOMemRedoType.pop();
#endif // !0
}

void
CIWEMovableObject::forgetLast()
{
}

void
CIWEMovableObject::forgetFirst()
{
#if 0
  switch (m_stackMOMemRedoType.top())
  {
    case MO_MEM_DELETE:
    case MO_MEM_CREATE:
    {
      if (m_pNode)
        m_pNode->forgetFirst();

      break;
    }
  }

  m_stackMOMemRedoType.pop();
#endif // !0
}