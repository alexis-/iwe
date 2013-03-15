#include "stdafx.h"

#include "UIMainMenu.h"
#include "UIManager.h"


/////////////////////////////////////////////////////////////////////////////////////////////
//  CUIMainMenu
/////////////////////////////////////////////////////////////////////////////////////////////

CUIMainMenu::CUIMainMenu(CMFCRibbonBar* pRibbon)
  : m_pRibbon(pRibbon)
{
}

IUICategory*
CUIMainMenu::addCategory(const std::string& sCatName, int iInsertAt)
{
  std::map<std::string, CUICategory*>::iterator it = m_mapCategories.find(sCatName);

  if (it != m_mapCategories.end())
    return (*it).second;

  CMFCRibbonCategory* pCat = m_pRibbon->AddCategory(sCatName.c_str(), 0, 0,
    CSize(16, 16), CSize(32, 32), iInsertAt);

  if (!pCat)
    return NULL;

  CUICategory* pUICat = new CUICategory(pCat, sCatName);
  
  pUICat->eventDestroy += FastDelegate1<CUICategory*>(this, &CUIMainMenu::onCategoryDestroyed);
  m_mapCategories[sCatName] = pUICat;

  return pUICat;
}

void
CUIMainMenu::onCategoryDestroyed(CUICategory* pCat)
{
  m_mapCategories.erase(pCat->getName());

  m_pRibbon->RemoveCategory(m_pRibbon->GetCategoryIndex(pCat->getCategory()));
}

void
CUIMainMenu::refreshLayout()
{
  m_pRibbon->ForceRecalcLayout();
}



/////////////////////////////////////////////////////////////////////////////////////////////
//  CUICategory
/////////////////////////////////////////////////////////////////////////////////////////////

CUICategory::CUICategory(CMFCRibbonCategory* pCat, const std::string& sName)
  : m_pCategory(pCat),
    m_sName(sName)
{
}

// TODO: Remove category
CUICategory::~CUICategory()
{
  for (std::map<std::string, CUIPanel*>::iterator it = m_mapPanels.begin();
       it != m_mapPanels.end();
       it++)
    delete (*it).second;

  m_mapPanels.clear();

  eventDestroy(this);
  
  delete m_pCategory;
}

void
CUICategory::rename(const std::string& sCatName)
{
  m_pCategory->SetName(sCatName.c_str());

  eventRename(sCatName);
}

void
CUICategory::setShortcut(const std::string& sShortcut)
{
  m_pCategory->SetKeys(sShortcut.c_str());
}

IUIPanel*
CUICategory::addPanel(const std::string& sPanelName, int iInsertAt)
{
  std::map<std::string, CUIPanel*>::iterator it = m_mapPanels.find(sPanelName);

  if (it != m_mapPanels.end())
    return (*it).second;

  CMFCRibbonPanel* pPanel = m_pCategory->AddPanel(sPanelName.c_str());

  if (!pPanel)
    return NULL;

  CUIPanel* pUIPanel = new CUIPanel(pPanel, sPanelName);

  pUIPanel->eventDestroy += FastDelegate1<CUIPanel*>(this, &CUICategory::onPanelDestroyed);
  m_mapPanels[sPanelName] = pUIPanel;

  return pUIPanel;
}

void
CUICategory::onPanelDestroyed(CUIPanel* pPanel)
{
  m_mapPanels.erase(pPanel->getName());

  m_pCategory->RemovePanel(m_pCategory->GetPanelIndex(pPanel->getPanel()), false);
}



/////////////////////////////////////////////////////////////////////////////////////////////
//  CUIPanel
/////////////////////////////////////////////////////////////////////////////////////////////

CUIPanel::CUIPanel(CMFCRibbonPanel* pPanel, const std::string& sName)
  : m_pPanel(pPanel),
    m_sName(sName)
{
}

