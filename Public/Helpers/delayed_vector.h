#ifndef DELAYED_VECTOR
# define DELAYED_VECTOR

# include <vector>

template <typename T>
class delayed_vector : public std::vector<T>
{
public:
  void push_back(T&& val)
  {
    m_vecToAdd.push_back(val);
  }
  
  void push_back(const T& val)
  {
    m_vecToAdd.push_back(val);
  }
    
  void pop_back()
  {
    m_vecToRemove.push_back(std::vector<T>::front());
  }

  typename std::vector<T>::iterator erase(typename std::vector<T>::const_iterator _Where)
  {
    m_vecToRemove.push_back(_Where);
    return std::vector<T>::end();
  }

  void execute()
  {
    for (size_t i = 0; i < m_vecToRemove.size(); i++)
      std::vector<T>::erase(m_vecToRemove[i]);

    for (size_t i = 0; i < m_vecToAdd.size(); i++)
      std::vector<T>::push_back(m_vecToAdd[i]);

    m_vecToRemove.clear();
    m_vecToAdd.clear();
  }
    
private:
  std::vector<T> m_vecToAdd;
  std::vector<typename std::vector<T>::const_iterator> m_vecToRemove;
};

#endif // !DELAYED_VECTOR