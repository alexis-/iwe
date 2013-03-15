#ifndef CBASEENTITY_H
# define CBASEENTITY_H

# include <OgreEntity.h>
# include <OgreSceneNode.h>
# include <OgreManualObject.h>

# include <stack>

# include "IBaseEntity.h"

namespace Ogre
{
	class SceneNode;
	class Entity;
	class AnimationState;

	typedef float		Real;
}

class CEntityManager;

enum e_mementoType
{
  ENT_MEM_POSITION,
  ENT_MEM_SCALE,
  ENT_MEM_ROTATE,
  ENT_MEM_CREATE,
  ENT_MEM_DELETE,
  ENT_MEM_MESH,
};

struct s_entMemento
{
  std::string           m_sMesh;
  Ogre::Vector3         m_vecPos;
  Ogre::Vector3         m_vecScale;
  Ogre::Quaternion      m_quatOrient;
};

class CBaseEntity : public IBaseEntity, public CMementoObject
{
public:
	virtual ~CBaseEntity();

// IBaseEntity interface
public:
	virtual uint										        getEntID() const;

// ISelectableTarget interface
  virtual void                            setTargetSelected(bool bSelected);
  virtual bool                            isTargetSelected() const;

  virtual Ogre::Vector3                   getTargetPosition() const;
  virtual Ogre::Vector3                   getTargetScale() const;
  virtual Ogre::Quaternion                getTargetOrientation() const;

  virtual void                            setTargetPosition(const Ogre::Vector3& vecDelta);
  virtual void                            setTargetScale(const Ogre::Vector3& vecScale);
  virtual void                            setTargetOrientation(const Ogre::Quaternion& quatRotate);

  virtual void                            moveTarget(const Ogre::Vector3& vecDelta);
  virtual void                            scaleTarget(const Ogre::Vector3& vecScale);
  virtual void                            rotateTarget(const Ogre::Quaternion& quatRotate);

  virtual void                            deleteTarget();

  virtual const char*                     getType()
  {
    return "CBaseEntity";
  }

// Misc
  void                                    displayWireframe(bool bDisplay);

// IMementoObject interface
public:
  virtual void                            undo();
  virtual void                            redo();
  virtual void                            forgetLast();
  virtual void                            forgetFirst();

  void                                    initialCreation();
  void                                    createEnt(const std::string& sMesh, const Ogre::Vector3& vecPos,
                                                    const Ogre::Vector3& vecScale, const Ogre::Quaternion& quatOrient);
  void                                    deleteEnt();

// IBaseEntityRender interface
public:
	virtual bool										        setVisibilityFlag(int										iFlag);
	virtual bool										        setVisible(bool													display);
	virtual bool										        setModel(const std::string&							modelName);
	virtual bool										        activateAnimation(const std::string&		animName);
	virtual bool										        deactivateAnimation(const std::string&	animName);
	virtual bool										        setAnimationLoop(const std::string&			animName,
																	        								 bool										loop);
	virtual bool										        setAnimationSpeed(float									speed);

	virtual void										        attachObject(Ogre::MovableObject*				pObj);

  bool                                    setModel(const std::string&							modelName,
                                                   bool                           bUpdateProp = true);

// IBaseEntityPhysx interface
public:
	virtual Ogre::AxisAlignedBox		        getBBox() const;
	virtual const std::string&			        getModelName() const;

	virtual Ogre::Vector3						        getPosition() const;
	virtual Ogre::Quaternion				        getOrientation() const;
	virtual Ogre::Vector3						        getScale() const;
	
	virtual void										        setPosition(Ogre::Vector3 vecPos);
	virtual void										        setOrientation(Ogre::Quaternion quatOrient);
	virtual void										        setScale(Ogre::Vector3 scaleVec);

// Misc
public:
	void														        think(Ogre::Real		deltaTime);

private:
	void														        loadAnimations();
	void														        renderCleanup();
	virtual void										        renderFrame(float deltaTime);

private:
	CPropVar<uint>													m_nEntID;

private:
  IWE_DEFINE_PROP_HOLDER();

  CPropVar<Ogre::Vector3>                 m_vecPosition;
  CPropVar<Ogre::Vector3>                 m_vecScale;
  CPropVar<Ogre::Quaternion>              m_quatOrientation;

private:
  std::stack<e_mementoType>               m_stackMemUndoType;
  std::stack<Ogre::Vector3>               m_stackMemUndoVec3;
  std::stack<Ogre::Quaternion>            m_stackMemUndoQuat;
  std::stack<std::string>                 m_stackMemUndoStr;
  std::stack<s_entMemento>                m_stackMemUndoDelete;
  
  std::stack<e_mementoType>               m_stackMemRedoType;
  std::stack<Ogre::Vector3>               m_stackMemRedoVec3;
  std::stack<Ogre::Quaternion>            m_stackMemRedoQuat;
  std::stack<std::string>                 m_stackMemRedoStr;
  std::stack<s_entMemento>                m_stackMemRedoDelete;

  // Entity SelectableTarget
private:
  bool                                    m_bTargetSelected;
	Ogre::ManualObject*							        m_pSelectBoxObject;

  std::vector<Ogre::MaterialPtr>          m_vecOriMat;

  // Entity Render
private:
	Ogre::SceneNode*								        m_pNode;
	Ogre::Entity*										        m_pOgreEnt;

	CPropVar<std::string>						        m_pModelName;

	struct													        s_AnimInfos
	{
		Ogre::AnimationState*					        m_pAnimState;
		std::string										        m_sName;
		float													        m_fAnimSpeed;
		bool													        m_bFadingIn;
		bool													        m_bFadingOut;
	};

  CPropVar<std::vector<std::string>>      m_vecAnimations;
	std::vector<s_AnimInfos>				        m_vecAnimInfo;

private:
	// Create through CEntityList
	friend CEntityManager;
	CBaseEntity(uint	entID);
};

#endif // !CBASEENTITY_H