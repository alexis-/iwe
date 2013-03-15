#ifndef IWE_PLUGIN_H
# define IWE_PLUGIN_H

//#pragma managed(push, off)
//extern "C" void tss_cleanup_implemented(void) {}
//#include <boost/thread/thread.hpp>
//#pragma managed(pop) 

# include "iwebase.h"
# include "RenderHelper\Tool.h"
# include "RenderHelper\MementoObject.h"

class CPlugin : public IPlugin
{
public:
  static CPlugin*           getInstance();

  void                      registerTool(ITool* pTool)
  {
    m_vecTools.push_back(pTool);
  }

public:
  virtual void              initialize(IGUIInterface*           pGUI,
                                       IEngineInterface*        pEngine,
                                       IEditorInterface*        pEditor);

	virtual bool				      onMouseInput(const s_inputDatas&    datas,
                                         IViewport*             pViewport);
  virtual void				      onKeyboardInput(IWEKeys::e_IWEKey   nChar,
                                            bool                keyDown,
                                            int                 repeatCount,
                                            IViewport*          pViewport);
	virtual void				      onMouseLeave(IViewport*             pViewport);

public:
  IGUIInterface*            getGUI()
  {
    return m_pGUI;
  }

  IEngineInterface*         getEngine()
  {
    return m_pEngine;
  }

  IEditorInterface*         getEditor()
  {
    return m_pEditor;
  }

private:
  IGUIInterface*            m_pGUI;
  IEngineInterface*         m_pEngine;
  IEditorInterface*         m_pEditor;

  std::vector<ITool*>       m_vecTools;

private:
  CPlugin();
  static CPlugin*           m_pInst;
};

extern "C" __declspec(dllexport) IPlugin* getPluginInterface();


inline IRenderEngine* GetRenderEngine()
{
  return CPlugin::getInstance()->getEngine()->getRenderEngine();
}

inline IEditorInteraction* GetEditorInteraction()
{
  return CPlugin::getInstance()->getEditor()->getEditorInteraction();
}

inline IGUIInterface* GetGUI()
{
  return CPlugin::getInstance()->getGUI();
}

inline IUIManager* GetUIManager()
{
  return GetGUI()->getUIManager();
}


#endif // !IWE_PLUGIN_H