#include "stdafx.h"

#include <float.h>

#include "EditorInteraction.h"
#include "ToolManager.h"
#include "ViewportGrid.h"
#include "ProjectManager.h"

// Engine stuffs moved to Engine module
//#include "EntityManager.h"
//#include "EngineUtils.h"

//#include "Viewport.h"

CEditorInteraction*						CEditorInteraction::m_pInst = NULL;

CEditorInteraction::CEditorInteraction()
	: m_bShift(false)
{
}

CEditorInteraction*
CEditorInteraction::getInstance()
{
	if (!m_pInst)
		m_pInst = NEW CEditorInteraction();

	return m_pInst;
}

void
CEditorInteraction::init()
{
  GetRenderEngine()->eventViewportCreated +=
    FastDelegate1<IViewport*>(this, &CEditorInteraction::onViewportCreated);
  GetRenderEngine()->eventCameraControllerCreated +=
    FastDelegate1<ICameraController*>(this, &CEditorInteraction::onCameraControllerCreated);
}

void
CEditorInteraction::postInit()
{
  IUIMainMenu* pMainMenu = GetUIManager()->getMainMenu();

  IUICategory* pCat = pMainMenu->addCategory("Home", 0);
  IUIPanel* pPanel = pCat->addPanel("Base", 0);
  //
  //s_IWEImageData imgData;
  //
  //imgData.pData = GetRenderEngine()->getBitmapMaterial("IWEIcon_32.bmp", "Editor", imgData.width, imgData.height, imgData.bpp);
  //
  m_pUndoButton = pPanel->addButton("Undo", "UndoIcon.bmp", "Editor", 0);
  m_pRedoButton = pPanel->addButton("Redo", "RedoIcon.bmp", "Editor", 1);
  
  m_pUndoButton->setLargeButton(true);
  m_pUndoButton->eventPushed +=
    FastDelegate0<>(this, &CEditorInteraction::onUndo);
  m_pUndoButton->eventUIDisplay +=
    FastDelegate2<bool&, bool&>(this, &CEditorInteraction::onUndoUIEvent);

  m_pRedoButton->setLargeButton(true);
  m_pRedoButton->eventPushed +=
    FastDelegate0<>(this, &CEditorInteraction::onRedo);
  m_pRedoButton->eventUIDisplay +=
    FastDelegate2<bool&, bool&>(this, &CEditorInteraction::onRedoUIEvent);
  
  pPanel = pCat->addPanel("Settings", -1);
  m_pConfigButton = pPanel->addButton("Settings", "Settings.bmp", "Editor");
  
  m_pConfigButton->setLargeButton(true);
  m_pConfigButton->eventUIDisplay +=
    FastDelegate2<bool&, bool&>(this, &CEditorInteraction::onUIEvent);



  //IUIProjectTree*           pProjTree = GetUIManager()->getProjectTree();
  //IUIProjectTreeCategory*   pAssetsCat = pProjTree->addCategory("Assets");
  //pAssetsCat->addRegexFilter(".*\\.mesh");



  IGUIInterface* pGUI = CEditorInterface::getInstance()->getGUI();
  std::vector<std::string> vecPlugins;

  pGUI->findPlugins("./Plugins", vecPlugins);

  for (std::vector<std::string>::const_iterator it = vecPlugins.begin();
       it != vecPlugins.end();
       it++)
  {
    IPlugin* pPlugin = pGUI->loadPlugin(*it);

    if (!pPlugin)
      continue;

    pPlugin->initialize(pGUI, CEditorInterface::getInstance()->getEngine(), CEditorInterface::getInstance());

    m_vecPlugins.push_back(pPlugin);
  }
}

void
CEditorInteraction::onTargetDeleted(ISelectableTarget* pTarget)
{
  for (std::vector<ISelectableTarget*>::iterator it = CEditorDatas::m_vecPickedUnits.begin();
       it != CEditorDatas::m_vecPickedUnits.end();
       it++)
  {
    if (*it == pTarget)
    {
      CEditorDatas::m_vecPickedUnits.erase(it);
      break;
    }
  }
}

void
CEditorInteraction::onUndoUIEvent(bool& bEnabled, bool& bChecked)
{
  bEnabled = CProjectManager::getInstance()->isProjectLoaded() && !m_stackUndo.empty();
  bChecked = false;
}

void
CEditorInteraction::onRedoUIEvent(bool& bEnabled, bool& bChecked)
{
  bEnabled = CProjectManager::getInstance()->isProjectLoaded() && !m_stackRedo.empty();
  bChecked = false;
}

void
CEditorInteraction::onUIEvent(bool& bEnabled, bool& bChecked)
{
  bEnabled = CProjectManager::getInstance()->isProjectLoaded();
  bChecked = false;
}

