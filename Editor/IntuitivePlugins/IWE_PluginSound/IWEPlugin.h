#ifndef IWE_PLUGIN_H
# define IWE_PLUGIN_H

# include "iwebase.h"
# include "RenderHelper\Tool.h"
# include "RenderHelper\MementoObject.h"

class CSound;

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

private:
  e_dropEffect              onDragEnter(const std::string& sDragText, IViewport* pViewport, const CIWEPoint<int>& pt);
  e_dropEffect              onDragOver(IViewport* pViewport, const CIWEPoint<int>& pt);
  bool                      onDrop(IViewport* pViewport, const CIWEPoint<int>& pt);
  void                      onDragLeave(IViewport* pViewport);

  void                      onRender(float fDelta);

  ISelectableTarget*        onPickRequest(CIWEPoint<int> pt, IViewport* pViewport, float& fDistOut, Ogre::Vector3& vecPickedPosOut);

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
  
  CSound*                   m_DragEnt;
  IUIProjectTreeElement*    m_pAssetsCat;

private:
  CPlugin();
  static CPlugin*     m_pInst;
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