#ifndef EDITOR_INTERFACE_H
# define EDITOR_INTERFACE_H

class CEditorInterface : public IEditorInterface
{
public:
  static CEditorInterface*             getInstance();

public:
  virtual IEditorInteraction*         getEditorInteraction();
  virtual IProjectManager*            getProjectMgr();
  virtual IToolManager*               getToolMgr();

public:
  virtual void                        initialize(IEngineInterface* pEngine, IGUIInterface* pGUI);

  inline IEngineInterface*            getEngine()
  {
    return m_pEngine;
  }

  inline IGUIInterface*               getGUI()
  {
    return m_pGUI;
  }

private:
  void                                onPostInit();

private:
  CEditorInterface();
  static CEditorInterface*            m_pInst;

  IEngineInterface*                   m_pEngine;
  IGUIInterface*                      m_pGUI;
};

extern "C" __declspec(dllexport) IEditorInterface* getEditorInterface();

inline IRenderEngine* GetRenderEngine()
{
  return CEditorInterface::getInstance()->getEngine()->getRenderEngine();
}

inline IResourceManager* GetResourceMgr()
{
  return CEditorInterface::getInstance()->getEngine()->getResourceMgr();
}

inline IUIManager* GetUIManager()
{
  return CEditorInterface::getInstance()->getGUI()->getUIManager();
}

#endif // !EDITOR_INTERFACE_H