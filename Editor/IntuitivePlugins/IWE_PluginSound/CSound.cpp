#include "IWEPlugin.h"

#include <OgreSceneManager.h>
#include <OgreOggISound.h>

#include "OgreAL.h"
#include "CSound.h"

#include "MovableTextOverlay.h"
#include "RenderHelper/EngineUtils.h"


IWE_IMPLEMENT_PROP_BEGIN(CSound)
  IWE_PROP_LEVEL_BEGIN("Sound settings");
    IWE_PROP_BOOL_S("Play", "Define if the audio track is playing.", m_bPlaying,
      [pInst](bool b) { pInst->m_stackMemUndoState.push(pInst->createState()); pInst->m_stackMemUndoType.push(SND_MEM_STATE); pInst->saveState();
        pInst->m_bPlaying.Get() = b; if (b) pInst->m_pSound->play(); else pInst->m_pSound->stop(); }, false);
    IWE_PROP_BOOL_S("3D Sound", "Switch's spatialisation on/off for mono sounds, no-effect for stereo sounds.", m_b3DSound,
      [pInst](bool b) { pInst->m_stackMemUndoState.push(pInst->createState()); pInst->m_stackMemUndoType.push(SND_MEM_STATE); pInst->saveState();
        pInst->m_b3DSound.Get() = b; pInst->m_pSound->disable3D(!b);
        if (!b) { pInst->m_pSound->setPosition(pInst->getPosition()); } pInst->refreshProps(); }, false);
    IWE_PROP_BOOL_S("Loop", "Sets looping status.", m_bLooping,
      [pInst](bool b) { pInst->m_stackMemUndoState.push(pInst->createState()); pInst->m_stackMemUndoType.push(SND_MEM_STATE); pInst->saveState();
        pInst->m_bLooping.Get() = b; pInst->m_pSound->loop(b); }, false);
    IWE_PROP_FLOAT_S("Loop Start", "Allows user to define any start point for a loopable sound, by default this would be 0", m_fLoopingStartTime,
      [pInst](float f) { pInst->m_stackMemUndoState.push(pInst->createState()); pInst->m_stackMemUndoType.push(SND_MEM_STATE); pInst->saveState();
        pInst->m_fLoopingStartTime.Get() = f; pInst->m_pSound->setLoopOffset(f); }, false);
    IWE_PROP_FLOAT_S("Max Distance", "This value sets the maximum distance at which attenuation is stopped. Beyond this distance the volume remains constant.", m_fMaxDistance,
      [pInst](float f) { pInst->m_stackMemUndoState.push(pInst->createState()); pInst->m_stackMemUndoType.push(SND_MEM_STATE); pInst->saveState();
        pInst->m_fMaxDistance.Get() = f; pInst->m_pSound->setMaxDistance(f); pInst->refreshProps(); }, false);
    IWE_PROP_FLOAT_S("Half-Volume", "This value sets the half-volume distance. The distance at which the volume would reduce by half.", m_fHalfVolumeDistance,
      [pInst](float f) { pInst->m_stackMemUndoState.push(pInst->createState()); pInst->m_stackMemUndoType.push(SND_MEM_STATE); pInst->saveState();
        pInst->m_fHalfVolumeDistance.Get() = f; pInst->m_pSound->setReferenceDistance(f); pInst->refreshProps(); }, false);
    IWE_PROP_FLOAT_S("Roll-Off", "This value sets the rolloff factor applied to the attenuation of the volume over distance. Effectively scales the volume change affect.", m_fRollOff,
      [pInst](float f) { pInst->m_stackMemUndoState.push(pInst->createState()); pInst->m_stackMemUndoType.push(SND_MEM_STATE); pInst->saveState();
        pInst->m_fRollOff.Get() = f; pInst->m_pSound->setRolloffFactor(f); pInst->refreshProps(); }, false);
    IWE_PROP_FLOAT_S("Pitch", "Sounds pitch", m_fPitch,
      [pInst](float f) { pInst->m_stackMemUndoState.push(pInst->createState()); pInst->m_stackMemUndoType.push(SND_MEM_STATE); pInst->saveState();
        pInst->m_fPitch.Get() = f; pInst->m_pSound->setPitch(f); pInst->refreshProps(); }, false);
  IWE_PROP_LEVEL_END();
  IWE_PROP_LEVEL_BEGIN("Sound volume");
    IWE_PROP_FLOAT_S("Volume", "Sounds volume", m_fVolume,
      [pInst](float f) { pInst->m_stackMemUndoState.push(pInst->createState()); pInst->m_stackMemUndoType.push(SND_MEM_STATE); pInst->saveState();
        pInst->m_fVolume.Get() = f; pInst->m_pSound->setVolume(f); }, false);
    IWE_PROP_FLOAT_BOUND_S("Max Volume", "Sets sounds maximum attenuation volume", m_fMaxVolume,
      [pInst](float f) { pInst->m_stackMemUndoState.push(pInst->createState()); pInst->m_stackMemUndoType.push(SND_MEM_STATE); pInst->saveState();
        pInst->m_fMaxVolume.Get() = f; pInst->m_pSound->setMaxVolume(f); pInst->refreshProps(); }, false, 0.0f, 1.0f);
    IWE_PROP_FLOAT_BOUND_S("Min Volume", "Sets sounds minimum attenuation volume", m_fMinVolume,
      [pInst](float f) { pInst->m_stackMemUndoState.push(pInst->createState()); pInst->m_stackMemUndoType.push(SND_MEM_STATE); pInst->saveState();
        pInst->m_fMinVolume.Get() = f; pInst->m_pSound->setMinVolume(f); pInst->refreshProps(); }, false, 0.0f, 1.0f);
  IWE_PROP_LEVEL_END();
  IWE_PROP_LEVEL_BEGIN("Sound infos");
    IWE_PROP_BOOL_GS("Mono", "Is this sound mono ?", m_bIsMono,
      [pInst]() -> bool { return pInst->m_pSound->isMono(); }, [pInst](bool b) { pInst->m_bIsMono = true; }, true);
    IWE_PROP_FLOAT_GS("Track Length", "Gets the length of the audio file in seconds", m_fAudioLength,
      [pInst]() -> float { return pInst->m_pSound->getAudioLength(); }, [pInst](float f) {}, true);
  IWE_PROP_LEVEL_END();
