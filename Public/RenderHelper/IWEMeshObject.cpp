#include "iwebase.h"
#include "IWEMeshObject.h"

#include <OgreSceneManager.h>
#include <OgreEntity.h>


CIWEMeshObject::CIWEMeshObject(const std::string& sMesh, Ogre::SceneManager* pSceneMgr, IEditorInteraction* pEditorInteraction)
  : CIWEMovableObject(createMesh(sMesh, pSceneMgr), pSceneMgr, pEditorInteraction)
{
}

CIWEMeshObject::~CIWEMeshObject()
{
  if (m_pEntObj)
    m_pSceneMgr->destroyEntity(m_pEntObj);

  m_pEntObj = NULL;
}

Ogre::Entity*
CIWEMeshObject::createMesh(const std::string& sMesh, Ogre::SceneManager* pSceneMgr)
{
  m_pEntObj = pSceneMgr->createEntity(sMesh);

  return m_pEntObj;
}