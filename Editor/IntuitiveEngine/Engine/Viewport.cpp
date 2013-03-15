#include "stdafx.h"

# include <OgreRay.h>

#include "RenderEngine.h"
#include "Viewport.h"
#include "EngineUtils.h"

#include "RenderHelper/EngineUtils.h"


class CDefaultCameraController : public ICameraController
{
public:
  CDefaultCameraController(IViewport* pViewport)
    : m_vecDirection(Ogre::Vector3::ZERO),
      m_vecPosition(Ogre::Vector3::ZERO),
		  m_fSpeed(1.0),
		  m_bMMiddle(false),
		  m_bNegZ(false),
		  m_bPosZ(false),
		  m_bNegX(false),
		  m_bPosX(false),
      m_pViewport(pViewport)
  {
  }

	virtual bool					    onMouseInput(const s_inputDatas& datas)
  {
    e_inputMouseButton type = datas.m_MouseButton;

	  if (type == IPT_MOUSE_MIDDLE)
	  {
		  m_bMMiddle = (bool)datas.m_MouseAction;
		  GetGUI()->showCursor(!m_bMMiddle);

		  m_vecPosition.x = Ogre::Real(datas.m_Pt.x);
		  m_vecPosition.y = Ogre::Real(datas.m_Pt.y);
	  }

	  else if (type == IPT_MOUSE_MOVE)
	  {
		  if (m_bMMiddle)
		  {
			  int deltaX = m_vecPosition.x - datas.m_Pt.x,
					  deltaY = m_vecPosition.y - datas.m_Pt.y;

			  m_pViewport->getCamera()->rotate(Ogre::Real(deltaX), Ogre::Real(deltaY));
        eventCameraRotated(this);

        CIWEPoint<int> curPt = GetGUI()->getCursorPos();
        GetGUI()->setCursorPos(curPt.x + deltaX, curPt.y + deltaY);

        // This is used to prevent deadlock in onMouseInput... SetCursorPos actually call a onMouseInput event, which in turns reach SetCursorPos, and so on..
		    return true;
		  }

		  //m_vecPosition.x = Ogre::Real(datas.m_Pt.x);
		  //m_vecPosition.y = Ogre::Real(datas.m_Pt.y);
	  }
    
		return false;
  }

	virtual void					    onKeyboardInput(IWEKeys::e_IWEKey nChar, bool keyDown, int repeatCount)
  {
	  if (nChar == IWEKeys::Z) //'Z' || nChar == 'z')
		  m_bNegZ = keyDown;

	  else if (nChar == IWEKeys::S) //'S' || nChar == 's')
		  m_bPosZ = keyDown;

	  else if (nChar == IWEKeys::Q) //'Q' || nChar == 'q')
		  m_bNegX = keyDown;

	  else if (nChar == IWEKeys::D) //'D' || nChar == 'd')
		  m_bPosX = keyDown;

	  else if (nChar == IWEKeys::Shift) //VK_SHIFT)
		  m_fSpeed = keyDown ? 3.0f : 1.0f;
  }

	virtual void					    onMouseLeave()
  {
	  m_bNegZ = m_bPosZ = m_bNegX = m_bPosX = false;
	  m_fSpeed = 1.0f;
  }

  virtual IViewport*        getViewport()
  {
    return m_pViewport;
  }

  virtual void              tick()
  {
	  m_vecDirection = Ogre::Vector3::ZERO;

	  if (m_bNegZ)
		  m_vecDirection += Ogre::Vector3::NEGATIVE_UNIT_Z;
	  if (m_bPosZ)
		  m_vecDirection += Ogre::Vector3::UNIT_Z;
	  if (m_bNegX)
		  m_vecDirection += Ogre::Vector3::NEGATIVE_UNIT_X;;
	  if (m_bPosX)
		  m_vecDirection += Ogre::Vector3::UNIT_X;
    
	  if (m_vecDirection != Ogre::Vector3::ZERO)
    {
  		m_pViewport->getCamera()->move(m_pViewport->getCamera()->getOrientation() * m_vecDirection.normalisedCopy(), m_fSpeed);
      eventCameraMoved(this);
    }
  }

private:
	float										  m_fSpeed;

	bool										  m_bMMiddle;
	bool										  m_bNegZ;
	bool										  m_bPosZ;
	bool										  m_bNegX;
	bool										  m_bPosX;

	Ogre::Vector3						  m_vecDirection;
	Ogre::Vector3						  m_vecPosition;

  IViewport*                m_pViewport;
};

