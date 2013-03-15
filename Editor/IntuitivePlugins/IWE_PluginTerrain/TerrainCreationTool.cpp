#include "IWEPlugin.h"
#include "TerrainManager.h"

#include <OgreSceneNode.h>
#include <OgreManualObject.h>


class CTerrainCreateTool;

class CTerrainGroupButtonProxy
{
public:
  CTerrainGroupButtonProxy(IUIButton* pButton, CTerrainCreateTool* pTool, uint nGroupID)
    : m_pButton(pButton),
      m_pTool(pTool),
      m_nGroupID(nGroupID)
  {
    pButton->eventPushed +=
      FastDelegate0<>(this, &CTerrainGroupButtonProxy::onCmd);
    pButton->eventUIDisplay +=
      FastDelegate2<bool&, bool&>(this, &CTerrainGroupButtonProxy::onCmdUI);
  }

private:
  void              onCmd();
  void              onCmdUI(bool& bEnabled, bool& bChecked);

private:
  IUIButton*            m_pButton;
  CTerrainCreateTool*   m_pTool;
  uint                  m_nGroupID;
};


class CTerrainCreateTool : public CTool
{
public:
	CTerrainCreateTool()
		: CTool(CPlugin::getInstance(), "TerrainCreateTool", "Terrain", "Terrain Tools", "New Terrain", "TerrainToolIcon.bmp", "Plugins", 1, 0, 0),
			m_fLastX(0),
			m_fLastY(0),
			m_pManualObject(NULL),
      m_pNode(NULL),
			m_bLButton(false),
      m_bAlt(false),
			m_fScale(CTerrainManager::m_fTerrainWorldSize),
      m_nGroupCount(0)
	{
    m_nID = -1;
	}
  
  void              onGroupSelected(uint nID)
  {
    m_nID = nID;

    if (CTerrainManager::getInstance()->isValidGroupID(m_nID))
    	m_fScale = CTerrainManager::getInstance()->getGroupSize(m_nID);

    m_pButton->eventPushed();
  }

private:
  float             round(float f)
  {
    return (f > 0.0) ? std::floor(f + 0.5) : std::ceil(f - 0.5);
  }
  
  void              SnapTo(float& x, float& y, float unitSize)
  {
		//x = ((float) ((Ogre::Math::Floor(x / unitSize)) * unitSize));
    //y = ((float) ((Ogre::Math::Floor(y / unitSize)) * unitSize));
		x = ((float) ((round(x / unitSize)) * unitSize));
    y = ((float) ((round(y / unitSize)) * unitSize));
  }

  float             getRealScale()
  {
    return (float)((int)m_fScale - (int)m_fScale % 10);
  }

  void              setTerrainPosition()
  {
    float x = m_fLastX;
    float y = m_fLastY;

    if (m_bAlt)
      SnapTo(x, y, 1);
    else
      SnapTo(x, y, getRealScale());

		m_pNode->setPosition(Ogre::Vector3((int)x - m_fWidthAdjust, 0.0f, (int)y - m_fHeightAdjust));
  }

  void              getTerrainPosition(float& x, float& y)
  {
    x = m_fLastX;
    y = m_fLastY;

    if (m_bAlt)
      SnapTo(x, y, 1);
    else
      SnapTo(x, y, getRealScale());
  }

  Ogre::RealRect    getTerrainRect()
  {
    float x = 0,
          y = 0;

    getTerrainPosition(x, y);

    return Ogre::RealRect(x,                    y,
    						          x + getRealScale(),   y + getRealScale());
  }

