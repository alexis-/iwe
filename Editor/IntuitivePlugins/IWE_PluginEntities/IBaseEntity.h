#ifndef IBASEENTITY_H
# define IBASEENTITY_H

//# include "IBaseEntityPhysX.h"
//# include "IBaseEntityRender.h"

namespace Ogre
{
	class Vector3;
	class Quaternion;
	class AxisAlignedBox;
	class MovableObject;
};

//# include "IEditorEntity.h"

abstract_class IBaseEntity : public IEntity, public IPropertyHolder
{
public:
	virtual ~IBaseEntity() {};

public:
	virtual uint									getEntID() const = 0;
	virtual void									renderFrame(float deltaTime) = 0;

//public:
	//virtual IEditorEntity*				getEditorEnt() const = 0;

public:
	virtual void									attachObject(Ogre::MovableObject* pObj) = 0;

// Possibly move these to physic interface later ?
public:
	//virtual Ogre::AxisAlignedBox	getBBox() const = 0;
	//virtual const std::string&		getModelName() const = 0;

	//virtual Ogre::Vector3					getPosition() const = 0;
	//virtual Ogre::Quaternion			getOrientation() const = 0;
	//virtual Ogre::Vector3					getScale() const = 0;
	//
	//virtual void									setPosition(Ogre::Vector3 vecPos) = 0;
	//virtual void									setOrientation(Ogre::Quaternion quatOrient) = 0;
	//virtual void									setScale(Ogre::Vector3 scaleVec) = 0;
};

#endif // !IBASEENTITY_H