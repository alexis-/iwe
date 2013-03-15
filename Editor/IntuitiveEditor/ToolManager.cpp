#include "stdafx.h"

#include "ToolManager.h"
#include "ProjectManager.h"

template <class C>
class CToolProxy
{
public:
  CToolProxy(ITool* pTool, C* pUIElement, CToolManager* pToolMgr)
    : m_pTool(pTool),
      m_pUIElement(pUIElement),
      m_pToolMgr(pToolMgr)
  {
    pUIElement->eventPushed +=
      FastDelegate0<>(this, &CToolProxy::onCmd);
    pUIElement->eventUIDisplay +=
      FastDelegate2<bool&, bool&>(this, &CToolProxy::onCmdUI);
  }

private:
  void          onCmd()
  {
    m_pToolMgr->selectTool(m_pTool->getName());
  }

  void          onCmdUI(bool& bEnabled, bool& bChecked)
  {
    bEnabled = CProjectManager::getInstance()->isProjectLoaded();
    bChecked = m_pToolMgr->isToolEnabled(m_pTool);
  }

private:
  ITool*        m_pTool;
  C*            m_pUIElement;
  CToolManager* m_pToolMgr;
};

CToolManager* CToolManager::m_pInst = NULL;

CToolManager::CToolManager()
	: pSelectedTool(NULL)
{
}

CToolManager*
CToolManager::getInstance()
{
	if (!m_pInst)
		m_pInst = NEW CToolManager();

	return m_pInst;
}

void
CToolManager::registerTool(ITool*         pTool,
                           IUIButton*     pButton,
                           bool           bGlobal)
{
	if (!pTool)
		return;

	if (bGlobal)
		m_mapGlobalTools[pTool->getName()] = pTool;
	else
		m_mapTools[pTool->getName()] = pTool;

  CToolProxy<IUIButton>* pToolProxy = new CToolProxy<IUIButton>(pTool, pButton, this);

  if (!pSelectedTool)
    pSelectedTool = pTool;
}

ITool*
CToolManager::getTool(std::string sName)
{
	if (m_mapTools.find(sName) != m_mapTools.end())
		return m_mapTools[sName];

	return NULL;
}

bool
CToolManager::isToolEnabled(ITool* pTool)
{
  return pSelectedTool == pTool;
}

bool
CToolManager::onMouseInput(const s_inputDatas& datas, IViewport* pViewport)
{
	for (std::map<std::string, ITool*>::iterator it = m_mapGlobalTools.begin();
			 it != m_mapGlobalTools.end();
			 it++)
		(*it).second->onMouseInput(datas, pViewport);

	if (pSelectedTool)
		return pSelectedTool->onMouseInput(datas, pViewport);

  return true;
}

void
CToolManager::onKeyboardInput(IWEKeys::e_IWEKey nChar, bool keyDown, int repeatCount, IViewport* pViewport)
{
	for (std::map<std::string, ITool*>::iterator it = m_mapGlobalTools.begin();
			 it != m_mapGlobalTools.end();
			 it++)
		(*it).second->onKeyboardInput(nChar, keyDown, repeatCount, pViewport);

	if (pSelectedTool)
		pSelectedTool->onKeyboardInput(nChar, keyDown, repeatCount, pViewport);
}

void
CToolManager::onMouseLeave()
{
	for (std::map<std::string, ITool*>::iterator it = m_mapGlobalTools.begin();
			 it != m_mapGlobalTools.end();
			 it++)
		(*it).second->onMouseLeave();

	if (pSelectedTool)
		pSelectedTool->onMouseLeave();
}

void
CToolManager::selectTool(const std::string& sTool)
{
	if (pSelectedTool)
		pSelectedTool->onToolChange();

	if (m_mapTools.find(sTool) != m_mapTools.end())
	{
		pSelectedTool = m_mapTools[sTool];
		pSelectedTool->onToolActivate();
	}
	else
		pSelectedTool = NULL;
}

//void
//CToolManager::selectTool(const std::string& sTool, void* pDatas)
//{
//	selectTool(sTool);
//
//	if (pSelectedTool)
//		pSelectedTool->setDatas(pDatas);
//}

void
CToolManager::tick()
{
	for (std::map<std::string, ITool*>::iterator it = m_mapGlobalTools.begin();
			 it != m_mapGlobalTools.end();
			 it++)
		(*it).second->tick();

	if (pSelectedTool)
		pSelectedTool->tick();
}