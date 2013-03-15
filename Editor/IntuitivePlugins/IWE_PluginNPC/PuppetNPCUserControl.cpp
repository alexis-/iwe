#include "IWECLRPlugin.h"
#include "PuppetNPCUserControl.h"

#include <OgreCamera.h>
#include <OgreViewport.h>

#define RenderMaskID 66

namespace IWE_PluginNPC
{


  PuppetNPCUserControl::PuppetNPCUserControl(void)
  {
  	InitializeComponent();

    ////////////

    m_editPuppet = nullptr;

    m_puppetList = gcnew System::Collections::Generic::List<CPuppet^>();
    m_puppetListFiltered = gcnew System::Collections::Generic::List<CPuppet^>();
    
    {
      m_puppetList->Add(gcnew CPuppet("test"));
      m_puppetList->Add(gcnew CPuppet("abra"));
      m_puppetList->Add(gcnew CPuppet("cada"));
      m_puppetList->Add(gcnew CPuppet("bra"));
      m_puppetList->Add(gcnew CPuppet("plouf"));
      m_puppetList->Add(gcnew CPuppet("plif"));
    }
    
    lbPuppets->DataSource = m_puppetList;

    ////////////

  
    size_t RenderHandle = (size_t)panelRenderTarget->Handle.ToPointer();

    CIWEPoint<ushort> size(panelRenderTarget->Size.Width, panelRenderTarget->Size.Height);

    m_pViewport = GetRenderEngine()->createViewport(RenderHandle, size, false, RenderMaskID);
  }
}