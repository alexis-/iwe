#include "stdafx.h"

#include <OgreManualObject.h>
#include <OgrePass.h>
#include <OgreTextureUnitState.h>

#include <OgreHardwarePixelBuffer.h>

#include "Tool.h"
#include "EditorDatas.h"
#include "BrushManager.h"
#include "TextureManager.h"
#include "TerrainManager.h"
#include "RenderEngine.h"
#include "EngineUtils.h"

#include "MainFrm.h"

/////////////////////////////////////////////////////////////////////////

#define MIN_TOOL_TICK_DELAY 25

typedef unsigned char uchar;

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
	int								brushSize = (int)fRadius;// & ~0x1;
	int								halfBrushSize = brushSize / 2;
	float							terrainSize = pTerrain->getSize() - 1;
	//Ogre::Vector3			vecTerrainPos;

	//pTerrain->getTerrainPosition(vecPos, &vecTerrainPos);
	//vecTerrainPos *= terrainSize;

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
		terrainRect = Ogre::Rect((int)(vecTerrainTopLeftBound.x), (int)(vecTerrainTopLeftBound.y),
															(int)(vecTerrainBottomRightBound.x), (int)(vecTerrainBottomRightBound.y));
	}

  if (terrainRect.left < 0)
  {
		brushRect.left += (0 - terrainRect.left);
    terrainRect.left = 0;
	}

  if (terrainRect.top < 0)
  {
    brushRect.top += (0 - terrainRect.top);
    terrainRect.top = 0;
	}

  if (terrainRect.right > terrainSize)
  {
    brushRect.right -= (terrainRect.right - terrainSize);
    terrainRect.right = terrainSize;
	}

  if (terrainRect.bottom > terrainSize)
  {
    brushRect.bottom -= (terrainRect.bottom - terrainSize);
    terrainRect.bottom = terrainSize;
	}

  if ((terrainRect.right - terrainRect.left < 1) || (terrainRect.bottom - terrainRect.top < 1))
	{
		terrainRect.setNull();
		brushRect.setNull();
	}
  else
  {	
		int iXSize = terrainRect.right - terrainRect.left;
		int iYSize = terrainRect.bottom - terrainRect.top;
		int iSize = (iXSize > iYSize) ? iXSize : iYSize;
			
		brushRect = Ogre::Rect(0, 0, iSize, iSize);
		brushImg.resize(iSize, iSize);
  }
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
  
	int								brushSize = (int)fRadius;// & ~0x1;
	int								halfBrushSize = brushSize / 2;

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

  if (blendMapRect.left < 0)
  {
		brushRect.left += (0 - blendMapRect.left);
    blendMapRect.left = 0;
	}

  if (blendMapRect.top < 0)
  {
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
    brushRect.bottom -= (blendMapRect.bottom - iBlendMapSize);
    blendMapRect.bottom = iBlendMapSize;
	}

  if ((blendMapRect.right - blendMapRect.left < 1) || (blendMapRect.bottom - blendMapRect.top < 1))
	{
		blendMapRect.setNull();
		brushRect.setNull();
	}
  else
  {
		int iXSize = blendMapRect.right - blendMapRect.left;
		int iYSize = blendMapRect.bottom - blendMapRect.top;
		int iSize = (iXSize > iYSize) ? iXSize : iYSize;
			
		brushRect = Ogre::Rect(0, 0, iSize, iSize);
		brushImg.resize(iSize, iSize);
  }
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

/////////////////////////////////////////////////////////////////////////

class CTerrainHeightTool : public CTool
{
public:
	CTerrainHeightTool(std::string sName)
		: CTool(sName),
			m_bLButton(false),
      m_bRButton(false),
			m_fRadius(10.0f),
			m_lastPickedPos(Ogre::Vector3::ZERO),
			m_ulLastTick(0),
      m_fStrength(0.25)
	{
	}

	~CTerrainHeightTool()
	{
	}

