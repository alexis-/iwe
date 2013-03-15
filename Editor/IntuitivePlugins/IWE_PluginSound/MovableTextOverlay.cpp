#include <OgreFontManager.h>
#include <OgrePrerequisites.h>
#include "MovableTextOverlay.h"

MovableTextOverlay::MovableTextOverlay(const Ogre::String & name, const Ogre::String & caption,
                                       const Ogre::MovableObject *mov, MovableTextOverlayAttributes *attrs,
                                       float fFadeDistStart, float fFadeDistEnd)
                                       : mpMov(mov)
                                       , mpOv(NULL)
                                       , mpOvContainer(NULL)
                                       , mpOvText(NULL)
                                       , mAttrs(attrs)
                                       , mName(name)
                                       , mCaption("")
                                       , mUpdateFrequency(0.01)
                                       , mNeedUpdate(TRUE)
                                       , mOnScreen(FALSE)
                                       , mEnabled(FALSE)
                                       , m_fFadeDistStart(fFadeDistStart)
                                       , m_fFadeDistEnd(fFadeDistEnd)
{
  if (name == "")
    Ogre::Exception(Ogre::Exception::ERR_INVALIDPARAMS, "Trying to create MovableTextOverlay without name", "MovableTextOverlay::MovableTextOverlay");

  if (caption == "")
    Ogre::Exception(Ogre::Exception::ERR_INVALIDPARAMS, "Trying to create MovableTextOverlay without caption", "MovableTextOverlay::MovableTextOverlay");

  if (mAttrs == NULL)
    Ogre::Exception(Ogre::Exception::ERR_INVALIDPARAMS, "Trying to create MovableTextOverlay without Attributes", "MovableTextOverlay::MovableTextOverlay");
  /*
  if(Ogre::OverlayManager::getSingleton().getByName(name + "_Ov")) 
  { 
  Ogre::Exception(Ogre::Exception::ERR_INVALIDPARAMS, "Trying to create MovableTextOverlay with a duplicate name", "MovableTextOverlay::MovableTextOverlay");
  }
  */
  // create an overlay that we can use for later
  mpOv = Ogre::OverlayManager::getSingleton().create(name+"_Ov");
  mpOv->hide();
  mpOvContainer = static_cast<Ogre::OverlayContainer*>(Ogre::OverlayManager::getSingleton().createOverlayElement(
    "Panel", name+"_OvC"));
  mpOvContainer->setDimensions(0.0, 0.0);

  mpOv->add2D(mpOvContainer);

  mpOvText = Ogre::OverlayManager::getSingleton().createOverlayElement("TextArea", name+"_OvTxt");
  mpOvContainer->addChild(mpOvText);

  mpOvText->setMetricsMode(Ogre::GMM_RELATIVE);
  mpOvText->setDimensions(1.0, 1.0);
  mpOvText->setMetricsMode(Ogre::GMM_PIXELS);
  mpOvText->setPosition(0, 0);

  _updateOverlayAttrs();

  setCaption(caption);
}

MovableTextOverlay::~MovableTextOverlay()
{
  // overlay cleanup -- Ogre would clean this up at app exit but if your app 
  // tends to create and delete these objects often it's a good idea to do it here.

  mpOv->hide();
  Ogre::OverlayManager *overlayManager = Ogre::OverlayManager::getSingletonPtr();
  mpOvContainer->removeChild(mName+"_OvTxt");
  mpOv->remove2D(mpOvContainer);
  overlayManager->destroyOverlayElement(mpOvText);
  overlayManager->destroyOverlayElement(mpOvContainer);
  overlayManager->destroy(mpOv);
}

void MovableTextOverlay::setCaption(const Ogre::String & caption)
{
  if (caption != mCaption)
  {
    mCaption = caption;
    mpOvText->setCaption(mCaption);
    mNeedUpdate = true;
  }
}

void MovableTextOverlay::_computeTextWidth()
{
  const Font *pFont = mAttrs->getFont();
  mTextWidth = 0;

  for(Ogre::String::iterator i = mCaption.begin(); i < mCaption.end();i++)
  {   
    if (*i == 0x0020)
      mTextWidth += pFont->getGlyphAspectRatio(0x0030);
    else
    {
      mTextWidth += pFont->getGlyphAspectRatio(*i);
    }
  }

  mTextWidth *= mAttrs->getCharacterHeight();
}

