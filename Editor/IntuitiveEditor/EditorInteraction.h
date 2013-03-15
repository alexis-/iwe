#ifndef EDITOR_INTERACTION_H
# define EDITOR_INTERACTION_H

class CEditorInteraction : public IEditorInteraction
{
public:
	static CEditorInteraction*					getInstance();
  
  void                                init();
  void                                postInit();

private:
  void                                onViewportCreated(IViewport* pViewport);
  void                                onCameraControllerCreated(ICameraController* pCameraController);
  void                                onCameraMoved(ICameraController* pCameraController);
  void                                onCameraRotated(ICameraController* pCameraController);

  void                                onUndoUIEvent(bool& bEnabled, bool& bChecked);
  void                                onRedoUIEvent(bool& bEnabled, bool& bChecked);
  void                                onUIEvent(bool& bEnabled, bool& bChecked);

  void                                onTargetDeleted(ISelectableTarget* pTarget);

public:
	virtual bool					              onMouseInput(const s_inputDatas& datas, IViewport* pViewport);
	virtual void					              onKeyboardInput(IWEKeys::e_IWEKey nChar, bool keyDown, int repeatCount, IViewport* pViewport);
	virtual void					              onMouseLeave(IViewport* pViewport);

  virtual void                        onUndo();
  virtual void                        onRedo();
  
  virtual void                        onCopy();
  virtual void                        onCut();
  virtual void                        onPaste();

  virtual void                        onSelectAll();
  virtual void                        onFind();

  virtual void                        saveState(IMementoObject* pMementoObj);
  
public:
  virtual void                        addStatData(const std::string& sLabel, const std::string& sValue) {};
  virtual void                        remStatData(const std::string& sLabel) {};
  virtual void                        setStatData(const std::string& sLabel, const std::string& sValue) {};
  virtual std::vector<std::pair<std::string, std::string>>&
                                      getStatDatas() { return m_vecStats; };

public:
  virtual void                        tick();

public:
  virtual const std::vector<ISelectableTarget*>& getSelectedTargets();

public:
  // Plugin stuffs here

private:
  std::stack<IMementoObject*>         m_stackUndo;
  std::stack<IMementoObject*>         m_stackRedo;
  
  IUIButton*                          m_pUndoButton;
  IUIButton*                          m_pRedoButton;
  IUIButton*                          m_pConfigButton;

private:
  std::vector<IPlugin*>               m_vecPlugins;
  std::vector<std::pair<std::string, std::string>>  m_vecStats;

private:
	CEditorInteraction();
	static CEditorInteraction*					m_pInst;

	bool																m_bShift;
};

#endif // !EDITOR_INTERACTION_H