#ifndef ENGINE_INTERFACE_H
# define ENGINE_INTERFACE_H


class CEngineInterface : public IEngineInterface
{
public:
  static CEngineInterface*            getInstance();

public:
  virtual IRenderEngine*              getRenderEngine();
  virtual IResourceManager*           getResourceMgr();

  virtual void                        logMessage(const char* pMsg, ...);

public:
  virtual void                        initialize(IEditorInterface* pEditor, IGUIInterface* pGUI);
  virtual void                        initialize(void* pCustomData);

  inline IEditorInterface*            getEditor()
  {
    return m_pEditor;
  }

  inline IGUIInterface*               getGUI()
  {
    return m_pGUI;
  }

private:
  CEngineInterface() {};
  static CEngineInterface*            m_pInst;

  IEditorInterface*                   m_pEditor;
  IGUIInterface*                      m_pGUI;
};


extern "C" __declspec(dllexport) IEngineInterface* getEngineInterface();

inline IEditorInteraction* GetEditorInteraction()
{
  return CEngineInterface::getInstance()->getEditor()->getEditorInteraction();
}

inline IGUIInterface* GetGUI()
{
  return CEngineInterface::getInstance()->getGUI();
}

#endif // !ENGINE_INTERFACE_H