void MovableTextOverlay::_getMinMaxEdgesOfTopAABBIn2D(Ogre::Real& MinX, Ogre::Real& MinY, Ogre::Real& MaxX, Ogre::Real& MaxY)
{
  const Ogre::Camera* mpCam = mAttrs->getCamera();

  MinX = 0;
  MinY = 0;
  MaxX = 0;
  MaxY = 0;

  Ogre::Real X[4];// the 2D dots of the AABB in screencoordinates
  Ogre::Real Y[4];

  if (!mpMov->isInScene())
    return;

  const Ogre::AxisAlignedBox &AABB = mpMov->getWorldBoundingBox(true);// the AABB of the target
  const Ogre::Vector3 CornersOfTopAABB[4] = {	AABB.getCorner(AxisAlignedBox::FAR_LEFT_TOP),
    AABB.getCorner(AxisAlignedBox::FAR_RIGHT_TOP),
    AABB.getCorner(AxisAlignedBox::NEAR_LEFT_TOP),
    AABB.getCorner(AxisAlignedBox::NEAR_RIGHT_TOP)};

  Ogre::Vector3 CameraPlainNormal = mpCam->getDerivedOrientation().zAxis();//The normal vector of the plaine.this points directly infront of the cam

  Ogre::Plane CameraPlain = Plane(CameraPlainNormal,mpCam->getDerivedPosition());//the plaine that devides the space bevor and behin the cam

  for (int i = 0; i < 4; i++)
  {
    X[i] = 0;
    Y[i] = 0;

    _getScreenCoordinates(CornersOfTopAABB[i],X[i],Y[i]);// transfor into 2d dots


    if (CameraPlain.getSide(CornersOfTopAABB[i]) == Plane::NEGATIVE_SIDE)
    {

      if (i == 0)// accept the first set of values, no matter how bad it might be.
      {
        MinX = X[i];
        MinY = Y[i];
        MaxX = X[i];
        MaxY = Y[i];
      }
      else// now compare if you get "better" values
      {
        if (MinX > X[i])// get the x minimum
        {
          MinX = X[i];
        }
        if (MinY > Y[i])// get the y minimum
        {
          MinY = Y[i];
        }
        if (MaxX < X[i])// get the x maximum
        {
          MaxX = X[i];
        }
        if (MaxY < Y[i])// get the y maximum
        {
          MaxY = Y[i];
        }
      }
    }
    else
    {
      MinX = 0;
      MinY = 0;
      MaxX = 0;
      MaxY = 0;
      break;
    }
  }
} 

void MovableTextOverlay::_getScreenCoordinates(const Ogre::Vector3& position, Ogre::Real& x, Ogre::Real& y)
{
  const Ogre::Camera* mpCam = mAttrs->getCamera();
  Vector3 hcsPosition = mpCam->getProjectionMatrix() * (mpCam->getViewMatrix() * position);

  x = 1.0f - ((hcsPosition.x * 0.5f) + 0.5f);// 0 <= x <= 1 // left := 0,right := 1
  y = ((hcsPosition.y * 0.5f) + 0.5f);// 0 <= y <= 1 // bottom := 0,top := 1
}

void MovableTextOverlay::enable(bool enable)
{
  if (mEnabled == enable)
    return;

  mEnabled = enable;
  if (mEnabled)
    mpOv->show();
  else
    mpOv->hide();
}

void MovableTextOverlay::update(Real timeSincelastFrame)
{
  static Real timeTillUpdate = 0;

  timeTillUpdate -= timeSincelastFrame;
  if (timeTillUpdate > 0)
    return;
  timeTillUpdate = mUpdateFrequency;

  Ogre::Real min_x, max_x, min_y, max_y;
  _getMinMaxEdgesOfTopAABBIn2D(min_x, min_y, max_x, max_y);

  if ((min_x>0.0) && (max_x<1.0) && (min_y>0.0) && (max_y<1.0))
    mOnScreen = true;
  else
    mOnScreen = false;

  if (mNeedUpdate)
  {
    _computeTextWidth();
    mNeedUpdate = false;
  }

  Real relTextWidth = mTextWidth / Ogre::OverlayManager::getSingleton().getViewportWidth() ;
  Real relTextHeight = mAttrs->getCharacterHeight() / Ogre::OverlayManager::getSingleton().getViewportHeight();
  
  float fFadeStart = m_fFadeDistStart;
  float fFadeEnd = m_fFadeDistEnd;
  float fFadeDist = fFadeEnd - fFadeStart;

  if (fFadeStart != -1 && fFadeEnd != -1)
  {
    const Ogre::Camera* mpCam = mAttrs->getCamera();
    float fDist = mpCam->getPosition().distance(mpMov->getParentSceneNode()->getPosition());

    if (fDist > fFadeEnd)
    {
      mpOv->hide();
    }
    else
    {
      mpOv->show();

      if (fDist > fFadeStart)
      {
        float ratio = (std::max(0.1f, fFadeEnd - fDist)) / fFadeDist;

        //relTextWidth *= ratio;
        //relTextHeight *= ratio;
        
        mpOvText->setMetricsMode(Ogre::GMM_RELATIVE);
        mpOvText->setDimensions(ratio, ratio);
        
        {
          Ogre::ColourValue newColour(mAttrs->mColor);
          newColour.a = ratio;

          mpOvText->setColour(newColour);
        }

        Ogre::MaterialPtr mat = mpOvContainer->getMaterial();
        
        if (!mat.isNull() && mat->getNumTechniques() > 0)
        {
          Ogre::Pass* pPass = mat->getTechnique(0)->getPass(0);

          if (pPass)
          {
            Ogre::TextureUnitState* pTexState = pPass->getTextureUnitState(0);
            
            if (pTexState)
              pTexState->setAlphaOperation(Ogre::LBX_MODULATE, Ogre::LBS_MANUAL, Ogre::LBS_CURRENT, 0.4f * ratio);
          }
        }
      }
      else
      {
        mpOvText->setColour(mAttrs->mColor);
        
        Ogre::MaterialPtr mat = mpOvContainer->getMaterial();
        
        if (!mat.isNull() && mat->getNumTechniques() > 0)
        {
          Ogre::Pass* pPass = mat->getTechnique(0)->getPass(0);

          if (pPass)
          {
            Ogre::TextureUnitState* pTexState = pPass->getTextureUnitState(0);
            
            if (pTexState)
              pTexState->setAlphaOperation(Ogre::LBX_MODULATE, Ogre::LBS_MANUAL, Ogre::LBS_CURRENT, 0.4f);
          }
        }
      }

      mpOvContainer->setPosition(1-(min_x + max_x + relTextWidth)/2, 1-max_y);
      mpOvContainer->setDimensions(relTextWidth, relTextHeight);
    }
  }
  else
  {
    mpOvContainer->setPosition(1-(min_x + max_x + relTextWidth)/2, 1-max_y);
    mpOvContainer->setDimensions(relTextWidth, relTextHeight);
  }
}

