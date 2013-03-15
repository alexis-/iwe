#ifndef TOOL_H
# define TOOL_H

class CTool : public ITool
{
public:
	CTool(IPlugin*      pPlugin,
        std::string   sName,
        std::string   sCategory,
        std::string   sPanel,
        std::string   sLabel,
        std::string   sImgName,
        std::string   sImgGroup,
        int           nInsertCategoryAt = -1,
        int           nInsertPanelAt = -1,
        int           nInsertButtonAt = -1,
        bool          bLarge = true,
        bool          bGlobal = false)
    : m_sName(sName),
      m_bGlobal(bGlobal),
      m_sLabel(sLabel),
      m_sImgName(sImgName),
      m_sImgGroup(sImgGroup),
      m_sCategory(sCategory),
      m_sPanel(sPanel),
      m_nInsertCategoryAt(nInsertCategoryAt),
      m_nInsertPanelAt(nInsertPanelAt),
      m_nInsertButtonAt(nInsertButtonAt),
      m_bLarge(bLarge),
      m_bInitButton(true)
  {
    pPlugin->eventPostInit +=
      FastDelegate3<IGUIInterface*, IEngineInterface*, IEditorInterface*>(this, &CTool::defaultToolInit);
  }
  
	CTool(IPlugin*      pPlugin,
        std::string   sName,
        bool          bGlobal = false)
    : m_sName(sName),
      m_bGlobal(bGlobal),
      m_bInitButton(false)
  {
    pPlugin->eventPostInit +=
      FastDelegate3<IGUIInterface*, IEngineInterface*, IEditorInterface*>(this, &CTool::defaultToolInit);
  }

public:
	virtual std::string						getName()
  {
    return m_sName;
  }
	
	virtual bool									onMouseInput(const s_inputDatas& datas, IViewport* pViewport) = 0;
	virtual void									onKeyboardInput(IWEKeys::e_IWEKey nChar, bool keyDown, int repeatCount, IViewport* pViewport) = 0;
	virtual void									onMouseLeave() = 0;
	
	virtual void									onToolChange() {};
	virtual void									onToolActivate() {};
	
	virtual void									tick() {};

  virtual bool                  onInit() { return true; };

private:
  void                          defaultToolInit(IGUIInterface* pGUI, IEngineInterface*, IEditorInterface* pEditor)
  {
    if (!onInit() || !m_bInitButton)
      return;

    IUIMainMenu* pMainMenu = pGUI->getUIManager()->getMainMenu();
    
    IUICategory* pCat = pMainMenu->addCategory(m_sCategory, m_nInsertCategoryAt);
    IUIPanel* pPanel = pCat->addPanel(m_sPanel, m_nInsertPanelAt);
    
    m_pButton = pPanel->addButton(m_sLabel, m_sImgName, m_sImgGroup, m_nInsertButtonAt);
    
    m_pButton->setLargeButton(m_bLarge);

    pEditor->getToolMgr()->registerTool(this, m_pButton, m_bGlobal);
  }


protected:
	std::string										m_sName;
  bool                          m_bGlobal;

  IUIButton*                    m_pButton;

  bool                          m_bLarge;
  int                           m_nInsertCategoryAt;
  int                           m_nInsertPanelAt;
  int                           m_nInsertButtonAt;

  std::string                   m_sCategory;
  std::string                   m_sPanel;
  std::string                   m_sLabel;
  std::string                   m_sImgName;
  std::string                   m_sImgGroup;

private:
  bool                          m_bInitButton;
};

#endif // !TOOL_H