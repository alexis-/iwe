#ifndef DELEGATE_EVENT_H
# define DELEGATE_EVENT_H

// Last modified by:    Alexis
// Last modified date:  02/13/2012
//
// Alexis - ? : Original version
// Alexis - 02/13/2012 : Lambda return value eval functions

# include <vector>
# include <functional>

# include "FastDelegate.h"
# include "delayed_vector.h"

namespace DelegateEvent
{
  using namespace fastdelegate;

  template <typename T>
  void
  _DERemoveFromVector(std::vector<T>& vec, const T& e)
  {
    std::vector<T>::iterator it;

    for (it = vec.begin(); it != vec.end(); it++)
      if (*it == e)
        break;

    if (it != vec.end())
      vec.erase(it);
  }
  
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
  // Events
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

  template <class RetType=detail::DefaultVoid>
  class Event0 : public FastDelegate0<RetType>
  {
  public:
    Event0& operator+=(const FastDelegate0<RetType>& del)
    {
      m_vecDelegates.push_back(del);

			return *this;
    }

    Event0& operator-=(const FastDelegate0<RetType>& del)
    {
      _DERemoveFromVector(m_vecDelegates, del);

      return *this;
    }

    void operator()() const
    {
      for (size_t i = 0; i < m_vecDelegates.size(); i++)
        (m_vecDelegates[i])();
    }

    void operator()(std::function<void(RetType)> fRetEval) const
    {
      for (size_t i = 0; i < m_vecDelegates.size(); i++)
        fRetEval((m_vecDelegates[i])());
    }

    void clear()
    {
      m_vecDelegates.clear();
    }

  private:
    std::vector<FastDelegate0<RetType>> m_vecDelegates;
  };

  template <class Param1, class RetType=detail::DefaultVoid>
  class Event1
  {
  public:
    Event1& operator+=(const FastDelegate1<Param1, RetType>& del)
    {
      m_vecDelegates.push_back(del);

			return *this;
    }

    Event1& operator-=(const FastDelegate1<Param1, RetType>& del)
    {
      _DERemoveFromVector(m_vecDelegates, del);

      return *this;
    }

    void operator()(Param1 p1) const
    {
      for (size_t i = 0; i < m_vecDelegates.size(); i++)
        (m_vecDelegates[i])(p1);
    }

    void operator()(Param1 p1, std::function<void(RetType)> fRetEval) const
    {
      for (size_t i = 0; i < m_vecDelegates.size(); i++)
        fRetEval((m_vecDelegates[i])(p1));
    }

    void clear()
    {
      m_vecDelegates.clear();
    }

  private:
    std::vector<FastDelegate1<Param1, RetType>> m_vecDelegates;
  };

  template <class Param1, class Param2, class RetType=detail::DefaultVoid>
  class Event2 : public FastDelegate2<Param1, Param2, RetType>
  {
  public:
    Event2& operator+=(const FastDelegate2<Param1, Param2, RetType>& del)
    {
      m_vecDelegates.push_back(del);

			return *this;
    }

    Event2& operator-=(const FastDelegate2<Param1, Param2, RetType>& del)
    {
      _DERemoveFromVector(m_vecDelegates, del);

      return *this;
    }

    void operator()(Param1 p1, Param2 p2) const
    {
      for (size_t i = 0; i < m_vecDelegates.size(); i++)
        (m_vecDelegates[i])(p1, p2);
    }

    void operator()(Param1 p1, Param2 p2, std::function<void(RetType)> fRetEval) const
    {
      for (size_t i = 0; i < m_vecDelegates.size(); i++)
        fRetEval((m_vecDelegates[i])(p1, p2));
    }

    void clear()
    {
      m_vecDelegates.clear();
    }

  private:
    std::vector<FastDelegate2<Param1, Param2, RetType>> m_vecDelegates;
  };

  template <class Param1, class Param2, class Param3, class RetType=detail::DefaultVoid>
  class Event3 : public FastDelegate3<Param1, Param2, Param3, RetType>
  {
  public:
    Event3& operator+=(const FastDelegate3<Param1, Param2, Param3, RetType>& del)
    {
      m_vecDelegates.push_back(del);

			return *this;
    }

    Event3& operator-=(const FastDelegate3<Param1, Param2, Param3, RetType>& del)
    {
      _DERemoveFromVector(m_vecDelegates, del);

      return *this;
    }

