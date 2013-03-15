#include "IWEPlugin.h"

#include <OgreSubEntity.h>

#include "CBaseEntity.h"
#include "EntityManager.h"

IWE_IMPLEMENT_PROP_BEGIN(CBaseEntity)
  IWE_PROP_LEVEL_BEGIN("Editor");
    IWE_PROP_INT_S("Id", "Internal id", m_nEntID, [](int n) {}, true);
  IWE_PROP_LEVEL_END();
  IWE_PROP_LEVEL_BEGIN("Entity");
    IWE_PROP_STRING_S("Mesh", "Mesh used for this entity",
      m_pModelName, [pInst](const std::string& sModelName) {
        pInst->m_stackMemUndoType.push(ENT_MEM_MESH); pInst->m_stackMemUndoStr.push(pInst->getModelName());
        pInst->setModel(sModelName, false); pInst->saveState();
      }, false);
    IWE_PROP_VECTOR3_S("Position", m_vecPosition,
      [pInst](float fX, float fY, float fZ) {
        pInst->m_stackMemUndoType.push(ENT_MEM_POSITION); pInst->m_stackMemUndoVec3.push(pInst->getPosition()); pInst->saveState();
        pInst->m_vecPosition.Get()[0] = fX; pInst->m_vecPosition.Get()[1] = fY; pInst->m_vecPosition.Get()[2] = fZ;
        pInst->setPosition(pInst->m_vecPosition);
      }, false);
    //IWE_PROP_VECTOR3_GS("Orientation (Degree)", m_vecPosition,
    //  [pInst](int i) -> float {
    //    if (i == 0) return pInst->m_quatOrientation.Get().getPitch();
    //  },
    //  [pInst](float fX, float fY, float fZ) {
    //    pInst->m_stackMemUndoType.push(ENT_MEM_ROTATE); pInst->m_stackMemUndoQuat.push(pInst->getOrientation()); pInst->saveState();
    //    pInst->m_quatOrientation.Get().FromAxes(Ogre::Vector3(fX), Ogre::Vector3(fY), Ogre::Vector3(fZ));
    //    pInst->setOrientation(pInst->m_quatOrientation);
    //  }, false);
    IWE_PROP_QUATERNION_S("Orientation (Quat)", m_quatOrientation,
      [pInst](float fW, float fX, float fY, float fZ) {
        pInst->m_stackMemUndoType.push(ENT_MEM_ROTATE); pInst->m_stackMemUndoQuat.push(pInst->getOrientation()); pInst->saveState();
        pInst->m_quatOrientation.Get()[0] = fW; pInst->m_quatOrientation.Get()[1] = fX; pInst->m_quatOrientation.Get()[2] = fY; pInst->m_quatOrientation.Get()[3] = fZ;
        pInst->setOrientation(pInst->m_quatOrientation);
      }, false);
    IWE_PROP_VECTOR3_S("Scale", m_vecScale,
      [pInst](float fX, float fY, float fZ) {
        pInst->m_stackMemUndoType.push(ENT_MEM_SCALE); pInst->m_stackMemUndoVec3.push(pInst->getScale()); pInst->saveState();
        pInst->m_vecScale.Get()[0] = fX; pInst->m_vecScale.Get()[1] = fY; pInst->m_vecScale.Get()[2] = fZ;
        pInst->setScale(pInst->m_vecScale);
      }, false);
  IWE_PROP_LEVEL_END();
IWE_IMPLEMENT_PROP_END()

CBaseEntity::CBaseEntity(uint	entID)
  : CMementoObject(GetEditorInteraction()),
    m_pNode (NULL),
	  m_pOgreEnt (NULL),
    m_bTargetSelected(false),
    m_pSelectBoxObject(NULL)
{
  m_nEntID = entID;
  m_pModelName = "";
	m_pNode = GetRenderEngine()->getSceneManager()->getRootSceneNode()->createChildSceneNode();
}

CBaseEntity::~CBaseEntity()
{
	CEntityManager::getInstance()->removeEnt(this);

	renderCleanup();
}

// IBaseEntity interface

uint
CBaseEntity::getEntID() const
{
	return m_nEntID;
}

// Others ...

