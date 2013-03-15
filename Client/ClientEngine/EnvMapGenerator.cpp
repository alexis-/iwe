#include "stdafx.h"

#include "EnvMapGenerator.h"

#include <OgreHardwarePixelBuffer.h>
#include <OgreSceneManager.h>
#include <OgreStringConverter.h>
#include <OgreRenderTarget.h>
#include <OgreTextureManager.h>
#include <OgreTexture.h>



CEnvMapGenerator::CEnvMapGenerator()
  : m_iCount(NULL),
  m_pCamera(NULL),
  m_sTexName("EnvMapTex" + Ogre::StringConverter::toString(howMany()))
{
}

CEnvMapGenerator::~CEnvMapGenerator()
{
}

void
CEnvMapGenerator::initialize(Ogre::SceneManager* pSceneMgr, Ogre::Viewport* pMainViewport)
{
  // create the camera used to render to our cubemap
  m_pCamera = pSceneMgr->createCamera("EnvMapCamera" + Ogre::StringConverter::toString(howMany()));
  m_pCamera->setFOVy(Ogre::Degree(90));
  m_pCamera->setAspectRatio(1);
  m_pCamera->setFixedYawAxis(false);
  m_pCamera->setNearClipDistance(.1);
  m_pCamera->setFarClipDistance(10000);

  const int SIZE = 256;

  // create our dynamic cube map texture
  Ogre::TexturePtr pTex = Ogre::TextureManager::getSingleton().createManual(
    m_sTexName,
    Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
    Ogre::TEX_TYPE_CUBE_MAP,
    SIZE, SIZE,
    0,
    Ogre::PF_R8G8B8,
    Ogre::TU_RENDERTARGET);

  // assign our camera to all 6 render targets of the texture (1 for each direction)
  for (unsigned int i = 0; i < 6; i++)
  {
    m_pTargets[i] = pTex->getBuffer(i)->getRenderTarget();
    Ogre::Viewport* vp = m_pTargets[i]->addViewport(m_pCamera);
    vp->setOverlaysEnabled(false);
    //vp->setMaterialScheme("Medium");
    vp->setShadowsEnabled(false);
    vp->setSkiesEnabled(true);
    vp->setClearEveryFrame(false);
    vp->setAutoUpdated(false);

    m_pTargets[i]->addListener(this);
  }

  Ogre::MaterialPtr base = Ogre::MaterialManager::getSingleton().getByName("base_dyncubicmap_material");

  std::string sMatName = "EnvMapMat" + Ogre::StringConverter::toString(howMany());

  Ogre::MaterialPtr mat = base->clone(sMatName);
  Ogre::TextureUnitState *tu = mat->getTechnique(0)->getPass(0)->createTextureUnitState();

  //	tu->setTextureName(mTexName);
  tu->setCubicTextureName(m_sTexName, true);
  tu->setEnvironmentMap(true, Ogre::TextureUnitState::ENV_REFLECTION);
  tu->setName("envMap");
  tu->setColourOperation(Ogre::LBO_ADD);
}