CViewport::CViewport(size_t handle, const CIWEPoint<ushort>& size, bool bEditorViewport, int maskID)
	: m_bEditorViewport(bEditorViewport)
{
	Ogre::NameValuePairList				misc;
	CRenderEngine*								pRenderEngine = GetRenderEngine();
	Ogre::SceneManager*						pSceneMgr = pRenderEngine->getSceneManager();
	Ogre::Root*										pRoot = pRenderEngine->getRoot();
	const char*										pName = getUniqueName("RenderWindow");

	if (!pSceneMgr)
		throw std::exception("CRITICAL ERROR: Scene manager not created");

	try
	{
		misc["externalWindowHandle"] = Ogre::StringConverter::toString((size_t)handle);
		m_pWindow = pRoot->createRenderWindow(pName,
			size.x, size.y, false, &misc);
	}
	catch (Ogre::Exception e)
	{
		TRACE(e.getFullDescription().c_str());
		pRenderEngine->logMessage(e.getFullDescription().c_str());
	}
	catch (std::exception e)
	{
		TRACE(e.what());
	}

	Ogre::Camera* pCamera = pRenderEngine->createCamera();
	m_pViewportCamera = NEW CCamera(pCamera);
  m_pViewportCamera->setAspectRatio(size.x, size.y);

  // Create one viewport, entire window
  m_pViewport = m_pWindow->addViewport(pCamera);
  m_pViewport->setBackgroundColour(Ogre::ColourValue(0.2f, 0.2f, 0.2f));
	m_pViewport->setVisibilityMask(maskID);

  if (bEditorViewport)
  {
    ICameraController* pController = new CDefaultCameraController(this);

    pRenderEngine->eventCameraControllerCreated(pController);
  }
}

CViewport::~CViewport()
{
  if (m_pViewportCamera)
    delete m_pViewportCamera;

  if (m_pViewport)
    delete m_pViewport;

  m_pWindow->destroy();

  m_pViewport = NULL;
  m_pViewportCamera = NULL;
  m_pWindow = NULL;
}

void
CViewport::showWindow(bool show)
{
	if (m_pWindow)
	{
		m_pWindow->setActive(show);
		m_pWindow->setVisible(show);
	}
}

void
CViewport::resize(int cx, int cy)
{
	if (m_pWindow)
	{
		//m_pWindow->resize(cx, cy);
		m_pWindow->windowMovedOrResized();
		m_pViewportCamera->setAspectRatio(cx, cy);
	}
}

bool
CViewport::onMouseInput(const s_inputDatas&			datas)
{
	return GetEditorInteraction()->onMouseInput(datas, this);
}

bool
CViewport::onKeyboardInput(IWEKeys::e_IWEKey nChar, bool keyDown, int repeatCount)
{
	GetEditorInteraction()->onKeyboardInput(nChar, keyDown, repeatCount, this);

  return false;
}

bool
CViewport::onMouseLeave()
{
	GetEditorInteraction()->onMouseLeave(this);

  return false;
}

Ogre::Ray
CViewport::getViewportRay(float x, float y)
{
	return m_pViewportCamera->getCamera()->getCameraToViewportRay(x / m_pWindow->getWidth(),
						y / m_pWindow->getHeight());
}

Ogre::Vector3
CViewport::GetPlaneCoord(float x, float y, const Ogre::Vector3& vecNormal, const Ogre::Vector3& vecPos)
{
	Ogre::Ray				ray = getViewportRay(x, y);
	Ogre::Plane			gridPlane(vecNormal, vecPos);
	CRaycastResult	rayRes(ray, gridPlane);

	if (rayRes.hasHit())
		return rayRes.getPosition();

	return Ogre::Vector3::ZERO;
}

Ogre::Vector3
CViewport::GetHeightCoord(float x, float y, const float fHeight)
{
	Ogre::Ray				ray = getViewportRay(x, y);
	Ogre::Plane			gridPlane(Ogre::Vector3::UNIT_Y, Ogre::Vector3(0.0f, fHeight, 0.0f));
	CRaycastResult	rayRes(ray, gridPlane);

	if (rayRes.hasHit())
		return rayRes.getPosition();

	return Ogre::Vector3::ZERO;
}

ISelectableTarget*
CViewport::PickTarget(const CIWEPoint<int>& viewportPos, Ogre::Vector3& pickedPos)
{
	ISelectableTarget* pPickedTarget = NULL;

  float fDist = FLT_MAX;
  float fBestDist = FLT_MAX;
  Ogre::Vector3 tmpPos;

  GetEditorInteraction()->requestTargetPick(viewportPos, this, fDist, tmpPos,
    [&fDist, &fBestDist, &pPickedTarget, &tmpPos, &pickedPos](ISelectableTarget* pTarget) {
      if (pTarget != NULL && fDist < fBestDist) { pPickedTarget = pTarget; pickedPos = tmpPos; fBestDist = fDist; }
  });

  return pPickedTarget;
}