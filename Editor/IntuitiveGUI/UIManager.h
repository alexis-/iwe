#ifndef UI_MANAGER_H
# define UI_MANAGER_H

# include "UISceneTree.h"
# include "UIMainMenu.h"
# include "UIProjectTree.h"
# include "UIPropertyGrid.h"

class CFileView;
class CClassView;

class CUIManager : public IUIManager
{
public:
  static CUIManager*                          getInstance();

  void                                        init(CMFCRibbonBar* pRibbon, CFileView* pViewTree, CClassView* pClassView, CMFCPropertyGridCtrl* pPropGridCtrl);
  bool                                        OnCmdMsg(uint nID);
  void                                        OnCmdUIMsg(CCmdUI* pCmdUI);

public:
  void                                        registerCmd(uint nID, FastDelegate0<>& cmdDelegate);
  void                                        unregisterCmd(uint nID);
  
  void                                        registerCmdUI(uint nID, FastDelegate2<bool&, bool&>& cmdDelegate);
  void                                        unregisterCmdUI(uint nID);

  uint                                        getNextUIID()
  {
    return m_uUIId++;
  }

  void                                        setCLIViewDoc(CMultiDocTemplate* pT)
  {
    m_pCLIViewDoc = pT;
  }

public:
  virtual IUIMainMenu*                        getMainMenu();
  virtual IUIProjectTree*                     getProjectTree();
  virtual IUITreeElement*                     getSceneTree();
  virtual IUIPropertyGrid*                    getPropertyGrid();
  
#ifdef __cplusplus_cli
  virtual void                                addDocument(System::Type^ docType, const std::string& sTitle);
#endif // !__cplusplus_cli

public:
  static uint                                 m_nUIStartingID;

private:
  CUIMainMenu*                                m_pMainMenu;
  CUIProjectTree*                             m_pProjectTree;
  CUIPropertyGrid*                            m_pPropGrid;
  CUITree<CClassView>*                        m_pSceneTree;
  CMultiDocTemplate*                          m_pCLIViewDoc;

  uint                                        m_uUIId;

  std::map<uint, FastDelegate0<>>             m_mapCmd;
  std::map<uint, FastDelegate2<bool&, bool&>> m_mapCmdUI;

private:
  static CUIManager*                          m_pInst;
  CUIManager();
};


#endif // !UI_MANAGER_H