IWE_IMPLEMENT_PROP_END()


CSound::CSound()
  : CMementoObject(GetEditorInteraction()),
    m_pSound(NULL),
    m_bTargetSelected(false),
    m_pSelectBoxObject(NULL),
    m_pTextOverlay(NULL),
    m_pNode(NULL),
    m_pOgreEnt(NULL)
{
}

CSound::~CSound()
{
	Ogre::SceneManager* pSceneMgr = GetRenderEngine()->getSceneManager();

  if (m_pNode)
  {
    if (m_pSound)
      m_pNode->detachObject(m_pSound);
    
    if (m_pSelectBoxObject)
      m_pNode->detachObject(m_pSelectBoxObject);

		RenderHelper::DestroyAllAttachedMovableObjects(m_pNode);
		pSceneMgr->destroySceneNode(m_pNode);
  }

  if (m_pTextOverlay)
    delete m_pTextOverlay;

  if (m_pSelectBoxObject)
		pSceneMgr->destroyManualObject(m_pSelectBoxObject);

  if (m_pSound)
  {
    GetRenderEngine()->eventRender -=
      FastDelegate1<float>(this, &CSound::onRender);

    COgreAL::getInstance()->destroySound(m_pSound);
  }

  m_pNode = NULL;
  m_pOgreEnt = NULL;
  m_pSound = NULL;
  m_pTextOverlay = NULL;
  m_pSelectBoxObject = NULL;
  m_bTargetSelected = false;
}

void
CSound::refreshProps()
{
  m_bPlaying = m_pSound->isPlaying();
  m_bLooping.Update();
  m_b3DSound.Update();
  m_fVolume = m_pSound->getVolume();
  m_fMaxVolume.Update();
  m_fMinVolume.Update();
  m_fMaxDistance = m_pSound->getMaxDistance();
  m_fHalfVolumeDistance = m_pSound->getReferenceDistance();
  m_fRollOff = m_pSound->getRolloffFactor();
  m_fPitch = m_pSound->getPitch();
}

// TODO: Move all the getUniqueName to an Util lib
static const char*
getUniqueName(const std::string& name)
{
	static std::map<std::string, unsigned long>		nameMap;
	static char																		pNameRet[2048];
	std::string																		sName(name);
	unsigned long																	id = 0;

	if (nameMap.find(sName) != nameMap.end())
	{
		id = ++nameMap[sName];
		nameMap[sName] = id;
	}
	else
		nameMap[sName] = id;

	sName += "." + Ogre::StringConverter::toString(id);
	strncpy(pNameRet, sName.c_str(), 2048);

	return pNameRet;
}

