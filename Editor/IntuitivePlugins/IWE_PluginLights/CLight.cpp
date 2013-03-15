#include "IWEPlugin.h"

#include <OgreSceneManager.h>

#include "CLight.h"
#include "RenderHelper/EngineUtils.h"

CPropVar<std::vector<std::string>> CLight::m_vecLightTypes = CPropVar<std::vector<std::string>>();


IWE_IMPLEMENT_PROP_OVERRIDE_BEGIN(CLight, CIWEMeshObject)
//IWE_PROP_BOOL_S("Cast", "", m_bTest, [pInst](bool b) { pInst->m_pOgreEnt->setCastShadows(b); }, false);

  IWE_PROP_LEVEL_BEGIN("Light");
    IWE_PROP_COMBO_S("Type", "Sets the type of light:\n - Point light sources give off light equally in all directions.\n - Directional lights simulate parallel light beams from a distant source.\n - Spotlights simulate a cone of light.",
      m_vecLightTypes, m_sLightType, FastDelegate2<const std::string&, int>(this, &CLight::onSetLightType), false);

    IWE_PROP_NTUPLE_S("Diffuse", m_Diffuse, FastDelegate1<float*>(this, &CLight::onSetDiffuse), false, 4, "R", "G", "B", "A", "Red component", "Green Component", "Blue Component", "Alpha Component", false, false, false, false, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
    IWE_PROP_NTUPLE_S("Specular", m_Specular, FastDelegate1<float*>(this, &CLight::onSetSpecular), false, 4, "R", "G", "B", "A", "Red component", "Green Component", "Blue Component", "Alpha Component", false, false, false, false, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);

    IWE_PROP_LEVEL_BEGIN("Attenuation");
      IWE_PROP_FLOAT_S("Range", "Absolute upper range of the light in world units.", m_fAttenuationRange,
        [pInst](float f) { pInst->m_fAttenuationRange = f; pInst->refreshAttenuation(); }, false);
      IWE_PROP_FLOAT_S("Constant", "The constant factor in the attenuation formula: 1.0 means never attenuate, 0.0 is complete attenuation.", m_fAttenuationConst,
        [pInst](float f) { pInst->m_fAttenuationConst = f; pInst->refreshAttenuation(); }, false);
      IWE_PROP_FLOAT_S("Linear", "The linear factor in the attenuation formula: 1 means attenuate evenly over the distance.", m_fAttenuationLinear,
        [pInst](float f) { pInst->m_fAttenuationLinear = f; pInst->refreshAttenuation(); }, false);
      IWE_PROP_FLOAT_S("Quadratic", "The quadratic factor in the attenuation formula: adds a curvature to the attenuation formula.", m_fAttenuationQuad,
        [pInst](float f) { pInst->m_fAttenuationQuad = f; pInst->refreshAttenuation(); }, false);
    IWE_PROP_LEVEL_END();
    
    IWE_PROP_LEVEL_BEGIN("Spotlight");
      IWE_PROP_FLOAT_S("Inner", "Angle covered by the bright inner cone. Applicable only to Direct3D, it'll always treat as zero in OpenGL.", m_fSpotInner,
        [pInst](float f) { pInst->m_fSpotInner = f; pInst->refreshSpot(); }, false);
      IWE_PROP_FLOAT_S("Outer", "Angle covered by the outer cone.", m_fSpotOuter,
        [pInst](float f) { pInst->m_fSpotOuter = f; pInst->refreshSpot(); }, false);
      IWE_PROP_FLOAT_S("Fall-Off", "The rate of falloff between the inner and outer cones. 1.0 means a linear falloff, less means slower falloff, higher means faster falloff.", m_fSpotFalloff,
        [pInst](float f) { pInst->m_fSpotFalloff = f; pInst->refreshSpot(); }, false);
    IWE_PROP_LEVEL_END();
    
    IWE_PROP_FLOAT_S("Power Scale", "Set a scaling factor to indicate the relative power of a light. This factor is only useful in High Dynamic Range (HDR) rendering.", m_fAttenuationQuad,
      [pInst](float f) {}, false);

    IWE_PROP_BOOL_S("Lock Visual Helper", "", m_bLockVisualHelper, [pInst](bool b) { pInst->m_bLockVisualHelper = b; pInst->m_pVisualHelper->setVisible(b || pInst->m_bTargetSelected); }, false);
  IWE_PROP_LEVEL_END();

IWE_IMPLEMENT_PROP_END();

CLight::CLight()
  : CIWEMeshObject("megaphone.mesh", GetRenderEngine()->getSceneManager(), GetEditorInteraction()),
    m_pLight(NULL),
    m_pSelectBoxObject(NULL),
    
    m_pVisualHelper(NULL),

    m_fSpotlightStartCircleOffset(0.65f),
    m_fDirectionalLightCircleRadius(2.0f),
    m_fDirectionalLightLength(6.0f)
{
  m_bLockVisualHelper = false;

  if (m_vecLightTypes.Get().size() == 0)
  {
    m_vecLightTypes.Get().push_back("Point Light");
    m_vecLightTypes.Get().push_back("Directional Light");
    m_vecLightTypes.Get().push_back("Spotlight");
  }

  createLight();
}

CLight::~CLight()
{
  deleteLight();
}

void
CLight::setPosition(Ogre::Vector3 vecPos)
{
  if (m_pLight)
    m_pLight->setPosition(vecPos);
}

void
CLight::setOrientation(Ogre::Quaternion quatOrient)
{
  if (m_pLight)
    m_pLight->setDirection(quatOrient * Ogre::Vector3::UNIT_Z);
}

void
CLight::setTargetSelected(bool bSelected)
{
	Ogre::SceneManager* pSceneMgr = GetRenderEngine()->getSceneManager();

	if (m_bTargetSelected == bSelected)
		return;

  CIWEMeshObject::setTargetSelected(bSelected);
  
  if (m_pVisualHelper)
    m_pVisualHelper->setVisible(bSelected || m_bLockVisualHelper);

	if (bSelected)
	{
    if (m_pSelectBoxObject)
    {
      if (m_pNode)
        m_pNode->detachObject(m_pSelectBoxObject);

		  pSceneMgr->destroyManualObject(m_pSelectBoxObject);
      m_pSelectBoxObject = NULL;
    }
    
    Ogre::AxisAlignedBox bbox = (m_pEntObj) ? m_pEntObj->getBoundingBox() : Ogre::AxisAlignedBox::BOX_NULL;
		Ogre::Vector3 boxSize = bbox.getSize();
		Ogre::Vector3 adjustSize = (Ogre::Vector3(1.0f, 0.0f, 1.0f) * bbox.getSize()) / 2;

		m_pSelectBoxObject = pSceneMgr->createManualObject(); //getUniqueName("selectBox"));

		m_pSelectBoxObject->setVisibilityFlags(1);
		m_pSelectBoxObject->begin("Editor/Grid", Ogre::RenderOperation::OT_LINE_LIST);

		/////////////////////////////////////////////////////////////////////

		m_pSelectBoxObject->position(Ogre::Vector3::ZERO - adjustSize);
		m_pSelectBoxObject->position(Ogre::Vector3(boxSize.x, 0.0f, 0.0f) - adjustSize);
		
		m_pSelectBoxObject->position(Ogre::Vector3::ZERO - adjustSize);
		m_pSelectBoxObject->position(Ogre::Vector3(0.0f, boxSize.y, 0.0f) - adjustSize);

		m_pSelectBoxObject->position(Ogre::Vector3::ZERO - adjustSize);
		m_pSelectBoxObject->position(Ogre::Vector3(0.0f, 0.0f, boxSize.z) - adjustSize);

		/////////////////////////////////////////////////////////////////////

		m_pSelectBoxObject->position(Ogre::Vector3(boxSize.x, 0.0f, boxSize.z) - adjustSize);
		m_pSelectBoxObject->position(Ogre::Vector3(0.0f, 0.0f, boxSize.z) - adjustSize);
		
		m_pSelectBoxObject->position(Ogre::Vector3(boxSize.x, 0.0f, boxSize.z) - adjustSize);
		m_pSelectBoxObject->position(Ogre::Vector3(boxSize.x, boxSize.y, boxSize.z) - adjustSize);

		m_pSelectBoxObject->position(Ogre::Vector3(boxSize.x, 0.0f, boxSize.z) - adjustSize);
		m_pSelectBoxObject->position(Ogre::Vector3(boxSize.x, 0.0f, 0.0f) - adjustSize);

		/////////////////////////////////////////////////////////////////////

		m_pSelectBoxObject->position(Ogre::Vector3(0.0f, boxSize.y, 0.0f) - adjustSize);
		m_pSelectBoxObject->position(Ogre::Vector3(0.0f, boxSize.y, boxSize.z) - adjustSize);

		m_pSelectBoxObject->position(Ogre::Vector3(boxSize.x, boxSize.y, 0.0f) - adjustSize);
		m_pSelectBoxObject->position(Ogre::Vector3(boxSize.x, 0.0f, 0.0f) - adjustSize);

		m_pSelectBoxObject->position(Ogre::Vector3(0.0f, boxSize.y, 0.0f) - adjustSize);
		m_pSelectBoxObject->position(Ogre::Vector3(boxSize.x, boxSize.y, 0.0f) - adjustSize);

		/////////////////////////////////////////////////////////////////////

		m_pSelectBoxObject->position(Ogre::Vector3(boxSize.x, boxSize.y, boxSize.z) - adjustSize);
		m_pSelectBoxObject->position(Ogre::Vector3(0.0f, boxSize.y, boxSize.z) - adjustSize);

		m_pSelectBoxObject->position(Ogre::Vector3(0.0f, boxSize.y, boxSize.z) - adjustSize);
		m_pSelectBoxObject->position(Ogre::Vector3(0.0f, 0.0f, boxSize.z) - adjustSize);

		m_pSelectBoxObject->position(Ogre::Vector3(boxSize.x, boxSize.y, boxSize.z) - adjustSize);
		m_pSelectBoxObject->position(Ogre::Vector3(boxSize.x, boxSize.y, 0.0f) - adjustSize);

		/////////////////////////////////////////////////////////////////////

		m_pSelectBoxObject->end();
		m_pNode->attachObject(m_pSelectBoxObject);
	}
	else if (m_pSelectBoxObject)
  {
    if (m_pNode)
      m_pNode->detachObject(m_pSelectBoxObject);

		pSceneMgr->destroyManualObject(m_pSelectBoxObject);
    m_pSelectBoxObject = NULL;
  }
}

void
CLight::onSetDiffuse(float* f)
{
  for (int i = 0; i < 4; i++)
    m_Diffuse.Get()[i] = f[i];
  
  m_pLight->setDiffuseColour(m_Diffuse);
}

void
CLight::onSetSpecular(float*f)
{
  for (int i = 0; i < 4; i++)
    m_Specular.Get()[i] = f[i];

  m_pLight->setSpecularColour(m_Specular);
}

void
CLight::onSetLightType(const std::string& sType, int i)
{
  if (i != -1 || !m_pLight)
    return;

  if (sType == "Point Light")
  {
    m_LightType = Ogre::Light::LT_POINT;

    _createPointLight();
  }
  else if (sType == "Directional Light")
  {
    m_LightType = Ogre::Light::LT_DIRECTIONAL;

    _createDirectionalLight();
  }
  else if (sType == "Spotlight")
  {
    m_LightType = Ogre::Light::LT_SPOTLIGHT;

    _createSpotLight();
  }
  else
    return;

  m_sLightType = sType;
  m_pLight->setType(m_LightType);
}

void
CLight::refreshVisualHelper()
{
  if (m_LightType == Ogre::Light::LT_POINT)
    _createPointLight();
  else if (m_LightType == Ogre::Light::LT_DIRECTIONAL)
    _createDirectionalLight();
  else if (m_LightType == Ogre::Light::LT_SPOTLIGHT)
    _createSpotLight();
}

void
CLight::refreshAttenuation()
{
  m_pLight->setAttenuation(m_fAttenuationRange, m_fAttenuationConst, m_fAttenuationLinear, m_fAttenuationQuad);

  refreshVisualHelper();
}

void
CLight::refreshSpot()
{
  m_pLight->setSpotlightRange(Ogre::Degree(m_fSpotInner), Ogre::Degree(m_fSpotOuter), m_fSpotFalloff);

  refreshVisualHelper();
}



#if 0
#define MEMENTO_UNDO_VAR(CaseType, StackType, Type, Var, Func, ...)       \
  case CaseType:                                                          \
  {                                                                       \
    Type tmp = m_stackMemUndo##StackType.top();                           \
    m_stackMemUndo##StackType.pop();                                      \
    m_stackMemRedoType.push(m_stackMemUndoType.top());                    \
    m_stackMemRedo##StackType.push(Var);                                  \
    Var = tmp;                                                            \
    Func(Var);                                                            \
    __VA_ARGS__;                                                          \
    break;                                                                \
  }

#define MEMENTO_REDO_VAR(CaseType, StackType, Type, Var, Func, ...)       \
  case CaseType:                                                          \
  {                                                                       \
    Type tmp = m_stackMemRedo##StackType.top();                           \
    m_stackMemRedo##StackType.pop();                                      \
    m_stackMemUndoType.push(m_stackMemRedoType.top());                    \
    m_stackMemUndo##StackType.push(Var);                                  \
    Var = tmp;                                                            \
    Func(Var);                                                            \
    __VA_ARGS__;                                                          \
    break;                                                                \
  }

void
CLight::undo()
{
#if 0
  switch (m_stackMemUndoType.top())
  {
    case SN_MEM_DELETE:
    {
      //s_SNMemento snState = m_stackSNMemUndoDelete.top();
      //
      //m_stackSNMemUndoDelete.pop();
      //m_stackMemRedoType.push(m_stackMemUndoType.top());
      //m_stackSNMemRedoDelete.push(snState);
      //
      //createSceneNode(snState.m_vecPos, snState.m_vecScale, snState.m_quatOrient);
      break;
    }
    case SN_MEM_CREATE:
    {
      //s_SNMemento snState = m_stackSNMemUndoDelete.top();
      //
      //m_stackSNMemUndoDelete.pop();
      //m_stackMemRedoType.push(m_stackMemUndoType.top());
      //m_stackSNMemRedoDelete.push(snState);
      //
      //deleteSceneNode();
      break;
    }
    default:
      CIWESceneNode::undo();
      return;
  }

  m_stackMemUndoType.pop();
#endif // !0
}

void
CLight::redo()
{
#if 0
  switch (m_stackMemRedoType.top())
  {
    case SN_MEM_DELETE:
    {
      //s_SNMemento snState = m_stackSNMemUndoDelete.top();
      //
      //m_stackSNMemUndoDelete.pop();
      //m_stackMemRedoType.push(m_stackMemUndoType.top());
      //m_stackSNMemRedoDelete.push(snState);
      //
      //createSceneNode(snState.m_vecPos, snState.m_vecScale, snState.m_quatOrient);
      break;
    }
    case SN_MEM_CREATE:
    {
      //s_SNMemento snState = m_stackSNMemUndoDelete.top();
      //
      //m_stackSNMemUndoDelete.pop();
      //m_stackMemRedoType.push(m_stackMemUndoType.top());
      //m_stackSNMemRedoDelete.push(snState);
      //
      //deleteSceneNode();
      break;
    }
    default:
      CIWESceneNode::redo();
      return;
  }
  
  m_stackMemRedoType.pop();
#endif // !0
}

void
CLight::forgetLast()
{
}

void
CLight::forgetFirst()
{
}
  
s_lightMemento 
CLight::createState()
{
  s_lightMemento ret;

  return ret;
}

void
CLight::setLightState(const s_lightMemento& light)
{
}
#endif // !0

void
CLight::destroy()
{
  // Save state
  deleteLight();

  CIWEMeshObject::destroy();
}

void
CLight::restore()
{
  createLight(); // Use memento

  CIWEMeshObject::restore();
}

void
CLight::createLight(const s_lightMemento& light)
{
  createLight();
  //setLightState(light);
}

void
CLight::createLight()
{
  try
  {
	  Ogre::SceneManager* pSceneMgr = GetRenderEngine()->getSceneManager();
  
    // Create visible mesh
    {
		  m_pEntObj->getUserObjectBindings().setUserAny(Ogre::Any((ISelectableTarget*)this));
      m_pEntObj->setQueryFlags(1);
      m_pEntObj->setCastShadows(false);
    }
    
    // Create Light
    {
      if (m_pLight)
      {
        //m_pNode->detachObject(m_pLight);
        pSceneMgr->destroyLight(m_pLight);
        
        m_pLight = NULL;
      }

      m_pLight = pSceneMgr->createLight();
      m_pLight->setType(Ogre::Light::LT_POINT);
      m_pLight->setDiffuseColour(Ogre::ColourValue());
      m_pLight->setSpecularColour(Ogre::ColourValue());
      m_pLight->setAttenuation(100.0f, 1.0f, 0.0f, 0.0f);

      //m_pNode->attachObject(m_pLight);
    }
  
    // Set default parameters
    {
      m_sLightType = "Point Light";
      m_LightType = m_pLight->getType();

      m_Diffuse = m_pLight->getDiffuseColour();
      m_Specular = m_pLight->getSpecularColour();

      m_fAttenuationRange = m_pLight->getAttenuationRange();
      m_fAttenuationConst = m_pLight->getAttenuationConstant();
      m_fAttenuationLinear = m_pLight->getAttenuationLinear();
      m_fAttenuationQuad = m_pLight->getAttenuationQuadric();

      m_fSpotInner = m_pLight->getSpotlightInnerAngle().valueDegrees();
      m_fSpotOuter = m_pLight->getSpotlightOuterAngle().valueDegrees();
      m_fSpotFalloff = m_pLight->getSpotlightFalloff();

      m_fPowerScale = m_pLight->getPowerScale();
    }

    // Visual Helper - This is taken from Ogitor LightVisualHelper.cpp
    if (!m_pVisualHelper)
    {
      m_pVisualHelper = pSceneMgr->createManualObject();
      m_pVisualHelper->estimateIndexCount(0);
      m_pVisualHelper->estimateVertexCount(144);
      m_pVisualHelper->setDynamic(true);

      // create 5 sections, four LineStrip (for max. 4 circles) and one Line version
      m_pVisualHelper->begin("Editor/Vertex", Ogre::RenderOperation::OT_LINE_STRIP);
      m_pVisualHelper->position(0,0,0);  // dummy
      m_pVisualHelper->colour(1,1,1);    // dummy
      m_pVisualHelper->position(0,0,0);  // dummy
      m_pVisualHelper->colour(1,1,1);    // dummy
      m_pVisualHelper->end();

      m_pVisualHelper->begin("Editor/Vertex", Ogre::RenderOperation::OT_LINE_STRIP);
      m_pVisualHelper->position(0,0,0);  // dummy
      m_pVisualHelper->colour(1,1,1);    // dummy
      m_pVisualHelper->position(0,0,0);  // dummy
      m_pVisualHelper->colour(1,1,1);    // dummy
      m_pVisualHelper->end();

      m_pVisualHelper->begin("Editor/Vertex", Ogre::RenderOperation::OT_LINE_STRIP);
      m_pVisualHelper->position(0,0,0);  // dummy
      m_pVisualHelper->colour(1,1,1);    // dummy
      m_pVisualHelper->position(0,0,0);  // dummy
      m_pVisualHelper->colour(1,1,1);    // dummy
      m_pVisualHelper->end();

      m_pVisualHelper->begin("Editor/Vertex", Ogre::RenderOperation::OT_LINE_STRIP);
      m_pVisualHelper->position(0,0,0);  // dummy
      m_pVisualHelper->colour(1,1,1);    // dummy
      m_pVisualHelper->position(0,0,0);  // dummy
      m_pVisualHelper->colour(1,1,1);    // dummy
      m_pVisualHelper->end();

      m_pVisualHelper->begin("Editor/Vertex", Ogre::RenderOperation::OT_LINE_LIST);
      m_pVisualHelper->position(0,0,0);  // dummy
      m_pVisualHelper->colour(1,1,1);    // dummy
      m_pVisualHelper->position(0,0,0);  // dummy
      m_pVisualHelper->colour(1,1,1);    // dummy
      m_pVisualHelper->end();
      
      _createPointLight();

      m_pVisualHelper->setVisible(m_bTargetSelected);
      m_pNode->attachObject(m_pVisualHelper);
    }
  }
  catch (std::exception ex)
  {
    printf(ex.what());

    deleteLight();
  }
}

void
CLight::deleteLight()
{
  if (m_pNode)
  {
    //if (m_pLight)
      //m_pNode->detachObject(m_pLight);
    
    if (m_pSelectBoxObject)
      m_pNode->detachObject(m_pSelectBoxObject);

    if (m_pVisualHelper)
      m_pNode->detachObject(m_pVisualHelper);

		//RenderHelper::DestroyAllAttachedMovableObjects(m_pNode);
    //deleteSceneNode();
  }

  if (m_pSelectBoxObject)
		m_pSceneMgr->destroyManualObject(m_pSelectBoxObject);

  if (m_pVisualHelper)
    m_pSceneMgr->destroyManualObject(m_pVisualHelper);

  if (m_pLight)
    m_pSceneMgr->destroyLight(m_pLight);


  m_pLight = NULL;
  m_pSelectBoxObject = NULL;
  m_pVisualHelper = NULL;
  m_bTargetSelected = false;
}

void
CLight::initialCreation()
{
  CIWEMeshObject::initialCreation();
}


/////////////////////////////////////////////////////////////////////////////
// Following code is taken from Ogitor
/////////////////////////////////////////////////////////////////////////////

void CLight::_createPointLight()
{
   // empty our sections
   m_pVisualHelper->beginUpdate(0);
   _drawPlaneCircle(m_fAttenuationRange, 0, m_Diffuse);
   m_pVisualHelper->end();

   m_pVisualHelper->beginUpdate(1);
   m_pVisualHelper->end();
   m_pVisualHelper->beginUpdate(2);
   m_pVisualHelper->end();
   m_pVisualHelper->beginUpdate(3);
   m_pVisualHelper->end();

   m_pVisualHelper->beginUpdate(4);

   m_pVisualHelper->position(0,   0,       -m_fAttenuationRange);
   m_pVisualHelper->colour(m_Diffuse);
   m_pVisualHelper->position(0,   0,       m_fAttenuationRange);
   m_pVisualHelper->colour(m_Diffuse);

   m_pVisualHelper->position(0,   0,       0);
   m_pVisualHelper->colour(m_Diffuse);
   m_pVisualHelper->position(0,   0,       0);
   m_pVisualHelper->colour(m_Diffuse);

   m_pVisualHelper->position(-m_fAttenuationRange,   0,       0);
   m_pVisualHelper->colour(m_Diffuse);
   m_pVisualHelper->position(m_fAttenuationRange,   0,       0);
   m_pVisualHelper->colour(m_Diffuse);

   m_pVisualHelper->end();
}

void CLight::_createDirectionalLight()
{
   // check for valid angles
   Ogre::Real radius;
   if(m_fDirectionalLightCircleRadius < 0.0f)
      radius = 0.0f;
   else
      radius = m_fDirectionalLightCircleRadius;

   // draw circles
   m_pVisualHelper->beginUpdate(0);
   _drawCircle(radius, 0, m_Diffuse);
   m_pVisualHelper->end();

   m_pVisualHelper->beginUpdate(1);
   _drawCircle(radius, m_fDirectionalLightLength, m_Diffuse);
   m_pVisualHelper->end();

   // empty sections
   m_pVisualHelper->beginUpdate(2);
   m_pVisualHelper->end();
   m_pVisualHelper->beginUpdate(3);
   m_pVisualHelper->end();

   // draw four lines between both circles
   m_pVisualHelper->beginUpdate(4);
   m_pVisualHelper->position(radius,   0,       0);
   m_pVisualHelper->colour(m_Diffuse);
   m_pVisualHelper->position(radius,   0,       m_fDirectionalLightLength);
   m_pVisualHelper->colour(m_Diffuse);
   m_pVisualHelper->position(0,        radius,  0);
   m_pVisualHelper->colour(m_Diffuse);
   m_pVisualHelper->position(0,        radius,  m_fDirectionalLightLength);
   m_pVisualHelper->colour(m_Diffuse);
   m_pVisualHelper->position(0,        -radius, 0);
   m_pVisualHelper->colour(m_Diffuse);
   m_pVisualHelper->position(0,        -radius, m_fDirectionalLightLength);
   m_pVisualHelper->colour(m_Diffuse);
   m_pVisualHelper->position(-radius,  0,       0);
   m_pVisualHelper->colour(m_Diffuse);
   m_pVisualHelper->position(-radius,  0,       m_fDirectionalLightLength);
   m_pVisualHelper->colour(m_Diffuse);
   m_pVisualHelper->end();
}

void CLight::_createSpotLight()
{
   // check for valid angles
   Ogre::Radian outerAngle, innerAngle;
   if(Ogre::Degree(m_fSpotOuter) < Ogre::Radian(0.0f))
   {
      outerAngle = 0.0f;
      innerAngle = 0.0f;
   }
   else
   {
      if(Ogre::Degree(m_fSpotOuter) > Ogre::Radian(Ogre::Math::PI))
         outerAngle = Ogre::Radian(Ogre::Math::PI);
      else
         outerAngle = Ogre::Degree(m_fSpotOuter);

      if(Ogre::Degree(m_fSpotInner) > outerAngle)
         innerAngle = outerAngle;
      else
         innerAngle = Ogre::Degree(m_fSpotInner);
   }

   // draw inner circles
   m_pVisualHelper->beginUpdate(0);
   Ogre::Real firstInnerRadius = Ogre::Math::Tan(innerAngle/2) * m_fSpotlightStartCircleOffset;
   _drawCircle(firstInnerRadius, m_fSpotlightStartCircleOffset, m_Specular);
   m_pVisualHelper->end();

   m_pVisualHelper->beginUpdate(1);
   Ogre::Real secondInnerRadius = Ogre::Math::Tan(innerAngle/2) * m_fAttenuationRange;
   _drawCircle(secondInnerRadius, m_fAttenuationRange, m_Specular);
   m_pVisualHelper->end();

   // draw first outer circles
   m_pVisualHelper->beginUpdate(2);
   Ogre::Real firstOuterRadius = Ogre::Math::Tan(outerAngle/2) * m_fSpotlightStartCircleOffset;
   _drawCircle(firstOuterRadius, m_fSpotlightStartCircleOffset, m_Diffuse);
   m_pVisualHelper->end();
   
   m_pVisualHelper->beginUpdate(3);
   Ogre::Real secondOuterRadius = Ogre::Math::Tan(outerAngle/2) * m_fAttenuationRange;
   _drawCircle(secondOuterRadius, m_fAttenuationRange, m_Diffuse);
   m_pVisualHelper->end();

   m_pVisualHelper->beginUpdate(4);  
   
   // draw 4 lines between inner circles
   m_pVisualHelper->position(firstInnerRadius,    0,             m_fSpotlightStartCircleOffset);
   m_pVisualHelper->colour(m_Specular);
   m_pVisualHelper->position(secondInnerRadius,   0,             m_fAttenuationRange);
   m_pVisualHelper->colour(m_Specular);
   m_pVisualHelper->position(0,              firstInnerRadius,   m_fSpotlightStartCircleOffset);
   m_pVisualHelper->colour(m_Specular);
   m_pVisualHelper->position(0,              secondInnerRadius,  m_fAttenuationRange);
   m_pVisualHelper->colour(m_Specular);
   m_pVisualHelper->position(0,              -firstInnerRadius,  m_fSpotlightStartCircleOffset);
   m_pVisualHelper->colour(m_Specular);
   m_pVisualHelper->position(0,              -secondInnerRadius, m_fAttenuationRange);
   m_pVisualHelper->colour(m_Specular);
   m_pVisualHelper->position(-firstInnerRadius,   0,             m_fSpotlightStartCircleOffset);
   m_pVisualHelper->colour(m_Specular);
   m_pVisualHelper->position(-secondInnerRadius,  0,             m_fAttenuationRange);
   m_pVisualHelper->colour(m_Specular);

   // draw 4 lines between outer circles
   m_pVisualHelper->position(firstOuterRadius,    0,             m_fSpotlightStartCircleOffset);
   m_pVisualHelper->colour(m_Diffuse);
   m_pVisualHelper->position(secondOuterRadius,   0,             m_fAttenuationRange);
   m_pVisualHelper->colour(m_Diffuse);
   m_pVisualHelper->position(0,              firstOuterRadius,   m_fSpotlightStartCircleOffset);
   m_pVisualHelper->colour(m_Diffuse);
   m_pVisualHelper->position(0,              secondOuterRadius,  m_fAttenuationRange);
   m_pVisualHelper->colour(m_Diffuse);
   m_pVisualHelper->position(0,              -firstOuterRadius,  m_fSpotlightStartCircleOffset);
   m_pVisualHelper->colour(m_Diffuse);
   m_pVisualHelper->position(0,              -secondOuterRadius, m_fAttenuationRange);
   m_pVisualHelper->colour(m_Diffuse);
   m_pVisualHelper->position(-firstOuterRadius,   0,             m_fSpotlightStartCircleOffset);
   m_pVisualHelper->colour(m_Diffuse);
   m_pVisualHelper->position(-secondOuterRadius,  0,             m_fAttenuationRange);
   m_pVisualHelper->colour(m_Diffuse);
   
   m_pVisualHelper->end();
}

void CLight::_drawCircle(const Ogre::Real radius, const Ogre::Real zOffset, const Ogre::ColourValue& colour)
{
   for (unsigned int i = 0; i <= 144; i++)
   {
      Ogre::Real x = Ogre::Math::Cos(Ogre::Radian((Ogre::Math::TWO_PI*i) / 144)) * radius;
      Ogre::Real y = Ogre::Math::Sin(Ogre::Radian((Ogre::Math::TWO_PI*i) / 144)) * radius;
      m_pVisualHelper->position(x, y, zOffset);
      m_pVisualHelper->colour(colour);
   }
}

/////////////////////////////////////////////////////////////////////////////
// !Ogitor
/////////////////////////////////////////////////////////////////////////////

void CLight::_drawPlaneCircle(const Ogre::Real radius, const Ogre::Real yOffset, const Ogre::ColourValue& colour)
{
   for (unsigned int i = 0; i <= 144; i++)
   {
      Ogre::Real x = Ogre::Math::Cos(Ogre::Radian((Ogre::Math::TWO_PI*i) / 144)) * radius;
      Ogre::Real z = Ogre::Math::Sin(Ogre::Radian((Ogre::Math::TWO_PI*i) / 144)) * radius;
      m_pVisualHelper->position(x, yOffset, z);
      m_pVisualHelper->colour(colour);
   }
}