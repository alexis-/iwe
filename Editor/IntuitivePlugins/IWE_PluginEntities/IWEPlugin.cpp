#include "IWEPlugin.h"

#include "EntityManager.h"
#include "CBaseEntity.h"

#include "RenderHelper/CommonUtils.h"
#include "RenderHelper/EngineUtils.h"

CPlugin* CPlugin::m_pInst = NULL;

IPlugin* getPluginInterface()
{
  return CPlugin::getInstance();
}


CPlugin*
CPlugin::getInstance()
{
  if (!m_pInst)
    m_pInst = new CPlugin();

  return m_pInst;
}

CPlugin::CPlugin()
  : m_pGUI(NULL),
    m_pEngine(NULL),
    m_pEditor(NULL),
    m_pAssetsCat(NULL),
    m_DragEnt(NULL),
    m_pOverEnt(NULL)
{
}

void testAnimationPush();
void testAnimationCmdUI(bool&, bool&);

void
CPlugin::initialize(IGUIInterface*           pGUI,
                    IEngineInterface*        pEngine,
                    IEditorInterface*        pEditor)
{
  m_pGUI = pGUI;
  m_pEngine = pEngine;
  m_pEditor = pEditor;

  GetUIManager()->eventDragEnter +=
    FastDelegate3<const std::string&, IViewport*, const CIWEPoint<int>&, e_dropEffect>(this, &CPlugin::onDragEnter);
  GetUIManager()->eventDragOver +=
    FastDelegate2<IViewport*, const CIWEPoint<int>&, e_dropEffect>(this, &CPlugin::onDragOver);
  GetUIManager()->eventDrop +=
    FastDelegate2<IViewport*, const CIWEPoint<int>&, bool>(this, &CPlugin::onDrop);
  GetUIManager()->eventDragLeave +=
    FastDelegate1<IViewport*>(this, &CPlugin::onDragLeave);

  GetEditorInteraction()->requestTargetPick +=
    FastDelegate4<CIWEPoint<int>, IViewport*, float&, Ogre::Vector3&, ISelectableTarget*>(this, &CPlugin::onPickRequest);

  GetRenderEngine()->eventRender +=
    FastDelegate1<float>(this, &CPlugin::onRender);

  IUIProjectTree*           pProjTree = GetUIManager()->getProjectTree();

  m_pAssetsCat = pProjTree->addCategory("Assets");
  m_pAssetsCat->addRegexFilter(".*\\.mesh");

  eventPostInit(pGUI, pEngine, pEditor);
}

bool
CPlugin::onMouseInput(const s_inputDatas&    datas,
                      IViewport*             pViewport)
{
#if defined (_DEBUG)
  static DWORD lastPickTime = 0;
  static uint uPickDelay = 5;

  if (datas.m_MouseButton == IPT_MOUSE_MOVE && lastPickTime + uPickDelay < GetTickCount())
  {
#else
  if (datas.m_MouseButton == IPT_MOUSE_MOVE)
  {
#endif // _DEBUG

    Ogre::Vector3 vecPickedPosOut;
    float fDistOut;



    CBaseEntity* pPickedEnt = RenderHelper::PickCustom<CBaseEntity>(datas.m_Pt.x, datas.m_Pt.y, GetRenderEngine()->getSceneManager(), pViewport, fDistOut, vecPickedPosOut, "CBaseEntity");

#if defined(_DEBUG)
    lastPickTime = GetTickCount();
#endif

    if (pPickedEnt && pPickedEnt != m_pOverEnt)
    {
      if (m_pOverEnt && m_pOverEnt->isTargetSelected() == false)
        m_pOverEnt->displayWireframe(false);

      if (pPickedEnt->isTargetSelected() == false)
      {
        m_pOverEnt = pPickedEnt;
        m_pOverEnt->displayWireframe(true);
      }
      else
        m_pOverEnt = NULL;
    }
    else if (!pPickedEnt && m_pOverEnt)
    {
      if (m_pOverEnt->isTargetSelected() == false)
        m_pOverEnt->displayWireframe(false);

      m_pOverEnt = NULL;
    }
  }

  return true;
}

void
CPlugin::onKeyboardInput(IWEKeys::e_IWEKey   nChar,
                         bool                keyDown,
                         int                 repeatCount,
                         IViewport*          pViewport)
{
}

void
CPlugin::onMouseLeave(IViewport*             pViewport)
{
}

void
CPlugin::onRender(float fDelta)
{
  m_vecEntities.execute();
  auto it = m_vecEntities.begin();

  for (; it != m_vecEntities.end(); it++)
    (*it)->renderFrame(fDelta);
}

e_dropEffect
CPlugin::onDragEnter(const std::string& sDragText, IViewport* pViewport, const CIWEPoint<int>& pt)
{
  if (hasCIEnding(sDragText, ".mesh"))
  {
    try
    {
      Ogre::Vector3 gridPos = pViewport->GetHeightCoord(pt.x, pt.y, 0); // TODO: Replace 0 with grid's height getter
      m_DragEnt = CEntityManager::getInstance()->createEntity();

      if (!m_DragEnt)
        return UI_DROPEFFECT_NONE;

      if (!m_DragEnt->setModel(sDragText))
      {
        delete m_DragEnt;
        m_DragEnt = NULL;

        return UI_DROPEFFECT_NONE;
      }

      m_DragEnt->setPosition(gridPos);

      return UI_DROPEFFECT_COPY;
    }
    catch (std::exception e)
    {
      printf(e.what());
    }
  }

  return UI_DROPEFFECT_NONE;
}

e_dropEffect
CPlugin::onDragOver(IViewport* pViewport, const CIWEPoint<int>& pt)
{
	if (m_DragEnt != NULL)
	{
    Ogre::Vector3 gridPos = pViewport->GetHeightCoord(pt.x, pt.y, 0); // TODO: Replace 0 with grid's height getter
		m_DragEnt->setPosition(gridPos);

		return UI_DROPEFFECT_COPY;
	}
  
  return UI_DROPEFFECT_NONE;
}

bool
CPlugin::onDrop(IViewport* pViewport, const CIWEPoint<int>& pt)
{
	if (m_DragEnt != NULL)
	{
    Ogre::Vector3 gridPos = pViewport->GetHeightCoord(pt.x, pt.y, 0); // TODO: Replace 0 with grid's height getter
		m_DragEnt->setPosition(gridPos);

    dynamic_cast<CBaseEntity*>(m_DragEnt)->initialCreation();

		m_DragEnt = NULL;

		return true;
	}

  return false;
}

void
CPlugin::onDragLeave(IViewport* pViewport)
{
	if (m_DragEnt != NULL)
	{
		delete m_DragEnt;
		m_DragEnt = NULL;
	}
}

ISelectableTarget*
CPlugin::onPickRequest(CIWEPoint<int> pt, IViewport* pViewport, float& fDistOut, Ogre::Vector3& vecPickedPosOut)
{
  return RenderHelper::PickSelectableTarget(pt.x, pt.y, GetRenderEngine()->getSceneManager(), pViewport, fDistOut, vecPickedPosOut, "CBaseEntity");
}