bool
CSound::setSound(const std::string& sSound)
{
  try
  {
	  Ogre::SceneManager* pSceneMgr = GetRenderEngine()->getSceneManager();

    // Create Sound
    {
      if (m_pSound)
      {
        m_pNode->detachObject(m_pSound);
        COgreAL::getInstance()->destroySound(m_pSound);
        
        m_pSound = NULL;
      }

      m_pSound = COgreAL::getInstance()->create(sSound, true, true, false);
    }
  
    // Create visible mesh
    {
		  m_pOgreEnt = pSceneMgr->createEntity("megaphone.mesh");

		  if (!m_pOgreEnt)
		  	printf("ERROR: CSound::CSound couldn't find model");

		  m_pOgreEnt->getUserObjectBindings().setUserAny(Ogre::Any((ISelectableTarget*)this));
      m_pOgreEnt->setQueryFlags(1);
    }
    
    // Create node
    {
      if (!m_pNode)
	      m_pNode = pSceneMgr->getRootSceneNode()->createChildSceneNode();

	    m_pNode->attachObject(m_pOgreEnt);
    }

    // Set default sound settings
    {
      m_pSound->disable3D(true);
		  m_pSound->setMaxDistance(500.0f);
		  m_pSound->setReferenceDistance(400.0f);
      m_pSound->setRolloffFactor(2.0f);

      m_pNode->attachObject(m_pSound);
      
      m_pSound->play();
    }
    
    // Create text overlay
    {
      Ogre::Camera* pCamera = (*GetRenderEngine()->getSceneManager()->getCameras().begin()).second;

      MovableTextOverlayAttributes *attrs = new MovableTextOverlayAttributes("CSoundTextOverlayAttrs", pCamera, "CSoundFont", 16, ColourValue::White, "RedTransparent");
      m_pTextOverlay = new MovableTextOverlay(getUniqueName("CSoundOverlay"), " " + sSound + " ", m_pOgreEnt, attrs, 150, 225);
		  m_pTextOverlay->enable(true); // make it invisible for now
		  m_pTextOverlay->setUpdateFrequency(0.04);// set update frequency to 0.01 seconds
    }
  
    // Set default parameters
    {
      m_bPlaying = true;
      m_b3DSound = false;
      m_bLooping = true;
      m_fLoopingStartTime = m_pSound->getLoopOffset();
      m_fVolume = m_pSound->getVolume();
      m_fMaxVolume = 1.0f;
      m_fMinVolume = 0.0f;
      m_fMaxDistance = 500.0f; //m_pSound->getMaxDistance();
      m_fHalfVolumeDistance = 400.0f; //m_pSound->getReferenceDistance();
      m_fRollOff = 2.0f; //m_pSound->getRolloffFactor();
      m_fPitch = 1.0f; //m_pSound->getPitch();
      m_bIsMono = m_pSound->isMono();
      m_fAudioLength = m_pSound->getAudioLength();

      m_sSoundName = sSound;
    }

    // Hook up the render event. This is used to update text overlay.
    GetRenderEngine()->eventRender +=
      FastDelegate1<float>(this, &CSound::onRender);

    return true;
  }
  catch (std::exception ex)
  {
    printf(ex.what());
    m_pSound = NULL;
  }
  
  return false;
}

void
CSound::onRender(float fDelta)
{
  if (m_pNode && m_pSound && m_pTextOverlay)
    m_pTextOverlay->update(fDelta);
}

Ogre::Vector3
CSound::getPosition() const
{
	if (m_pNode)
		return m_pNode->getPosition();

	printf("CSound::getPosition() called but m_pNode is NULL"); // TODO: Log this

	return Ogre::Vector3::ZERO;
}

Ogre::Quaternion
CSound::getOrientation() const
{
	if (m_pNode)
		return m_pNode->getOrientation();

	printf("CSound::getOrientation() called but m_pNode is NULL");

	return Ogre::Quaternion::ZERO;
}

Ogre::Vector3
CSound::getScale() const
{
	if (m_pNode)
		return m_pNode->getScale();

	printf("CSound::getScale() called but m_pNode is NULL");

	return Ogre::Vector3::ZERO;
}

void
CSound::setPosition(Ogre::Vector3 vecPos)
{
	if (m_pNode)
  {
		m_pNode->setPosition(vecPos);

    if (!m_b3DSound)
      m_pSound->setPosition(vecPos);
    else
      m_pSound->setPosition(Ogre::Vector3::ZERO);
  }
	else
		printf("CSound::setPosition() called but m_pNode is NULL");
}

