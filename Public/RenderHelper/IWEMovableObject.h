#ifndef IWE_MOVABLE_OBJECT_H
# define IWE_MOVABLE_OBJECT_H

# include "IWESceneNode.h"
# include "MementoObject.h"


namespace Ogre
{
  class SceneNode;
  class SceneManager;
}

enum e_MOMementoType
{
  MO_MEM_CREATE,
  MO_MEM_DELETE,
};


// Inherit this if your object is attached to a scene node, and can be moved/rotated/scaled
class IWEAPI CIWEMovableObject : public IPropertyHolder, public ISelectableTarget, public CMementoObject
{
public:
  CIWEMovableObject(Ogre::MovableObject* pObj, Ogre::SceneManager* pSceneMgr, IEditorInteraction* pEditorInteraction);
  virtual ~CIWEMovableObject();

// ISelectableTarget interface
public:
  virtual void                            setTargetSelected(bool bSelected);
  virtual bool                            isTargetSelected() const;

  virtual Ogre::Vector3                   getTargetPosition() const;
  virtual Ogre::Vector3                   getTargetScale() const;
  virtual Ogre::Quaternion                getTargetOrientation() const;

  virtual void                            setTargetPosition(const Ogre::Vector3& vecDelta);
  virtual void                            setTargetScale(const Ogre::Vector3& vecScale);
  virtual void                            setTargetOrientation(const Ogre::Quaternion& quatRotate);

  virtual void                            deleteTarget();

  virtual const char*                     getType()
  {
    return "CIWEMovableObject";
  }

public:
  virtual void                            destroy();
  virtual void                            restore();

// Override for custom behaviour
public:
	virtual void										        setPosition(Ogre::Vector3 vecPos) {};
	virtual void										        setOrientation(Ogre::Quaternion quatOrient) {};
  virtual void										        setScale(Ogre::Vector3 scaleVec) {};

// IPropertyHolder interface
protected:
  IWE_DEFINE_PROP_HOLDER();

// IMementoObject interface
public:
  virtual void                            undo();
  virtual void                            redo();
  virtual void                            forgetLast();
  virtual void                            forgetFirst();

// CMementoObject
public:
  std::stack<e_MOMementoType>             m_stackMOMemUndoType;
  std::stack<e_MOMementoType>             m_stackMOMemRedoType;

  virtual void                            initialCreation();

protected:
  bool                                    m_bTargetSelected;
  
  CIWESceneNode*								          m_pNode;
  CIWESceneNode*								          m_pOriNode;

  Ogre::SceneManager*                     m_pSceneMgr;
  Ogre::MovableObject*                    m_pMovObj;
};

#endif // !IWE_MOVABLE_OBJECT_H