void
CEditorInteraction::saveState(IMementoObject* pMementoObj)
{
  while (!m_stackRedo.empty())
  {
    m_stackRedo.top()->forgetFirst();
    m_stackRedo.pop();
  }

  m_stackUndo.push(pMementoObj);
}

void
CEditorInteraction::onUndo()
{
  IMementoObject* pMemObj = NULL;

  if (!m_stackUndo.empty())
  {
    pMemObj = m_stackUndo.top();

    m_stackRedo.push(pMemObj);
    m_stackUndo.pop();

    pMemObj->undo();
  }
}

void
CEditorInteraction::onRedo()
{
  IMementoObject* pMemObj = NULL;

  if (!m_stackRedo.empty())
  {
    pMemObj = m_stackRedo.top();

    m_stackUndo.push(pMemObj);
    m_stackRedo.pop();

    pMemObj->redo();
  }
}

void
CEditorInteraction::onCopy()
{
}

void
CEditorInteraction::onCut()
{
}

void
CEditorInteraction::onPaste()
{
}

void
CEditorInteraction::onSelectAll()
{
}

void
CEditorInteraction::onFind()
{
}

void
CEditorInteraction::onViewportCreated(IViewport* pViewport)
{
  if (pViewport->isEditorViewport())
  {
    CViewportGrid* pViewportGrid = NEW CViewportGrid(400, 400, 1.0f, "PerspectiveGrid", Ogre::Vector3::UNIT_Y, 1);

    pViewport->setUserData(pViewportGrid, "ViewportGrid");
  }
}

void
CEditorInteraction::onCameraMoved(ICameraController* pCameraController)
{
  eventCameraMoved(pCameraController);

  CViewportGrid* pViewportGrid = (CViewportGrid*)pCameraController->getViewport()->getUserData("ViewportGrid");

  if (pViewportGrid)
    pViewportGrid->OnCameraMoved(pCameraController->getViewport()->getCamera()->getPosition());
}

void
CEditorInteraction::onCameraRotated(ICameraController* pCameraController)
{
  eventCameraRotated(pCameraController);
}

void
CEditorInteraction::onCameraControllerCreated(ICameraController* pCameraController)
{
  IViewport* pViewport = pCameraController->getViewport();

  if (!pViewport)
    return;

  pViewport->setUserData(pCameraController, "CameraController");

  if (pViewport->isEditorViewport())
  {
    pCameraController->eventCameraMoved += FastDelegate1<ICameraController*>(this, &CEditorInteraction::onCameraMoved);
    pCameraController->eventCameraRotated += FastDelegate1<ICameraController*>(this, &CEditorInteraction::onCameraRotated);
  }
}

bool
CEditorInteraction::onMouseInput(const s_inputDatas& datas, IViewport* pViewport)
{
	CIWEPoint<int> point = datas.m_Pt;
  e_inputMouseButton type = datas.m_MouseButton;
  bool bPick = true;

	bPick = bPick && CToolManager::getInstance()->onMouseInput(datas, pViewport);

  for (std::vector<IPlugin*>::const_iterator it = m_vecPlugins.begin();
       bPick && it != m_vecPlugins.end();
       it++)
    bPick = bPick && (*it)->onMouseInput(datas, pViewport);
  
	if (bPick && type == IPT_MOUSE_LEFT && datas.m_MouseAction == IPT_MOUSE_DOWN)
	{
    Ogre::Vector3 pickedPos;
		ISelectableTarget* pPickedTarget = pViewport->PickTarget(point, pickedPos);

		if (pPickedTarget) // We found a target
		{
			if (!vecItemExists(CEditorDatas::m_vecPickedUnits, pPickedTarget)) // This target isn't selected yet
			{
				if (!m_bShift && !CEditorDatas::m_vecPickedUnits.empty()) // Deselect all other targets if shift isn't held
				{
					for (std::vector<ISelectableTarget*>::iterator it = CEditorDatas::m_vecPickedUnits.begin();
							 it != CEditorDatas::m_vecPickedUnits.end();
							 it++)
          {
            eventTargetUnselected(*it);

						(*it)->setTargetSelected(false);
            (*it)->eventTargetDeleted -=
              FastDelegate1<ISelectableTarget*>(this, &CEditorInteraction::onTargetDeleted);
          }

					CEditorDatas::m_vecPickedUnits.clear();
				}
        
        eventTargetSelected(pPickedTarget);
        pPickedTarget->setTargetSelected(true);
        pPickedTarget->eventTargetDeleted +=
          FastDelegate1<ISelectableTarget*>(this, &CEditorInteraction::onTargetDeleted);

				CEditorDatas::m_vecPickedUnits.push_back(pPickedTarget); // Select this target
			}
			else if (m_bShift && CEditorDatas::m_vecPickedUnits.size() > 1) // If it is selected and we are holding shift, deselect it
			{
				std::vector<ISelectableTarget*>::iterator it = vecGetItemIterator(CEditorDatas::m_vecPickedUnits, pPickedTarget);

				if (it != CEditorDatas::m_vecPickedUnits.end())
					CEditorDatas::m_vecPickedUnits.erase(it);
				else
					TRACE("CEditorInteraction::onMouseInput tried to remove a selected entity from"
							  " CEditorDatas::m_vecPickedUnits, but couldn't find its iterator"); // TODO: Log this
        
        eventTargetUnselected(pPickedTarget);
        pPickedTarget->setTargetSelected(false);
        pPickedTarget->eventTargetDeleted -=
          FastDelegate1<ISelectableTarget*>(this, &CEditorInteraction::onTargetDeleted);
			}
		}
		else if (!m_bShift && !CEditorDatas::m_vecPickedUnits.empty()) // We didn't pick any entity, if we are not holding shift, ...
		{
			if (!CEditorDatas::m_vecPickedUnits.empty()) // And we have already selected some units, ...
			{
				for (std::vector<ISelectableTarget*>::iterator it = CEditorDatas::m_vecPickedUnits.begin();
						 it != CEditorDatas::m_vecPickedUnits.end();
						 it++)
        {
          eventTargetUnselected(*it);

					(*it)->setTargetSelected(false);
          (*it)->eventTargetDeleted -=
            FastDelegate1<ISelectableTarget*>(this, &CEditorInteraction::onTargetDeleted);
        }
			
				CEditorDatas::m_vecPickedUnits.clear();
			}
		}
	}


  ICameraController* pCameraCtrl = (ICameraController*)pViewport->getUserData("CameraController");

  if (pCameraCtrl)
    return pCameraCtrl->onMouseInput(datas);

	return false;
}

