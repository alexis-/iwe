#ifndef ISELECTABLE_TARGET_H
# define ISELECTABLE_TARGET_H

interface_class ISelectableTarget
{
public:
  virtual void                        setTargetSelected(bool bSelected) = 0;
  virtual bool                        isTargetSelected() const = 0;

  virtual Ogre::Vector3               getTargetPosition() const = 0;
  virtual Ogre::Vector3               getTargetScale() const = 0;
  virtual Ogre::Quaternion            getTargetOrientation() const = 0;

  virtual void                        setTargetPosition(const Ogre::Vector3& vecPos) = 0;
  virtual void                        setTargetScale(const Ogre::Vector3& vecScale) = 0;
  virtual void                        setTargetOrientation(const Ogre::Quaternion& quatRotate) = 0;

  virtual void                        deleteTarget() = 0;

  virtual const char*                 getType() = 0;

public:
  Event1<ISelectableTarget*>          eventTargetSelected;
  Event1<ISelectableTarget*>          eventTargetUnselected;
  AutoResetEvent1<ISelectableTarget*> eventTargetDeleted;
  Event1<ISelectableTarget*>          eventTargetMoved;
  Event1<ISelectableTarget*>          eventTargetScaled;
  Event1<ISelectableTarget*>          eventTargetRotated;
};

#endif // !ISELECTABLE_TARGET_H