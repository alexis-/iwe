#include "stdafx.h"

#include "Interface.h"
#include "RenderEngine.h"
#include "ResourceManager.h"

CEngineInterface* CEngineInterface::m_pInst = NULL;

CEngineInterface*
CEngineInterface::getInstance()
{
  if (!m_pInst)
    m_pInst = new CEngineInterface();

  return m_pInst;
}

IRenderEngine*
CEngineInterface::getRenderEngine()
{
  return ::GetRenderEngine();
}

IResourceManager*
CEngineInterface::getResourceMgr()
{
  return CResourceManager::getInstance();
}

void
CEngineInterface::logMessage(const char* pMsg, ...)
{
}

void
CEngineInterface::initialize(IEditorInterface* pEditor, IGUIInterface* pGUI)
{
  m_pEditor = pEditor;
  m_pGUI = pGUI;
}

void
CEngineInterface::initialize(void* pCustomData)
{
}

IEngineInterface* getEngineInterface()
{
  return CEngineInterface::getInstance();
}