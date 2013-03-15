/*
-----------------------------------------------------------------------------
This source file is part of OGRE
(Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2012 Torus Knot Software Ltd
Also see acknowledgements in Readme.html

You may use this sample code for anything you like, it is not covered by the
same license as the rest of the engine.
-----------------------------------------------------------------------------
*/

#include "stdafx.h"

#include "RenderSettings.h"

#include <OgreConfigFile.h>
#include <OgreStringConverter.h>
#include <OgreException.h>

#include <OgreRoot.h>
#include <OgreRenderSystem.h>

#include <OgreEntity.h>
#include <OgreSubEntity.h>
#include <OgreRoot.h>

#include <OgreCompositor.h>
#include <OgreCompositorManager.h>
#include <OgreCompositorChain.h>
#include <OgreCompositorInstance.h>

#include "DeferredShading/DeferredLightCP.h"
#include "DeferredShading/SSAOLogic.h"
#include "DeferredShading/GBufferSchemeHandler.h"
#include "DeferredShading/NullSchemeHandler.h"

RenderSettings* RenderSettings::m_pInst = NULL;

const Ogre::uint8 RenderSettings::PRE_GBUFFER_RENDER_QUEUE = Ogre::RENDER_QUEUE_1;
const Ogre::uint8 RenderSettings::POST_GBUFFER_RENDER_QUEUE = Ogre::RENDER_QUEUE_8;

RenderSettings*
RenderSettings::getInstance()
{
  if (!m_pInst)
    m_pInst = new RenderSettings();

  return m_pInst;
}

RenderSettings::RenderSettings()
  : m_pViewport(NULL),
    m_pSceneMgr(NULL),
    m_pCamera(NULL)
{
}

RenderSettings::~RenderSettings()
{
	Ogre::CompositorChain *chain = Ogre::CompositorManager::getSingleton().getCompositorChain(m_pViewport);

	chain->_removeInstance(m_pShowLitInstance);
	Ogre::CompositorManager::getSingleton().removeCompositorChain(m_pViewport);

	Ogre::CompositorManager& compMgr = Ogre::CompositorManager::getSingleton();

	CompositorLogicMap::const_iterator itor = m_mapCompositorLogics.begin();
	CompositorLogicMap::const_iterator end  = m_mapCompositorLogics.end();

	while(itor != end )
	{
		compMgr.unregisterCompositorLogic(itor->first );
		delete itor->second;
		++itor;
	}

	m_mapCompositorLogics.clear();
}

Ogre::CompositorInstance* pGBufferInstance = NULL;
Ogre::CompositorInstance* pShowLitInstance = NULL;
Ogre::CompositorInstance* pSSAOInstance = NULL;

void testPush()
{
  static bool bActive = false;

  pGBufferInstance->setEnabled(bActive);
  pShowLitInstance->setEnabled(bActive);
  pSSAOInstance->setEnabled(bActive);

  bActive = !bActive;
}

void testCmdUI(bool& a, bool& b)
{
  a = true;
  b = false;
}