// TODO: Remove panel
CUIPanel::~CUIPanel()
{
  eventDestroy(this);

  delete m_pPanel;
}

void
CUIPanel::setShortcut(const std::string& sShortcut)
{
  m_pPanel->SetKeys(sShortcut.c_str());
}

IUIButton*
CUIPanel::addButton(const std::string& sLabel, const std::string& sBitmapName, const std::string& sGroupName, int iInsertAt)
{
  s_IWEImageData imgData;
  
  imgData.pData = GetResourceMgr()->getBitmapMaterial(sBitmapName, sGroupName, imgData.width, imgData.height, imgData.bpp);

  return addButton(sLabel, imgData, iInsertAt);
}

IUIButton*
CUIPanel::addButton(const std::string& sLabel, const s_IWEImageData& imgData, int iInsertAt)
{
  if (!imgData.pData)
    return NULL;

  std::map<std::string, CUIButton*>::iterator it = m_mapButtons.find(sLabel);

  if (it != m_mapButtons.end())
    return (*it).second;

  HICON icon = HICONFromData(imgData);

  CMFCRibbonFixedButton* pButton = new CMFCRibbonFixedButton(CUIManager::getInstance()->getNextUIID(),
    sLabel.c_str(), icon, 0, icon, true);
  
  if (!pButton)
    return NULL;
  
  pButton->SetSmallMode();
  //m_pPanel->Add(pButton);
  m_pPanel->Insert(pButton, iInsertAt);

  CUIButton* pUIButton = new CUIButton(pButton, sLabel);

  pUIButton->eventDestroy += FastDelegate1<CUIButton*>(this, &CUIPanel::onButtonDestroyed);
  m_mapButtons[sLabel] = pUIButton;

  return pUIButton;
}

IUISeparator*
CUIPanel::addSeparator(int iInsertAt)
{
  CMFCRibbonSeparator* pSeparator = new CMFCRibbonSeparator();

  if (!pSeparator)
    return NULL;

  m_pPanel->Insert(pSeparator, iInsertAt);

  CUISeparator* pUISeparator = new CUISeparator(pSeparator);
  // TODO: Kepp track of pUISeparator

  return pUISeparator;
}

IUIGallery*
CUIPanel::addGallery(const std::string& sLabel, int iInsertAt)
{
  std::map<std::string, CUIGallery*>::iterator it = m_mapGalleries.find(sLabel);

  if (it != m_mapGalleries.end())
    return (*it).second;

  CMFCRibbonGallery* pGallery = new CMFCRibbonGallery(CUIManager::getInstance()->getNextUIID(),
    sLabel.c_str(), 0, 0);
  
  if (!pGallery)
    return NULL;
  
  pGallery->SetButtonMode(false);
  m_pPanel->Insert(pGallery, iInsertAt);

  CUIGallery* pUIGallery = new CUIGallery(pGallery);

  //pUIGallery->eventDestroy += FastDelegate1<CUIGallery*>(this, &CUIPanel::onGalleryDestroyed);
  m_mapGalleries[sLabel] = pUIGallery;

  return pUIGallery;
}

IUISlider*
CUIPanel::addSlider(const std::string& sLabel, int iMin, int iMax, int iPos, bool bButtons, int iInsertAt)
{
  std::map<std::string, CUISlider*>::iterator it = m_mapSliders.find(sLabel);

  if (it != m_mapSliders.end())
    return (*it).second;

  CMFCRibbonSlider* pSlider = new CMFCRibbonSlider(CUIManager::getInstance()->getNextUIID());

  if (!pSlider)
    return NULL;

  pSlider->SetRange(iMin, iMax);
  pSlider->SetPos(iPos);
  pSlider->SetZoomButtons(bButtons);

  m_pPanel->Insert(pSlider, iInsertAt);

  CUISlider* pUISlider = new CUISlider(pSlider);

  m_mapSliders[sLabel] = pUISlider;

  return pUISlider;
}

