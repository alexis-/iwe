#ifndef ICAMERA_CONTROLLER_H
# define ICAMERA_CONTROLLER_H

interface_class IViewport;

interface_class ICameraController
{
public:
	virtual bool					        onMouseInput(const s_inputDatas& datas) = 0;
	virtual void					        onKeyboardInput(IWEKeys::e_IWEKey nChar, bool keyDown, int repeatCount) = 0;
	virtual void					        onMouseLeave() = 0;
  
  virtual IViewport*            getViewport() = 0;

public:
  virtual void                  tick() = 0;

public:
  Event1<ICameraController*>    eventCameraMoved;
  Event1<ICameraController*>    eventCameraRotated;
};

#endif // !ICAMERA_CONTROLLER_H