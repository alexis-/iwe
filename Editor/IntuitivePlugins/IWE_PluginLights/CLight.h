#ifndef CLIGHT_H
# define CLIGHT_H

# include <OgreEntity.h>
# include <OgreSceneNode.h>
# include <OgreManualObject.h>
# include <OgreLight.h>

# include "RenderHelper/IWEMeshObject.h"

struct s_lightMemento
{
};

class CLight : public CIWEMeshObject
{
public:
  CLight();
  virtual ~CLight();

// ISelectableTarget interface
public:
  virtual void                            setTargetSelected(bool bSelected);

  virtual const char*                     getType()
  {
    return "CLight";
  }

public:
  virtual void                            destroy();
  virtual void                            restore();

// IMementoObject interface
public:
#if 0
  virtual void                            undo();
  virtual void                            redo();
  virtual void                            forgetLast();
  virtual void                            forgetFirst();
  
  s_lightMemento                          createState();
  void                                    setLightState(const s_lightMemento& light);
#endif // !0
  void                                    createLight(const s_lightMemento& light);
  void                                    createLight();
  void                                    deleteLight();
  void                                    initialCreation();

// Visual helper
private:
  void                                    _createPointLight();
  void                                    _createDirectionalLight();
  void                                    _createSpotLight();
  void                                    _drawCircle(const Ogre::Real          radius,
                                                      const Ogre::Real          zOffset,
                                                      const Ogre::ColourValue&  colour);
  void                                    _drawPlaneCircle(const Ogre::Real          radius,
                                                           const Ogre::Real          zOffset,
                                                           const Ogre::ColourValue&  colour);
  
  void                                    refreshVisualHelper();
  
private:
  IWE_DEFINE_PROP_HOLDER_OVERRIDE();
  
  void                                    onSetDiffuse(float*);
  void                                    onSetSpecular(float*);

  void                                    onSetLightType(const std::string&, int);
  void                                    refreshAttenuation();
  void                                    refreshSpot();
  
private:
	void										                setPosition(Ogre::Vector3 vecPos);
	void										                setOrientation(Ogre::Quaternion quatOrient);

private:
  static CPropVar<std::vector<std::string>>   m_vecLightTypes;

  //CPropVar<bool> m_bTest;

  CPropVar<std::string>                   m_sLightType;
	CPropVar<Ogre::Light::LightTypes>       m_LightType;
  CPropVar<Ogre::ColourValue>             m_Diffuse;
  CPropVar<Ogre::ColourValue>             m_Specular;
  CPropVar<float>                         m_fSpotOuter;
  CPropVar<float>                         m_fSpotInner;
  CPropVar<float>                         m_fSpotFalloff;
  CPropVar<float>                         m_fAttenuationRange;
  CPropVar<float>                         m_fAttenuationConst;
  CPropVar<float>                         m_fAttenuationLinear;
  CPropVar<float>                         m_fAttenuationQuad;
	CPropVar<float>                         m_fPowerScale;

  float                                   m_fSpotlightStartCircleOffset;
  float                                   m_fDirectionalLightCircleRadius;
  float                                   m_fDirectionalLightLength;

  Ogre::ManualObject*                     m_pVisualHelper;
  CPropVar<bool>                          m_bLockVisualHelper;

  // Entity SelectableTarget
private:
	Ogre::ManualObject*							        m_pSelectBoxObject;

  // Entity Render
private:
  Ogre::Light*                            m_pLight;
};

#endif // !CLIGHT_H