void
CBaseEntity::think(Ogre::Real		deltaTime)
{
	//m_pGameEnt->Think();

	renderFrame(deltaTime);
}

//IEditorEntity*
//CBaseEntity::getEditorEnt() const
//{
//	return (IEditorEntity*)this;
//}

Ogre::AxisAlignedBox
CBaseEntity::getBBox() const
{
	if (m_pOgreEnt)
		return m_pOgreEnt->getBoundingBox();

	printf("CBaseEntity::getBBox() called but m_pOgreEnt is NULL");
	
	return Ogre::AxisAlignedBox::BOX_NULL;
}

Ogre::Vector3
CBaseEntity::getPosition() const
{
	if (m_pNode)
		return m_pNode->getPosition();

	printf("CBaseEntity::getPosition() called but m_pNode is NULL"); // TODO: Log this

	return Ogre::Vector3::ZERO;
}

Ogre::Quaternion
CBaseEntity::getOrientation() const
{
	if (m_pNode)
		return m_pNode->getOrientation();

	printf("CBaseEntity::getOrientation() called but m_pNode is NULL");

	return Ogre::Quaternion::ZERO;
}

Ogre::Vector3
CBaseEntity::getScale() const
{
	if (m_pNode)
		return m_pNode->getScale();

	printf("CBaseEntity::getScale() called but m_pNode is NULL");

	return Ogre::Vector3::ZERO;
}

void
CBaseEntity::setPosition(Ogre::Vector3 vecPos)
{
	if (m_pNode)
  {
		m_pNode->setPosition(vecPos);
    m_vecPosition = vecPos;
  }
	else
		printf("CBaseEntity::setPosition() called but m_pNode is NULL");
}

void
CBaseEntity::setOrientation(Ogre::Quaternion quatOrient)
{
	if (m_pNode)
  {
		m_pNode->setOrientation(quatOrient);
    m_quatOrientation = quatOrient;
  }
	else
		printf("CBaseEntity::setPosition() called but m_pNode is NULL");
}

void
CBaseEntity::setScale(Ogre::Vector3 scaleVec)
{
	if (m_pNode)
  {
		m_pNode->setScale(scaleVec);
    m_vecScale = scaleVec;
  }
	else
		printf("CBaseEntity::setScale() called but m_pNode is NULL");
}

void
CBaseEntity::displayWireframe(bool bDisplay)
{
  if (!m_pOgreEnt)
    return;

  // Display wireframe
  if (bDisplay && m_vecOriMat.size() == 0)
  {
    for (int i = 0; i < m_pOgreEnt->getNumSubEntities(); i++)
    {
      Ogre::SubEntity* pSubEnt = m_pOgreEnt->getSubEntity(i);
      Ogre::MaterialPtr pOriMat = pSubEnt->getMaterial();

      std::string sMatName = pOriMat->getName() + m_pOgreEnt->getName() + Ogre::StringConverter::toString(i);

      Ogre::MaterialPtr pMat = Ogre::MaterialManager::getSingleton().getByName(sMatName);

      if (pMat.isNull())
      {
        pMat = pOriMat->clone(sMatName);
        
        Ogre::Pass* pPass = pMat->getTechnique(0)->createPass();
        pPass->setLightingEnabled(false);
        pPass->setPolygonMode(Ogre::PM_WIREFRAME);
        //pPass->setDepthCheckEnabled(false);
        pPass->createTextureUnitState()->setColourOperationEx(Ogre::LBX_SOURCE1, Ogre::LBS_MANUAL, Ogre::LBS_CURRENT, Ogre::ColourValue(1.0f, 1.0f, 0.0));
      }

      pSubEnt->setMaterial(pMat);
      m_vecOriMat.push_back(pOriMat);
    }
  }
  else if (!bDisplay && m_vecOriMat.size() > 0)
  {
    // Hide wireframe
    for (int i = 0; i < m_pOgreEnt->getNumSubEntities(); i++)
    {
      Ogre::SubEntity* pSubEnt = m_pOgreEnt->getSubEntity(i);

      pSubEnt->setMaterial(m_vecOriMat[i]);
    }

    m_vecOriMat.clear();
  }
}

