#ifndef IENGINE_INTERFACE_H
# define IENGINE_INTERFACE_H

interface_class IEditorInterface;

interface_class IEngineInterface
{
public:
  virtual IRenderEngine*              getRenderEngine() = 0;
  virtual IResourceManager*           getResourceMgr() = 0;

public:
  virtual void                        logMessage(const char* pMsg, ...) = 0;

public:
  virtual void                        initialize(IEditorInterface* pEditor, IGUIInterface* pGUI) = 0;
  virtual void                        initialize(void* pCustomData) = 0;
};



#endif // !IENGINE_INTERFACE_H