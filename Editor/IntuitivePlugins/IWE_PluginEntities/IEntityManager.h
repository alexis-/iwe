#ifndef IENTITY_MANAGER_H
# define IENTITY_MANAGER_H

interface_class IEntityManager
{
public:
  virtual IEntity*                createEntity() = 0;
	virtual void										clear() = 0;

public:
	typedef std::vector<IEntity*>::iterator					iterator;
	typedef std::vector<IEntity*>::const_iterator		const_iterator;
  
	virtual iterator                begin() = 0;
	virtual iterator                end() = 0;

	virtual const_iterator          begin() const = 0;
	virtual const_iterator          end() const = 0;

	virtual uint                    getCount() const = 0;

public:
  Event1<IEntity*>                eventEntityCreated;
  Event1<IEntity*>                eventEntityRemoved;
};

#endif // !IENTITY_MANAGER_H