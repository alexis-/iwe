#ifndef OGRE_AL_H
# define OGRE_AL_H

namespace OgreOggSound
{
  class OgreOggSoundManager;
  class OgreOggISound;
}

class COgreAL
{
public:
  static COgreAL*                         getInstance();
  ~COgreAL();

public:
  void                                    init();
  OgreOggSound::OgreOggISound*            create(const std::string& sSoundFile, bool bStream, bool bLoop, bool bPreBuffer);
  OgreOggSound::OgreOggISound*            create(const std::string& sName, const std::string& sSoundFile, bool bStream, bool bLoop, bool bPreBuffer);
  void                                    destroySound(OgreOggSound::OgreOggISound* pOgreSound);

  void                                    update(float fDelta);

private:
  void                                    onCameraMoved(ICameraController* pCameraController);
  void                                    onCameraRotated(ICameraController* pCameraController);

private:
  OgreOggSound::OgreOggSoundManager*      m_pOgreSound;
  Ogre::SceneNode*                        m_pNodeListener;
  Ogre::SceneNode*                        m_pNodeCam;

private:
  static COgreAL*                         m_pInst;
  COgreAL();
};

#endif // !OGRE_AL_H