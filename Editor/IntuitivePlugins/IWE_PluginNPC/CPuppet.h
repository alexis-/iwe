#pragma once



using namespace System;
using namespace System::ComponentModel;

namespace IWE_PluginNPC {
  
  public ref class CPuppet
  {
  public:
    String^           m_sName;


  public:
    CPuppet(String^ name)
    {
      m_sName = name;
    }

  public:
    virtual String^ ToString() override
    {
      return m_sName;
    }
  };
}