void
CBaseEntity::setTargetSelected(bool bSelected)
{
	Ogre::SceneManager* pSceneMgr = GetRenderEngine()->getSceneManager();

	if (m_bTargetSelected == bSelected)
		return;

  m_bTargetSelected = bSelected;

	if (bSelected)
	{
    if (m_pSelectBoxObject)
    {
      if (m_pNode)
        m_pNode->detachObject(m_pSelectBoxObject);

		  pSceneMgr->destroyManualObject(m_pSelectBoxObject);
      m_pSelectBoxObject = NULL;
    }

		Ogre::AxisAlignedBox bbox = getBBox();
		Ogre::Vector3 boxSize = bbox.getSize();
		Ogre::Vector3 adjustSize = (Ogre::Vector3(1.0f, 0.0f, 1.0f) * bbox.getSize()) / 2;

		m_pSelectBoxObject = pSceneMgr->createManualObject(); //getUniqueName("selectBox"));

		m_pSelectBoxObject->setVisibilityFlags(1);
		m_pSelectBoxObject->begin("Editor/Grid", Ogre::RenderOperation::OT_LINE_LIST);

		/////////////////////////////////////////////////////////////////////

		m_pSelectBoxObject->position(Ogre::Vector3::ZERO - adjustSize);
		m_pSelectBoxObject->position(Ogre::Vector3(boxSize.x, 0.0f, 0.0f) - adjustSize);
		
		m_pSelectBoxObject->position(Ogre::Vector3::ZERO - adjustSize);
		m_pSelectBoxObject->position(Ogre::Vector3(0.0f, boxSize.y, 0.0f) - adjustSize);

		m_pSelectBoxObject->position(Ogre::Vector3::ZERO - adjustSize);
		m_pSelectBoxObject->position(Ogre::Vector3(0.0f, 0.0f, boxSize.z) - adjustSize);

		/////////////////////////////////////////////////////////////////////

		m_pSelectBoxObject->position(Ogre::Vector3(boxSize.x, 0.0f, boxSize.z) - adjustSize);
		m_pSelectBoxObject->position(Ogre::Vector3(0.0f, 0.0f, boxSize.z) - adjustSize);
		
		m_pSelectBoxObject->position(Ogre::Vector3(boxSize.x, 0.0f, boxSize.z) - adjustSize);
		m_pSelectBoxObject->position(Ogre::Vector3(boxSize.x, boxSize.y, boxSize.z) - adjustSize);

		m_pSelectBoxObject->position(Ogre::Vector3(boxSize.x, 0.0f, boxSize.z) - adjustSize);
		m_pSelectBoxObject->position(Ogre::Vector3(boxSize.x, 0.0f, 0.0f) - adjustSize);

		/////////////////////////////////////////////////////////////////////

		m_pSelectBoxObject->position(Ogre::Vector3(0.0f, boxSize.y, 0.0f) - adjustSize);
		m_pSelectBoxObject->position(Ogre::Vector3(0.0f, boxSize.y, boxSize.z) - adjustSize);

		m_pSelectBoxObject->position(Ogre::Vector3(boxSize.x, boxSize.y, 0.0f) - adjustSize);
		m_pSelectBoxObject->position(Ogre::Vector3(boxSize.x, 0.0f, 0.0f) - adjustSize);

		m_pSelectBoxObject->position(Ogre::Vector3(0.0f, boxSize.y, 0.0f) - adjustSize);
		m_pSelectBoxObject->position(Ogre::Vector3(boxSize.x, boxSize.y, 0.0f) - adjustSize);

		/////////////////////////////////////////////////////////////////////

		m_pSelectBoxObject->position(Ogre::Vector3(boxSize.x, boxSize.y, boxSize.z) - adjustSize);
		m_pSelectBoxObject->position(Ogre::Vector3(0.0f, boxSize.y, boxSize.z) - adjustSize);

		m_pSelectBoxObject->position(Ogre::Vector3(0.0f, boxSize.y, boxSize.z) - adjustSize);
		m_pSelectBoxObject->position(Ogre::Vector3(0.0f, 0.0f, boxSize.z) - adjustSize);

		m_pSelectBoxObject->position(Ogre::Vector3(boxSize.x, boxSize.y, boxSize.z) - adjustSize);
		m_pSelectBoxObject->position(Ogre::Vector3(boxSize.x, boxSize.y, 0.0f) - adjustSize);

		/////////////////////////////////////////////////////////////////////

		m_pSelectBoxObject->end();
		attachObject(m_pSelectBoxObject);

    //m_pSelectBoxObject->setVisible(false);

    displayWireframe(true);

    eventTargetSelected(this);
	}
	else if (m_pSelectBoxObject)
  {
    if (m_pNode)
      m_pNode->detachObject(m_pSelectBoxObject);

		pSceneMgr->destroyManualObject(m_pSelectBoxObject);
    m_pSelectBoxObject = NULL;

    displayWireframe(false);

    eventTargetUnselected(this);
  }
}

