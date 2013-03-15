#ifndef IEDITOR_INTERFACE_H
# define IEDITOR_INTERFACE_H

interface_class IEngineInterface;

interface_class IEditorInterface
{
public:
  virtual IEditorInteraction*               getEditorInteraction() = 0;
  virtual IProjectManager*                  getProjectMgr() = 0;
  virtual IToolManager*                     getToolMgr() = 0;

public:
  virtual void                              initialize(IEngineInterface* pEngine, IGUIInterface* pGUI) = 0;
};



#endif // !IEDITOR_INTERFACE_H