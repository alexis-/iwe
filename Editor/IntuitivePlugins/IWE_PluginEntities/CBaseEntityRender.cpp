#include "IWEPlugin.h"

#include "CBaseEntity.h"

#include "RenderHelper/EngineUtils.h"


// IBaseEntityRender interface
# define ANIM_FADE_SPEED	7.5f

void
CBaseEntity::renderCleanup()
{
	Ogre::SceneManager* pSceneMgr = GetRenderEngine()->getSceneManager();

  if (m_pSelectBoxObject)
  {
    if (m_pNode)
      m_pNode->detachObject(m_pSelectBoxObject);

		pSceneMgr->destroyManualObject(m_pSelectBoxObject);
  }

	if (m_pOgreEnt)
	{
		RenderHelper::DestroyAllAttachedMovableObjects(m_pNode);
		pSceneMgr->destroySceneNode(m_pNode);
	}

  m_vecAnimInfo.clear();

  m_pOgreEnt = NULL;
  m_pNode = NULL;
  m_pSelectBoxObject = NULL;
  m_bTargetSelected = false;
}

void
CBaseEntity::renderFrame(float deltaTime)
{
	unsigned int animCount = m_vecAnimInfo.size();

	for (unsigned int i = 0; i < animCount; i++)
	{
		s_AnimInfos info = m_vecAnimInfo[i];

		if (info.m_pAnimState->getEnabled())
		{
			Ogre::Real animTime = deltaTime * info.m_fAnimSpeed;

			info.m_pAnimState->addTime(animTime);
		}

		if (info.m_bFadingIn)
		{
			// slowly fade this animation in until it has full weight
			Ogre::Real newWeight = info.m_pAnimState->getWeight() + deltaTime * ANIM_FADE_SPEED;
			info.m_pAnimState->setWeight(Ogre::Math::Clamp<Ogre::Real>(newWeight, 0, 1));

			if (newWeight >= 1)
				info.m_bFadingIn = false;
		}
		else if (info.m_bFadingOut)
		{
			// slowly fade this animation out until it has no weight, and then disable it
			Ogre::Real newWeight = info.m_pAnimState->getWeight() - deltaTime * ANIM_FADE_SPEED;
			info.m_pAnimState->setWeight(Ogre::Math::Clamp<Ogre::Real>(newWeight, 0, 1));
			if (newWeight <= 0)
			{
				info.m_pAnimState->setEnabled(false);
				info.m_bFadingOut = false;
			}
		}
	}
}

void
CBaseEntity::attachObject(Ogre::MovableObject* pObj)
{
	if (m_pNode)
		m_pNode->attachObject(pObj);
	else
		printf("CBaseEntity::attachObject called but m_pNode is NULL"); // Log this
}

bool
CBaseEntity::setVisible(bool					display)
{
	if (m_pNode)
		m_pNode->setVisible(display, true);
	else
		printf("CBaseEntity::setVisible called but m_pNode is NULL"); // Log this

	return true;
}

bool
CBaseEntity::setVisibilityFlag(int		iFlag)
{
	if (m_pOgreEnt)
	{
		m_pOgreEnt->setVisibilityFlags(iFlag);
		m_pOgreEnt->setQueryFlags(iFlag);

		return true;
	}

	printf("CBaseEntity::setVisibilityMask called but m_pOgreEnt is NULL"); // Log this
	return false;
}

const std::string&
CBaseEntity::getModelName() const
{
	return m_pModelName;
}

