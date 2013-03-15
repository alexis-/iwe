#ifndef IWE_SCENE_NODE_H
# define IWE_SCENE_NODE_H

# include "MementoObject.h"

namespace Ogre
{
  class SceneNode;
  class SceneManager;
}

enum e_SNMementoType
{
  SN_MEM_POSITION,
  SN_MEM_SCALE,
  SN_MEM_ROTATE,
  SN_MEM_CREATE,
  SN_MEM_DELETE,
};

struct s_SNMemento
{
  Ogre::Vector3         m_vecPos;
  Ogre::Vector3         m_vecScale;
  Ogre::Quaternion      m_quatOrient;
};


// Wrap Ogre::SceneNode to expose properties and handle undo/redo
class IWEAPI CIWESceneNode : public IPropertyHolder, public CMementoObject
{
public:
  CIWESceneNode(Ogre::SceneManager* pSceneMgr, IEditorInteraction* pEditorInteraction);
  ~CIWESceneNode();
  
// Override for custom behaviour
public:
  virtual Ogre::Vector3                   getPosition() const;
  virtual Ogre::Vector3                   getScale() const;
  virtual Ogre::Quaternion                getOrientation() const;

  virtual void                            setPosition(const Ogre::Vector3& vecDelta);
  virtual void                            setScale(const Ogre::Vector3& vecScale);
  virtual void                            setOrientation(const Ogre::Quaternion& quatRotate);

  virtual void                            destroy();
  
  virtual void                            attachObject(Ogre::MovableObject* pObj);
  virtual void                            detachObject(Ogre::MovableObject* pObj);

  virtual Ogre::SceneNode*                getSceneNode();

// IPropertyHolder interface
public:
  IWE_DEFINE_PROP_HOLDER();

protected:
  CPropVar<Ogre::Vector3>                 m_vecPosition;
  CPropVar<Ogre::Vector3>                 m_vecScale;
  CPropVar<Ogre::Quaternion>              m_quatOrientation;

// IMementoObject interface
public:
  virtual void                            undo();
  virtual void                            redo();
  virtual void                            forgetLast();
  virtual void                            forgetFirst();

// CMementoObject
protected:
  std::stack<e_SNMementoType>             m_stackMemUndoType;
  std::stack<Ogre::Vector3>               m_stackMemUndoVec3;
  std::stack<Ogre::Quaternion>            m_stackMemUndoQuat;
  std::stack<s_SNMemento>                 m_stackSNMemUndoDelete;
  
  std::stack<e_SNMementoType>             m_stackMemRedoType;
  std::stack<Ogre::Vector3>               m_stackMemRedoVec3;
  std::stack<Ogre::Quaternion>            m_stackMemRedoQuat;
  std::stack<s_SNMemento>                 m_stackSNMemRedoDelete;

  s_SNMemento                             generateSNState();

public:
  virtual void                            initialCreation();

protected:
  void                                    createSceneNode();
  void                                    createSceneNode(const Ogre::Vector3& vecPos, const Ogre::Vector3& vecScale, const Ogre::Quaternion& quatOrient);
  void                                    deleteSceneNode();
  
public:
  Event0<>                                eventDestroyed;
  Event0<>                                eventRestored;

protected:
  bool                                    m_bTargetSelected;
	Ogre::SceneNode*								        m_pNode;

  Ogre::SceneManager*                     m_pSceneMgr;
};

#endif // !IWE_SCENE_NODE_H