void
RenderSettings::initialize(Ogre::Viewport* pViewport, Ogre::SceneManager* pSceneMgr, Ogre::Camera* pCamera)
{
  m_pViewport = pViewport;
  m_pSceneMgr = pSceneMgr;
  m_pCamera = pCamera;

	m_pSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_TEXTURE_ADDITIVE);
	m_pSceneMgr->setShadowTextureCasterMaterial("DeferredShading/Shadows/Caster");

	m_pSceneMgr->setShadowTextureCount(1);
	m_pSceneMgr->setShadowFarDistance(150);

	//Use a value of "2" to use a different depth buffer pool and avoid sharing this with the Backbuffer's
  //for (int i = 0; i < 4; i++)
	m_pSceneMgr->setShadowTextureConfig(0, 1024, 1024, Ogre::PF_FLOAT16_R, 2);

	m_pSceneMgr->setShadowDirectionalLightExtrusionDistance(75);
  //m_pSceneMgr->setShadowCasterRenderBackFaces(true);

  
  //Shadow camera setup.
  //Ogre::PSSMShadowCameraSetup::SplitPointList splits;
  //splits.push_back(0.1f);
  //splits.push_back(18.7167f);
  //splits.push_back(77.4301f);
  //splits.push_back(1000.0f);
  //
  //Ogre::PSSMShadowCameraSetup* pssmSetup = new Ogre::PSSMShadowCameraSetup();
  //pssmSetup->setSplitPoints(splits);
  //pssmSetup->setSplitPadding(10);
  //pssmSetup->setOptimalAdjustFactor(0, 5);
  //pssmSetup->setOptimalAdjustFactor(1, 3);
  //pssmSetup->setOptimalAdjustFactor(2, 0.1);
  //m_pSceneMgr->setShadowCameraSetup(Ogre::ShadowCameraSetupPtr(pssmSetup));


  //////////////////////////////////////

	createResources();

  m_pGBufferInstance->setEnabled(true);
  m_pShowLitInstance->setEnabled(true);
  
  pGBufferInstance = m_pGBufferInstance;
  pShowLitInstance = m_pShowLitInstance;
  pSSAOInstance = m_pSSAOInstance;

  m_pSSAOInstance->setEnabled(true);
  m_pSSAAInstance->setEnabled(true);
  //m_pBloomInstance->setEnabled(true);
  
      IUIMainMenu* pMainMenu = GetGUI()->getUIManager()->getMainMenu();
    
      IUICategory* pCat = pMainMenu->addCategory("Home", 0);
      IUIPanel* pPanel = pCat->addPanel("TestDeferred", -1);
    
      // Main Button
      IUIButton* m_pButton = pPanel->addButton("TestDeferred", "MoveToolIcon.bmp", "Plugins", -1);
      m_pButton->setLargeButton(true);
      m_pButton->eventPushed +=
        FastDelegate0<>(&testPush);
      m_pButton->eventUIDisplay +=
        FastDelegate2<bool&, bool&>(&testCmdUI);
}

void
RenderSettings::setCompositorState(const std::string& sComp, bool bState)
{
  if (sComp == "DeferredShading")
  {
		m_pGBufferInstance->setEnabled(bState);
    m_pShowLitInstance->setEnabled(bState);

    if (m_pSSAOInstance->getEnabled() && !bState)
      m_pSSAOInstance->setEnabled(false);
  }

  else if (sComp == "SSAO")
    m_pSSAOInstance->setEnabled(bState && m_pGBufferInstance->getEnabled());

  else if (sComp == "SSAA")
    m_pSSAAInstance->setEnabled(bState);
  
  else if (sComp == "LDRBloom")
    m_pBloomInstance->setEnabled(bState);
}

void RenderSettings::createResources(void)
{
	Ogre::CompositorManager& compMan = Ogre::CompositorManager::getSingleton();

	//Hook up the compositor logic and scheme handlers.
	Ogre::MaterialManager::getSingleton().addListener(new GBufferSchemeHandler, "GBuffer");
	Ogre::MaterialManager::getSingleton().addListener(new NullSchemeHandler, "NoGBuffer");

	compMan.registerCustomCompositionPass("DeferredLight", new DeferredLightCompositionPass);

	m_mapCompositorLogics["SSAOLogic"] = new SSAOLogic;
	compMan.registerCompositorLogic("SSAOLogic", m_mapCompositorLogics["SSAOLogic"]);

	// Create the main GBuffer compositor
	m_pGBufferInstance = compMan.addCompositor(m_pViewport, "DeferredShading/GBuffer");
	
	// Create filters
	m_pShowLitInstance = compMan.addCompositor(m_pViewport, "DeferredShading/ShowLit");

	m_pSSAOInstance = compMan.addCompositor(m_pViewport, "DeferredShading/SSAO");
  m_pSSAAInstance = compMan.addCompositor(m_pViewport, "SSAA");
  //m_pBloomInstance = compMan.addCompositor(m_pViewport, "LDRBloom");
}