void
CSound::setOrientation(Ogre::Quaternion quatOrient)
{
	if (m_pNode)
  {
		m_pNode->setOrientation(quatOrient);

    //if (!m_b3DSound)
    //  m_pSound->setDirection(m_pNode->getDirection());
  }
	else
		printf("CSound::setPosition() called but m_pNode is NULL");
}

void
CSound::setScale(Ogre::Vector3 scaleVec)
{
	if (m_pNode)
  {
		m_pNode->setScale(scaleVec);
  }
	else
		printf("CSound::setScale() called but m_pNode is NULL");
}

void
CSound::setTargetSelected(bool bSelected)
{
	Ogre::SceneManager* pSceneMgr = GetRenderEngine()->getSceneManager();

	if (m_bTargetSelected == bSelected)
		return;

  m_bTargetSelected = bSelected;

	if (bSelected)
	{
    if (m_pSelectBoxObject)
    {
      if (m_pNode)
        m_pNode->detachObject(m_pSelectBoxObject);

		  pSceneMgr->destroyManualObject(m_pSelectBoxObject);
      m_pSelectBoxObject = NULL;
    }
    
		Ogre::AxisAlignedBox bbox = (m_pOgreEnt) ? m_pOgreEnt->getBoundingBox() : Ogre::AxisAlignedBox::BOX_NULL;
		Ogre::Vector3 boxSize = bbox.getSize();
		Ogre::Vector3 adjustSize = (Ogre::Vector3(1.0f, 0.0f, 1.0f) * bbox.getSize()) / 2;

		m_pSelectBoxObject = pSceneMgr->createManualObject(); //getUniqueName("selectBox"));

		m_pSelectBoxObject->setVisibilityFlags(1);
		m_pSelectBoxObject->begin("Editor/Grid", Ogre::RenderOperation::OT_LINE_LIST);

		/////////////////////////////////////////////////////////////////////

		m_pSelectBoxObject->position(Ogre::Vector3::ZERO - adjustSize);
		m_pSelectBoxObject->position(Ogre::Vector3(boxSize.x, 0.0f, 0.0f) - adjustSize);
		
		m_pSelectBoxObject->position(Ogre::Vector3::ZERO - adjustSize);
		m_pSelectBoxObject->position(Ogre::Vector3(0.0f, boxSize.y, 0.0f) - adjustSize);

		m_pSelectBoxObject->position(Ogre::Vector3::ZERO - adjustSize);
		m_pSelectBoxObject->position(Ogre::Vector3(0.0f, 0.0f, boxSize.z) - adjustSize);

		/////////////////////////////////////////////////////////////////////

		m_pSelectBoxObject->position(Ogre::Vector3(boxSize.x, 0.0f, boxSize.z) - adjustSize);
		m_pSelectBoxObject->position(Ogre::Vector3(0.0f, 0.0f, boxSize.z) - adjustSize);
		
		m_pSelectBoxObject->position(Ogre::Vector3(boxSize.x, 0.0f, boxSize.z) - adjustSize);
		m_pSelectBoxObject->position(Ogre::Vector3(boxSize.x, boxSize.y, boxSize.z) - adjustSize);

		m_pSelectBoxObject->position(Ogre::Vector3(boxSize.x, 0.0f, boxSize.z) - adjustSize);
		m_pSelectBoxObject->position(Ogre::Vector3(boxSize.x, 0.0f, 0.0f) - adjustSize);

		/////////////////////////////////////////////////////////////////////

		m_pSelectBoxObject->position(Ogre::Vector3(0.0f, boxSize.y, 0.0f) - adjustSize);
		m_pSelectBoxObject->position(Ogre::Vector3(0.0f, boxSize.y, boxSize.z) - adjustSize);

		m_pSelectBoxObject->position(Ogre::Vector3(boxSize.x, boxSize.y, 0.0f) - adjustSize);
		m_pSelectBoxObject->position(Ogre::Vector3(boxSize.x, 0.0f, 0.0f) - adjustSize);

		m_pSelectBoxObject->position(Ogre::Vector3(0.0f, boxSize.y, 0.0f) - adjustSize);
		m_pSelectBoxObject->position(Ogre::Vector3(boxSize.x, boxSize.y, 0.0f) - adjustSize);

		/////////////////////////////////////////////////////////////////////

		m_pSelectBoxObject->position(Ogre::Vector3(boxSize.x, boxSize.y, boxSize.z) - adjustSize);
		m_pSelectBoxObject->position(Ogre::Vector3(0.0f, boxSize.y, boxSize.z) - adjustSize);

		m_pSelectBoxObject->position(Ogre::Vector3(0.0f, boxSize.y, boxSize.z) - adjustSize);
		m_pSelectBoxObject->position(Ogre::Vector3(0.0f, 0.0f, boxSize.z) - adjustSize);

		m_pSelectBoxObject->position(Ogre::Vector3(boxSize.x, boxSize.y, boxSize.z) - adjustSize);
		m_pSelectBoxObject->position(Ogre::Vector3(boxSize.x, boxSize.y, 0.0f) - adjustSize);

		/////////////////////////////////////////////////////////////////////

		m_pSelectBoxObject->end();
		m_pNode->attachObject(m_pSelectBoxObject);

    eventTargetSelected(this);
	}
	else if (m_pSelectBoxObject)
  {
    if (m_pNode)
      m_pNode->detachObject(m_pSelectBoxObject);

		pSceneMgr->destroyManualObject(m_pSelectBoxObject);
    m_pSelectBoxObject = NULL;

    eventTargetUnselected(this);
  }
}

