#include "IWEPlugin.h"

#include "CLight.h"
#include "RenderHelper/EngineUtils.h"

CPlugin* CPlugin::m_pInst = NULL;

IPlugin* getPluginInterface()
{
  return CPlugin::getInstance();
}


CPlugin*
CPlugin::getInstance()
{
  if (!m_pInst)
    m_pInst = new CPlugin();

  return m_pInst;
}

CPlugin::CPlugin()
  : m_pGUI(NULL),
    m_pEngine(NULL),
    m_pEditor(NULL)
{
}

void
CPlugin::initialize(IGUIInterface*           pGUI,
                    IEngineInterface*        pEngine,
                    IEditorInterface*        pEditor)
{
  m_pGUI = pGUI;
  m_pEngine = pEngine;
  m_pEditor = pEditor;

  GetEditorInteraction()->requestTargetPick +=
    FastDelegate4<CIWEPoint<int>, IViewport*, float&, Ogre::Vector3&, ISelectableTarget*>(this, &CPlugin::onPickRequest);

  eventPostInit(pGUI, pEngine, pEditor);

  //IUITreeElement* pTreeElem1 = pGUI->getUIManager()->getSceneTree()->addCategory("Test #1");
  //pTreeElem1->addCategory("Test #2");
  //
  //pGUI->getUIManager()->getSceneTree()->addCategory(3, "Test #1", "Toto 2", "Toto 3");
}

bool
CPlugin::onMouseInput(const s_inputDatas&    datas,
                      IViewport*             pViewport)
{
  return true;
}

void
CPlugin::onKeyboardInput(IWEKeys::e_IWEKey   nChar,
                         bool                keyDown,
                         int                 repeatCount,
                         IViewport*          pViewport)
{
}

void
CPlugin::onMouseLeave(IViewport*             pViewport)
{
}

ISelectableTarget*
CPlugin::onPickRequest(CIWEPoint<int> pt, IViewport* pViewport, float& fDistOut, Ogre::Vector3& vecPickedPosOut)
{
  return RenderHelper::PickSelectableTarget(pt.x, pt.y, GetRenderEngine()->getSceneManager(), pViewport, fDistOut, vecPickedPosOut, "CLight");
}