	void							deformZone(bool bPositive)
	{
		CMainFrame::getInstance()->getCriticalSection().Lock();
		
		if (m_lastPickedPos != m_vecDeformZonePos)
		{
			updateDeformZone(m_vecDeformZones, m_lastPickedPos, m_fRadius);
			m_vecDeformZonePos = m_lastPickedPos;
		}

		int										deformZoneNum = m_vecDeformZones.size();
		std::set<Ogre::TerrainGroup*>		setTerrainGroup;
		
		for (int i = 0; i < deformZoneNum; i++)
		{
			s_deformZoneInfos		deformZone = m_vecDeformZones[i];

			Ogre::Rect					brushRect = deformZone.m_brushRect;
			Ogre::Image					brushImg = deformZone.m_brushImg;
			Ogre::Rect					terrainRect = deformZone.m_terrainRect;
			Ogre::Terrain*			pTerrain = deformZone.m_pTerrain;
			float*							pTerrainDatas = pTerrain->getHeightData();
			int									iTerrainSize = pTerrain->getSize();
			int									brushWidth = brushImg.getWidth();
			
			int brushY = brushRect.bottom - 1;

			for (int terrY = terrainRect.top; terrY < terrainRect.bottom; terrY++)
			{
				int brushX = brushRect.left;

				for (int terrX = terrainRect.left; terrX < terrainRect.right; terrX++)
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

					brushX++;
				}

				brushY--;
			}

			pTerrain->dirtyRect(terrainRect);
			pTerrain->update(true);

			pTerrain->finaliseHeightDeltas(terrainRect, false);
		}

		CMainFrame::getInstance()->getCriticalSection().Unlock();
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

	virtual bool			OnMouseInput(e_inputMouseButton type, const s_inputDatas& datas, CViewport* pViewport)
	{
		if (type == IPT_MOUSE_LEFT)
		{
			m_bLButton = datas.m_MouseAction;
			m_lastPos = datas.m_Pt;

			if (m_bLButton)
			{
				deformZone(true);

				//static CTestMesh* mesh = NULL;

				//if (!mesh)
				//	mesh = NEW CTestMesh("triPlanarMaterial");

				//mesh->update(m_lastPickedPos.x, m_lastPickedPos.z, m_fRadius);
			}
		}
    
    else if (type == IPT_MOUSE_RIGHT)
    {
			m_bRButton = datas.m_MouseAction;
			m_lastPos = datas.m_Pt;

			if (m_bRButton)
				deformZone(false);
    }

		else if (type == IPT_MOUSE_MOVE)
		{
			CPoint						point = datas.m_Pt;
			Ogre::Vector3			pickedPos;

			//if (!m_pDecalMesh)
			//	m_pDecalMesh = NEW CMeshDecal("Editor/AlphaBrushes/Brush");

			CTerrainManager::getInstance()->setDecalVisible(true);

			if (CTerrainManager::getInstance()->getTerrainCoord(point.x, point.y, pViewport, pickedPos))
			{
				m_lastPickedPos = pickedPos;
				CTerrainManager::getInstance()->setDecalPosition(pickedPos);
			}
			else
			{
				m_lastPickedPos = pickedPos = CEngineUtils::GetGridCoord(point.x, point.y, pViewport);
				CTerrainManager::getInstance()->setDecalPosition(pickedPos);
			}

			//m_pDecalMesh->update(m_lastPickedPos.x, m_lastPickedPos.z, m_fRadius);
			//if (m_bLButton)
			//{
			//	int yDelta = point.y - m_lastPos.y;

			//	deformZone();
			//}
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

			if (m_fRadius < 10.0f)
				m_fRadius = 10.0f;
			
			CTerrainManager::getInstance()->setDecalSize(m_fRadius);
			//m_pDecalMesh->update(m_lastPickedPos.x, m_lastPickedPos.z, m_fRadius);
		}

		return true;
	}

	virtual void			OnKeyboardInput(UINT nChar, bool keyDown, int repeatCount, CViewport* pViewport)
	{
	}