bool
CBaseEntity::isTargetSelected() const
{
  return m_bTargetSelected;
}

Ogre::Vector3
CBaseEntity::getTargetPosition() const
{
  return getPosition();
}

Ogre::Vector3
CBaseEntity::getTargetScale() const
{
  return getScale();
}

Ogre::Quaternion
CBaseEntity::getTargetOrientation() const
{
  return getOrientation();
}

void
CBaseEntity::setTargetPosition(const Ogre::Vector3& vecPos)
{
  setPosition(vecPos);
}

void
CBaseEntity::setTargetScale(const Ogre::Vector3& vecScale)
{
  setScale(vecScale);
}

void
CBaseEntity::setTargetOrientation(const Ogre::Quaternion& quatRotate)
{
  setOrientation(quatRotate);
}

void
CBaseEntity::moveTarget(const Ogre::Vector3& vecDelta)
{
  setPosition(getPosition() + vecDelta);
  eventTargetMoved(this);
}

void
CBaseEntity::scaleTarget(const Ogre::Vector3& vecScale)
{
  setScale(getScale() * vecScale);
  eventTargetScaled(this);
}

void
CBaseEntity::rotateTarget(const Ogre::Quaternion& quatRotate)
{
  setOrientation(getOrientation() * quatRotate);
  eventTargetRotated(this);
}

void
CBaseEntity::deleteTarget()
{
  s_entMemento entState = { getModelName(), getPosition(), getScale(), getOrientation() };

  m_stackMemUndoType.push(ENT_MEM_DELETE);
  m_stackMemUndoDelete.push(entState);
  
  saveState();

  deleteEnt();
  //delete this;
}


void
CBaseEntity::createEnt(const std::string& sMesh, const Ogre::Vector3& vecPos,
                       const Ogre::Vector3& vecScale, const Ogre::Quaternion& quatOrient)
{
  setModel(sMesh, true);
  setPosition(vecPos);
  setScale(vecScale);
  setOrientation(quatOrient);

  CPlugin::getInstance()->addEntity(this);
}

void
CBaseEntity::deleteEnt()
{
  setTargetSelected(false);

  renderCleanup();
  eventTargetDeleted(this);

  CPlugin::getInstance()->removeEntity(this);
}

void
CBaseEntity::initialCreation()
{
  s_entMemento entState = { getModelName(), getPosition(), getScale(), getOrientation() };

  m_stackMemUndoType.push(ENT_MEM_CREATE);
  m_stackMemUndoDelete.push(entState);

  saveState();
  
  CPlugin::getInstance()->addEntity(this);
}

