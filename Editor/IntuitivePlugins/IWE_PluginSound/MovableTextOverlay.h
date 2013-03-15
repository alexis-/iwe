#ifndef __MovableTextOverlay_H__
#define __MovableTextOverlay_H__

#include "Ogre.h"
#include "OgreFont.h"
#include "OgreFontManager.h"

using namespace Ogre;

class MovableTextOverlayAttributes
{
public:
  MovableTextOverlayAttributes(const Ogre::String & name, const Ogre::Camera *cam,
    const Ogre::String & fontName = "BlueHighway", int charHeight = 16, const Ogre::ColourValue & color = Ogre::ColourValue::White, const Ogre::String & materialName = "");
  ~MovableTextOverlayAttributes();

  void setFontName(const Ogre::String & fontName);
  void setMaterialName(const Ogre::String & materialName);
  void setColor(const Ogre::ColourValue & color);
  void setCharacterHeight(unsigned int height);

  const Ogre::String& getName() const {return mName;}
  const Ogre::Camera* getCamera() const {return mpCam;}
  const Ogre::Font* getFont() const {return mpFont;}
  const Ogre::String& getFontName() const {return mFontName;}
  const Ogre::String& getMaterialName() const {return mMaterialName;}
  const Ogre::ColourValue& getColor() const {return mColor;}
  const Ogre::Real getCharacterHeight() const {return mCharHeight;}

  const Ogre::String mName;
  const Ogre::Camera *mpCam;

  Ogre::Font* mpFont;
  Ogre::String mFontName;
  Ogre::String mMaterialName;
  Ogre::ColourValue mColor;
  Ogre::Real mCharHeight;
};

class MovableTextOverlay
{
public:
  MovableTextOverlay(const Ogre::String & name, const Ogre::String & caption,
    const Ogre::MovableObject *mov, MovableTextOverlayAttributes *attrs,
    float fFadeDistStart = -1, float fFadeDistEnd = -1);

  virtual ~MovableTextOverlay();

  void setCaption(const Ogre::String & caption);
  void setUpdateFrequency(Ogre::Real updateFrequency) {mUpdateFrequency = updateFrequency;}
  void setAttributes(MovableTextOverlayAttributes *attrs)
  {
    mAttrs = attrs;
    _updateOverlayAttrs();
  }

  const Ogre::String&	getName() const {return mName;}
  const Ogre::String&	getCaption() const {return mCaption;}
  const Ogre::Real getUpdateFrequency() const {return mUpdateFrequency;}
  const bool isOnScreen() const {return mOnScreen;}
  const bool isEnabled() const {return mEnabled;}
  const MovableTextOverlayAttributes* getAttributes() const {return mAttrs;}

  void enable(bool enable);
  void update(Ogre::Real timeSincelastFrame);

  // Needed for RectLayoutManager.
  int getPixelsTop() { return (int)Ogre::OverlayManager::getSingleton().getViewportHeight() * (int)(mpOvContainer->getTop());}
  int getPixelsBottom() { return (int)Ogre::OverlayManager::getSingleton().getViewportHeight() * (int)(mpOvContainer->getTop() + mpOvContainer->getHeight());}
  int getPixelsLeft() { return (int)Ogre::OverlayManager::getSingleton().getViewportWidth() * (int)mpOvContainer->getLeft();}
  int getPixelsRight() { return (int)Ogre::OverlayManager::getSingleton().getViewportWidth() * (int)(mpOvContainer->getLeft() + mpOvContainer->getWidth());}

  void setPixelsTop(int px) {mpOvContainer->setTop((Ogre::Real)px / Ogre::OverlayManager::getSingleton().getViewportHeight());}
  // end

protected:
  void _computeTextWidth();
  void _updateOverlayAttrs();
  void _getMinMaxEdgesOfTopAABBIn2D(Ogre::Real& MinX, Ogre::Real& MinY, Ogre::Real& MaxX, Ogre::Real& MaxY);
  void _getScreenCoordinates(const Ogre::Vector3& position, Ogre::Real& x, Ogre::Real& y);

  const Ogre::String mName;
  const Ogre::MovableObject* mpMov;

  Ogre::Overlay* mpOv;
  Ogre::OverlayContainer* mpOvContainer;
  Ogre::OverlayElement* mpOvText;

  float m_fFadeDistStart;
  float m_fFadeDistEnd;

  // true if mpOvContainer is visible, false otherwise
  bool mEnabled;

  // true if mTextWidth needs to be recalculated
  bool mNeedUpdate;

  // Text width in pixels
  Ogre::Real mTextWidth;

  // the Text
  Ogre::String mCaption;

  // true if the upper vertices projections of the -MovableObject are on screen
  bool mOnScreen;

  // the update frequency in seconds
  // mpOvContainer coordinates get updated each mUpdateFrequency seconds.
  Ogre::Real mUpdateFrequency;

  // the Font/Material/Color text attributes
  MovableTextOverlayAttributes *mAttrs;
};

#endif /* __MovableTextOverlay_H__ */