	virtual bool			onMouseInput(const s_inputDatas& datas, IViewport* pViewport)
	{
    e_inputMouseButton type = datas.m_MouseButton;

		if (type == IPT_MOUSE_LEFT && datas.m_MouseAction)
		{
			float		x = 0,
              y = 0;
			
      getTerrainPosition(x, y);
      Ogre::RealRect rect = getTerrainRect();

			if (m_pManualObject && CTerrainManager::getInstance()->isValidTerrainSpot(x, y, rect))
			{
				if (!CTerrainManager::getInstance()->isValidGroupID(m_nID))
				{
          float fWorldSize = getRealScale();

          m_nID = CTerrainManager::getInstance()->createTerrainGroup(fWorldSize);

					if (!CTerrainManager::getInstance()->isValidGroupID(m_nID))
						return true;

          if (m_nGroupCount++ == 0)
            m_pButton->addSeparator(0);

          std::stringstream sGroupName;
	        sGroupName << "Group " << (m_nID) << " (" << (int)(fWorldSize)
	        	<< "x" << (int)(fWorldSize) << ")";

          new CTerrainGroupButtonProxy(m_pButton->addButton(sGroupName.str(), 0), this, m_nID);
				}

				if (m_bAlt)
          CTerrainManager::getInstance()->createTerrain(x, y, m_nID, Ogre::Vector3(m_fLastX, 0.0f, m_fLastY));
        else
          CTerrainManager::getInstance()->createTerrain((int)(x / getRealScale()), (int)(y / getRealScale()), m_nID);
			}
		}

		else if (type == IPT_MOUSE_MOVE)
		{
      Ogre::Vector3		pos = pViewport->GetHeightCoord((float)datas.m_Pt.x, (float)datas.m_Pt.y, 0.0f); // TODO: Replace 0.0f with grid coord (?)
			float						x = pos.x;
			float						y = pos.z;
					
			m_fLastX = pos.x;
			m_fLastY = pos.z;

      m_pManualObject->setVisible(true);
      setTerrainPosition();

      Ogre::RealRect rect = getTerrainRect();

			if (CTerrainManager::getInstance()->checkTerrainIntersect(rect))
				m_pManualObject->setMaterialName(0, "Editor/Vertex/Selected");
			else
				m_pManualObject->setMaterialName(0, "Editor/Plugin/OgreTerrain/CreateGrid", "Plugins");
		}

		else if (type == IPT_MOUSE_WHEEL && !CTerrainManager::getInstance()->isValidGroupID(m_nID))
		{
			if (datas.m_sDelta >= 0 || m_fScale > 50.0f)
			{
        if (datas.m_sDelta > 0)
				  m_fScale *= 1.2f;
        else
          m_fScale /= 1.2f;

        float fScale = getRealScale();

				m_fWidthAdjust = fScale / 2;
				m_fHeightAdjust = fScale / 2;
			
				m_pNode->setScale(fScale, fScale, fScale);
        setTerrainPosition();
				
        Ogre::RealRect rect = getTerrainRect();

				if (CTerrainManager::getInstance()->checkTerrainIntersect(rect))
					m_pManualObject->setMaterialName(0, "Editor/Vertex/Selected");
				else
					m_pManualObject->setMaterialName(0, "Editor/Plugin/OgreTerrain/CreateGrid", "Plugins");
			}
		}

		return true;
	}

	virtual void			onKeyboardInput(IWEKeys::e_IWEKey nChar, bool keyDown, int repeatCount, IViewport* pViewport)
	{
    if (nChar == IWEKeys::Shift)
    {
      //m_bAlt = keyDown;
      //setTerrainPosition();
    }
	}

	virtual void			onMouseLeave()
	{
    m_pManualObject->setVisible(false);
	}

	virtual void			onToolChange()
	{
		if (m_pManualObject)
		{
			m_pNode->detachAllObjects();
			GetRenderEngine()->getSceneManager()->destroyManualObject(m_pManualObject);
			GetRenderEngine()->getSceneManager()->destroySceneNode(m_pNode);

			m_pManualObject = NULL;
			m_pNode = NULL;
		}
	}

  virtual void      onToolActivate()
  {
      if (!m_pManualObject)
      {
		    m_pManualObject = GetRenderEngine()->getSceneManager()->createManualObject();
		    m_pNode = GetRenderEngine()->getSceneManager()->getRootSceneNode()->createChildSceneNode();

		    m_pManualObject->setVisibilityFlags(1);
		    m_pManualObject->begin("Editor/Plugin/OgreTerrain/CreateGrid", Ogre::RenderOperation::OT_LINE_LIST, "Plugins");
		    		
		    m_pManualObject->position(Ogre::Vector3(0.0f, 0.0f, 0.0f));
		    m_pManualObject->position(Ogre::Vector3(1.0f, 0.0f, 0.0f));

		    m_pManualObject->position(Ogre::Vector3(0.0f, 0.0f, 0.0f));
		    m_pManualObject->position(Ogre::Vector3(0.0f, 0.0f, 1.0f));

		    m_pManualObject->position(Ogre::Vector3(0.0f, 0.0f, 0.0f));
		    m_pManualObject->position(Ogre::Vector3(1.0f, 0.0f, 1.0f));

		    m_pManualObject->position(Ogre::Vector3(1.0f, 0.0f, 1.0f));
		    m_pManualObject->position(Ogre::Vector3(0.0f, 0.0f, 1.0f));

		    m_pManualObject->position(Ogre::Vector3(1.0f, 0.0f, 1.0f));
		    m_pManualObject->position(Ogre::Vector3(1.0f, 0.0f, 0.0f));

		    m_pManualObject->position(Ogre::Vector3(1.0f, 0.0f, 0.0f));
		    m_pManualObject->position(Ogre::Vector3(0.0f, 0.0f, 1.0f));

		    m_pManualObject->end();

		    m_pNode->attachObject(m_pManualObject);

		    if (!CTerrainManager::getInstance()->isValidGroupID(m_nID))
		    	m_fScale = CTerrainManager::m_fTerrainWorldSize;

		    m_fWidthAdjust = m_fScale / 2;
		    m_fHeightAdjust = m_fScale / 2;

		    m_pNode->setScale(Ogre::Vector3(m_fScale, m_fScale, m_fScale));
        m_pManualObject->setVisible(false);
      }
  }

