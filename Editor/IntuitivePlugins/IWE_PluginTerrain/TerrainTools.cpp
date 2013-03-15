#include "IWEPlugin.h"

#include <OgreManualObject.h>
#include <OgrePass.h>
#include <OgreTextureUnitState.h>

#include "TerrainManager.h"





/////////////////////////////////////////////////////////////////////////////////////////////
//  CTerrainToolsButtonContainer
/////////////////////////////////////////////////////////////////////////////////////////////

class CTerrainToolsButtonContainer
{
public:
  static CTerrainToolsButtonContainer*  getInstance()
  {
    if (!m_pInst)
      m_pInst = new CTerrainToolsButtonContainer();

    return m_pInst;
  }

public:
  IUIButton*                            m_pLinearHeight;
  IUIButton*                            m_pProgressiveHeight;
  IUIButton*                            m_pPaint;
  IUISlider*                            m_pHeightSlider;
  IUISlider*                            m_pPaintSlider;
  IUIGallery*                           m_pBrushGallery;
  IUIGallery*                           m_pTexGallery;

private:
  CTerrainToolsButtonContainer()
  {
    IUIMainMenu* pMainMenu = GetGUI()->getUIManager()->getMainMenu();
    
    IUICategory* pCat = pMainMenu->addCategory("Terrain", 1);
    IUIPanel* pPanel = pCat->addPanel("Terrain Tools", 0);

    int iPos = 2;
    
    // Linear Height Button
    m_pLinearHeight = pPanel->addButton("Linear Height", "HeightEditToolIcon.bmp", "Plugins", iPos++);
    m_pLinearHeight->setLargeButton(true);

    // Progressive Height Button
    m_pProgressiveHeight = pPanel->addButton("Progressive Height", "HeightEditToolIcon.bmp", "Plugins", iPos++);
    m_pProgressiveHeight->setLargeButton(true);

    // Progressive Height Button
    m_pPaint = pPanel->addButton("Paint Terrain", "TerrainSprayToolIcon.bmp", "Plugins", iPos++);
    m_pPaint->setLargeButton(true);

    // Separator
    pPanel->addSeparator(iPos++);

    // Height Slider
    m_pHeightSlider = pPanel->addSlider("Height Slider", 0, 100, 25, true, iPos++);

    // Paint Slider
    m_pPaintSlider = pPanel->addSlider("Paint Slider", 0, 100, 25, true, iPos++);

    // Separator
    pPanel->addSeparator(iPos++);

    // Brush Gallery
    m_pBrushGallery = pPanel->addGallery("Brush Selection", iPos++);

    // Texture Gallery
    m_pTexGallery = pPanel->addGallery("Texture Selection", iPos++);
    
    // UI Events
    m_pHeightSlider->eventUIDisplay +=
      FastDelegate2<bool&, bool&>(this, &CTerrainToolsButtonContainer::onUIDisplay);
    m_pPaintSlider->eventUIDisplay +=
      FastDelegate2<bool&, bool&>(this, &CTerrainToolsButtonContainer::onUIDisplay);
    m_pBrushGallery->eventUIDisplay +=
      FastDelegate2<bool&, bool&>(this, &CTerrainToolsButtonContainer::onUIDisplay);
    m_pTexGallery->eventUIDisplay +=
      FastDelegate2<bool&, bool&>(this, &CTerrainToolsButtonContainer::onUIDisplay);


    IResourceManager* pResMgr = CPlugin::getInstance()->getEngine()->getResourceMgr();

    pResMgr->eventUpdateFinished +=
      FastDelegate0<>(this, &CTerrainToolsButtonContainer::onResourceUpdate);

    bool bBrushInit = false;

    const std::vector<std::pair<const Ogre::FileInfo, const std::string>>& vecRes = pResMgr->getFiles();
    for (int i = 0; i < vecRes.size(); i++)
    {
      const std::pair<const Ogre::FileInfo, const std::string>& resPair = vecRes[i];
      const Ogre::FileInfo& resFInfo = resPair.first;
      const std::string& resGroup = resPair.second;

      if (resGroup == "EditorAlphaBrushes")
      {
        Ogre::Image imgOri;
        imgOri.load(resFInfo.filename, resGroup);
        
        Ogre::uchar* pTmpData = new Ogre::uchar[4 * imgOri.getWidth() * imgOri.getHeight()];
        Ogre::Image imgTmp;
        imgTmp.loadDynamicImage(pTmpData, imgOri.getWidth(), imgOri.getHeight(), Ogre::PixelFormat::PF_A8R8G8B8);
        for (int i = 0; i < imgOri.getWidth(); i++)
          for (int j = 0; j < imgOri.getHeight(); j++)
            imgTmp.setColourAt(imgOri.getColourAt(i, j, 0), i, j, 0);

        s_IWEImageData imgData = { imgOri.getWidth(), imgOri.getHeight(), 32, pTmpData };
        m_pBrushGallery->addItem(resFInfo.filename, imgData);
        
        if (!bBrushInit)
        {
          CTerrainManager::getInstance()->setDecalTexture(resFInfo.filename);
          bBrushInit = true;
        }

        // pTmpData gets automatically destroyed in ~s_IWEImageData
      }
      else if (resGroup == "AssetsTerrainTex")
      {
        if (resFInfo.filename.find("_diffusespecular") != std::string::npos ||
            resFInfo.filename.find("_diffuse") != std::string::npos)
          m_pTexGallery->addItem(resFInfo.filename, resFInfo.filename, resGroup);
      }
    }
    
    m_pBrushGallery->eventItemSelected +=
      FastDelegate2<int, const std::string&>(this, &CTerrainToolsButtonContainer::onBrushChange);
  }

