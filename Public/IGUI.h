#ifndef IGUI_H
# define IGUI_H

interface_class IGUIInterface
{
public:
  virtual IUIManager*         getUIManager() = 0;

public:
  virtual void                showCursor(bool bShow) = 0;

  virtual CIWEPoint<int>      getCursorPos() = 0;
  virtual void                setCursorPos(int x, int y) = 0;
  
  virtual void                lockRender() = 0;
  virtual void                unlockRender() = 0;

public:
  virtual void                findPlugins(const std::string& sPluginFolder,
                                          std::vector<std::string>& vecFiles) = 0;
  virtual IPlugin*            loadPlugin(const std::string& sPluginPath) = 0;

public:
  Event0<>                    eventPostInit;
};


#endif // !IGUI_H