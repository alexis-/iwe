#ifndef IRENDERENGINE_H
# define IRENDERENGINE_H

interface_class IViewport;

namespace Ogre
{
  class SceneManager;
  class Root;
}

interface_class IRenderEngine
{
public:
	virtual ~IRenderEngine() {};

  // Init stuffs
public:
	virtual bool																    init() = 0;

	virtual IViewport*													    createViewport(size_t handle, const CIWEPoint<ushort>& size, bool bEditorViewport = true, int maskID = 1) = 0;

	virtual const std::vector<IViewport*>&			    getViewports() = 0;


  // Misc
public:
	virtual void																    render() = 0;
  virtual const std::vector<IWEVariantSetting>&   getRenderSettings() = 0;
  virtual void                                    setRenderSetting(const IWEVariantSetting& setting) = 0;

  virtual void                                    customCmd(int nArgs, ...) = 0;

public:
  virtual Ogre::Root*                             getRoot() = 0;
  virtual Ogre::SceneManager*                     getSceneManager() = 0;

public:
  Event1<IViewport*>                              eventViewportCreated;
  Event1<ICameraController*>                      eventCameraControllerCreated;

  Event0<>                                        eventEngineInit;
  Event1<float>                                   eventRender;
};

#endif // !IRENDERENGINE_H