#include "IWEPlugin.h"

#include <OgreFontManager.h>

#include "OgreAL.h"
#include "CSound.h"

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
    m_DragEnt(NULL),
    m_pAssetsCat(NULL)
{
}

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

  m_pAssetsCat = pProjTree->addCategory("Sounds");
  m_pAssetsCat->addRegexFilter(".*\\.(wav|ogg)");

  COgreAL::getInstance()->init();

  eventPostInit(pGUI, pEngine, pEditor);
}

bool
CPlugin::onMouseInput(const s_inputDatas&    datas,
                      IViewport*             pViewport)
{
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

void CPlugin::onRender(float fDelta)
{
  COgreAL::getInstance()->update(fDelta);
}

e_dropEffect
CPlugin::onDragEnter(const std::string& sDragText, IViewport* pViewport, const CIWEPoint<int>& pt)
{
  if (hasCIEnding(sDragText, ".ogg") || hasCIEnding(sDragText, ".wav"))
  {
    try
    {
      Ogre::Vector3 gridPos = pViewport->GetHeightCoord(pt.x, pt.y, 0); // TODO: Replace 0 with grid's height getter
      m_DragEnt = new CSound();

      if (!m_DragEnt)
        return UI_DROPEFFECT_NONE;

      if (!m_DragEnt->setSound(sDragText))
      {
        delete m_DragEnt;
        m_DragEnt = NULL;

        return UI_DROPEFFECT_NONE;
      }

      m_DragEnt->setTargetPosition(gridPos);

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
		m_DragEnt->setTargetPosition(gridPos);

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
		m_DragEnt->setTargetPosition(gridPos);
    m_DragEnt->initialCreation();

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
  return RenderHelper::PickSelectableTarget(pt.x, pt.y, GetRenderEngine()->getSceneManager(), pViewport, fDistOut, vecPickedPosOut, "CSound");
}