  void onResourceUpdate()
  {
  }

  void onBrushChange(int, const std::string& sBrushName)
  {
	  CTerrainManager::getInstance()->setDecalTexture(sBrushName);
  }

  void onUIDisplay(bool& bEnabled, bool& bChecked)
  {
    bEnabled = CPlugin::getInstance()->getEditor()->getProjectMgr()->isProjectLoaded();
    bChecked = false;
  }

  static CTerrainToolsButtonContainer*  m_pInst;
};

CTerrainToolsButtonContainer* CTerrainToolsButtonContainer::m_pInst = NULL;

inline CTerrainToolsButtonContainer* getBtn()
{
  return CTerrainToolsButtonContainer::getInstance();
}





/////////////////////////////////////////////////////////////////////////////////////////////
//  Edit stuffs
/////////////////////////////////////////////////////////////////////////////////////////////

#define MIN_TOOL_TICK_DELAY 25

struct s_deformZoneInfos
{
	Ogre::Terrain*				m_pTerrain;
	Ogre::Rect						m_brushRect;
	Ogre::Rect						m_terrainRect;
	Ogre::Image						m_brushImg;
};

struct s_splatZoneInfos
{
	Ogre::Terrain*				m_pTerrain;
	Ogre::Rect						m_brushRect;
	Ogre::Rect						m_splatRect;
	Ogre::Image						m_brushImg;
  int                   m_iLayer;
};

void getEditZone(const Ogre::Vector3& vecPos, Ogre::Terrain* pTerrain,
								 const Ogre::RealRect& terrainBounds, Ogre::Image& brushImg,
								 Ogre::Rect& brushRect, Ogre::Rect& terrainRect, float fRadius)
{
  int								brushSize = (int)Ogre::Math::Ceil(fRadius);
	float							halfBrushSize = brushSize / 2;
	float							terrainSize = pTerrain->getSize() - 1;

	{
		Ogre::Vector3 vecTerrainTopLeftBound;
		Ogre::Vector3 vecTerrainBottomRightBound;
		
		Ogre::Vector3 vecTopLeftBound = vecPos + Ogre::Vector3(-halfBrushSize, 0.0f, halfBrushSize);
		Ogre::Vector3 vecBottomRightBound = vecPos + Ogre::Vector3(halfBrushSize, 0.0f, -halfBrushSize);
			
		pTerrain->getTerrainPosition(vecTopLeftBound, &vecTerrainTopLeftBound);
		pTerrain->getTerrainPosition(vecBottomRightBound, &vecTerrainBottomRightBound);
			
		vecTerrainTopLeftBound *= terrainSize;
		vecTerrainBottomRightBound *= terrainSize;
			
		brushImg = Ogre::Image(CTerrainManager::getInstance()->getDecalImage());
    terrainRect = Ogre::Rect((int)Ogre::Math::Floor(vecTerrainTopLeftBound.x), (int)Ogre::Math::Floor(vecTerrainTopLeftBound.y),
                             (int)Ogre::Math::Ceil(vecTerrainBottomRightBound.x), (int)Ogre::Math::Ceil(vecTerrainBottomRightBound.y));
	}

	int iXSize = terrainRect.right - terrainRect.left;
	int iYSize = terrainRect.bottom - terrainRect.top;
			
	brushRect = Ogre::Rect(0, 0, iXSize, iYSize);

  if (terrainRect.left < 0)
  {
		brushRect.left += (0 - terrainRect.left);
    terrainRect.left = 0;
	}

  if (terrainRect.top < 0)
  {
    brushRect.bottom -= (0 - terrainRect.top);
    terrainRect.top = 0;
	}

  if (terrainRect.right > terrainSize)
  {
    brushRect.right -= (terrainRect.right - terrainSize);
    terrainRect.right = terrainSize;
	}

  if (terrainRect.bottom > terrainSize)
  {
    brushRect.top += (terrainRect.bottom - terrainSize);
    terrainRect.bottom = terrainSize;
	}

  if ((terrainRect.right - terrainRect.left < 1) || (terrainRect.bottom - terrainRect.top < 1))
	{
		terrainRect.setNull();
		brushRect.setNull();
	}
  else
    brushImg.resize(iXSize, iYSize, Ogre::Image::FILTER_BICUBIC);
}

void updateDeformZone(std::vector<s_deformZoneInfos>& vecDeformZones, const Ogre::Vector3& vecPos, float fRadius)
{
	std::vector<Ogre::Terrain*>				vecTerrains = CTerrainManager::getInstance()->getTerrains();
	std::vector<Ogre::RealRect>				vecTerrainBounds = CTerrainManager::getInstance()->getTerrainBounds();
	uint															terrainNum = vecTerrains.size();
		
	vecDeformZones.clear();

	for (int i = 0; i < terrainNum; i++)
	{
		s_deformZoneInfos		deformZoneInfos;
		Ogre::Rect					brushRect;
		Ogre::Rect					terrainRect;
		Ogre::Image					brushImg;

		getEditZone(vecPos, vecTerrains[i], vecTerrainBounds[i], brushImg, brushRect, terrainRect, fRadius);

		if (brushRect.isNull() || terrainRect.isNull())
			continue;

		deformZoneInfos.m_brushRect = brushRect;
		deformZoneInfos.m_terrainRect = terrainRect;
		deformZoneInfos.m_brushImg = brushImg;
		deformZoneInfos.m_pTerrain = vecTerrains[i];

		vecDeformZones.push_back(deformZoneInfos);
	}
}