	virtual void			OnMouseLeave()
	{
	}

	virtual void			setDatas(void* pDatas)
	{
	}
	
	virtual void			onToolActivate()
	{
		//CMainFrame::getInstance()->OnToolsTerrainBrushes();
    CMainFrame::getInstance()->eventTerrainHeightEditStrength += FastDelegate1<int>(this, &CTerrainHeightTool::onHeightStrengthChange);
    CMainFrame::getInstance()->OnToolsTerrainHeightSlider();

		CTerrainManager::getInstance()->setDecalSize(m_fRadius);

    m_bRButton = m_bLButton = false;
	}

	virtual void			onToolChange()
	{
    CMainFrame::getInstance()->eventTerrainHeightEditStrength -= FastDelegate1<int>(this, &CTerrainHeightTool::onHeightStrengthChange);

		CTerrainManager::getInstance()->setDecalVisible(false);
		m_lastPickedPos = Ogre::Vector3::ZERO;
		//m_fRadius = 10.0f;
	}

  void              onHeightStrengthChange(int iStrength)
  {
    m_fStrength = iStrength / 100.0f;
  }
	
private:
	CPoint														m_lastPos;
	float															m_fRadius;
	Ogre::Vector3											m_lastPickedPos;

	bool															m_bLButton;
  bool                              m_bRButton;

	std::vector<s_deformZoneInfos>		m_vecDeformZones;
	Ogre::Vector3											m_vecDeformZonePos;

	unsigned long											m_ulLastTick;

  float                             m_fStrength;

	//CMeshDecal*												m_pDecalMesh;
}										g_TerrainHeightTool("TerrainHeightTool");











class CTerrainProgressiveHeightTool : public CTool
{
public:
	CTerrainProgressiveHeightTool(std::string sName)
		: CTool(sName),
			m_bLButton(false),
      m_bRButton(false),
			m_fRadius(10.0f),
			m_lastPickedPos(Ogre::Vector3::ZERO),
			m_ulLastTick(0),
      m_fStrength(0.25)
	{
	}

	~CTerrainProgressiveHeightTool()
	{
	}

	void							deformZone(bool bPositive)
	{
		CMainFrame::getInstance()->getCriticalSection().Lock();
		
		if (m_lastPickedPos != m_vecDeformZonePos)
		{
			updateDeformZone(m_vecDeformZones, m_lastPickedPos, m_fRadius);
			m_vecDeformZonePos = m_lastPickedPos;
		}

		int										deformZoneNum = m_vecDeformZones.size();
		std::set<Ogre::TerrainGroup*>		setTerrainGroup;
		
		for (int i = 0; i < deformZoneNum; i++)
		{
			s_deformZoneInfos		deformZone = m_vecDeformZones[i];

			Ogre::Rect					brushRect = deformZone.m_brushRect;
			Ogre::Image					brushImg = deformZone.m_brushImg;
			Ogre::Rect					terrainRect = deformZone.m_terrainRect;
			Ogre::Terrain*			pTerrain = deformZone.m_pTerrain;
			float*							pTerrainDatas = pTerrain->getHeightData();
			int									iTerrainSize = pTerrain->getSize();
			int									brushWidth = brushImg.getWidth();
			
			int brushY = brushRect.bottom - 1;
      float centerX = terrainRect.left + (terrainRect.right - terrainRect.left) / 2;
      float centerY = terrainRect.top + (terrainRect.bottom - terrainRect.top) / 2;
      Ogre::Vector3 vecCenter(centerX, 0.0f, centerY);
      float maxRange = vecCenter.distance(Ogre::Vector3(terrainRect.left, 0.0f, terrainRect.top));

			for (int terrY = terrainRect.top; terrY < terrainRect.bottom; terrY++)
			{
				int brushX = brushRect.left;

				for (int terrX = terrainRect.left; terrX < terrainRect.right; terrX++)
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

					brushX++;
				}

				brushY--;
			}

			pTerrain->dirtyRect(terrainRect);
			pTerrain->update(true);

			pTerrain->finaliseHeightDeltas(terrainRect, false);
		}

