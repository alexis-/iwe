#ifndef RH_ENGINE_UTILS_H
# define RH_ENGINE_UTILS_H

# include "iwebase.h"


# include <OgreMeshManager.h>
# include <OgreAnimationState.h>
# include <OgreEntity.h>
# include <OgreSkeletonInstance.h>
# include <OgreAnimation.h>
# include <OgreStringConverter.h>
# include <OgreTagPoint.h>
# include <OgreBone.h>
# include <OgreSceneManager.h>
# include <OgreSceneNode.h>


namespace RenderHelper
{
  IWEAPI void                     GetMeshInformation(Ogre::Entity *entity,
                                                     size_t &vertex_count,
                                                     Ogre::Vector3* &vertices,
                                                     size_t &index_count,
                                                     unsigned long* &indices,
                                                     const Ogre::Vector3 &position,
                                                     const Ogre::Quaternion &orient,
                                                     const Ogre::Vector3 &scale);

  IWEAPI ISelectableTarget* PickSelectableTarget(float x, float y, Ogre::SceneManager* pSceneMgr, IViewport* pViewport, float& fDist, Ogre::Vector3& vecPickedPos);
  IWEAPI ISelectableTarget* PickSelectableTarget(float x, float y, Ogre::SceneManager* pSceneMgr, IViewport* pViewport, float& fDist, Ogre::Vector3& vecPickedPos, const std::string& sType);

  template<typename T>
  T* PickCustom(float x, float y, Ogre::SceneManager* pSceneMgr, IViewport* pViewport, float& fDist, Ogre::Vector3& vecPickedPos, const std::string& sType)
  {
    ISelectableTarget* ret = PickSelectableTarget(x, y, pSceneMgr, pViewport, fDist, vecPickedPos, sType);

    return dynamic_cast<T*>(ret);
  }
  
	IWEAPI void										DestroyAllAttachedMovableObjects(Ogre::SceneNode* pSceneNode);
};

class IWEAPI CRaycastResult
{
public:
	CRaycastResult(Ogre::Ray ray, Ogre::Plane);

public:
	Ogre::Vector3									getPosition();
	bool													hasHit();

private:
	void													castRay(Ogre::Ray ray, std::pair<bool, Ogre::Real> result);

private:
	bool													m_bHit;
	Ogre::Real										m_fDist;
	Ogre::Vector3									m_vecPosition;
};

#endif // !RH_ENGINE_UTILS_H