int
getLayerNum(Ogre::Terrain* pTerrain, const std::string& sTexName)
{
  int iLayerNum = pTerrain->getLayerCount();
  int iLayer = 0;

  for (; iLayer < iLayerNum; iLayer++)
  {
    std::string res;
    int j = 0;

    do
    {
      res = pTerrain->getLayerTextureName(iLayer, j++);
    } while (res != "" && res != sTexName);

    if (res == sTexName)
      break;
  }

  if (iLayer >= iLayerNum)
    return -1;

	return iLayer;
}

void getSplatZone(const Ogre::Vector3& vecPos, Ogre::TerrainLayerBlendMap* pTerrainBlendMap,
								  int iBlendMapSize, Ogre::Image& brushImg, Ogre::Rect& brushRect,
                  Ogre::Rect& blendMapRect, float fRadius)
{
  
  int								brushSize = (int)Ogre::Math::Ceil(fRadius);
	float							halfBrushSize = brushSize / 2;

	{
		Ogre::Vector2 vecBlendMapTopLeftBound;
		Ogre::Vector2 vecBlendMapBottomRightBound;
		
		Ogre::Vector3 vecTopLeftBound = vecPos + Ogre::Vector3(-halfBrushSize, 0.0f, -halfBrushSize);
		Ogre::Vector3 vecBottomRightBound = vecPos + Ogre::Vector3(halfBrushSize, 0.0f, halfBrushSize);
    
    pTerrainBlendMap->convertWorldToUVSpace(vecTopLeftBound, &vecBlendMapTopLeftBound.x, &vecBlendMapTopLeftBound.y);
    pTerrainBlendMap->convertWorldToUVSpace(vecBottomRightBound, &vecBlendMapBottomRightBound.x, &vecBlendMapBottomRightBound.y);
    
    blendMapRect = Ogre::Rect();
    
    size_t tmpX1;
    size_t tmpX2;
    size_t tmpY1;
    size_t tmpY2;

    pTerrainBlendMap->convertUVToImageSpace(vecBlendMapTopLeftBound.x, vecBlendMapTopLeftBound.y, &tmpX1, &tmpY1);
    pTerrainBlendMap->convertUVToImageSpace(vecBlendMapBottomRightBound.x, vecBlendMapBottomRightBound.y, &tmpX2, &tmpY2);
    
    blendMapRect.left = (long)tmpX1;
    blendMapRect.top = (long)tmpY1;
    blendMapRect.right = (long)tmpX2;
    blendMapRect.bottom = (long)tmpY2;

		brushImg = Ogre::Image(CTerrainManager::getInstance()->getDecalImage());
	}
  
	int iXSize = blendMapRect.right - blendMapRect.left;
	int iYSize = blendMapRect.bottom - blendMapRect.top;
			
	brushRect = Ogre::Rect(0, 0, iXSize, iYSize);

  if (blendMapRect.left < 0)
  {
		brushRect.left += (0 - blendMapRect.left);
    blendMapRect.left = 0;
	}

  if (blendMapRect.top < 0)
  {
    //brushRect.bottom -= (0 - blendMapRect.top);
    brushRect.top += (0 - blendMapRect.top);
    blendMapRect.top = 0;
	}

  if (blendMapRect.right > iBlendMapSize)
  {
    brushRect.right -= (blendMapRect.right - iBlendMapSize);
    blendMapRect.right = iBlendMapSize;
	}

  if (blendMapRect.bottom > iBlendMapSize)
  {
    //brushRect.top += (blendMapRect.bottom - iBlendMapSize);
    brushRect.bottom -= (blendMapRect.bottom - iBlendMapSize);
    blendMapRect.bottom = iBlendMapSize;
	}

  if ((blendMapRect.right - blendMapRect.left < 1) || (blendMapRect.bottom - blendMapRect.top < 1))
	{
		blendMapRect.setNull();
		brushRect.setNull();
	}
  else
    brushImg.resize(iXSize, iYSize, Ogre::Image::FILTER_BICUBIC);
}