void
CUIPanel::onButtonDestroyed(CUIButton* pButton)
{
  std::string sName = pButton->getName();
  CMFCRibbonBaseElement* pElem = pButton->getButton();

  m_mapButtons.erase(sName);

  int i = m_pPanel->GetIndex(pElem);
  m_pPanel->Remove(i, false);
}



/////////////////////////////////////////////////////////////////////////////////////////////
//  CUIButton
/////////////////////////////////////////////////////////////////////////////////////////////

CUIButton::CUIButton(CMFCRibbonFixedButton* pButton, const std::string& sLabel)
  : m_pButton(pButton),
    m_sLabel(sLabel)
{
  CUIManager::getInstance()->registerCmd(pButton->GetID(),
    FastDelegate0<>(this, &CUIButton::onPush));
  CUIManager::getInstance()->registerCmdUI(pButton->GetID(),
    FastDelegate2<bool&, bool&>(this, &CUIButton::onUIDisplay));
}

CUIButton::~CUIButton()
{
  eventDestroy(this);

  delete m_pButton;
}

void
CUIButton::rename(const std::string& sButtonName)
{
  m_sLabel = sButtonName;
  m_pButton->SetText(sButtonName.c_str());

  eventRename(sButtonName);
}

void
CUIButton::setShortcut(const std::string& sShortcut)
{
  m_pButton->SetKeys(sShortcut.c_str());
}

void
CUIButton::setLargeButton(bool bLarge)
{
  if (bLarge)
    m_pButton->SetLargeMode();
  else
    m_pButton->SetSmallMode();
}

void
CUIButton::setSplitButton(bool bSplit)
{
  m_pButton->SetDefaultCommand(bSplit);
}

IUIButton*
CUIButton::addButton(const std::string& sLabel, const std::string& sBitmapName, const std::string& sGroupName, int iInsertAt)
{
  s_IWEImageData imgData;
  
  imgData.pData = GetResourceMgr()->getBitmapMaterial(sBitmapName, sGroupName, imgData.width, imgData.height, imgData.bpp);

  return addButton(sLabel, imgData, iInsertAt);
}

IUIButton*
CUIButton::addButton(const std::string& sLabel, const s_IWEImageData& imgData, int iInsertAt)
{
  if (!imgData.pData)
    return NULL;

  std::map<std::string, CUIButton*>::iterator it = m_mapButtons.find(sLabel);

  if (it != m_mapButtons.end())
    return (*it).second;

  HICON icon = HICONFromData(imgData);

  CMFCRibbonFixedButton* pButton = new CMFCRibbonFixedButton(CUIManager::getInstance()->getNextUIID(),
    sLabel.c_str(), icon, 0, icon, true);
  
  if (!pButton)
    return NULL;
  
  pButton->SetSmallMode();
  m_pButton->AddSubItem(pButton, iInsertAt);

  CUIButton* pUIButton = new CUIButton(pButton, sLabel);

  pUIButton->eventDestroy += FastDelegate1<CUIButton*>(this, &CUIButton::onButtonDestroyed);
  m_mapButtons[sLabel] = pUIButton;

  return pUIButton;
}

IUIButton*
CUIButton::addButton(const std::string& sLabel, int iInsertAt)
{
  std::map<std::string, CUIButton*>::iterator it = m_mapButtons.find(sLabel);

  if (it != m_mapButtons.end())
    return (*it).second;

  CMFCRibbonFixedButton* pButton = new CMFCRibbonFixedButton(CUIManager::getInstance()->getNextUIID(),
    sLabel.c_str(), NULL, 0, NULL);
  
  if (!pButton)
    return NULL;
  
  pButton->SetSmallMode();
  m_pButton->AddSubItem(pButton, iInsertAt);

  CUIButton* pUIButton = new CUIButton(pButton, sLabel);

  pUIButton->eventDestroy += FastDelegate1<CUIButton*>(this, &CUIButton::onButtonDestroyed);
  m_mapButtons[sLabel] = pUIButton;

  return pUIButton;
}

