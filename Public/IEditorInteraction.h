#ifndef IEDITORINTERACTION_H
# define IEDITORINTERACTION_H

interface_class IEditorInteraction
{
public:
	virtual bool						onMouseInput(const s_inputDatas& datas, IViewport* pViewport) = 0;
  virtual void						onKeyboardInput(IWEKeys::e_IWEKey nChar, bool keyDown, int repeatCount, IViewport* pViewport) = 0;
	virtual void						onMouseLeave(IViewport* pViewport) = 0;
  
  virtual void            onUndo() = 0;
  virtual void            onRedo() = 0;
  
  virtual void            onCopy() = 0;
  virtual void            onCut() = 0;
  virtual void            onPaste() = 0;

  virtual void            onSelectAll() = 0;
  virtual void            onFind() = 0;

public:
  virtual void            addStatData(const std::string& sLabel, const std::string& sValue) = 0;
  virtual void            remStatData(const std::string& sLabel) = 0;
  virtual void            setStatData(const std::string& sLabel, const std::string& sValue) = 0;
  virtual std::vector<std::pair<std::string, std::string>>&
                          getStatDatas() = 0;

public:
  virtual void            tick() = 0;

public:
  virtual const std::vector<ISelectableTarget*>&  getSelectedTargets() = 0;
  
  virtual void            saveState(IMementoObject* pMemento) = 0;

public:
  Event1<ISelectableTarget*>        eventTargetSelected;
  Event1<ISelectableTarget*>        eventTargetUnselected;
  
  Event1<ICameraController*>        eventCameraMoved;
  Event1<ICameraController*>        eventCameraRotated;

  Event1<IUIPropertyGridElement*>   eventPropHolderDisplayed;

  Event0<IPropertyHolder*>          requestSettingsProps;
  
  Event4<CIWEPoint<int>, IViewport*, float&, Ogre::Vector3&, ISelectableTarget*>   requestTargetPick;
  Event3<CIWEPoint<int>, IViewport*, float&, Ogre::Vector3>                        requestCollisionPick;
};

#endif // !IEDITORINTERACTION_H