void updateSplatZone(std::vector<s_splatZoneInfos>& vecSplatZones, const Ogre::Vector3& vecPos, float fRadius, const std::string& sTexName)
{
	std::vector<Ogre::Terrain*>				vecTerrains = CTerrainManager::getInstance()->getTerrains();
	std::vector<Ogre::RealRect>				vecTerrainBounds = CTerrainManager::getInstance()->getTerrainBounds();
	uint															terrainNum = vecTerrains.size();
		
	vecSplatZones.clear();

	for (int i = 0; i < terrainNum; i++)
	{
		s_splatZoneInfos		splatZoneInfos;
		Ogre::Rect					brushRect;
		Ogre::Rect					terrainRect;
		Ogre::Image					brushImg;

    Ogre::Terrain* pTerrain = vecTerrains[i];

    if (pTerrain->getLayerCount() <= 1)
      continue;

    int iLayer = getLayerNum(pTerrain, sTexName);

    if (iLayer < 0)
    {
      if (pTerrain->getLayerCount() >= pTerrain->getMaxLayers())
        continue; // Already max layer count

      //pTerrain->addLayer(0.0f, NULL);
      //iLayer++;
      continue; // FIX ME
    }

    getSplatZone(vecPos, pTerrain->getLayerBlendMap(std::max(1, iLayer)), pTerrain->getLayerBlendMapSize(), brushImg, brushRect, terrainRect, fRadius);

		if (brushRect.isNull() || terrainRect.isNull())
			continue;

		splatZoneInfos.m_brushRect = brushRect;
    splatZoneInfos.m_splatRect = terrainRect;
		splatZoneInfos.m_brushImg = brushImg;
		splatZoneInfos.m_pTerrain = vecTerrains[i];
    splatZoneInfos.m_iLayer = iLayer;

		vecSplatZones.push_back(splatZoneInfos);
	}
}





/////////////////////////////////////////////////////////////////////////////////////////////
//  CTerrainHeightTool
/////////////////////////////////////////////////////////////////////////////////////////////

class CTerrainHeightTool : public CTool
{
public:
	CTerrainHeightTool()
		: CTool(CPlugin::getInstance(), "TerrainHeightTool"),
			m_bLButton(false),
      m_bRButton(false),
			m_fRadius(10.0f),
			m_lastPickedPos(Ogre::Vector3::ZERO),
			m_ulLastTick(0),
      m_fStrength(0.25)
	{
	}

	void							deformZone(bool bPositive)
	{
		
		//if (m_lastPickedPos != m_vecDeformZonePos)
		{
			updateDeformZone(m_vecDeformZones, m_lastPickedPos, m_fRadius);
			m_vecDeformZonePos = m_lastPickedPos;
		}

		int										deformZoneNum = m_vecDeformZones.size();
		
		for (int i = 0; i < deformZoneNum; i++)
		{
			s_deformZoneInfos		deformZone = m_vecDeformZones[i];

			Ogre::Rect					brushRect = deformZone.m_brushRect;
			Ogre::Image					brushImg = deformZone.m_brushImg;
			Ogre::Rect					terrainRect = deformZone.m_terrainRect;
			Ogre::Terrain*			pTerrain = deformZone.m_pTerrain;
			float*							pTerrainDatas = pTerrain->getHeightData();
			int									iTerrainSize = pTerrain->getSize();

      for (int terrY = terrainRect.top, brushY = brushRect.bottom - 1;
           brushY >= brushRect.top;
           terrY++, brushY--)
			{
        for (int terrX = terrainRect.left, brushX = brushRect.left;
             brushX < brushRect.right;
             terrX++, brushX++)
				{
					float fHeight = pTerrainDatas[terrY * iTerrainSize + terrX];
					Ogre::ColourValue cVal = brushImg.getColourAt(brushX, brushY, 0);

					float fBrightness = 0;
					float	fDummy = 0;

					cVal.getHSB(&fDummy, &fDummy, &fBrightness);

          if (bPositive)
					  pTerrainDatas[terrY * iTerrainSize + terrX] = fHeight + (fBrightness * m_fStrength * 0.5f);
          else
					  pTerrainDatas[terrY * iTerrainSize + terrX] = fHeight - (fBrightness * m_fStrength * 0.5f);
				}
			}

			pTerrain->dirtyRect(terrainRect);
			pTerrain->update(true);

			pTerrain->finaliseHeightDeltas(terrainRect, false);
		}
	}

	virtual void			tick()
	{
    if (GetTickCount() - m_ulLastTick > MIN_TOOL_TICK_DELAY)
    {
		  if (m_bLButton)
		  {
		  	deformZone(true);

		  	m_ulLastTick = GetTickCount();
		  }
      
      else if (m_bRButton)
		  {
		  	deformZone(false);

		  	m_ulLastTick = GetTickCount();
		  }
    }
	}

	virtual bool			onMouseInput(const s_inputDatas& datas, IViewport* pViewport)
	{
    e_inputMouseButton type = datas.m_MouseButton;
    
		if (type == IPT_MOUSE_LEFT)
		{
			m_bLButton = datas.m_MouseAction;

			if (m_bLButton)
				deformZone(true);
		}
    
    else if (type == IPT_MOUSE_RIGHT)
    {
			m_bRButton = datas.m_MouseAction;

			if (m_bRButton)
				deformZone(false);
    }

		else if (type == IPT_MOUSE_MOVE)
		{
      CIWEPoint<int>		point = datas.m_Pt;
			Ogre::Vector3			pickedPos;

			CTerrainManager::getInstance()->setDecalVisible(true);

			if (CTerrainManager::getInstance()->getTerrainCoord(point.x, point.y, pViewport, pickedPos))
			{
				m_lastPickedPos = pickedPos;
				CTerrainManager::getInstance()->setDecalPosition(pickedPos);
			}
			else
			{
        m_lastPickedPos = pickedPos = pViewport->GetHeightCoord(point.x, point.y, 0.0f);
				CTerrainManager::getInstance()->setDecalPosition(pickedPos);
			}
		}

		else if (type == IPT_MOUSE_WHEEL)
		{
			float fDelta = (float)datas.m_sDelta / 75;//(datas.m_sDelta > 0 ? 0.5f : -0.5f);

			// TODO: Progressive delta
			//if (m_fRadius < 20.0f)
			//	fDelta *= 0.5;
			//else
			fDelta *= 1.0f + (m_fRadius / 10);

			m_fRadius += fDelta;

			if (m_fRadius < 2.0f)
				m_fRadius = 2.0f;
			
			CTerrainManager::getInstance()->setDecalSize(m_fRadius);
		}

		return true;
	}

