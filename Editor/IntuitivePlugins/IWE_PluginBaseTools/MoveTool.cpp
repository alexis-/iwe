#include "IWEPlugin.h"

struct s_moveMemento
{
  std::vector<ISelectableTarget*>     m_vecTargets;
  std::vector<Ogre::Vector3>          m_vecPos;
};

class CMoveTool : public CTool, public IMementoObject
{
public:
	CMoveTool()
    : CTool(CPlugin::getInstance(), "MoveTool", "Home", "Base", "Move", "MoveToolIcon.bmp", "Plugins", 0, 1, 3),
			m_bLButton(false),
			m_bShift(false),
			m_bCtrl(false),
			m_pEnt(NULL),
      m_bPendingSave(false)
	{
    CPlugin::getInstance()->registerTool(this);
	}

	virtual bool			  onMouseInput(const s_inputDatas& datas, IViewport* pViewport)
	{
    e_inputMouseButton type = datas.m_MouseButton;

		if (type == IPT_MOUSE_LEFT)
		{
			m_bLButton = datas.m_MouseAction;
      m_pEnt = (m_bLButton) ? pViewport->PickTarget(datas.m_Pt, m_lastPos) : NULL;

      if (!m_bLButton && m_bPendingSave)
      {
        m_curState = createState();
        m_bPendingSave = false;
      }
		}

		else if (type == IPT_MOUSE_MOVE && m_bLButton && m_pEnt)
		{
			Ogre::Vector3				vecCursorPos;

      if (!m_bPendingSave)
        saveMove();

			if (m_bShift)
				vecCursorPos = pViewport->GetPlaneCoord(datas.m_Pt.x, datas.m_Pt.y, pViewport->getCamera()->getDirection(), m_lastPos);
			else
				vecCursorPos = pViewport->GetHeightCoord(datas.m_Pt.x, datas.m_Pt.y, m_lastPos.y);
			
			Ogre::Vector3				vecDiff = (vecCursorPos - m_lastPos) * (m_bShift ? Ogre::Vector3(0.0f, 1.0f, 0.0f) : Ogre::Vector3(1.0f, 0.0f, 1.0f));

      const std::vector<ISelectableTarget*>& vecTargets = GetEditorInteraction()->getSelectedTargets();

      for (std::vector<ISelectableTarget*>::const_iterator it = vecTargets.begin();
					 it != vecTargets.end();
					 it++)
			{
        if (!m_bShift || !m_bCtrl)
          (*it)->setTargetPosition((*it)->getTargetPosition() + vecDiff);
				else
          (*it)->setTargetPosition((*it)->getTargetPosition() * Ogre::Vector3(1.0f, 0.0f, 1.0f));
			}

      m_bPendingSave = true;
      m_lastPos = vecCursorPos;
		}

		return true;
	}

	virtual void			  onMouseLeave()
	{
		//m_bLButton = m_bShift = m_bCtrl = false;
	}

	virtual void			  onKeyboardInput(IWEKeys::e_IWEKey nChar, bool keyDown, int repeatCount, IViewport* pViewport)
	{
    if (nChar == IWEKeys::Shift)
			m_bShift = keyDown;

    else if (nChar == IWEKeys::Control)
			m_bCtrl = keyDown;
	}

	virtual void			  onToolActivate()
	{
		m_bCtrl = false;
		m_bShift = false;
		m_bLButton = false;
		m_pEnt = NULL;
	}

  s_moveMemento       createState()
  {
    const std::vector<ISelectableTarget*>& vecTargets = GetEditorInteraction()->getSelectedTargets();
    s_moveMemento moveMemento;

    for (std::vector<ISelectableTarget*>::const_iterator it = vecTargets.begin();
					it != vecTargets.end();
					it++)
    {
      moveMemento.m_vecTargets.push_back(*it);
      moveMemento.m_vecPos.push_back((*it)->getTargetPosition());
    }

    return moveMemento;
  }

  void                saveMove()
  {
    m_stackUndo.push(createState());
    GetEditorInteraction()->saveState(this);
  }

  virtual void        undo()
  {
    s_moveMemento state = m_stackUndo.top();
    
    m_stackUndo.pop();
    m_stackRedo.push(m_curState);

    for (int i = 0; i < state.m_vecTargets.size(); i++)
      state.m_vecTargets[i]->setTargetPosition(state.m_vecPos[i]);

    m_curState = state;
  }

  virtual void        redo()
  {
    s_moveMemento state = m_stackRedo.top();
    
    m_stackRedo.pop();
    m_stackUndo.push(m_curState);

    for (int i = 0; i < state.m_vecTargets.size(); i++)
      state.m_vecTargets[i]->setTargetPosition(state.m_vecPos[i]);

    m_curState = state;
  }

  virtual void        forgetLast()
  {
  }

  virtual void        forgetFirst()
  {
    m_stackRedo.pop();
  }

  virtual bool        onInit()
  {
    IUIMainMenu* pMainMenu = GetGUI()->getUIManager()->getMainMenu();
    
    IUICategory* pCat = pMainMenu->addCategory(m_sCategory, m_nInsertCategoryAt);
    IUIPanel* pPanel = pCat->addPanel(m_sPanel, m_nInsertPanelAt);
    
    pPanel->addSeparator(m_nInsertButtonAt - 1);
    m_pButton = pPanel->addButton(m_sLabel, m_sImgName, m_sImgGroup, m_nInsertButtonAt);
    
    m_pButton->setLargeButton(m_bLarge);

    CPlugin::getInstance()->getEditor()->getToolMgr()->registerTool(this, m_pButton, m_bGlobal);

    return false;
  }
	
private:
	bool							          m_bLButton;
	bool							          m_bShift;
	bool							          m_bCtrl;
	Ogre::Vector3		            m_lastPos;
	ISelectableTarget*	        m_pEnt;
  
  bool                        m_bPendingSave;
  
  s_moveMemento               m_curState;
  std::stack<s_moveMemento>   m_stackUndo;
  std::stack<s_moveMemento>   m_stackRedo;
};

CMoveTool g_MoveTool;