		CMainFrame::getInstance()->getCriticalSection().Unlock();
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

	virtual bool			OnMouseInput(e_inputMouseButton type, const s_inputDatas& datas, CViewport* pViewport)
	{
		if (type == IPT_MOUSE_LEFT)
		{
			m_bLButton = datas.m_MouseAction;
			m_lastPos = datas.m_Pt;

			if (m_bLButton)
				deformZone(true);
		}
    
    else if (type == IPT_MOUSE_RIGHT)
    {
			m_bRButton = datas.m_MouseAction;
			m_lastPos = datas.m_Pt;

			if (m_bRButton)
				deformZone(false);
    }

		else if (type == IPT_MOUSE_MOVE)
		{
			CPoint						point = datas.m_Pt;
			Ogre::Vector3			pickedPos;

			CTerrainManager::getInstance()->setDecalVisible(true);

			if (CTerrainManager::getInstance()->getTerrainCoord(point.x, point.y, pViewport, pickedPos))
			{
				m_lastPickedPos = pickedPos;
				CTerrainManager::getInstance()->setDecalPosition(pickedPos);
			}
			else
			{
				m_lastPickedPos = pickedPos = CEngineUtils::GetGridCoord(point.x, point.y, pViewport);
				CTerrainManager::getInstance()->setDecalPosition(pickedPos);
			}
		}

		else if (type == IPT_MOUSE_WHEEL)
		{
			float fDelta = (float)datas.m_sDelta / 75;

			// TODO: Progressive delta
			fDelta *= 1.0f + (m_fRadius / 10);

			m_fRadius += fDelta;

			if (m_fRadius < 10.0f)
				m_fRadius = 10.0f;
			
			CTerrainManager::getInstance()->setDecalSize(m_fRadius);
		}

		return true;
	}

	virtual void			OnKeyboardInput(UINT nChar, bool keyDown, int repeatCount, CViewport* pViewport)
	{
	}

	virtual void			OnMouseLeave()
	{
	}

	virtual void			setDatas(void* pDatas)
	{
	}
	
	virtual void			onToolActivate()
	{
    CMainFrame::getInstance()->eventTerrainHeightEditStrength += FastDelegate1<int>(this, &CTerrainProgressiveHeightTool::onHeightStrengthChange);
    CMainFrame::getInstance()->OnToolsTerrainHeightSlider();

		CTerrainManager::getInstance()->setDecalSize(m_fRadius);

    m_bRButton = m_bLButton = false;
	}

	virtual void			onToolChange()
	{
    CMainFrame::getInstance()->eventTerrainHeightEditStrength -= FastDelegate1<int>(this, &CTerrainProgressiveHeightTool::onHeightStrengthChange);

		CTerrainManager::getInstance()->setDecalVisible(false);
		m_lastPickedPos = Ogre::Vector3::ZERO;
	}

  void              onHeightStrengthChange(int iStrength)
  {
    m_fStrength = iStrength / 100.0f;
  }
	
private:
	CPoint														m_lastPos;
	float															m_fRadius;
	Ogre::Vector3											m_lastPickedPos;

	bool															m_bLButton;
  bool                              m_bRButton;

	std::vector<s_deformZoneInfos>		m_vecDeformZones;
	Ogre::Vector3											m_vecDeformZonePos;

	unsigned long											m_ulLastTick;

  float                             m_fStrength;
}										g_TerrainProgressiveHeightTool("TerrainProgressiveHeightTool");








class CTerrainPaintTool : public CTool
{
public:
	CTerrainPaintTool(std::string sName)
		: CTool(sName),
			m_bLButton(false),
      m_bRButton(false),
			m_fRadius(10.0f),
			m_lastPickedPos(Ogre::Vector3::ZERO),
			m_ulLastTick(0)
	{
    CTextureManager::getInstance()->eventOnTextureChange += FastDelegate1<const std::string&>(this, &CTerrainPaintTool::onTextureChange);
	}