  virtual void			onKeyboardInput(IWEKeys::e_IWEKey nChar, bool keyDown, int repeatCount, IViewport* pViewport)
	{
	}

	virtual void			onMouseLeave()
	{
    CTerrainManager::getInstance()->setDecalVisible(false);
	}

  virtual bool      onInit()
  {
    IUIButton* pButton = getBtn()->m_pLinearHeight;
    IUISlider* pSlider = getBtn()->m_pHeightSlider;
    
    CPlugin::getInstance()->getEditor()->getToolMgr()->registerTool(this, pButton, false);

    pSlider->eventValueChanged +=
      FastDelegate1<int>(this, &CTerrainHeightTool::onSliderChange);
    
    return false;
  }

	virtual void			onToolActivate()
	{
		m_lastPickedPos = Ogre::Vector3::ZERO;
    m_bRButton = m_bLButton = false;

    CTerrainManager::getInstance()->setDecalSize(m_fRadius);
	}

	virtual void			onToolChange()
	{
		CTerrainManager::getInstance()->setDecalVisible(false);
	}

private:
  void              onSliderChange(int iVal)
  {
    m_fStrength = iVal / 100.0f;
  }
	
private:
	float															m_fRadius;
	Ogre::Vector3											m_lastPickedPos;
  
	bool															m_bLButton;
  bool                              m_bRButton;

	std::vector<s_deformZoneInfos>		m_vecDeformZones;
	Ogre::Vector3											m_vecDeformZonePos;

	unsigned long											m_ulLastTick;

  float                             m_fStrength;
};

CTerrainHeightTool g_TerrainHeightTool;






/////////////////////////////////////////////////////////////////////////////////////////////
//  CTerrainProgressiveHeightTool
/////////////////////////////////////////////////////////////////////////////////////////////

class CTerrainProgressiveHeightTool : public CTool
{
public:
	CTerrainProgressiveHeightTool()
		: CTool(CPlugin::getInstance(), "TerrainProgressiveHeightTool"),
			m_bLButton(false),
      m_bRButton(false),
			m_fRadius(10.0f),
			m_lastPickedPos(Ogre::Vector3::ZERO),
			m_ulLastTick(0),
      m_fStrength(0.25)
	{
	}

	void							deformZone(bool bPositive)
	{
    //GetGUI()->lockRender();
		
		//if (m_lastPickedPos != m_vecDeformZonePos)
		{
			updateDeformZone(m_vecDeformZones, m_lastPickedPos, m_fRadius);
			m_vecDeformZonePos = m_lastPickedPos;
		}

		int										deformZoneNum = m_vecDeformZones.size();
		
		for (int i = 0; i < deformZoneNum; i++)
		{
			s_deformZoneInfos		deformZone = m_vecDeformZones[i];

			Ogre::Rect					brushRect = deformZone.m_brushRect;
			Ogre::Image					brushImg = deformZone.m_brushImg;
			Ogre::Rect					terrainRect = deformZone.m_terrainRect;
			Ogre::Terrain*			pTerrain = deformZone.m_pTerrain;
			float*							pTerrainDatas = pTerrain->getHeightData();
			int									iTerrainSize = pTerrain->getSize();

      float centerX = terrainRect.left + (terrainRect.right - terrainRect.left) / 2;
      float centerY = terrainRect.top + (terrainRect.bottom - terrainRect.top) / 2;
      Ogre::Vector3 vecCenter(centerX, 0.0f, centerY);
      float maxRange = vecCenter.distance(Ogre::Vector3(terrainRect.left, 0.0f, terrainRect.top));

      for (int terrY = terrainRect.top, brushY = brushRect.bottom - 1;
           brushY >= brushRect.top;
           terrY++, brushY--)
			{
        for (int terrX = terrainRect.left, brushX = brushRect.left;
             brushX < brushRect.right;
             terrX++, brushX++)
				{
					float fHeight = pTerrainDatas[terrY * iTerrainSize + terrX];
					Ogre::ColourValue cVal = brushImg.getColourAt(brushX, brushY, 0);

					float fBrightness = 0;
					float	fDummy = 0;

					cVal.getHSB(&fDummy, &fDummy, &fBrightness);

          float curRangeRatio = 1.0f - vecCenter.distance(Ogre::Vector3((float)terrX, 0.0f, (float)terrY)) / maxRange;

          if (bPositive)
					  pTerrainDatas[terrY * iTerrainSize + terrX] = fHeight + (fBrightness * m_fStrength * curRangeRatio * 0.5f);
          else
					  pTerrainDatas[terrY * iTerrainSize + terrX] = fHeight - (fBrightness * m_fStrength * curRangeRatio * 0.5f);
				}
			}

			pTerrain->dirtyRect(terrainRect);
			pTerrain->update(true);

			pTerrain->finaliseHeightDeltas(terrainRect, false);
		}
    
    //GetGUI()->unlockRender();
	}