    void operator()(Param1 p1, Param2 p2, Param3 p3) const
    {
      for (size_t i = 0; i < m_vecDelegates.size(); i++)
        (m_vecDelegates[i])(p1, p2, p3);
    }

    void operator()(Param1 p1, Param2 p2, Param3 p3, std::function<void(RetType)> fRetEval) const
    {
      for (size_t i = 0; i < m_vecDelegates.size(); i++)
        fRetEval((m_vecDelegates[i])(p1, p2, p3));
    }

    void clear()
    {
      m_vecDelegates.clear();
    }

  private:
    std::vector<FastDelegate3<Param1, Param2, Param3, RetType>> m_vecDelegates;
  };

  template <class Param1, class Param2, class Param3, class Param4, class RetType=detail::DefaultVoid>
  class Event4 : public FastDelegate4<Param1, Param2, Param3, Param4, RetType>
  {
  public:
    Event4& operator+=(const FastDelegate4<Param1, Param2, Param3, Param4, RetType>& del)
    {
      m_vecDelegates.push_back(del);

			return *this;
    }

    Event4& operator-=(const FastDelegate4<Param1, Param2, Param3, Param4, RetType>& del)
    {
      _DERemoveFromVector(m_vecDelegates, del);

      return *this;
    }

    void operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4) const
    {
      for (size_t i = 0; i < m_vecDelegates.size(); i++)
        (m_vecDelegates[i])(p1, p2, p3, p4);
    }

