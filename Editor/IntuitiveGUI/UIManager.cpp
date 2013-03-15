#include "stdafx.h"

#include "UIManager.h"

#include "ClassView.h"
#include "FileView.h"

#ifdef __cplusplus_cli
#include "CLIView.h"
#include "CLIChildFrm.h"
#endif // !__cplusplus_cli

CUIManager* CUIManager::m_pInst = NULL;
uint CUIManager::m_nUIStartingID = 27300;

CUIManager*
CUIManager::getInstance()
{
  if (!m_pInst)
    m_pInst = new CUIManager();

  return m_pInst;
}

CUIManager::CUIManager()
  : m_uUIId(m_nUIStartingID)
{
}

void
CUIManager::init(CMFCRibbonBar* pRibbon, CFileView* pFileView, CClassView* pClassView, CMFCPropertyGridCtrl* pPropGridCtrl)
{
  m_pMainMenu = new CUIMainMenu(pRibbon);
  m_pProjectTree = new CUIProjectTree(pFileView);
  m_pSceneTree = new CUITree<CClassView>(&(pClassView->getViewTree()), pClassView);
  m_pPropGrid = new CUIPropertyGrid(pPropGridCtrl);
}

IUIMainMenu*
CUIManager::getMainMenu()
{
  return m_pMainMenu;
}

IUIProjectTree*
CUIManager::getProjectTree()
{
  return m_pProjectTree;
}

IUITreeElement*
CUIManager::getSceneTree()
{
  return m_pSceneTree;
}

IUIPropertyGrid*
CUIManager::getPropertyGrid()
{
  return m_pPropGrid;
}

#ifdef __cplusplus_cli
void
CUIManager::addDocument(System::Type^ docType, const std::string& sTitle)
{
  CCLIView::m_type = docType;
  CCLIView::m_title = sTitle.c_str();
  
  CDocument* pDoc = m_pCLIViewDoc->CreateNewDocument();
  CFrameWnd* pFrame = m_pCLIViewDoc->CreateNewFrame(pDoc, NULL);

  m_pCLIViewDoc->InitialUpdateFrame(pFrame, pDoc);
}
#endif // !__cplusplus_cli

bool
CUIManager::OnCmdMsg(uint nID)
{
  std::map<uint, FastDelegate0<>>::iterator it = m_mapCmd.find(nID);

  if (it != m_mapCmd.end())
  {
    (*it).second();
    return true;
  }

  return false;
}

void
CUIManager::OnCmdUIMsg(CCmdUI* pCmdUI)
{
  uint nID = pCmdUI->m_nID;
  
  std::map<uint, FastDelegate2<bool&, bool&>>::iterator it = m_mapCmdUI.find(nID);
  bool bEnabled = false;
  bool bChecked = false;

  if (it != m_mapCmdUI.end())
    (*it).second(bEnabled, bChecked);

  pCmdUI->Enable(bEnabled);
	pCmdUI->SetCheck(bChecked);
	pCmdUI->SetRadio(bChecked);
}

void
CUIManager::registerCmd(uint nID, FastDelegate0<>& cmdDelegate)
{
  m_mapCmd[nID] = cmdDelegate;
}

void
CUIManager::unregisterCmd(uint nID)
{
  m_mapCmd.erase(nID);
}

void
CUIManager::registerCmdUI(uint nID, FastDelegate2<bool&, bool&>& cmdDelegate)
{
  m_mapCmdUI[nID] = cmdDelegate;
}

void
CUIManager::unregisterCmdUI(uint nID)
{
  m_mapCmdUI.erase(nID);
}