	virtual void			tick()
	{
    if (GetTickCount() - m_ulLastTick > MIN_TOOL_TICK_DELAY)
    {
		  if (m_bLButton)
		  {
		  	deformZone(true);

		  	m_ulLastTick = GetTickCount();
		  }
      
      else if (m_bRButton)
		  {
		  	deformZone(false);

		  	m_ulLastTick = GetTickCount();
		  }
    }
	}

	virtual bool			onMouseInput(const s_inputDatas& datas, IViewport* pViewport)
	{
    e_inputMouseButton type = datas.m_MouseButton;
    
		if (type == IPT_MOUSE_LEFT)
		{
			m_bLButton = datas.m_MouseAction;

			if (m_bLButton)
				deformZone(true);
		}
    
    else if (type == IPT_MOUSE_RIGHT)
    {
			m_bRButton = datas.m_MouseAction;

			if (m_bRButton)
				deformZone(false);
    }

		else if (type == IPT_MOUSE_MOVE)
		{
      CIWEPoint<int>		point = datas.m_Pt;
			Ogre::Vector3			pickedPos;

			CTerrainManager::getInstance()->setDecalVisible(true);

			if (CTerrainManager::getInstance()->getTerrainCoord(point.x, point.y, pViewport, pickedPos))
			{
				m_lastPickedPos = pickedPos;
				CTerrainManager::getInstance()->setDecalPosition(pickedPos);
			}
			else
			{
        m_lastPickedPos = pickedPos = pViewport->GetHeightCoord(point.x, point.y, 0.0f);
				CTerrainManager::getInstance()->setDecalPosition(pickedPos);
			}
		}

		else if (type == IPT_MOUSE_WHEEL)
		{
			float fDelta = (float)datas.m_sDelta / 75;//(datas.m_sDelta > 0 ? 0.5f : -0.5f);

			// TODO: Progressive delta
			//if (m_fRadius < 20.0f)
			//	fDelta *= 0.5;
			//else
			fDelta *= 1.0f + (m_fRadius / 10);

			m_fRadius += fDelta;

			if (m_fRadius < 2.0f)
				m_fRadius = 2.0f;
			
			CTerrainManager::getInstance()->setDecalSize(m_fRadius);
		}

		return true;
	}

  virtual void			onKeyboardInput(IWEKeys::e_IWEKey nChar, bool keyDown, int repeatCount, IViewport* pViewport)
	{
	}

	virtual void			onMouseLeave()
	{
    CTerrainManager::getInstance()->setDecalVisible(false);
	}

  virtual bool      onInit()
  {
    IUIButton* pButton = getBtn()->m_pProgressiveHeight;
    IUISlider* pSlider = getBtn()->m_pHeightSlider;
    
    CPlugin::getInstance()->getEditor()->getToolMgr()->registerTool(this, pButton, false);

    pSlider->eventValueChanged +=
      FastDelegate1<int>(this, &CTerrainProgressiveHeightTool::onSliderChange);
    
    return false;
  }

	virtual void			onToolActivate()
	{
		m_lastPickedPos = Ogre::Vector3::ZERO;
    m_bRButton = m_bLButton = false;

    CTerrainManager::getInstance()->setDecalSize(m_fRadius);
	}

	virtual void			onToolChange()
	{
		CTerrainManager::getInstance()->setDecalVisible(false);
	}

private:
  void              onSliderChange(int iVal)
  {
    m_fStrength = iVal / 100.0f;
  }
	
private:
	float															m_fRadius;
	Ogre::Vector3											m_lastPickedPos;
  
	bool															m_bLButton;
  bool                              m_bRButton;

	std::vector<s_deformZoneInfos>		m_vecDeformZones;
	Ogre::Vector3											m_vecDeformZonePos;

	unsigned long											m_ulLastTick;

  float                             m_fStrength;
};

CTerrainProgressiveHeightTool g_TerrainProgressiveHeightTool;







/////////////////////////////////////////////////////////////////////////////////////////////
//  CTerrainProgressiveHeightTool
/////////////////////////////////////////////////////////////////////////////////////////////

class CTerrainPaintTool : public CTool
{
public:
	CTerrainPaintTool()
		: CTool(CPlugin::getInstance(), "TerrainPaintTool"),
			m_bLButton(false),
      m_bRButton(false),
			m_fRadius(10.0f),
			m_lastPickedPos(Ogre::Vector3::ZERO),
			m_ulLastTick(0),
      m_fStrength(0.25)
	{
	}

