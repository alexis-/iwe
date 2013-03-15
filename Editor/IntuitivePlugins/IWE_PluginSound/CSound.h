#ifndef CSOUND_H
# define CSOUND_H

# include <OgreEntity.h>
# include <OgreSceneNode.h>
# include <OgreManualObject.h>

namespace OgreOggSound
{
  class OgreOggISound;
}

class MovableTextOverlay;

enum e_mementoType
{
  SND_MEM_STATE,
  SND_MEM_CREATE,
  SND_MEM_DELETE,
};

struct s_soundMemento
{
  bool                m_bPlaying;
  bool                m_b3DSound;
  bool                m_bLooping;
  float               m_fLoopingStartTime;
  float               m_fVolume;
  float               m_fMaxVolume;
  float               m_fMinVolume;
  float               m_fMaxDistance;
  float               m_fHalfVolumeDistance;
  float               m_fRollOff;
  float               m_fPitch;
  std::string         m_sSoundName;
  Ogre::Vector3       m_vecPos;
  Ogre::Vector3       m_vecScale;
  Ogre::Quaternion    m_quatOrient;
};

class CSound : public IPropertyHolder, public ISelectableTarget, public CMementoObject
{
public:
  CSound();
  virtual ~CSound();

public:
  virtual bool                            setSound(const std::string& sSound);

public:
// ISelectableTarget interface
  virtual void                            setTargetSelected(bool bSelected);
  virtual bool                            isTargetSelected() const;

  virtual Ogre::Vector3                   getTargetPosition() const;
  virtual Ogre::Vector3                   getTargetScale() const;
  virtual Ogre::Quaternion                getTargetOrientation() const;

  virtual void                            setTargetPosition(const Ogre::Vector3& vecDelta);
  virtual void                            setTargetScale(const Ogre::Vector3& vecScale);
  virtual void                            setTargetOrientation(const Ogre::Quaternion& quatRotate);

  virtual void                            moveTarget(const Ogre::Vector3& vecDelta);
  virtual void                            scaleTarget(const Ogre::Vector3& vecScale);
  virtual void                            rotateTarget(const Ogre::Quaternion& quatRotate);

  virtual void                            deleteTarget();

  virtual const char*                     getType()
  {
    return "CSound";
  }
  
private:
	Ogre::Vector3						                getPosition() const;
	Ogre::Quaternion				                getOrientation() const;
	Ogre::Vector3						                getScale() const;
	
	void										                setPosition(Ogre::Vector3 vecPos);
	void										                setOrientation(Ogre::Quaternion quatOrient);
	void										                setScale(Ogre::Vector3 scaleVec);

  void                                    refreshProps();
  void                                    onRender(float fDelta);

// IMementoObject interface
public:
  virtual void                            undo();
  virtual void                            redo();
  virtual void                            forgetLast();
  virtual void                            forgetFirst();
  
  s_soundMemento                          createState();
  void                                    setSoundState(const s_soundMemento& sound);
  void                                    createSound(const s_soundMemento& sound);
  void                                    deleteSound();
  void                                    initialCreation();
  
private:
  IWE_DEFINE_PROP_HOLDER();

private:
  std::stack<e_mementoType>               m_stackMemUndoType;
  std::stack<s_soundMemento>              m_stackMemUndoState;
  
  std::stack<e_mementoType>               m_stackMemRedoType;
  std::stack<s_soundMemento>              m_stackMemRedoState;

  // Entity SelectableTarget
private:
  bool                                    m_bTargetSelected;
	Ogre::ManualObject*							        m_pSelectBoxObject;

  // Sound stuffs
private:
  CPropVar<bool>                          m_bPlaying;
  CPropVar<bool>                          m_b3DSound;
  CPropVar<bool>                          m_bLooping;
  CPropVar<float>                         m_fLoopingStartTime;
  CPropVar<bool>                          m_bIsMono;
  CPropVar<float>                         m_fVolume;
  CPropVar<float>                         m_fMaxVolume;
  CPropVar<float>                         m_fMinVolume;
  CPropVar<float>                         m_fMaxDistance;
  CPropVar<float>                         m_fHalfVolumeDistance;
  CPropVar<float>                         m_fRollOff;
  CPropVar<float>                         m_fPitch;
  CPropVar<float>                         m_fAudioLength;
  
	CPropVar<std::string>						        m_sSoundName;

  // Entity Render
private:
	Ogre::SceneNode*								        m_pNode;
	Ogre::Entity*										        m_pOgreEnt;
  OgreOggSound::OgreOggISound*            m_pSound;

  MovableTextOverlay*                     m_pTextOverlay;
};

#endif // !CSOUND_H