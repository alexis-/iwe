#ifndef IPLUGIN_H
# define IPLUGIN_H

interface_class IGUIInterface;
interface_class IEngineInterface;
interface_class IEditorInterface;

interface_class IViewport;

interface_class IPlugin
{
public:
  virtual void        initialize(IGUIInterface*           pGUI,
                                 IEngineInterface*        pEngine,
                                 IEditorInterface*        pEditor) = 0;

	virtual bool				onMouseInput(const s_inputDatas&    datas,
                                   IViewport*             pViewport) = 0;
  virtual void				onKeyboardInput(IWEKeys::e_IWEKey   nChar,
                                      bool                keyDown,
                                      int                 repeatCount,
                                      IViewport*          pViewport) = 0;
	virtual void				onMouseLeave(IViewport*             pViewport) = 0;

public:
  Event3<IGUIInterface*, IEngineInterface*, IEditorInterface*>      eventPostInit;
};

#endif // !IPLUGIN_H