	void							splatZone(bool bPositive)
	{
    //GetGUI()->lockRender();
		
		//if (m_lastPickedPos != m_vecSplatZonePos)
		{
      updateSplatZone(m_vecSplatZones, m_lastPickedPos, m_fRadius, m_sTexName);
			m_vecSplatZonePos = m_lastPickedPos;
		}

		int										splatZoneNum = m_vecSplatZones.size();
		
		for (int i = 0; i < splatZoneNum; i++)
		{
			s_splatZoneInfos		splatZone = m_vecSplatZones[i];

			Ogre::Rect					brushRect = splatZone.m_brushRect;
			Ogre::Image					brushImg = splatZone.m_brushImg;
      Ogre::Rect					splatRect = splatZone.m_splatRect;
			Ogre::Terrain*			pTerrain = splatZone.m_pTerrain;
      int                 iBlendMapSize = pTerrain->getLayerBlendMapSize();
      int                 iLayer = splatZone.m_iLayer;
      int                 iLayerNum = pTerrain->getLayerCount();

      bool                _bPositive = bPositive;

      if (iLayerNum == 1)
        continue;

      if (iLayer == 0)
      {
        iLayer++;

        Ogre::TerrainLayerBlendMap*   blendMaps[10];
        float*                        blendMapDatas[10];
        
        for (int i = iLayer; i < iLayerNum; i++)
        {
          blendMaps[i] = pTerrain->getLayerBlendMap(i);
          blendMapDatas[i] = blendMaps[i]->getBlendPointer();
        }
        
        for (int splatY = splatRect.top, brushY = brushRect.top;
             brushY < brushRect.bottom;
             splatY++, brushY++)
			  {
			  	for (int splatX = splatRect.left, brushX = brushRect.left, splatPos = splatY * iBlendMapSize + splatRect.left;
            brushX < brushRect.right;
            splatX++, brushX++, splatPos++)
			  	{
            if (_bPositive)
            {
			  		  Ogre::ColourValue cVal = brushImg.getColourAt(brushX, brushY, 0);

			  		  float fBrightness = 0;
			  		  float	fDummy = 0;

			  		  cVal.getHSB(&fDummy, &fDummy, &fBrightness);
              float fVal = fBrightness * m_fStrength * 0.175f;
              //float fAdjust = fVal / (iLayerNum - 1);

              for (int i = iLayer; i < iLayerNum; i++)
                blendMapDatas[i][splatPos] = std::max(0.0f, blendMapDatas[i][splatPos] - fVal);

              //std::vector<float*> vecVals;
              //float fAdjust = 0;

              //for (int i = iLayer; i < iLayerNum; i++)
              //  vecVals.push_back(blendMapDatas[i]);

              //std::sort(vecVals.begin(), vecVals.end(), [&splatPos](float* a, float* b) { return a[splatPos] < b[splatPos]; });

              //for (int i = iLayer; i < iLayerNum; i++)
              //{
              //  float* fCurLayer = vecVals[i - 1];
              //  float fCurVal = fCurLayer[splatPos];

              //  fCurVal = std::max(0, fCurVal - fAdjust);
              //}
            }
            else
            {
			  		  Ogre::ColourValue cVal = brushImg.getColourAt(brushX, brushY, 0);

			  		  float fBrightness = 0;
			  		  float	fDummy = 0;

			  		  cVal.getHSB(&fDummy, &fDummy, &fBrightness);
              float fVal = fBrightness * m_fStrength * 0.175f;
              //float fAdjust = fVal / (iLayerNum - 1);

              for (int i = iLayer; i < iLayerNum; i++)
              {
                float fCurVal = blendMapDatas[i][splatPos];
                float fCurAdj = fCurVal / 1.0f * fVal;
                blendMapDatas[i][splatPos] = std::min(1.0f, blendMapDatas[i][splatPos] + fCurAdj);
              }
            }
			  	}
        }
        
        for (int i = iLayer; i < iLayerNum; i++)
        {
          blendMaps[i]->dirtyRect(splatRect);
          blendMaps[i]->update();
        }
      }
      else
      {
        Ogre::TerrainLayerBlendMap*   curBlendMap = pTerrain->getLayerBlendMap(iLayer);
        float*                        curBlendMapDatas = curBlendMap->getBlendPointer();

        Ogre::TerrainLayerBlendMap*   blendMaps[10];
        float*                        blendMapDatas[10];

        for (int i = iLayer; i < iLayerNum; i++)
        {
          blendMaps[i] = pTerrain->getLayerBlendMap(i);
          blendMapDatas[i] = blendMaps[i]->getBlendPointer();
        }
        
        for (int splatY = splatRect.top, brushY = brushRect.top;
             brushY < brushRect.bottom;
             splatY++, brushY++)
			  {
			  	for (int splatX = splatRect.left, brushX = brushRect.left, splatPos = splatY * iBlendMapSize + splatRect.left;
            brushX < brushRect.right;
            splatX++, brushX++, splatPos++)
			  	{
            if (_bPositive)
            {
			  		  float fVal = curBlendMapDatas[splatPos];

			  		  Ogre::ColourValue cVal = brushImg.getColourAt(brushX, brushY, 0);

			  		  float fBrightness = 0;
			  		  float	fDummy = 0;

			  		  cVal.getHSB(&fDummy, &fDummy, &fBrightness);

              fVal += fBrightness * m_fStrength * 0.175f;
            
              float fSum = fVal;

              for (int i = iLayer + 1; i < iLayerNum; i++)
                fSum += blendMapDatas[i][splatPos];

              if (fSum > 1.0f)
              {
                float fRatio = 1.0f / fSum;

                curBlendMapDatas[splatPos] = fVal * fRatio;

                for (int i = iLayer + 1; i < iLayerNum; i++)
                  blendMapDatas[i][splatPos] *= fRatio;
              }
              else
                curBlendMapDatas[splatPos] = fVal;
            }
            else
            {
			  		  float fVal = curBlendMapDatas[splatPos];

			  		  Ogre::ColourValue cVal = brushImg.getColourAt(brushX, brushY, 0);

			  		  float fBrightness = 0;
			  		  float	fDummy = 0;

			  		  cVal.getHSB(&fDummy, &fDummy, &fBrightness);

              fVal -= fBrightness * m_fStrength * 0.175f;
              fVal = std::max(0.0f, fVal);

              curBlendMapDatas[splatPos] = fVal;
            }
			  	}
			  }
        
        if (_bPositive)
          for (int i = iLayer; i < iLayerNum; i++)
          {
            blendMaps[i]->dirtyRect(splatRect);
            blendMaps[i]->update();
          }
        else
        {
          curBlendMap->dirtyRect(splatRect);
          curBlendMap->update();
        }
      }
		}
    
    //GetGUI()->unlockRender();
	}