void MovableTextOverlay::_updateOverlayAttrs()
{
  const String &newMatName = mAttrs->getMaterialName();
  const String &oldMatName = mpOvContainer->getMaterialName();
  if (oldMatName != newMatName)
  {
    if (oldMatName.length())
      mpOvContainer->getMaterial()->unload();

    if (newMatName.length())
      mpOvContainer->setMaterialName(newMatName);

  }

  mpOvText->setColour(mAttrs->getColor());

  mpOvText->setParameter("font_name", mAttrs->getFontName());
  mpOvText->setParameter("char_height", Ogre::StringConverter::toString(mAttrs->getCharacterHeight()));
  mpOvText->setParameter("horz_align", "left");
  mpOvText->setParameter("vert_align", "top");
}


MovableTextOverlayAttributes::MovableTextOverlayAttributes(const Ogre::String & name, const Ogre::Camera *cam,
                                                           const Ogre::String & fontName, int charHeight, const Ogre::ColourValue & color, const Ogre::String & materialName)
                                                           : mpCam(cam)
                                                           , mpFont(NULL)
                                                           , mName(name)
                                                           , mFontName("")
                                                           , mMaterialName("")
                                                           , mCharHeight(charHeight)
                                                           , mColor(ColourValue::ZERO)
{
  if (fontName.length() == 0)
    Ogre::Exception(Ogre::Exception::ERR_INVALIDPARAMS, "Invalid font name", "MovableTextOverlayAttributes::MovableTextOverlayAttributes");

  setFontName(fontName);
  setMaterialName(materialName);
  setColor(color);
}

MovableTextOverlayAttributes::~MovableTextOverlayAttributes()
{
  setFontName("");
  setMaterialName("");
}

void MovableTextOverlayAttributes::setFontName(const Ogre::String & fontName)
{
  if (mFontName != fontName || !mpFont)
  {
    if (mpFont)
    {
      mpFont->unload();
      mpFont = NULL;
    }

    mFontName = fontName;
    if (mFontName.length())
    {
      mpFont = dynamic_cast<Ogre::Font*>(Ogre::FontManager::getSingleton().getByName(mFontName).getPointer());

      if (!mpFont)
        Ogre::Exception(Ogre::Exception::ERR_ITEM_NOT_FOUND, "Could not find font " + fontName, "MovableTextOverlay::setFontName");
      else
        mpFont->load();
    }
  }
}

void MovableTextOverlayAttributes::setMaterialName(const Ogre::String & materialName)
{
  if (mMaterialName != materialName)
  {
    if (mMaterialName.length())
      Ogre::MaterialManager::getSingletonPtr()->getByName(mMaterialName).getPointer()->unload();

    mMaterialName = materialName;
    if (mMaterialName.length())
    {
      Ogre::Material *mpMaterial = dynamic_cast<Ogre::Material*>(Ogre::MaterialManager::getSingletonPtr()->getByName(mMaterialName).getPointer());
      if (!mpMaterial)
        Ogre::Exception(Ogre::Exception::ERR_ITEM_NOT_FOUND, "Could not find font " + materialName, "MovableTextOverlay::setMaterialName");
      mpMaterial->load();
    }
  }
}

void MovableTextOverlayAttributes::setColor(const Ogre::ColourValue & color)
{
  mColor = color;
}

void MovableTextOverlayAttributes::setCharacterHeight(unsigned int height)
{
  mCharHeight = height;
}