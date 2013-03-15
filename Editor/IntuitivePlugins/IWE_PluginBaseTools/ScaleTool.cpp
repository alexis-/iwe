#include "IWEPlugin.h"

struct s_scaleMemento
{
  std::vector<ISelectableTarget*>     m_vecTargets;
  std::vector<Ogre::Vector3>          m_vecScale;
};

class CScaleTool : public CTool, public IMementoObject
{
public:
	CScaleTool()
    : CTool(CPlugin::getInstance(), "ScaleTool", "Home", "Base", "Scale", "ScaleToolIcon.bmp", "Plugins", 0, 1, 5),
			m_bLButton(false),
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
      Ogre::Vector3 vecDummy;

			m_bLButton = datas.m_MouseAction;
      m_lastPos = datas.m_Pt;

      // We don't care about the picked pos... Use a dummy var
      m_pEnt = (m_bLButton) ? pViewport->PickTarget(datas.m_Pt, vecDummy) : NULL;

      if (!m_bLButton && m_bPendingSave)
      {
        m_curState = createState();
        m_bPendingSave = false;
      }
		}

		else if (type == IPT_MOUSE_MOVE && m_bLButton && m_pEnt)
		{
      if (!m_bPendingSave)
        saveMove();

			int yDelta = datas.m_Pt.y - m_lastPos.y;
      const std::vector<ISelectableTarget*>& vecTargets = GetEditorInteraction()->getSelectedTargets();

      for (std::vector<ISelectableTarget*>::const_iterator it = vecTargets.begin();
					 it != vecTargets.end();
					 it++)
      {
        if (!m_bCtrl)
				  (*it)->setTargetScale((*it)->getTargetScale() - (yDelta * 0.02f));
        else
          (*it)->setTargetScale(Ogre::Vector3::UNIT_SCALE);
      }
      
      m_bPendingSave = true;
      m_lastPos = datas.m_Pt;
		}

		return true;
	}

	virtual void			  onMouseLeave()
	{
	}

	virtual void			  onKeyboardInput(IWEKeys::e_IWEKey nChar, bool keyDown, int repeatCount, IViewport* pViewport)
	{
    if (nChar == IWEKeys::Control)
			m_bCtrl = keyDown;
	}

	virtual void			  onToolActivate()
	{
		m_bCtrl = false;
		m_bLButton = false;
		m_pEnt = NULL;
	}

  s_scaleMemento       createState()
  {
    const std::vector<ISelectableTarget*>& vecTargets = GetEditorInteraction()->getSelectedTargets();
    s_scaleMemento moveMemento;

    for (std::vector<ISelectableTarget*>::const_iterator it = vecTargets.begin();
					it != vecTargets.end();
					it++)
    {
      moveMemento.m_vecTargets.push_back(*it);
      moveMemento.m_vecScale.push_back((*it)->getTargetScale());
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
    s_scaleMemento state = m_stackUndo.top();
    
    m_stackUndo.pop();
    m_stackRedo.push(m_curState);

    for (int i = 0; i < state.m_vecTargets.size(); i++)
      state.m_vecTargets[i]->setTargetScale(state.m_vecScale[i]);

    m_curState = state;
  }

  virtual void        redo()
  {
    s_scaleMemento state = m_stackRedo.top();
    
    m_stackRedo.pop();
    m_stackUndo.push(m_curState);

    for (int i = 0; i < state.m_vecTargets.size(); i++)
      state.m_vecTargets[i]->setTargetScale(state.m_vecScale[i]);

    m_curState = state;
  }

  virtual void        forgetLast()
  {
  }

  virtual void        forgetFirst()
  {
    m_stackRedo.pop();
  }
	
private:
	bool							  m_bLButton;
	bool							  m_bCtrl;
	CIWEPoint<int>		  m_lastPos;
	ISelectableTarget*	m_pEnt;
  
  bool                        m_bPendingSave;
  
  s_scaleMemento              m_curState;
  std::stack<s_scaleMemento>  m_stackUndo;
  std::stack<s_scaleMemento>  m_stackRedo;
};

CScaleTool g_ScaleTool;