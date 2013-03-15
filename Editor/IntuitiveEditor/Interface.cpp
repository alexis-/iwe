#include "stdafx.h"
#include "Interface.h"

#include "EditorInteraction.h"
#include "ProjectManager.h"
#include "ToolManager.h"

CEditorInterface* CEditorInterface::m_pInst = NULL;

CEditorInterface*
CEditorInterface::getInstance()
{
   if (!m_pInst)
     m_pInst = new CEditorInterface();

   return m_pInst;
}

CEditorInterface::CEditorInterface()
  : m_pEngine(NULL)
{
}

IEditorInteraction*
CEditorInterface::getEditorInteraction()
{
  return CEditorInteraction::getInstance();
}

IProjectManager*
CEditorInterface::getProjectMgr()
{
  return CProjectManager::getInstance();
}

IToolManager*
CEditorInterface::getToolMgr()
{
  return CToolManager::getInstance();
}

void
CEditorInterface::initialize(IEngineInterface* pEngine, IGUIInterface* pGUI)
{
  m_pEngine = pEngine;
  m_pGUI = pGUI;
  
  CEditorInteraction::getInstance()->init();

  pGUI->eventPostInit += FastDelegate0<>(this, &CEditorInterface::onPostInit);
}

void
CEditorInterface::onPostInit()
{
  CEditorInteraction::getInstance()->postInit();
}

IEditorInterface*
getEditorInterface()
{
  return CEditorInterface::getInstance();
}