IUISeparator*
CUIButton::addSeparator(int iInsertAt)
{
  CMFCRibbonSeparator* pSeparator = new CMFCRibbonSeparator(true);

  if (!pSeparator)
    return NULL;

  m_pButton->AddSubItem(pSeparator, iInsertAt);

  CUISeparator* pUISeparator = new CUISeparator(pSeparator);
  // TODO: Kepp track of pUISeparator

  return pUISeparator;
}

void
CUIButton::onPush()
{
  eventPushed();
}

void
CUIButton::onUIDisplay(bool& bEnabled, bool& bChecked)
{
  bEnabled = bChecked = false;

  eventUIDisplay(bEnabled, bChecked);
}

void
CUIButton::onButtonDestroyed(CUIButton* pButton)
{
  std::string sName = pButton->getName();
  CMFCRibbonBaseElement* pElem = pButton->getButton();

  m_mapButtons.erase(sName);

  const CArray<CMFCRibbonBaseElement*, CMFCRibbonBaseElement*>& subItems = m_pButton->GetSubItems();
  int subItemsNum = (int)subItems.GetCount();
  int i = 0;

  for (i = 0; i < subItemsNum; i++)
    if (subItems[i] == (CMFCRibbonBaseElement*)pButton)
      break;

  if (i < subItemsNum)
  {
    m_pButton->RemoveSubItem(i);
  }
}



/////////////////////////////////////////////////////////////////////////////////////////////
//  CUISeparator
/////////////////////////////////////////////////////////////////////////////////////////////

CUISeparator::CUISeparator(CMFCRibbonSeparator* pSeparator)
  : m_pSeparator(pSeparator)
{
}

CUISeparator::~CUISeparator()
{
}



/////////////////////////////////////////////////////////////////////////////////////////////
//  CUIGallery
/////////////////////////////////////////////////////////////////////////////////////////////


CUIGallery::CUIGallery(CMFCRibbonGallery*   pGallery)
  : m_pGallery(pGallery)
{
  CUIManager::getInstance()->registerCmd(pGallery->GetID(),
    FastDelegate0<>(this, &CUIGallery::onPush));
  CUIManager::getInstance()->registerCmdUI(pGallery->GetID(),
    FastDelegate2<bool&, bool&>(this, &CUIGallery::onUIDisplay));
}

CUIGallery::~CUIGallery()
{
  eventDestroy(this);

  m_pGallery->Clear();
  
  for (std::map<std::string, CMFCToolBarImages*>::iterator it = m_mapTB.begin();
       it != m_mapTB.end();
       it++)
  {
    CMFCToolBarImages* pTB = (*it).second;

    if (pTB)
    {
      while (pTB->GetCount() > 0)
        pTB->DeleteImage(0);

      delete pTB;
    }
  }

  m_mapTB.clear();
  m_vecItems.clear();

  delete m_pGallery;
}

void
CUIGallery::rename(const std::string&       sGalleryName)
{
  m_pGallery->SetText(sGalleryName.c_str());
}

void
CUIGallery::setButtonMode(bool              bButton)
{
  m_pGallery->SetButtonMode(bButton);
}

void
CUIGallery::addItem(const std::string&      sLabel,
                    const std::string&      sBitmapName,
                    const std::string&      sBitmapGroupName,
                    char*                   pGroupName)
{
  s_IWEImageData imgData;
  
  imgData.pData = GetResourceMgr()->getBitmapMaterial(sBitmapName, sBitmapGroupName, imgData.width, imgData.height, imgData.bpp);

  addItem(sLabel, imgData, pGroupName);
}

