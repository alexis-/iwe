#ifndef MEMENTO_OBJECT_H
# define MEMENTO_OBJECT_H

class CMementoObject : public IMementoObject
{
protected:
  CMementoObject(IEditorInteraction* pEditorInteraction)
    : m_pEditorInteraction(pEditorInteraction)
  {
  }

  void                        saveState()
  {
    m_pEditorInteraction->saveState(this);
  }

protected:
  IEditorInteraction*         m_pEditorInteraction;
};

#endif // !MEMENTO_OBJECT_H