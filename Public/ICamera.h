#ifndef ICAMERA_H
# define ICAMERA_H

namespace Ogre
{
  class Camera;
}

interface_class ICamera
{
public:
  virtual ~ICamera() {};
  
public:
	virtual const Ogre::Vector3&		          getPosition() = 0;
	virtual const Ogre::Vector3 		          getDirection() = 0;
	virtual const Ogre::Quaternion&	          getOrientation() = 0;

	virtual void										          setPosition(const Ogre::Vector3& vecPos) = 0;
  virtual void                              setDirection(const Ogre::Vector3& vecDir) = 0;
	virtual void										          setOrientation(const Ogre::Quaternion& quatOrientation) = 0;
	virtual void										          lookAt(const Ogre::Vector3& vecPos) = 0;
  
	virtual void										          zoom(float delta) = 0;
	virtual void										          move(const Ogre::Vector3& vDirection, float fDistance) = 0;
	virtual void										          rotate(float x, float y) = 0;

  virtual Ogre::Camera*                     getOgreCamera() = 0;
};


#endif // !ICAMERA_H