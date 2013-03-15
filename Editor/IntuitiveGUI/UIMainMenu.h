#ifndef CUIMAIN_MENU_H
# define CUIMAIN_MENU_H




class CUISlider : public IUISlider
{
public:
  CUISlider(CMFCRibbonSlider* pSlider);
  virtual ~CUISlider();
  
  virtual void                setMin(int iMin);
  virtual void                setMax(int iMax);
  virtual void                setPosition(int iPos);

public:
  Event1<CUISlider*>          eventDestroy;

private:
  void                        onPush();
  void                        onUIDisplay(bool& bEnabled, bool& bChecked);

private:
  CMFCRibbonSlider*           m_pSlider;
};




class CUIGallery : public IUIGallery
{
public:
  CUIGallery(CMFCRibbonGallery* pGallery);
  virtual ~CUIGallery();
  
  virtual void                rename(const std::string& sGalleryName);

  virtual void                setButtonMode(bool bButton);
  virtual void                getSelectedItem(std::string& sItemOut, int& iItemOut);

public:
  virtual void                addItem(const std::string& sLabel, const std::string& sBitmapName, const std::string& sBitmapGroupName, char* pGroupName = "");
  virtual void                addItem(const std::string& sLabel, const s_IWEImageData& imgData, char* pGroupName = "");

public:
  Event1<CUIGallery*>         eventDestroy;

private:
  void                        onPush();
  void                        onUIDisplay(bool& bEnabled, bool& bChecked);

private:
  CMFCRibbonGallery*          m_pGallery;

  std::vector<std::string>                  m_vecItems;
  std::vector<std::pair<std::string, int>>  m_vecGroups;
  std::map<std::string, CMFCToolBarImages*> m_mapTB;
};




class CUISeparator : public IUISeparator
{
public:
  CUISeparator(CMFCRibbonSeparator* pSeparator);
  virtual ~CUISeparator();

private:
  CMFCRibbonSeparator*        m_pSeparator;
};




class CUIButton : public IUIButton
{
public:
  CUIButton(CMFCRibbonFixedButton* pButton, const std::string& sLabel);
  virtual ~CUIButton();

  virtual void                rename(const std::string& sButtonName);
  virtual void                setShortcut(const std::string& sShortcut);

  virtual void                setLargeButton(bool bLarge);
  virtual void                setSplitButton(bool bSplit);
  
public:
  virtual IUIButton*          addButton(const std::string& sLabel, const std::string& sBitmapName, const std::string& sGroupName, int iInsertAt = -1);
  virtual IUIButton*          addButton(const std::string& sLabel, const s_IWEImageData& imgData, int iInsertAt = -1);
  virtual IUIButton*          addButton(const std::string& sLabel, int iInsertAt = -1);

  virtual IUISeparator*       addSeparator(int iInsertAt = -1);

public:
  CMFCRibbonFixedButton*      getButton()
  {
    return m_pButton;
  }

  std::string                 getName()
  {
    return m_sLabel;
  }

private:
  void                        onPush();
  void                        onUIDisplay(bool& bEnabled, bool& bChecked);
  
  void                        onButtonDestroyed(CUIButton* pButton);

public:
  Event1<CUIButton*>          eventDestroy;
  Event1<const std::string&>  eventRename;

private:
  CMFCRibbonFixedButton*      m_pButton;
  std::string                 m_sLabel;
  
  std::map<std::string, CUIButton*>       m_mapButtons;
};




class CUIPanel : public IUIPanel
{
public:
  CUIPanel(CMFCRibbonPanel* pPanel, const std::string& sName);
  virtual ~CUIPanel();
  
  virtual void                setShortcut(const std::string& sShortcut);

public:
  virtual IUIButton*          addButton(const std::string& sLabel, const std::string& sBitmapName, const std::string& sGroupName, int iInsertAt = -1);
  virtual IUIButton*          addButton(const std::string& sLabel, const s_IWEImageData& imgData, int iInsertAt = -1);

  virtual IUISeparator*       addSeparator(int iInsertAt = -1);
  
  virtual IUIGallery*         addGallery(const std::string& sLabel, int iInsertAt = -1);

  virtual IUISlider*          addSlider(const std::string& sLabel, int iMin, int iMax, int iPos, bool bButtons, int iInsertAt = -1);

public:
  CMFCRibbonPanel*            getPanel()
  {
    return m_pPanel;
  }

  const std::string&          getName()
  {
    return m_sName;
  }

private:
  void                        onButtonDestroyed(CUIButton* pButton);

public:
  Event1<CUIPanel*>           eventDestroy;

private:
  CMFCRibbonPanel*            m_pPanel;
  std::string                 m_sName;

  std::map<std::string, CUIButton*>       m_mapButtons;
  std::map<std::string, CUIGallery*>      m_mapGalleries;
  std::map<std::string, CUISlider*>       m_mapSliders;
};




class CUICategory : public IUICategory
{
public:
  CUICategory(CMFCRibbonCategory* pCat, const std::string& sName);
  virtual ~CUICategory();

  virtual void                rename(const std::string& sCatName);
  virtual void                setShortcut(const std::string& sShortcut);

public:
  virtual IUIPanel*           addPanel(const std::string& sPanelName, int iInsertAt = -1);

public:
  CMFCRibbonCategory*         getCategory()
  {
    return m_pCategory;
  }

  const std::string&          getName()
  {
    return m_sName;
  }

public:
  Event1<CUICategory*>        eventDestroy;
  Event1<const std::string&>  eventRename;

private:
  void                        onPanelDestroyed(CUIPanel* pPanel);
  
private:
  CMFCRibbonCategory*               m_pCategory;
  std::string                       m_sName;
  std::map<std::string, CUIPanel*>  m_mapPanels;
};




class CUIMainMenu : public IUIMainMenu
{
public:
  CUIMainMenu(CMFCRibbonBar* pRibbon);

  virtual IUICategory*        addCategory(const std::string& sCatName, int iInsertAt = 1);
  
  void                        refreshLayout();

private:
  void                        onCategoryDestroyed(CUICategory* pCat);

private:
  CMFCRibbonBar*                        m_pRibbon;
  std::map<std::string, CUICategory*>   m_mapCategories;
};




#endif // !CUIMAIN_MENU_H