  virtual bool      onInit()
  {
    IUIMainMenu* pMainMenu = GetGUI()->getUIManager()->getMainMenu();
    
    IUICategory* pCat = pMainMenu->addCategory(m_sCategory, m_nInsertCategoryAt);
    IUIPanel* pPanel = pCat->addPanel(m_sPanel, m_nInsertPanelAt);
    
    // Main Button
    m_pButton = pPanel->addButton(m_sLabel, m_sImgName, m_sImgGroup, m_nInsertButtonAt);
    m_pButton->setLargeButton(m_bLarge);

    // Sub Button
    IUIButton* pNewGroupButton = m_pButton->addButton("New Group");
    pNewGroupButton->eventPushed +=
      FastDelegate0<>(this, &CTerrainCreateTool::onNewGroupCmd);
    pNewGroupButton->eventUIDisplay +=
      FastDelegate2<bool&, bool&>(this, &CTerrainCreateTool::onNewGroupCmdUI);

    // Separator
    pPanel->addSeparator(1);

    CPlugin::getInstance()->getEditor()->getToolMgr()->registerTool(this, m_pButton, m_bGlobal);


    //////////////////////////////////
    // Hack

    for (int i = 0; i < 2; i++)
    {
      if (!m_pManualObject)
      {
		    m_pManualObject = GetRenderEngine()->getSceneManager()->createManualObject();
		    m_pNode = GetRenderEngine()->getSceneManager()->getRootSceneNode()->createChildSceneNode();

		    m_pManualObject->setVisibilityFlags(1);
		    m_pManualObject->begin("Editor/Plugin/OgreTerrain/CreateGrid", Ogre::RenderOperation::OT_LINE_LIST, "Plugins");
		    		
		    m_pManualObject->position(Ogre::Vector3(0.0f, 0.0f, 0.0f));
		    m_pManualObject->position(Ogre::Vector3(1.0f, 0.0f, 0.0f));

		    m_pManualObject->position(Ogre::Vector3(0.0f, 0.0f, 0.0f));
		    m_pManualObject->position(Ogre::Vector3(0.0f, 0.0f, 1.0f));

		    m_pManualObject->position(Ogre::Vector3(0.0f, 0.0f, 0.0f));
		    m_pManualObject->position(Ogre::Vector3(1.0f, 0.0f, 1.0f));

		    m_pManualObject->position(Ogre::Vector3(1.0f, 0.0f, 1.0f));
		    m_pManualObject->position(Ogre::Vector3(0.0f, 0.0f, 1.0f));

		    m_pManualObject->position(Ogre::Vector3(1.0f, 0.0f, 1.0f));
		    m_pManualObject->position(Ogre::Vector3(1.0f, 0.0f, 0.0f));

		    m_pManualObject->position(Ogre::Vector3(1.0f, 0.0f, 0.0f));
		    m_pManualObject->position(Ogre::Vector3(0.0f, 0.0f, 1.0f));

		    m_pManualObject->end();

		    m_pNode->attachObject(m_pManualObject);

		    if (!CTerrainManager::getInstance()->isValidGroupID(m_nID))
		    	m_fScale = CTerrainManager::m_fTerrainWorldSize;

		    m_fWidthAdjust = m_fScale / 2;
		    m_fHeightAdjust = m_fScale / 2;

		    m_pNode->setScale(Ogre::Vector3(m_fScale, m_fScale, m_fScale));
        m_pManualObject->setVisible(false);
      }

      onToolChange();
    }


    return false;
  }

  void          onNewGroupCmd()
  {
    m_nID = -1;

    m_pButton->eventPushed();
  }

  void          onNewGroupCmdUI(bool& bEnabled, bool& bChecked)
  {
    bEnabled = true;
    bChecked = false;
  }

#if 0
	virtual void			setDatas(void* pDatas)
	{
		m_nID = (UINT)pDatas;

		if (CTerrainManager::getInstance()->isValidGroupID(m_nID))
			m_fScale = CTerrainManager::getInstance()->getGroupSize(m_nID) / CTerrainManager::m_fTerrainWorldSize;

		onToolChange();
	}
#endif // !0
	
private:
	float									m_fLastX;
	float									m_fLastY;

	bool									m_bLButton;
  bool                  m_bAlt;

	UINT									m_nID;
  uint                  m_nGroupCount;
  
	Ogre::ManualObject*		m_pManualObject;
	Ogre::SceneNode*			m_pNode;
	float									m_fScale;
	float									m_fWidthAdjust;
	float									m_fHeightAdjust;
};


void
CTerrainGroupButtonProxy::onCmd()
{
  m_pTool->onGroupSelected(m_nGroupID);
}

void
CTerrainGroupButtonProxy::onCmdUI(bool& bEnabled, bool& bChecked)
{
  bEnabled = true;
  bChecked = false;
}

CTerrainCreateTool g_TerrainCreateTool;