bool
CBaseEntity::setModel(const std::string&							modelName,
                      bool                            bUpdateProp)
{
	Ogre::SceneManager* sceneMgr = GetRenderEngine()->getSceneManager();

	if (m_pOgreEnt)
	{
		m_pNode->detachObject(m_pOgreEnt);
		sceneMgr->destroyEntity(m_pOgreEnt);

    m_pOgreEnt = NULL;
	}

	try
	{
		std::ostringstream os;
		os << modelName;
		os << ".";
		os << m_nEntID;

		m_pOgreEnt = sceneMgr->createEntity(os.str(), modelName);

		if (!m_pOgreEnt)
			return false;

    m_pOgreEnt->getUserObjectBindings().setUserAny(Ogre::Any((ISelectableTarget*)this));
	}
	catch (Ogre::Exception e)
	{
		printf(e.getFullDescription().c_str()); // TODO: Log this somewhere
		return false;
	}
	catch (...)
	{
		printf("Caught an unknown exception in CBaseEntity::setModel");
		return false;
	}

	if (!m_pNode)
		m_pNode = sceneMgr->getRootSceneNode()->createChildSceneNode();
  
  m_vecPosition = m_pNode->getPosition();
  m_vecScale = m_pNode->getScale();
  m_quatOrientation = m_pNode->getOrientation();
  
  if (bUpdateProp)
	  m_pModelName = modelName;
  else
    m_pModelName.Get() = modelName;

	loadAnimations();

	m_pNode->attachObject(m_pOgreEnt);
  //m_pOgreEnt->setQueryFlags(2048);
	setVisibilityFlag(1);

	return true;
}

bool
CBaseEntity::setModel(const std::string&		modelName)
{
  return setModel(modelName, true);
}

void
CBaseEntity::loadAnimations()
{
	Ogre::AnimationStateSet*						stateSet = m_pOgreEnt->getAllAnimationStates();

	if (!stateSet)
		return;

	Ogre::AnimationStateIterator				stateSetIt = stateSet->getAnimationStateIterator();

	for (uint i = 0; stateSetIt.hasMoreElements(); i++)
	{
		Ogre::AnimationState*							anim = stateSetIt.getNext();

		if (anim)
		{
			s_AnimInfos info = { anim, anim->getAnimationName(), 1.0f, false, false };
			m_vecAnimInfo.push_back(info);

			anim->setLoop(true);

      if (m_pModelName.Get() == "TEST.mesh" && i == 0)
        anim->setEnabled(true);
		}
	}

	m_pOgreEnt->getSkeleton()->setBlendMode(Ogre::ANIMBLEND_CUMULATIVE);
}

bool
CBaseEntity::setAnimationLoop(const std::string&		animName,
															bool									loop)
{
	//s_animInfo*		animInfo = AnimationsManager::Instance()->GetFromName(m_pModelName, animName);

	//if (!animInfo)
	//	return false;

	unsigned int animCount = m_vecAnimInfo.size();

	for (unsigned int i = 0; i < animCount; i++)
	{
		s_AnimInfos info = m_vecAnimInfo[i];

		if (info.m_sName.compare(animName) == 0)
		{
			info.m_pAnimState->setLoop(loop);

			return true;
		}
	}

	return false;
}

bool
CBaseEntity::activateAnimation(const std::string&		animName)
{
	//s_animInfo*		animInfo = AnimationsManager::Instance()->GetFromName(m_pModelName, animName);

	//if (!animInfo)
	//	return false;

	unsigned int animCount = m_vecAnimInfo.size();

	for (unsigned int i = 0; i < animCount; i++)
	{
		s_AnimInfos info = m_vecAnimInfo[i];

		if (info.m_sName.compare(animName) == 0)
		{
			info.m_pAnimState->setEnabled(true);
			//info.m_fAnimSpeed = animInfo->m_fSpeedRatio;

			return true;
		}
	}

	return false;
}

bool
CBaseEntity::deactivateAnimation(const std::string&		animName)
{
	//s_animInfo*		animInfo = AnimationsManager::Instance()->GetFromName(m_pModelName, animName);

	//if (!animInfo)
	//	return false;

	unsigned int animCount = m_vecAnimInfo.size();

	for (unsigned int i = 0; i < animCount; i++)
	{
		s_AnimInfos info = m_vecAnimInfo[i];

		if (info.m_sName.compare(animName) == 0)
		{
			info.m_pAnimState->setEnabled(false);

			return true;
		}
	}

	return false;
}

bool
CBaseEntity::setAnimationSpeed(float					speed)
{
	return true;
}