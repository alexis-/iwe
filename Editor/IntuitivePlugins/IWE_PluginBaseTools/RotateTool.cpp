#include "IWEPlugin.h"

struct s_rotateMemento
{
  std::vector<ISelectableTarget*>     m_vecTargets;
  std::vector<Ogre::Quaternion>       m_quatOrient;
};

class CRotateTool : public CTool, public IMementoObject
{
public:
	CRotateTool()
    : CTool(CPlugin::getInstance(), "RotateTool", "Home", "Base", "Rotate", "RotateToolIcon.bmp", "Plugins", 0, 1, 4),
			m_bLButton(false),
			m_bCtrl(false),
			m_bShift(false),
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
        createState(m_curState);
        m_bPendingSave = false;
      }
		}

		else if (type == IPT_MOUSE_MOVE && m_bLButton && m_pEnt)
		{
      if (!m_bPendingSave)
        saveMove();

      const std::vector<ISelectableTarget*>& vecTargets = GetEditorInteraction()->getSelectedTargets();
			Ogre::Quaternion quatRotation;
			
			if (!m_bShift)
			{
				float xDelta = (datas.m_Pt.x - m_lastPos.x) * 0.01f;
				Ogre::Radian radian(xDelta);
				quatRotation = Ogre::Quaternion(radian, Ogre::Vector3::UNIT_Y);
			}
			else
			{
				float yDelta = (datas.m_Pt.y - m_lastPos.y) * 0.01f;
				Ogre::Radian radian(yDelta);
				quatRotation = Ogre::Quaternion(radian, Ogre::Vector3::UNIT_X);
			}

      for (std::vector<ISelectableTarget*>::const_iterator it = vecTargets.begin();
					 it != vecTargets.end();
					 it++)
				if (!m_bCtrl)
					(*it)->setTargetOrientation((*it)->getTargetOrientation() * quatRotation);
				else
					(*it)->setTargetOrientation(Ogre::Quaternion::IDENTITY);
      
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

  void                 createState(s_rotateMemento& moveMemento)
  {
    const std::vector<ISelectableTarget*>& vecTargets = GetEditorInteraction()->getSelectedTargets();

    for (std::vector<ISelectableTarget*>::const_iterator it = vecTargets.begin();
					it != vecTargets.end();
					it++)
    {
      moveMemento.m_vecTargets.push_back(*it);
      moveMemento.m_quatOrient.push_back((*it)->getTargetOrientation());
    }
  }

  void                saveMove()
  {
    s_rotateMemento moveMemento;

    createState(moveMemento);
    m_stackUndo.push(moveMemento);
    GetEditorInteraction()->saveState(this);
  }

  virtual void        undo()
  {
    s_rotateMemento state = m_stackUndo.top();
    
    m_stackUndo.pop();
    m_stackRedo.push(m_curState);

    for (int i = 0; i < state.m_vecTargets.size(); i++)
      state.m_vecTargets[i]->setTargetOrientation(state.m_quatOrient[i]);

    m_curState = state;
  }

  virtual void        redo()
  {
    s_rotateMemento state = m_stackRedo.top();
    
    m_stackRedo.pop();
    m_stackUndo.push(m_curState);

    for (int i = 0; i < state.m_vecTargets.size(); i++)
      state.m_vecTargets[i]->setTargetOrientation(state.m_quatOrient[i]);

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
	bool							  m_bShift;
	CIWEPoint<int>		  m_lastPos;
	ISelectableTarget*	m_pEnt;
  
  bool                          m_bPendingSave;
  
  s_rotateMemento               m_curState;
  std::stack<s_rotateMemento>   m_stackUndo;
  std::stack<s_rotateMemento>   m_stackRedo;
};

CRotateTool g_RotateTool;