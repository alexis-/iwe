#ifndef CAMERA_H
# define CAMERA_H

namespace Ogre
{
	class Camera;
	class Quaternion;
}

class CCamera : public ICamera
{
public:
	CCamera(Ogre::Camera* pCamera);

public:
	virtual const Ogre::Vector3&		          getPosition();
	virtual const Ogre::Vector3 		          getDirection();
	virtual const Ogre::Quaternion&	          getOrientation();

	virtual void										          setPosition(const Ogre::Vector3& vecPos);
  virtual void                              setDirection(const Ogre::Vector3& vecDir);
	virtual void										          setOrientation(const Ogre::Quaternion& quatOrientation);
	virtual void										          lookAt(const Ogre::Vector3& vecPos);

	virtual void										          zoom(float delta);
	virtual void										          move(const Ogre::Vector3& vDirection, float fDistance);
	virtual void										          rotate(float x, float y);

  virtual Ogre::Camera*                     getOgreCamera();

public:
	void										                  setAspectRatio(int cx, int cy);

	inline Ogre::Camera*		                  getCamera()
	{
		return m_pCamera;
	}

private:
	Ogre::Camera*						                  m_pCamera;
};

#endif // !CAMERA_H