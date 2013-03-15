#ifndef VIEWPORT_H
# define VIEWPORT_H

namespace Ogre
{
	class Viewport;
	class Camera;
	class RenderWindow;
	class Ray;
}

# include "Camera.h"

class CViewport : public IViewport
{
public:
	//CViewport(size_t handle, const CIWEPoint<ushort>& rc);
	CViewport(size_t handle, const CIWEPoint<ushort>& rc, bool bEditorViewport = true, int maskID = 1);

  ~CViewport();

  // Interface stuffs
public:
	virtual void						    showWindow(bool bShow);
	virtual void						    resize(int iX, int iY);

	virtual bool						    onMouseInput(const s_inputDatas& mouseDatas);
	virtual bool						    onKeyboardInput(IWEKeys::e_IWEKey nChar, bool KeyDown, int iRepeatCount);
	virtual bool						    onMouseLeave();

	virtual ICamera*            getCamera()
	{
		return m_pViewportCamera;
	}

  virtual bool                isEditorViewport()
  {
    return m_bEditorViewport;
  }
  
	virtual Ogre::Vector3			  GetPlaneCoord(float x, float y, const Ogre::Vector3& vecNormal, const Ogre::Vector3& vecPos);
	virtual Ogre::Vector3			  GetHeightCoord(float x, float y, const float fHeight);
  virtual ISelectableTarget*  PickTarget(const CIWEPoint<int>& viewportPos, Ogre::Vector3& pickedPos);
  
public:
	virtual Ogre::Ray				    getViewportRay(float x, float y);

private:
	CCamera*				            m_pViewportCamera;
	Ogre::Viewport*					    m_pViewport;
  Ogre::RenderWindow*			    m_pWindow;

  bool                        m_bEditorViewport;
};

#endif // !VIEWPORT_H