    void operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, std::function<void(RetType)> fRetEval) const
    {
      for (size_t i = 0; i < m_vecDelegates.size(); i++)
        fRetEval((m_vecDelegates[i])(p1, p2, p3, p4));
    }

    void clear()
    {
      m_vecDelegates.clear();
    }

  private:
    std::vector<FastDelegate4<Param1, Param2, Param3, Param4, RetType>> m_vecDelegates;
  };

  template <class Param1, class Param2, class Param3, class Param4, class Param5, class RetType=detail::DefaultVoid>
  class Event5 : public FastDelegate5<Param1, Param2, Param3, Param4, Param5, RetType>
  {
  public:
    Event5& operator+=(const FastDelegate5<Param1, Param2, Param3, Param4, Param5, RetType>& del)
    {
      m_vecDelegates.push_back(del);

			return *this;
    }

    Event5& operator-=(const FastDelegate5<Param1, Param2, Param3, Param4, Param5, RetType>& del)
    {
      _DERemoveFromVector(m_vecDelegates, del);

      return *this;
    }

    void operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5) const
    {
      for (size_t i = 0; i < m_vecDelegates.size(); i++)
        (m_vecDelegates[i])(p1, p2, p3, p4, p5);
    }

    void operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, std::function<void(RetType)> fRetEval) const
    {
      for (size_t i = 0; i < m_vecDelegates.size(); i++)
        fRetEval((m_vecDelegates[i])(p1, p2, p3, p4, p5));
    }

    void clear()
    {
      m_vecDelegates.clear();
    }

  private:
    std::vector<FastDelegate5<Param1, Param2, Param3, Param4, Param5, RetType>> m_vecDelegates;
  };
  
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
  // Delayed Events
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

  
  template <class RetType=detail::DefaultVoid>
  class DelDelEvent0 : public FastDelegate0<RetType>
  {
  public:
    DelDelEvent0& operator+=(const FastDelegate0<RetType>& del)
    {
      m_vecDelegates.push_back(del);

			return *this;
    }

    DelDelEvent0& operator-=(const FastDelegate0<RetType>& del)
    {
      _DERemoveFromVector(m_vecDelegates, del);

      return *this;
    }

    void operator()() const
    {
      for (size_t i = 0; i < m_vecDelegates.size(); i++)
        (m_vecDelegates[i])();
    }

    void operator()(std::function<void(RetType)> fRetEval) const
    {
      for (size_t i = 0; i < m_vecDelegates.size(); i++)
        fRetEval((m_vecDelegates[i])());
    }

    void update()
    {
      m_vecDelegates.execute();
    }

    void clear()
    {
      m_vecDelegates.clear();
    }

  private:
    delayed_vector<FastDelegate0<RetType>> m_vecDelegates;
  };

  template <class Param1, class RetType=detail::DefaultVoid>
  class DelEvent1
  {
  public:
    DelEvent1& operator+=(const FastDelegate1<Param1, RetType>& del)
    {
      m_vecDelegates.push_back(del);

			return *this;
    }

    DelEvent1& operator-=(const FastDelegate1<Param1, RetType>& del)
    {
      _DERemoveFromVector(m_vecDelegates, del);

      return *this;
    }

    void operator()(Param1 p1) const
    {
      for (size_t i = 0; i < m_vecDelegates.size(); i++)
        (m_vecDelegates[i])(p1);
    }

    void operator()(Param1 p1, std::function<void(RetType)> fRetEval) const
    {
      for (size_t i = 0; i < m_vecDelegates.size(); i++)
        fRetEval((m_vecDelegates[i])(p1));
    }

    void update()
    {
      m_vecDelegates.execute();
    }

    void clear()
    {
      m_vecDelegates.clear();
    }

  private:
    delayed_vector<FastDelegate1<Param1, RetType>> m_vecDelegates;
  };

  template <class Param1, class Param2, class RetType=detail::DefaultVoid>
  class DelEvent2 : public FastDelegate2<Param1, Param2, RetType>
  {
  public:
    DelEvent2& operator+=(const FastDelegate2<Param1, Param2, RetType>& del)
    {
      m_vecDelegates.push_back(del);

			return *this;
    }

    DelEvent2& operator-=(const FastDelegate2<Param1, Param2, RetType>& del)
    {
      _DERemoveFromVector(m_vecDelegates, del);

      return *this;
    }

    void operator()(Param1 p1, Param2 p2) const
    {
      for (size_t i = 0; i < m_vecDelegates.size(); i++)
        (m_vecDelegates[i])(p1, p2);
    }

    void operator()(Param1 p1, Param2 p2, std::function<void(RetType)> fRetEval) const
    {
      for (size_t i = 0; i < m_vecDelegates.size(); i++)
        fRetEval((m_vecDelegates[i])(p1, p2));
    }

    void update()
    {
      m_vecDelegates.execute();
    }

    void clear()
    {
      m_vecDelegates.clear();
    }

  private:
    delayed_vector<FastDelegate2<Param1, Param2, RetType>> m_vecDelegates;
  };

  template <class Param1, class Param2, class Param3, class RetType=detail::DefaultVoid>
  class DelEvent3 : public FastDelegate3<Param1, Param2, Param3, RetType>
  {
  public:
    DelEvent3& operator+=(const FastDelegate3<Param1, Param2, Param3, RetType>& del)
    {
      m_vecDelegates.push_back(del);

			return *this;
    }

    DelEvent3& operator-=(const FastDelegate3<Param1, Param2, Param3, RetType>& del)
    {
      _DERemoveFromVector(m_vecDelegates, del);

      return *this;
    }

    void operator()(Param1 p1, Param2 p2, Param3 p3) const
    {
      for (size_t i = 0; i < m_vecDelegates.size(); i++)
        (m_vecDelegates[i])(p1, p2, p3);
    }

    void operator()(Param1 p1, Param2 p2, Param3 p3, std::function<void(RetType)> fRetEval) const
    {
      for (size_t i = 0; i < m_vecDelegates.size(); i++)
        fRetEval((m_vecDelegates[i])(p1, p2, p3));
    }

    void update()
    {
      m_vecDelegates.execute();
    }

    void clear()
    {
      m_vecDelegates.clear();
    }

  private:
    delayed_vector<FastDelegate3<Param1, Param2, Param3, RetType>> m_vecDelegates;
  };

  template <class Param1, class Param2, class Param3, class Param4, class RetType=detail::DefaultVoid>
  class DelEvent4 : public FastDelegate4<Param1, Param2, Param3, Param4, RetType>
  {
  public:
    DelEvent4& operator+=(const FastDelegate4<Param1, Param2, Param3, Param4, RetType>& del)
    {
      m_vecDelegates.push_back(del);

			return *this;
    }

    DelEvent4& operator-=(const FastDelegate4<Param1, Param2, Param3, Param4, RetType>& del)
    {
      _DERemoveFromVector(m_vecDelegates, del);

      return *this;
    }

    void operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4) const
    {
      for (size_t i = 0; i < m_vecDelegates.size(); i++)
        (m_vecDelegates[i])(p1, p2, p3, p4);
    }

    void operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, std::function<void(RetType)> fRetEval) const
    {
      for (size_t i = 0; i < m_vecDelegates.size(); i++)
        fRetEval((m_vecDelegates[i])(p1, p2, p3, p4));
    }

    void update()
    {
      m_vecDelegates.execute();
    }

  private:
    delayed_vector<FastDelegate4<Param1, Param2, Param3, Param4, RetType>> m_vecDelegates;
  };

  template <class Param1, class Param2, class Param3, class Param4, class Param5, class RetType=detail::DefaultVoid>
  class DelEvent5 : public FastDelegate5<Param1, Param2, Param3, Param4, Param5, RetType>
  {
  public:
    DelEvent5& operator+=(const FastDelegate5<Param1, Param2, Param3, Param4, Param5, RetType>& del)
    {
      m_vecDelegates.push_back(del);

			return *this;
    }

    DelEvent5& operator-=(const FastDelegate5<Param1, Param2, Param3, Param4, Param5, RetType>& del)
    {
      _DERemoveFromVector(m_vecDelegates, del);

      return *this;
    }

    void operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5) const
    {
      for (size_t i = 0; i < m_vecDelegates.size(); i++)
        (m_vecDelegates[i])(p1, p2, p3, p4, p5);
    }

    void operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, std::function<void(RetType)> fRetEval) const
    {
      for (size_t i = 0; i < m_vecDelegates.size(); i++)
        fRetEval((m_vecDelegates[i])(p1, p2, p3, p4, p5));
    }

    void update()
    {
      m_vecDelegates.execute();
    }

    void clear()
    {
      m_vecDelegates.clear();
    }

  private:
    delayed_vector<FastDelegate5<Param1, Param2, Param3, Param4, Param5, RetType>> m_vecDelegates;
  };
  
  
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
  // Auto Reset Events
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

  template <class RetType=detail::DefaultVoid>
  class AutoResetEvent0 : public FastDelegate0<RetType>
  {
  public:
    AutoResetEvent0& operator+=(const FastDelegate0<RetType>& del)
    {
      m_vecDelegates.push_back(del);

			return *this;
    }

    AutoResetEvent0& operator-=(const FastDelegate0<RetType>& del)
    {
      _DERemoveFromVector(m_vecDelegates, del);

      return *this;
    }

    void operator()()
    {
      for (size_t i = 0; i < m_vecDelegates.size(); i++)
        (m_vecDelegates[i])();

      clear();
    }

    void operator()(std::function<void(RetType)> fRetEval)
    {
      for (size_t i = 0; i < m_vecDelegates.size(); i++)
        fRetEval((m_vecDelegates[i])());

      clear();
    }

    void clear()
    {
      m_vecDelegates.clear();
    }

  private:
    std::vector<FastDelegate0<RetType>> m_vecDelegates;
  };

  template <class Param1, class RetType=detail::DefaultVoid>
  class AutoResetEvent1
  {
  public:
    AutoResetEvent1& operator+=(const FastDelegate1<Param1, RetType>& del)
    {
      m_vecDelegates.push_back(del);

			return *this;
    }

    AutoResetEvent1& operator-=(const FastDelegate1<Param1, RetType>& del)
    {
      _DERemoveFromVector(m_vecDelegates, del);

      return *this;
    }

    void operator()(Param1 p1)
    {
      for (size_t i = 0; i < m_vecDelegates.size(); i++)
        (m_vecDelegates[i])(p1);
      
      clear();
    }

    void operator()(Param1 p1, std::function<void(RetType)> fRetEval)
    {
      for (size_t i = 0; i < m_vecDelegates.size(); i++)
        fRetEval((m_vecDelegates[i])(p1));

      clear();
    }

    void clear()
    {
      m_vecDelegates.clear();
    }

  private:
    std::vector<FastDelegate1<Param1, RetType>> m_vecDelegates;
  };

  template <class Param1, class Param2, class RetType=detail::DefaultVoid>
  class AutoResetEvent2 : public FastDelegate2<Param1, Param2, RetType>
  {
  public:
    AutoResetEvent2& operator+=(const FastDelegate2<Param1, Param2, RetType>& del)
    {
      m_vecDelegates.push_back(del);

			return *this;
    }

    AutoResetEvent2& operator-=(const FastDelegate2<Param1, Param2, RetType>& del)
    {
      _DERemoveFromVector(m_vecDelegates, del);

      return *this;
    }

    void operator()(Param1 p1, Param2 p2)
    {
      for (size_t i = 0; i < m_vecDelegates.size(); i++)
        (m_vecDelegates[i])(p1, p2);

      clear();
    }

    void operator()(Param1 p1, Param2 p2, std::function<void(RetType)> fRetEval)
    {
      for (size_t i = 0; i < m_vecDelegates.size(); i++)
        fRetEval((m_vecDelegates[i])(p1, p2));

      clear();
    }

    void clear()
    {
      m_vecDelegates.clear();
    }

  private:
    std::vector<FastDelegate2<Param1, Param2, RetType>> m_vecDelegates;
  };

  template <class Param1, class Param2, class Param3, class RetType=detail::DefaultVoid>
  class AutoResetEvent3 : public FastDelegate3<Param1, Param2, Param3, RetType>
  {
  public:
    AutoResetEvent3& operator+=(const FastDelegate3<Param1, Param2, Param3, RetType>& del)
    {
      m_vecDelegates.push_back(del);

			return *this;
    }

    AutoResetEvent3& operator-=(const FastDelegate3<Param1, Param2, Param3, RetType>& del)
    {
      _DERemoveFromVector(m_vecDelegates, del);

      return *this;
    }

    void operator()(Param1 p1, Param2 p2, Param3 p3)
    {
      for (size_t i = 0; i < m_vecDelegates.size(); i++)
        (m_vecDelegates[i])(p1, p2, p3);
      
      clear();
    }

    void operator()(Param1 p1, Param2 p2, Param3 p3, std::function<void(RetType)> fRetEval)
    {
      for (size_t i = 0; i < m_vecDelegates.size(); i++)
        fRetEval((m_vecDelegates[i])(p1, p2, p3));
      
      clear();
    }

    void clear()
    {
      m_vecDelegates.clear();
    }

  private:
    std::vector<FastDelegate3<Param1, Param2, Param3, RetType>> m_vecDelegates;
  };

  template <class Param1, class Param2, class Param3, class Param4, class RetType=detail::DefaultVoid>
  class AutoResetEvent4 : public FastDelegate4<Param1, Param2, Param3, Param4, RetType>
  {
  public:
    AutoResetEvent4& operator+=(const FastDelegate4<Param1, Param2, Param3, Param4, RetType>& del)
    {
      m_vecDelegates.push_back(del);

			return *this;
    }

    AutoResetEvent4& operator-=(const FastDelegate4<Param1, Param2, Param3, Param4, RetType>& del)
    {
      _DERemoveFromVector(m_vecDelegates, del);

      return *this;
    }

    void operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4)
    {
      for (size_t i = 0; i < m_vecDelegates.size(); i++)
        (m_vecDelegates[i])(p1, p2, p3, p4);

      clear();
    }

    void operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, std::function<void(RetType)> fRetEval)
    {
      for (size_t i = 0; i < m_vecDelegates.size(); i++)
        fRetEval((m_vecDelegates[i])(p1, p2, p3, p4));

      clear();
    }

    void clear()
    {
      m_vecDelegates.clear();
    }

  private:
    std::vector<FastDelegate4<Param1, Param2, Param3, Param4, RetType>> m_vecDelegates;
  };

  template <class Param1, class Param2, class Param3, class Param4, class Param5, class RetType=detail::DefaultVoid>
  class AutoResetEvent5 : public FastDelegate5<Param1, Param2, Param3, Param4, Param5, RetType>
  {
  public:
    AutoResetEvent5& operator+=(const FastDelegate5<Param1, Param2, Param3, Param4, Param5, RetType>& del)
    {
      m_vecDelegates.push_back(del);

			return *this;
    }

    AutoResetEvent5& operator-=(const FastDelegate5<Param1, Param2, Param3, Param4, Param5, RetType>& del)
    {
      _DERemoveFromVector(m_vecDelegates, del);

      return *this;
    }

    void operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5)
    {
      for (size_t i = 0; i < m_vecDelegates.size(); i++)
        (m_vecDelegates[i])(p1, p2, p3, p4, p5);

      clear();
    }

    void operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, std::function<void(RetType)> fRetEval)
    {
      for (size_t i = 0; i < m_vecDelegates.size(); i++)
        fRetEval((m_vecDelegates[i])(p1, p2, p3, p4, p5));

      clear();
    }

    void clear()
    {
      m_vecDelegates.clear();
    }

  private:
    std::vector<FastDelegate5<Param1, Param2, Param3, Param4, Param5, RetType>> m_vecDelegates;
  };
}

#endif // !DELEGATE_EVENT_H