void
CEditorInteraction::onKeyboardInput(IWEKeys::e_IWEKey nChar, bool keyDown, int repeatCount, IViewport* pViewport)
{
  CToolManager::getInstance()->onKeyboardInput(nChar, keyDown, repeatCount, pViewport);

  for (std::vector<IPlugin*>::const_iterator it = m_vecPlugins.begin();
       it != m_vecPlugins.end();
       it++)
    (*it)->onKeyboardInput(nChar, keyDown, repeatCount, pViewport);

  if (keyDown)
  {
    if (nChar == IWEKeys::Delete && !CEditorDatas::m_vecPickedUnits.empty())
    {
      //for (std::vector<ISelectableTarget*>::iterator it = CEditorDatas::m_vecPickedUnits.begin();
      //     it != CEditorDatas::m_vecPickedUnits.end();
      //     it++)
      ISelectableTarget* pTarget = NULL;
      int i = 0;

      while (CEditorDatas::m_vecPickedUnits.size() > i)
      {
        if (pTarget == CEditorDatas::m_vecPickedUnits[i])
        {
          i++;

          if (CEditorDatas::m_vecPickedUnits.size() <= i)
            break;
        }

        pTarget = CEditorDatas::m_vecPickedUnits[i];
        pTarget->deleteTarget();
      }

      CEditorDatas::m_vecPickedUnits.clear();
    }
  }
	
  if (nChar == IWEKeys::Shift) //LeftShift || nChar == IWEKeys::RightShift)
		m_bShift = keyDown;
  
  ICameraController* pCameraCtrl = (ICameraController*)pViewport->getUserData("CameraController");

  if (pCameraCtrl)
    pCameraCtrl->onKeyboardInput(nChar, keyDown, repeatCount);
}

void
CEditorInteraction::onMouseLeave(IViewport* pViewport)
{
  CToolManager::getInstance()->onMouseLeave();

  for (std::vector<IPlugin*>::const_iterator it = m_vecPlugins.begin();
       it != m_vecPlugins.end();
       it++)
    (*it)->onMouseLeave(pViewport);

  ICameraController* pCameraCtrl = (ICameraController*)pViewport->getUserData("CameraController");

  if (pCameraCtrl)
    pCameraCtrl->onMouseLeave();
}

void
CEditorInteraction::tick()
{
  const std::vector<IViewport*>& vecViewports = GetRenderEngine()->getViewports();
  
  for (std::vector<IViewport*>::const_iterator it = vecViewports.begin();
       it != vecViewports.end();
       it++)
  {
    IViewport* pViewport = *it;

    if (!pViewport)
      continue;

    ICameraController* pCameraCtrl = (ICameraController*)pViewport->getUserData("CameraController");

    if (pCameraCtrl)
      pCameraCtrl->tick();
  }
}

const std::vector<ISelectableTarget*>&
CEditorInteraction::getSelectedTargets()
{
  return CEditorDatas::m_vecPickedUnits;
}