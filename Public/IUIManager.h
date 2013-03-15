#ifndef IUIMANAGER_H
# define IUIMANAGER_H



enum e_dropEffect
{
  UI_DROPEFFECT_NONE,
  UI_DROPEFFECT_COPY,
  UI_DROPEFFECT_MOVE,
  UI_DROPEFFECT_LINK,
};

interface_class IUIManager
{
public:
  virtual IUIMainMenu*                        getMainMenu() = 0;
  virtual IUIProjectTree*                     getProjectTree() = 0;
  virtual IUITreeElement*                     getSceneTree() = 0;
  virtual IUIPropertyGrid*                    getPropertyGrid() = 0;

#ifdef __cplusplus_cli
  virtual void                                addDocument(System::Type^ docType, const std::string& sTitle) = 0;
#endif // !__cplusplus_cli

public:
  Event3<const std::string&, IViewport*, const CIWEPoint<int>&, e_dropEffect>   eventDragEnter;
  Event2<IViewport*, const CIWEPoint<int>&, e_dropEffect>                       eventDragOver;
  Event2<IViewport*, const CIWEPoint<int>&, bool>                               eventDrop;
  Event1<IViewport*>                                                            eventDragLeave;
};

#endif // !IUIMANAGER_H