void
CUIGallery::addItem(const std::string&      sLabel,
                    const s_IWEImageData&   imgData,
                    char*                   pGroupName)
{
  if (vecItemExists(m_vecItems, sLabel))
    return;

  std::map<std::string, CMFCToolBarImages*>::const_iterator it;
  CMFCToolBarImages* pTB = NULL;

  if ((it = m_mapTB.find(pGroupName)) != m_mapTB.end())
    pTB = (*it).second;

  else
  {
    pTB = new CMFCToolBarImages();
    pTB->SetImageSize(CSize(50, 50), FALSE);

    //m_pGallery->AddGroup(pGroupName, *pTB);
    m_mapTB[pGroupName] = pTB;

    int iPos = 0;

    if (m_vecGroups.size() > 0)
      iPos = m_vecGroups.back().second;

    m_vecGroups.push_back(std::pair<std::string, int>(pGroupName, iPos));
  }

  CBitmap bmp;

  CBitmapFromData(bmp, imgData, 50, 50);
  pTB->AddImage(bmp);

  int iPos = -1;
  for (int i = 0; i < m_vecGroups.size(); i++)
  {
    std::pair<std::string, int>& grp = m_vecGroups[i];

    if (grp.first == pGroupName)
    {
      iPos = grp.second;
      grp.second = grp.second++;
    }
    else if (iPos >= 0)
      grp.second = grp.second++;
  }

  std::vector<std::string>::iterator itemIt = m_vecItems.begin();
  for (; itemIt != m_vecItems.end() && iPos-- > 0; itemIt++)
    ;

  m_vecItems.insert(itemIt, sLabel);

  m_pGallery->Clear();
  for (it = m_mapTB.begin(); it != m_mapTB.end(); it++)
    m_pGallery->AddGroup((*it).first.c_str(), *(*it).second);

  //CUIMainMenu* pMM = dynamic_cast<CUIMainMenu*>(CUIManager::getInstance()->getMainMenu());
  //pMM->refreshLayout();
}

void
CUIGallery::getSelectedItem(std::string& sItemOut, int& iItemOut)
{
  iItemOut = m_pGallery->GetSelectedItem();

  if (iItemOut >= 0 && iItemOut < m_vecItems.size())
    sItemOut = m_vecItems[iItemOut];
}

void
CUIGallery::onPush()
{
  int id = m_pGallery->GetSelectedItem();

  if (id >= 0 && id < m_vecItems.size())
    eventItemSelected(id, m_vecItems[id]);
}

void
CUIGallery::onUIDisplay(bool& bEnabled, bool& bChecked)
{
  bEnabled = bChecked = false;

  eventUIDisplay(bEnabled, bChecked);
}



/////////////////////////////////////////////////////////////////////////////////////////////
//  CUISlider
/////////////////////////////////////////////////////////////////////////////////////////////

CUISlider::CUISlider(CMFCRibbonSlider* pSlider)
  : m_pSlider(pSlider)
{
  CUIManager::getInstance()->registerCmd(pSlider->GetID(),
    FastDelegate0<>(this, &CUISlider::onPush));
  CUIManager::getInstance()->registerCmdUI(pSlider->GetID(),
    FastDelegate2<bool&, bool&>(this, &CUISlider::onUIDisplay));
}

CUISlider::~CUISlider()
{
  eventDestroy(this);

  delete m_pSlider;
}

void
CUISlider::setMin(int iMin)
{
  m_pSlider->SetRange(iMin, m_pSlider->GetRangeMax());
}

void
CUISlider::setMax(int iMax)
{
  m_pSlider->SetRange(m_pSlider->GetRangeMin(), iMax);
}

void
CUISlider::setPosition(int iPos)
{
  m_pSlider->SetPos(iPos, true);
}

void
CUISlider::onPush()
{
  eventValueChanged(m_pSlider->GetPos());
}

void
CUISlider::onUIDisplay(bool& bEnabled, bool& bChecked)
{
  eventUIDisplay(bEnabled, bChecked);
}