bool
CSound::isTargetSelected() const
{
  return m_bTargetSelected;
}

Ogre::Vector3
CSound::getTargetPosition() const
{
  return getPosition();
}

Ogre::Vector3
CSound::getTargetScale() const
{
  return getScale();
}

Ogre::Quaternion
CSound::getTargetOrientation() const
{
  return getOrientation();
}

void
CSound::setTargetPosition(const Ogre::Vector3& vecPos)
{
  setPosition(vecPos);
}

void
CSound::setTargetScale(const Ogre::Vector3& vecScale)
{
  setScale(vecScale);
}

void
CSound::setTargetOrientation(const Ogre::Quaternion& quatRotate)
{
  setOrientation(quatRotate);
}

void
CSound::moveTarget(const Ogre::Vector3& vecDelta)
{
  setPosition(getPosition() + vecDelta);
  eventTargetMoved(this);
}

void
CSound::scaleTarget(const Ogre::Vector3& vecScale)
{
  setScale(getScale() * vecScale);
  eventTargetScaled(this);
}

void
CSound::rotateTarget(const Ogre::Quaternion& quatRotate)
{
  setOrientation(getOrientation() * quatRotate);
  eventTargetRotated(this);
}

void
CSound::deleteTarget()
{
  s_soundMemento soundState = createState();

  m_stackMemUndoType.push(SND_MEM_DELETE);
  m_stackMemUndoState.push(soundState);

  saveState();

  deleteSound();

  //eventTargetDeleted(this);
  //delete this;
}

void
CSound::setSoundState(const s_soundMemento& sound)
{
  if (m_pSound)
  {
    if (!sound.m_bPlaying) // && m_pSound->isPlaying())
      m_pSound->stop();
    else if (sound.m_bPlaying) // && !m_pSound->isPlaying())
      m_pSound->play();

    m_pSound->disable3D(!sound.m_b3DSound);
    m_pSound->setMaxDistance(sound.m_fMaxDistance);
    m_pSound->setReferenceDistance(sound.m_fHalfVolumeDistance);
    m_pSound->setRolloffFactor(sound.m_fRollOff);
    m_pSound->setPitch(sound.m_fPitch);
    m_pSound->loop(sound.m_bLooping);
    m_pSound->setLoopOffset(sound.m_fLoopingStartTime);
    m_pSound->setMaxVolume(sound.m_fMaxVolume);
    m_pSound->setMinVolume(sound.m_fMinVolume);
    m_pSound->setVolume(sound.m_fVolume);
    
    m_bPlaying = sound.m_bPlaying;
    m_b3DSound = sound.m_b3DSound;
    m_bLooping = sound.m_bLooping;
    m_fMaxDistance = sound.m_fMaxDistance;
    m_fHalfVolumeDistance = sound.m_fHalfVolumeDistance;
    m_fRollOff = sound.m_fRollOff;
    m_fPitch = sound.m_fPitch;
    m_fLoopingStartTime = sound.m_fLoopingStartTime;
    m_fMaxVolume = sound.m_fMaxVolume;
    m_fMinVolume = sound.m_fMinVolume;
  }
}

