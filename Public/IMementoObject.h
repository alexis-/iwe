#ifndef IMEMENTO_OBJECT_H
# define IMEMENTO_OBJECT_H

interface_class IMementoObject
{
public:
  virtual void                        undo() = 0;
  virtual void                        redo() = 0;
  virtual void                        forgetLast() = 0;
  virtual void                        forgetFirst() = 0;
};

#endif // !IMEMENTO_OBJECT_H