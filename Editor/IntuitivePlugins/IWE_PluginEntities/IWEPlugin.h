#ifndef IWE_PLUGIN_H
# define IWE_PLUGIN_H

# include <OgreRoot.h>
# include <OgreSceneManager.h>

# include "iwebase.h"
# include "RenderHelper\Tool.h"
# include "RenderHelper\MementoObject.h"

# include "IEntity.h"
# include "IEntityManager.h"

class CBaseEntity;

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

  ISelectableTarget*        onPickRequest(CIWEPoint<int> pt, IViewport* pViewport, float& fDistOut, Ogre::Vector3& vecPickedPosOut);

  void                      onRender(float fDelta);

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

public:
  void                      addEntity(IEntity* pEnt)
  {
    m_vecEntities.push_back(pEnt);
  }
  
  void                      removeEntity(IEntity* pEnt)
  {
    auto it = m_vecEntities.begin();

    for (; it != m_vecEntities.end(); it++)
      if (*it == pEnt)
        break;

    if (it != m_vecEntities.end())
      m_vecEntities.erase(it);
  }

private:
  IGUIInterface*            m_pGUI;
  IEngineInterface*         m_pEngine;
  IEditorInterface*         m_pEditor;

  std::vector<ITool*>       m_vecTools;

  CBaseEntity*              m_pOverEnt;
  IEntity*                  m_DragEnt;
  IUIProjectTreeElement*    m_pAssetsCat;

  delayed_vector<IEntity*>  m_vecEntities;

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