#ifndef IWE_MESH_OBJECT_H
# define IWE_MESH_OBJECT_H

# include "IWEMovableObject.h"

namespace Ogre
{
  class Entity;
}


class IWEAPI CIWEMeshObject : public CIWEMovableObject
{
public:
  CIWEMeshObject(const std::string& sMesh, Ogre::SceneManager* pSceneMgr, IEditorInteraction* pEditorInteraction);
  virtual ~CIWEMeshObject();

private:
  Ogre::Entity*                           createMesh(const std::string& sMesh, Ogre::SceneManager* pSceneMgr);

protected:
  Ogre::Entity*                           m_pEntObj;
};

#endif // !IWE_MESH_OBJECT_H