#include "IWEPlugin.h"

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
    m_pEditor(NULL),
    m_pPropHolder(NULL)
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

  GetEditorInteraction()->eventTargetSelected +=
    FastDelegate1<ISelectableTarget*>(this, &CPlugin::onTargetSelected);

  eventPostInit(pGUI, pEngine, pEditor);
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

void
CPlugin::onTargetSelected(ISelectableTarget* pTarget)
{
  IPropertyHolder* pPropHolder = dynamic_cast<IPropertyHolder*>(pTarget);

  if (pPropHolder != NULL)
  {
    m_pPropHolder = pPropHolder;

    pTarget->eventTargetDeleted +=
      FastDelegate1<ISelectableTarget*>(this, &CPlugin::onTargetDeleted);
    
    pTarget->eventTargetUnselected +=
      FastDelegate1<ISelectableTarget*>(this, &CPlugin::onTargetUnselected);

    m_pGUI->getUIManager()->getPropertyGrid()->setPropertyHolder(pPropHolder);
  }
}

void
CPlugin::onTargetUnselected(ISelectableTarget* pTarget)
{
  pTarget->eventTargetDeleted -=
    FastDelegate1<ISelectableTarget*>(this, &CPlugin::onTargetDeleted);
  
  pTarget->eventTargetUnselected -=
    FastDelegate1<ISelectableTarget*>(this, &CPlugin::onTargetUnselected);

  m_pPropHolder = NULL;
  m_pGUI->getUIManager()->getPropertyGrid()->setPropertyHolder(NULL);
}

void
CPlugin::onTargetDeleted(ISelectableTarget* pTarget)
{
  pTarget->eventTargetDeleted -=
    FastDelegate1<ISelectableTarget*>(this, &CPlugin::onTargetDeleted);
  
  pTarget->eventTargetUnselected -=
    FastDelegate1<ISelectableTarget*>(this, &CPlugin::onTargetUnselected);

  m_pPropHolder = NULL;
  m_pGUI->getUIManager()->getPropertyGrid()->setPropertyHolder(NULL);
}