	virtual void			tick()
	{
    if (GetTickCount() - m_ulLastTick > MIN_TOOL_TICK_DELAY)
    {
		  if (m_bLButton)
		  {
		  	splatZone(true);

		  	m_ulLastTick = GetTickCount();
		  }
      
      else if (m_bRButton)
		  {
		  	splatZone(false);

		  	m_ulLastTick = GetTickCount();
		  }
    }
	}

	virtual bool			onMouseInput(const s_inputDatas& datas, IViewport* pViewport)
	{
    e_inputMouseButton type = datas.m_MouseButton;
    
		if (type == IPT_MOUSE_LEFT)
		{
			m_bLButton = datas.m_MouseAction;

			if (m_bLButton)
        splatZone(true);
		}
    
    else if (type == IPT_MOUSE_RIGHT)
    {
			m_bRButton = datas.m_MouseAction;

			if (m_bRButton)
				splatZone(false);
    }

		else if (type == IPT_MOUSE_MOVE)
		{
      CIWEPoint<int>		point = datas.m_Pt;
			Ogre::Vector3			pickedPos;

			CTerrainManager::getInstance()->setDecalVisible(true);

			if (CTerrainManager::getInstance()->getTerrainCoord(point.x, point.y, pViewport, pickedPos))
			{
				m_lastPickedPos = pickedPos;
				CTerrainManager::getInstance()->setDecalPosition(pickedPos);
			}
			else
			{
        m_lastPickedPos = pickedPos = pViewport->GetHeightCoord(point.x, point.y, 0.0f);
				CTerrainManager::getInstance()->setDecalPosition(pickedPos);
			}
		}

		else if (type == IPT_MOUSE_WHEEL)
		{
			float fDelta = (float)datas.m_sDelta / 75;//(datas.m_sDelta > 0 ? 0.5f : -0.5f);

			// TODO: Progressive delta
			//if (m_fRadius < 20.0f)
			//	fDelta *= 0.5;
			//else
			fDelta *= 1.0f + (m_fRadius / 10);

			m_fRadius += fDelta;

			if (m_fRadius < 2.0f)
				m_fRadius = 2.0f;
			
			CTerrainManager::getInstance()->setDecalSize(m_fRadius);
		}

		return true;
	}

  virtual void			onKeyboardInput(IWEKeys::e_IWEKey nChar, bool keyDown, int repeatCount, IViewport* pViewport)
	{
	}

	virtual void			onMouseLeave()
	{
    CTerrainManager::getInstance()->setDecalVisible(false);
	}

  virtual bool      onInit()
  {
    IUIButton* pButton = getBtn()->m_pPaint;
    IUISlider* pSlider = getBtn()->m_pHeightSlider;

    IUIGallery* pTexGallery = getBtn()->m_pTexGallery;
    
    CPlugin::getInstance()->getEditor()->getToolMgr()->registerTool(this, pButton, false);

    pTexGallery->eventItemSelected +=
      FastDelegate2<int, const std::string&>(this, &CTerrainPaintTool::onTexChange);
    pSlider->eventValueChanged +=
      FastDelegate1<int>(this, &CTerrainPaintTool::onSliderChange);

    int dummy;
    pTexGallery->getSelectedItem(m_sTexName, dummy);
    
    return false;
  }

	virtual void			onToolActivate()
	{
		m_lastPickedPos = Ogre::Vector3::ZERO;
    m_bRButton = m_bLButton = false;

    CTerrainManager::getInstance()->setDecalSize(m_fRadius);
	}

	virtual void			onToolChange()
	{
		CTerrainManager::getInstance()->setDecalVisible(false);
	}

private:
  void              onTexChange(int, const std::string& sTexName)
  {
		m_sTexName = sTexName;
  }

  void              onSliderChange(int iVal)
  {
    m_fStrength = iVal / 100.0f;
  }
	
private:
	float															m_fRadius;
	Ogre::Vector3											m_lastPickedPos;

	bool															m_bLButton;
  bool                              m_bRButton;

	std::vector<s_splatZoneInfos>		  m_vecSplatZones;
	Ogre::Vector3											m_vecSplatZonePos;

	std::string												m_sTexName;

	unsigned long											m_ulLastTick;
  
  float                             m_fStrength;
};

CTerrainPaintTool g_TerrainPaintTool;
