#ifndef IVIEWPORT_H
# define IVIEWPORT_H

namespace Ogre
{
  class Ray;
}

interface_class ICamera;

interface_class IViewport : public UserDataHolder
{
public:
  virtual ~IViewport() {};

	virtual void						      showWindow(bool bShow) = 0;
	virtual void						      resize(int iX, int iY) = 0;

  // Input events. 
	virtual bool						      onMouseInput(const s_inputDatas& mouseDatas) = 0;
	virtual bool						      onKeyboardInput(IWEKeys::e_IWEKey nChar, bool KeyDown, int iRepeatCount) = 0;
	virtual bool						      onMouseLeave() = 0;

  virtual ICamera*              getCamera() = 0;
  virtual bool                  isEditorViewport() = 0;
  
  virtual Ogre::Vector3					GetPlaneCoord(float x, float y, const Ogre::Vector3& vecNormal, const Ogre::Vector3& vecPos) = 0;
  virtual Ogre::Vector3					GetHeightCoord(float x, float y, const float fHeight) = 0;
  virtual ISelectableTarget*    PickTarget(const CIWEPoint<int>& viewportPos, Ogre::Vector3& pickedPos) = 0;
  
	virtual Ogre::Ray				      getViewportRay(float x, float y) = 0;
};


#endif // !IVIEWPORT_H