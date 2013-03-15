#ifndef IUI_MAIN_MENU_H
# define IUI_MAIN_MENU_H

interface_class IUISeparator;
interface_class IUIButton;
interface_class IUIPanel;
interface_class IUICategory;
interface_class IUIGallery;
interface_class IUISlider;

interface_class IUIMainMenu
{
public:
  virtual IUICategory*        addCategory(const std::string& sCatName, int nInsertAt = 1) = 0;
};


interface_class IUICategory
{
public:
  virtual ~IUICategory() {};

  virtual void                rename(const std::string& sCatName) = 0;
  virtual void                setShortcut(const std::string& sShortcut) = 0;

public:
  virtual IUIPanel*           addPanel(const std::string& sPanelName, int nInsertAt = -1) = 0;
};


interface_class IUIPanel
{
public:
  virtual ~IUIPanel() {};
  
  virtual void                setShortcut(const std::string& sShortcut) = 0;

public:
  virtual IUIButton*          addButton(const std::string& sLabel, const std::string& sBitmapName, const std::string& sGroupName, int nInsertAt = -1) = 0;
  virtual IUIButton*          addButton(const std::string& sLabel, const s_IWEImageData& imgData, int nInsertAt = -1) = 0;

  virtual IUISeparator*       addSeparator(int nInsertAt = -1) = 0;

  virtual IUIGallery*         addGallery(const std::string& sLabel, int nInsertAt = -1) = 0;
  
  virtual IUISlider*          addSlider(const std::string& sLabel, int iMin, int iMax, int iPos, bool bButtons, int iInsertAt = -1) = 0;
};


interface_class IUIButton
{
public:
  virtual ~IUIButton() {};
  
  virtual void                rename(const std::string& sButtonName) = 0;
  virtual void                setShortcut(const std::string& sShortcut) = 0;

  virtual void                setLargeButton(bool bLarge) = 0;
  virtual void                setSplitButton(bool bSplit) = 0;

public:
  virtual IUIButton*          addButton(const std::string& sLabel, const std::string& sBitmapName, const std::string& sGroupName, int nInsertAt = -1) = 0;
  virtual IUIButton*          addButton(const std::string& sLabel, const s_IWEImageData& imgData, int nInsertAt = -1) = 0;
  virtual IUIButton*          addButton(const std::string& sLabel, int nInsertAt = -1) = 0;

  virtual IUISeparator*       addSeparator(int nInsertAt = -1) = 0;

public:
  Event0<>                    eventPushed;

  // Arg 1 : bEnabled - Is button enabled or disabled
  // Arg 2 : bChecked - Is button checked or not (Tools)
  Event2<bool&, bool&>        eventUIDisplay;
};


interface_class IUISeparator
{
public:
  virtual ~IUISeparator() {};
};


interface_class IUIGallery
{
public:
  virtual ~IUIGallery() {};
  
  virtual void                rename(const std::string& sGalleryName) = 0;

  virtual void                setButtonMode(bool bButton) = 0;
  virtual void                getSelectedItem(std::string& sItemOut, int& iItemOut) = 0;

public:
  virtual void                addItem(const std::string& sLabel, const std::string& sBitmapName, const std::string& sBitmapGroupName, char* pGroupName = "") = 0;
  virtual void                addItem(const std::string& sLabel, const s_IWEImageData& imgData, char* pGroupName = "") = 0;

public:
  Event2<int, const std::string&> eventItemSelected;

  // Arg 1 : bEnabled - Is button enabled or disabled
  // Arg 2 : bChecked - Is button checked or not (Tools)
  Event2<bool&, bool&>        eventUIDisplay;
};

interface_class IUISlider
{
public:
  virtual ~IUISlider() {};
  
  virtual void                setMin(int iMin) = 0;
  virtual void                setMax(int iMax) = 0;
  virtual void                setPosition(int iPos) = 0;
  
public:
  Event1<int>                 eventValueChanged;

  // Arg 1 : bEnabled - Is button enabled or disabled
  // Arg 2 : bChecked - Is button checked or not (Tools)
  Event2<bool&, bool&>        eventUIDisplay;
};


#endif // !IUI_MAIN_MENU_H