void
CSound::createSound(const s_soundMemento& sound)
{
  setSound(sound.m_sSoundName);
  setSoundState(sound);
}

void
CSound::deleteSound()
{
	Ogre::SceneManager* pSceneMgr = GetRenderEngine()->getSceneManager();

  eventTargetDeleted(this);

  if (m_pNode)
  {
    if (m_pSound)
      m_pNode->detachObject(m_pSound);
    
    if (m_pSelectBoxObject)
      m_pNode->detachObject(m_pSelectBoxObject);

		RenderHelper::DestroyAllAttachedMovableObjects(m_pNode);
		pSceneMgr->destroySceneNode(m_pNode);
  }

  if (m_pTextOverlay)
    delete m_pTextOverlay;

  if (m_pSelectBoxObject)
		pSceneMgr->destroyManualObject(m_pSelectBoxObject);

  if (m_pSound)
  {
    GetRenderEngine()->eventRender -=
      FastDelegate1<float>(this, &CSound::onRender);

    COgreAL::getInstance()->destroySound(m_pSound);
  }

  m_pNode = NULL;
  m_pOgreEnt = NULL;
  m_pSound = NULL;
  m_pTextOverlay = NULL;
  m_pSelectBoxObject = NULL;
  m_bTargetSelected = false;
}

void
CSound::initialCreation()
{
  s_soundMemento soundState = createState();

  m_stackMemUndoType.push(SND_MEM_CREATE);
  m_stackMemUndoState.push(soundState);

  saveState();
}

s_soundMemento
CSound::createState()
{
  s_soundMemento soundState = { m_bPlaying, m_b3DSound, m_bLooping, m_fLoopingStartTime,
    m_fVolume, m_fMaxVolume, m_fMinVolume, m_fMaxDistance, m_fHalfVolumeDistance,
    m_fRollOff, m_fPitch, m_sSoundName, getPosition(), getScale(), getOrientation() };

  return soundState;
}

void
CSound::undo()
{
  switch (m_stackMemUndoType.top())
  {
    case SND_MEM_STATE:
    {
      s_soundMemento state = m_stackMemUndoState.top();

      m_stackMemUndoState.pop();
      m_stackMemRedoType.push(m_stackMemUndoType.top());
      m_stackMemRedoState.push(createState());
    
      setSoundState(state);

      break;
    }
    case SND_MEM_CREATE:
    {
      s_soundMemento state = m_stackMemUndoState.top();

      m_stackMemUndoState.pop();
      m_stackMemRedoType.push(m_stackMemUndoType.top());
      m_stackMemRedoState.push(state);
    
      deleteSound();

      break;
    }
    case SND_MEM_DELETE:
    {
      s_soundMemento state = m_stackMemUndoState.top();

      m_stackMemUndoState.pop();
      m_stackMemRedoType.push(m_stackMemUndoType.top());
      m_stackMemRedoState.push(state);
    
      createSound(state);
      
      setPosition(state.m_vecPos);
      setOrientation(state.m_quatOrient);
      setScale(state.m_vecScale);

      break;
    }
  }

  m_stackMemUndoType.pop();
}

void
CSound::redo()
{
  switch (m_stackMemRedoType.top())
  {
    case SND_MEM_STATE:
    {
      s_soundMemento state = m_stackMemRedoState.top();

      m_stackMemRedoState.pop();
      m_stackMemUndoType.push(m_stackMemRedoType.top());
      m_stackMemUndoState.push(createState());
    
      setSoundState(state);

      break;
    }
    case SND_MEM_CREATE:
    {
      s_soundMemento state = m_stackMemRedoState.top();

      m_stackMemRedoState.pop();
      m_stackMemUndoType.push(m_stackMemRedoType.top());
      m_stackMemUndoState.push(state);
    
      createSound(state);
      
      setPosition(state.m_vecPos);
      setOrientation(state.m_quatOrient);
      setScale(state.m_vecScale);

      break;
    }
    case SND_MEM_DELETE:
    {
      s_soundMemento state = m_stackMemRedoState.top();

      m_stackMemRedoState.pop();
      m_stackMemUndoType.push(m_stackMemRedoType.top());
      m_stackMemUndoState.push(state);
    
      deleteSound();

      break;
    }
  }

  m_stackMemRedoType.pop();
}

void
CSound::forgetLast()
{
}

void
CSound::forgetFirst()
{
  m_stackMemRedoState.pop();
  m_stackMemRedoType.pop();
}