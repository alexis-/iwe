#ifndef GUI_INTERFACE_H
# define GUI_INTERFACE_H


#include "MemoryModule.h"


class CGUIInterface : public IGUIInterface
{
public:
  static CGUIInterface*                       getInstance();
  ~CGUIInterface();

  void                                        Init();

public:
  virtual IUIManager*                         getUIManager();
  
public:
  virtual void                                showCursor(bool bShow);

  virtual CIWEPoint<int>                      getCursorPos();
  virtual void                                setCursorPos(int x, int y);
  
  virtual void                                lockRender();
  virtual void                                unlockRender();

private:
  void*                                       loadPluginModule(const std::string& sFileName, bool bMemory);

public:
  virtual void                                findPlugins(const std::string& sPluginFolder,
                                                          std::vector<std::string>& vecFiles);
  virtual IPlugin*                            loadPlugin(const std::string& sPluginPath);

public:
  inline IEngineInterface*                    getEngineInterface()
  {
    return m_pEngineInterface;
  }

  inline IEditorInterface*                    getEditorInterface()
  {
    return m_pEditorInterface;
  }

private:
  IEngineInterface*                           m_pEngineInterface;
  IEditorInterface*                           m_pEditorInterface;

  std::vector<HMEMORYMODULE>                  m_vecMemModules;
  std::vector<HMODULE>                        m_vecModules;

private:
  CGUIInterface();
  static CGUIInterface*                       m_pInst;
};

inline IRenderEngine* GetRenderEngine()
{
  return CGUIInterface::getInstance()->getEngineInterface()->getRenderEngine();
}

inline IResourceManager* GetResourceMgr()
{
  return CGUIInterface::getInstance()->getEngineInterface()->getResourceMgr();
}

inline IProjectManager* GetProjectMgr()
{
  return CGUIInterface::getInstance()->getEditorInterface()->getProjectMgr();
}

inline IToolManager* GetToolMgr()
{
  return CGUIInterface::getInstance()->getEditorInterface()->getToolMgr();
}

inline IEditorInteraction* GetEditorInteraction()
{
  return CGUIInterface::getInstance()->getEditorInterface()->getEditorInteraction();
}


#endif // !GUI_INTERFACE_H