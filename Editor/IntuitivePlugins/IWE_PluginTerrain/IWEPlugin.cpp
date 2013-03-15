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
