#ifndef IENTITY_H
# define IENTITY_H


interface_class IEntityRender
{
public:
	virtual ~IEntityRender() {};
  
public:
	virtual void									renderFrame(float deltaTime) = 0;

public:
	virtual bool					        setVisibilityFlag(int											iFlag) = 0;
	virtual bool					        setVisible(bool														display) = 0;

	virtual bool					        setModel(const std::string&								modelName) = 0;

	virtual bool					        activateAnimation(const std::string&			animName) = 0;
	virtual bool					        deactivateAnimation(const std::string&		animName) = 0;

	virtual bool					        setAnimationSpeed(float										speed) = 0;
};

interface_class IEntity : public IEntityRender, public ISelectableTarget
{
public:
  virtual ~IEntity() {};
  
public:
	virtual Ogre::AxisAlignedBox	getBBox() const = 0;
	virtual const std::string&		getModelName() const = 0;

	virtual Ogre::Vector3					getPosition() const = 0;
	virtual Ogre::Quaternion			getOrientation() const = 0;
	virtual Ogre::Vector3					getScale() const = 0;
	
	virtual void									setPosition(Ogre::Vector3 vecPos) = 0;
	virtual void									setOrientation(Ogre::Quaternion quatOrient) = 0;
	virtual void									setScale(Ogre::Vector3 scaleVec) = 0;
};

#endif // !IENTITY_H