void
CBaseEntity::undo()
{
  switch (m_stackMemUndoType.top())
  {
    case ENT_MEM_POSITION:
    {
      Ogre::Vector3 vecPos = m_stackMemUndoVec3.top();

      m_stackMemUndoVec3.pop();
      m_stackMemRedoType.push(m_stackMemUndoType.top());
      m_stackMemRedoVec3.push(getPosition());
    
      setPosition(vecPos);

      break;
    }
    case ENT_MEM_SCALE:
    {
      Ogre::Vector3 vecScale = m_stackMemUndoVec3.top();

      m_stackMemUndoVec3.pop();
      m_stackMemRedoType.push(m_stackMemUndoType.top());
      m_stackMemRedoVec3.push(getScale());
    
      setScale(vecScale);
      break;
    }
    case ENT_MEM_ROTATE:
    {
      Ogre::Quaternion quatOrient = m_stackMemUndoQuat.top();

      m_stackMemUndoQuat.pop();
      m_stackMemRedoType.push(m_stackMemUndoType.top());
      m_stackMemRedoQuat.push(getOrientation());

      setOrientation(quatOrient);
      break;
    }
    case ENT_MEM_DELETE:
    {
      s_entMemento entState = m_stackMemUndoDelete.top();
      
      m_stackMemUndoDelete.pop();
      m_stackMemRedoType.push(m_stackMemUndoType.top());
      m_stackMemRedoDelete.push(entState);

      createEnt(entState.m_sMesh, entState.m_vecPos, entState.m_vecScale, entState.m_quatOrient);
      break;
    }
    case ENT_MEM_CREATE:
    {
      s_entMemento entState = m_stackMemUndoDelete.top();
      
      m_stackMemUndoDelete.pop();
      m_stackMemRedoType.push(m_stackMemUndoType.top());
      m_stackMemRedoDelete.push(entState);

      deleteEnt();
      break;
    }
    case ENT_MEM_MESH:
    {
      std::string sMesh = m_stackMemUndoStr.top();

      m_stackMemUndoStr.pop();
      m_stackMemRedoType.push(m_stackMemUndoType.top());
      m_stackMemRedoStr.push(getModelName());

      setModel(sMesh, true);
      break;
    }
  }

  m_stackMemUndoType.pop();
}

void
CBaseEntity::redo()
{
  switch (m_stackMemRedoType.top())
  {
    case ENT_MEM_POSITION:
    {
      Ogre::Vector3 vecPos = m_stackMemRedoVec3.top();

      m_stackMemRedoVec3.pop();
      m_stackMemUndoType.push(m_stackMemRedoType.top());
      m_stackMemUndoVec3.push(getPosition());
    
      setPosition(vecPos);

      break;
    }
    case ENT_MEM_SCALE:
    {
      Ogre::Vector3 vecScale = m_stackMemRedoVec3.top();

      m_stackMemRedoVec3.pop();
      m_stackMemUndoType.push(m_stackMemRedoType.top());
      m_stackMemUndoVec3.push(getScale());
    
      setScale(vecScale);
      break;
    }
    case ENT_MEM_ROTATE:
    {
      Ogre::Quaternion quatOrient = m_stackMemRedoQuat.top();

      m_stackMemRedoQuat.pop();
      m_stackMemUndoType.push(m_stackMemRedoType.top());
      m_stackMemUndoQuat.push(getOrientation());

      setOrientation(quatOrient);
      break;
    }
    case ENT_MEM_DELETE:
    {
      s_entMemento entState = m_stackMemRedoDelete.top();
      
      m_stackMemRedoDelete.pop();
      m_stackMemUndoType.push(m_stackMemRedoType.top());
      m_stackMemUndoDelete.push(entState);

      deleteEnt();
      break;
    }
    case ENT_MEM_CREATE:
    {
      s_entMemento entState = m_stackMemRedoDelete.top();
      
      m_stackMemRedoDelete.pop();
      m_stackMemUndoType.push(m_stackMemRedoType.top());
      m_stackMemUndoDelete.push(entState);
      
      createEnt(entState.m_sMesh, entState.m_vecPos, entState.m_vecScale, entState.m_quatOrient);
      break;
    }
    case ENT_MEM_MESH:
    {
      std::string sMesh = m_stackMemRedoStr.top();

      m_stackMemRedoStr.pop();
      m_stackMemUndoType.push(m_stackMemRedoType.top());
      m_stackMemUndoStr.push(getModelName());

      setModel(sMesh, true);
      break;
    }
  }

  m_stackMemRedoType.pop();
}

void
CBaseEntity::forgetLast()
{
}

void
CBaseEntity::forgetFirst()
{
  switch (m_stackMemRedoType.top())
  {
    case ENT_MEM_POSITION:
    case ENT_MEM_SCALE:
    {
      m_stackMemRedoVec3.pop();
      break;
    }
    case ENT_MEM_ROTATE:
    {
      m_stackMemRedoQuat.pop();
      break;
    }
    case ENT_MEM_CREATE:
    case ENT_MEM_DELETE:
    {
      m_stackMemRedoDelete.pop();
      break;
    }
    case ENT_MEM_MESH:
    {
      m_stackMemRedoStr.pop();
      break;
    }
  }

  m_stackMemRedoType.pop();
}