	~CTerrainPaintTool()
	{
	}

	void							splatZone(bool bPositive)
	{
		CMainFrame::getInstance()->getCriticalSection().Lock();
		
		if (m_lastPickedPos != m_vecSplatZonePos)
		{
      updateSplatZone(m_vecSplatZones, m_lastPickedPos, m_fRadius, m_sTexName);
			m_vecSplatZonePos = m_lastPickedPos;
		}

		int										splatZoneNum = m_vecSplatZones.size();
		std::set<Ogre::TerrainGroup*>		setTerrainGroup;
		
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
        
			  int brushY = brushRect.top;

			  for (int splatY = splatRect.top; splatY < splatRect.bottom; splatY++)
			  {
			  	int brushX = brushRect.left;
          int splatPos = splatY * iBlendMapSize + splatRect.left;

			  	for (int splatX = splatRect.left; splatX < splatRect.right; splatX++)
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

			  		brushX++;
            splatPos++;
			  	}

			  	brushY++;
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

			  int brushY = brushRect.top;

			  for (int splatY = splatRect.top; splatY < splatRect.bottom; splatY++)
			  {
			  	int brushX = brushRect.left;
          int splatPos = splatY * iBlendMapSize + splatRect.left;

			  	for (int splatX = splatRect.left; splatX < splatRect.right; splatX++)
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

			  		brushX++;
            splatPos++;
			  	}

			  	brushY++;
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

		CMainFrame::getInstance()->getCriticalSection().Unlock();
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

	virtual bool			OnMouseInput(e_inputMouseButton type, const s_inputDatas& datas, CViewport* pViewport)
	{
		if (type == IPT_MOUSE_LEFT)
		{
			m_bLButton = datas.m_MouseAction;
			m_lastPos = datas.m_Pt;

			if (m_bLButton)
				splatZone(true);
		}

		else if (type == IPT_MOUSE_RIGHT)
		{
			m_bRButton = datas.m_MouseAction;
			m_lastPos = datas.m_Pt;

			if (m_bRButton)
				splatZone(false);
		}

		else if (type == IPT_MOUSE_MOVE)
		{
			CPoint						point = datas.m_Pt;
			Ogre::Vector3			pickedPos;

			CTerrainManager::getInstance()->setDecalVisible(true);

			if (CTerrainManager::getInstance()->getTerrainCoord(point.x, point.y, pViewport, pickedPos))
			{
				m_lastPickedPos = pickedPos;
				CTerrainManager::getInstance()->setDecalPosition(pickedPos);
			}
			else
			{
				m_lastPickedPos = pickedPos = CEngineUtils::GetGridCoord(point.x, point.y, pViewport);
				CTerrainManager::getInstance()->setDecalPosition(pickedPos);
			}
		}

		else if (type == IPT_MOUSE_WHEEL)
		{
			float fDelta = (float)datas.m_sDelta / 75;

			fDelta *= 1.0f + (m_fRadius / 10);

			m_fRadius += fDelta;

			if (m_fRadius < 10.0f)
				m_fRadius = 10.0f;
			
			CTerrainManager::getInstance()->setDecalSize(m_fRadius);
		}

		return true;
	}

	virtual void			OnKeyboardInput(UINT nChar, bool keyDown, int repeatCount, CViewport* pViewport)
	{
	}

	virtual void			OnMouseLeave()
	{
	}

	virtual void			setDatas(void* pDatas)
	{
	}
	
	virtual void			onToolActivate()
	{
    CMainFrame::getInstance()->eventTerrainPaintEditStrength += FastDelegate1<int>(this, &CTerrainPaintTool::onPaintStrengthChange);
    CMainFrame::getInstance()->OnToolsTerrainPaintSlider();
		CTerrainManager::getInstance()->setDecalVisible(true);
	}

	virtual void			onToolChange()
	{
    CMainFrame::getInstance()->eventTerrainPaintEditStrength -= FastDelegate1<int>(this, &CTerrainPaintTool::onPaintStrengthChange);
		CTerrainManager::getInstance()->setDecalVisible(false);
		m_lastPickedPos = Ogre::Vector3::ZERO;
	}

  void              onTextureChange(const std::string& sTexName)
  {
		m_sTexName = sTexName;
  }

  void              onPaintStrengthChange(int iStrength)
  {
    m_fStrength = iStrength / 100.0f;
  }
	
private:
	CPoint														m_lastPos;
	float															m_fRadius;
	Ogre::Vector3											m_lastPickedPos;

	bool															m_bLButton;
  bool                              m_bRButton;

	std::vector<s_splatZoneInfos>		  m_vecSplatZones;
	Ogre::Vector3											m_vecSplatZonePos;

	std::string												m_sTexName;

	unsigned long											m_ulLastTick;
  
  float                             m_fStrength;
}										g_TerrainPaintTool("TerrainPaintTool");



























//class CTerrainPaintTool : public CTool
//{
//public:
//	CTerrainPaintTool(std::string sName)
//		: CTool(sName),
//			m_bLButton(false),
//			m_fRadius(10.0f),
//			m_lastPickedPos(Ogre::Vector3::ZERO),
//			m_ulLastTick(0)
//	{
//    CTextureManager::getInstance()->eventOnTextureChange += FastDelegate1<const std::string&>(this, &CTerrainPaintTool::onTextureChange);
//	}
//
//	~CTerrainPaintTool()
//	{
//	}
//
//	void							paintZone()
//	{
//		CMainFrame::getInstance()->getCriticalSection().Lock();
//		
//		if (m_lastPickedPos != m_vecDeformZonePos)
//		{
//			updateDeformZone(m_vecDeformZones, m_lastPickedPos, m_fRadius);
//			m_vecDeformZonePos = m_lastPickedPos;
//		}
//
//		int										deformZoneNum = m_vecDeformZones.size();
//		std::set<Ogre::TerrainGroup*>		setTerrainGroup;
//		
//		for (int i = 0; i < deformZoneNum; i++)
//		{
//			s_deformZoneInfos		deformZone = m_vecDeformZones[i];
//
//			Ogre::Rect					brushRect = deformZone.m_brushRect;
//			Ogre::Image					brushImg = deformZone.m_brushImg;
//			Ogre::Rect					terrainRect = deformZone.m_terrainRect;
//			Ogre::Terrain*			pTerrain = deformZone.m_pTerrain;
//			float*							pTerrainDatas = pTerrain->getHeightData();
//			int									iTerrainSize = pTerrain->getSize();
//			int									brushWidth = brushImg.getWidth();
//
//			Ogre::TexturePtr colorMapTex = pTerrain->getGlobalColourMap();
//
//			Ogre::HardwarePixelBufferSharedPtr pixBuffPtr = colorMapTex->getBuffer();
//			char* dataPtr = (char*)pixBuffPtr->lock(Ogre::HardwareBuffer::HBL_NORMAL);
//
//			int max = 4 * pixBuffPtr->getWidth() * pixBuffPtr->getHeight();
//
//			for (int i = 0; i < max; i += 4)
//				dataPtr[i] = 255;
//			//for (int i = 0; i < 100; i++)
//			//	for (int j = 0; j < 100; j++)
//			//		colorBox.setColourAt(Ogre::ColourValue(1.0f, 0.0f, 0.0f, 1.0f), i, j, 0);
//
//			pixBuffPtr->unlock();
//
//			/*int brushY = brushRect.top;
//
//			for (int terrY = terrainRect.top; terrY < terrainRect.bottom; terrY++)
//			{
//				int brushX = brushRect.left;
//
//				for (int terrX = terrainRect.left; terrX < terrainRect.right; terrX++)
//				{
//					float fHeight = pTerrainDatas[terrY * iTerrainSize + terrX];
//					Ogre::ColourValue cVal = brushImg.getColourAt(brushX, brushY, 0);
//
//					float fBrightness = 0;
//					float	fDummy = 0;
//
//					cVal.getHSB(&fDummy, &fDummy, &fBrightness);
//
//					pTerrainDatas[terrY * iTerrainSize + terrX] = fHeight + fBrightness;
//
//					brushX++;
//				}
//
//				brushY++;
//			}
//
//			pTerrain->dirtyRect(terrainRect);
//			pTerrain->update(true);
//
//			pTerrain->finaliseHeightDeltas(terrainRect, false);*/
//		}
//
//		CMainFrame::getInstance()->getCriticalSection().Unlock();
//	}
//
//	virtual void			tick()
//	{
//		if (m_bLButton && GetTickCount() - m_ulLastTick > MIN_TOOL_TICK_DELAY)
//		{
//			//deformZone();
//
//			m_ulLastTick = GetTickCount();
//		}
//	}
//
//	virtual bool			OnMouseInput(e_inputMouseButton type, const s_inputDatas& datas, CViewport* pViewport)
//	{
//		if (type == IPT_MOUSE_LEFT)
//		{
//			m_bLButton = datas.m_MouseAction;
//			m_lastPos = datas.m_Pt;
//
//			if (m_bLButton)
//				paintZone();
//				//deformZone();
//		}
//
//		else if (type == IPT_MOUSE_MOVE)
//		{
//			CPoint						point = datas.m_Pt;
//			Ogre::Vector3			pickedPos;
//
//			CTerrainManager::getInstance()->setDecalVisible(true);
//
//			if (CTerrainManager::getInstance()->getTerrainCoord(point.x, point.y, pViewport, pickedPos))
//			{
//				m_lastPickedPos = pickedPos;
//				CTerrainManager::getInstance()->setDecalPosition(pickedPos);
//			}
//			else
//			{
//				m_lastPickedPos = pickedPos = CEngineUtils::GetGridCoord(point.x, point.y, pViewport);
//				CTerrainManager::getInstance()->setDecalPosition(pickedPos);
//			}
//		}
//
//		else if (type == IPT_MOUSE_WHEEL)
//		{
//			float fDelta = (float)datas.m_sDelta / 75;
//
//			fDelta *= 1.0f + (m_fRadius / 10);
//
//			m_fRadius += fDelta;
//
//			if (m_fRadius < 10.0f)
//				m_fRadius = 10.0f;
//			
//			CTerrainManager::getInstance()->setDecalSize(m_fRadius);
//		}
//
//		return true;
//	}
//
//	virtual void			OnKeyboardInput(UINT nChar, bool keyDown, int repeatCount, CViewport* pViewport)
//	{
//	}
//
//	virtual void			OnMouseLeave()
//	{
//	}
//
//	virtual void			setDatas(void* pDatas)
//	{
//	}
//	
//	virtual void			onToolActivate()
//	{
//    //CMainFrame::getInstance()->OnToolsTerrainTextures();
//	}
//
//	virtual void			onToolChange()
//	{
//		CTerrainManager::getInstance()->setDecalVisible(false);
//		m_lastPickedPos = Ogre::Vector3::ZERO;
//	}
//
//	int								getLayerNum(Ogre::Terrain* pTerrain)
//	{
//		//const Ogre::TerrainLayerDeclaration& layers = pTerrain->getLayerDeclaration();
//		return 0;
//	}
//
//  void              onTextureChange(const std::string& sTexName)
//  {
//		m_sTexName = sTexName;
//
//  }
//	
//private:
//	CPoint														m_lastPos;
//	float															m_fRadius;
//	Ogre::Vector3											m_lastPickedPos;
//
//	bool															m_bLButton;
//
//	std::vector<s_deformZoneInfos>		m_vecDeformZones;
//	Ogre::Vector3											m_vecDeformZonePos;
//
//	std::string												m_sTexName;
//
//	unsigned long											m_ulLastTick;
//}										g_TerrainPaintTool("TerrainPaintTool");