#include "IWEPlugin.h"

#include "CLight.h"

class CCreateTool : public CTool
{
public:
	CCreateTool()
    : CTool(CPlugin::getInstance(), "LightScreateTool", "Home", "Light", "Create Light", "ScaleToolIcon.bmp", "Plugins", 0, 1, 0),
			m_bLButton(false),
			m_bShift(false),
			m_bCtrl(false),
			m_pLight(NULL)
	{
    CPlugin::getInstance()->registerTool(this);
	}

	virtual bool			  onMouseInput(const s_inputDatas& datas, IViewport* pViewport)
	{
    e_inputMouseButton type = datas.m_MouseButton;

    if (type == IPT_MOUSE_LEFT && !datas.m_MouseAction && m_pLight)
		{
      m_pLight->initialCreation();

      m_pLight = NULL;
		}

		else if (type == IPT_MOUSE_MOVE)
		{
      if (!m_pLight)
        m_pLight = new CLight();

			Ogre::Vector3				vecCursorPos;

			if (m_bShift)
				vecCursorPos = pViewport->GetPlaneCoord(datas.m_Pt.x, datas.m_Pt.y, pViewport->getCamera()->getDirection(), m_pLight->getTargetPosition());
			else
        vecCursorPos = pViewport->GetHeightCoord(datas.m_Pt.x, datas.m_Pt.y, m_pLight->getTargetPosition().y);
      
      if (!m_bShift || !m_bCtrl)
        m_pLight->setTargetPosition(vecCursorPos);
			else
        m_pLight->setTargetPosition(m_pLight->getTargetPosition() * Ogre::Vector3(1.0f, 0.0f, 1.0f));      
		}

		return false;
	}

	virtual void			  onMouseLeave()
	{
    if (m_pLight)
    {
      delete m_pLight;

      m_pLight = NULL;
    }
	}

	virtual void			  onKeyboardInput(IWEKeys::e_IWEKey nChar, bool keyDown, int repeatCount, IViewport* pViewport)
	{
    if (nChar == IWEKeys::Control)
			m_bCtrl = keyDown;
    
    if (nChar == IWEKeys::Shift)
			m_bCtrl = keyDown;
	}

	virtual void			  onToolActivate()
	{
		m_bCtrl = false;
    m_bShift = false;
		m_bLButton = false;
		m_pLight = NULL;
	}
	
private:
	bool							  m_bLButton;
	bool							  m_bShift;
	bool							  m_bCtrl;
	CLight*	            m_pLight;
};

CCreateTool g_CreateTool;