#pragma once

#include <afxWinForms.h>

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

using namespace Microsoft::VisualC;

#include "CPuppet.h"


namespace IWE_PluginNPC {

  /// <summary>
  /// Summary for NPCUserControl
  /// </summary>
  public ref class PuppetNPCUserControl : public System::Windows::Forms::UserControl
    , public Microsoft::VisualC::MFC::ICommandTarget 
    , public Microsoft::VisualC::MFC::IView
  {
  public:
    PuppetNPCUserControl(void);

  public:
    virtual void OnInitialUpdate(){}
    virtual void OnUpdate(){}
    virtual void OnActivateView(bool activate){}
  private: System::Windows::Forms::GroupBox^  gbPuppetView;
  public: 

  private: System::Windows::Forms::Panel^  panelRenderTarget;
  private: System::Windows::Forms::GroupBox^  gbPuppetEdit;
  private: System::Windows::Forms::TabControl^  tabCtrlPuppet;
  private: System::Windows::Forms::TabPage^  tabAppearance;
  private: System::Windows::Forms::TabPage^  tabStats;

  private: System::Windows::Forms::TextBox^  tbSearchPuppet;

  private: System::Windows::Forms::GroupBox^  gbEditActions;
  private: System::Windows::Forms::Button^  btnNewPuppet;
  private: System::Windows::Forms::Button^  BtnDelPuppet;
  private: System::Windows::Forms::ListBox^  lbPuppets;

  public: 
    Microsoft::VisualC::MFC::ICommandSource^ m_CmdSource;

    virtual void Initialize(Microsoft::VisualC::MFC::ICommandSource^ src ) 
    {
      m_CmdSource = src;
    }

  protected:
    /// <summary>
    /// Clean up any resources being used.
    /// </summary>
    ~PuppetNPCUserControl()
    {
      if (components)
      {
        delete components;
      }
    }

  private:
    /// <summary>
    /// Required designer variable.
    /// </summary>
    System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
    /// <summary>
    /// Required method for Designer support - do not modify
    /// the contents of this method with the code editor.
    /// </summary>
    void InitializeComponent(void)
    {
      this->gbPuppetView = (gcnew System::Windows::Forms::GroupBox());
      this->panelRenderTarget = (gcnew System::Windows::Forms::Panel());
      this->gbPuppetEdit = (gcnew System::Windows::Forms::GroupBox());
      this->gbEditActions = (gcnew System::Windows::Forms::GroupBox());
      this->btnNewPuppet = (gcnew System::Windows::Forms::Button());
      this->BtnDelPuppet = (gcnew System::Windows::Forms::Button());
      this->tbSearchPuppet = (gcnew System::Windows::Forms::TextBox());
      this->tabCtrlPuppet = (gcnew System::Windows::Forms::TabControl());
      this->tabAppearance = (gcnew System::Windows::Forms::TabPage());
      this->tabStats = (gcnew System::Windows::Forms::TabPage());
      this->lbPuppets = (gcnew System::Windows::Forms::ListBox());
      this->gbPuppetView->SuspendLayout();
      this->gbPuppetEdit->SuspendLayout();
      this->gbEditActions->SuspendLayout();
      this->tabCtrlPuppet->SuspendLayout();
      this->SuspendLayout();
      // 
      // gbPuppetView
      // 
      this->gbPuppetView->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
        | System::Windows::Forms::AnchorStyles::Left));
      this->gbPuppetView->Controls->Add(this->panelRenderTarget);
      this->gbPuppetView->Location = System::Drawing::Point(3, 3);
      this->gbPuppetView->Name = L"gbPuppetView";
      this->gbPuppetView->Size = System::Drawing::Size(270, 500);
      this->gbPuppetView->TabIndex = 2;
      this->gbPuppetView->TabStop = false;
      this->gbPuppetView->Text = L"Visualization";
      // 
      // panelRenderTarget
      // 
      this->panelRenderTarget->BackColor = System::Drawing::SystemColors::ControlDarkDark;
      this->panelRenderTarget->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
      this->panelRenderTarget->Location = System::Drawing::Point(6, 19);
      this->panelRenderTarget->Name = L"panelRenderTarget";
      this->panelRenderTarget->Size = System::Drawing::Size(258, 360);
      this->panelRenderTarget->TabIndex = 2;
      // 
      // gbPuppetEdit
      // 
      this->gbPuppetEdit->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
        | System::Windows::Forms::AnchorStyles::Left) 
        | System::Windows::Forms::AnchorStyles::Right));
      this->gbPuppetEdit->Controls->Add(this->gbEditActions);
      this->gbPuppetEdit->Controls->Add(this->tbSearchPuppet);
      this->gbPuppetEdit->Controls->Add(this->tabCtrlPuppet);
      this->gbPuppetEdit->Controls->Add(this->lbPuppets);
      this->gbPuppetEdit->Location = System::Drawing::Point(280, 4);
      this->gbPuppetEdit->Name = L"gbPuppetEdit";
      this->gbPuppetEdit->Size = System::Drawing::Size(740, 499);
      this->gbPuppetEdit->TabIndex = 4;
      this->gbPuppetEdit->TabStop = false;
      this->gbPuppetEdit->Text = L"Editing";
      // 
      // gbEditActions
      // 
      this->gbEditActions->Controls->Add(this->btnNewPuppet);
      this->gbEditActions->Controls->Add(this->BtnDelPuppet);
      this->gbEditActions->Location = System::Drawing::Point(7, 18);
      this->gbEditActions->Name = L"gbEditActions";
      this->gbEditActions->RightToLeft = System::Windows::Forms::RightToLeft::Yes;
      this->gbEditActions->Size = System::Drawing::Size(180, 83);
      this->gbEditActions->TabIndex = 8;
      this->gbEditActions->TabStop = false;
      this->gbEditActions->Text = L"Actions";
      // 
      // btnNewPuppet
      // 
      this->btnNewPuppet->Location = System::Drawing::Point(7, 20);
      this->btnNewPuppet->Name = L"btnNewPuppet";
      this->btnNewPuppet->Size = System::Drawing::Size(167, 23);
      this->btnNewPuppet->TabIndex = 1;
      this->btnNewPuppet->Text = L"New Puppet";
      this->btnNewPuppet->UseVisualStyleBackColor = true;
      // 
      // BtnDelPuppet
      // 
      this->BtnDelPuppet->Location = System::Drawing::Point(7, 49);
      this->BtnDelPuppet->Name = L"BtnDelPuppet";
      this->BtnDelPuppet->Size = System::Drawing::Size(167, 23);
      this->BtnDelPuppet->TabIndex = 0;
      this->BtnDelPuppet->Text = L"Delete Puppet";
      this->BtnDelPuppet->UseVisualStyleBackColor = true;
      // 
      // tbSearchPuppet
      // 
      this->tbSearchPuppet->BackColor = System::Drawing::SystemColors::Window;
      this->tbSearchPuppet->Location = System::Drawing::Point(6, 120);
      this->tbSearchPuppet->Name = L"tbSearchPuppet";
      this->tbSearchPuppet->Size = System::Drawing::Size(169, 20);
      this->tbSearchPuppet->TabIndex = 7;
      this->tbSearchPuppet->Text = L"Search";
      this->tbSearchPuppet->Click += gcnew System::EventHandler(this, &PuppetNPCUserControl::tbSearchPuppet_Click);
      this->tbSearchPuppet->TextChanged += gcnew System::EventHandler(this, &PuppetNPCUserControl::tbSearchPuppet_TextChanged);
      this->tbSearchPuppet->Enter += gcnew System::EventHandler(this, &PuppetNPCUserControl::tbSearchPuppet_Enter);
      this->tbSearchPuppet->Leave += gcnew System::EventHandler(this, &PuppetNPCUserControl::tbSearchPuppet_Leave);
      // 
      // tabCtrlPuppet
      // 
      this->tabCtrlPuppet->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
        | System::Windows::Forms::AnchorStyles::Left) 
        | System::Windows::Forms::AnchorStyles::Right));
      this->tabCtrlPuppet->Controls->Add(this->tabAppearance);
      this->tabCtrlPuppet->Controls->Add(this->tabStats);
      this->tabCtrlPuppet->Location = System::Drawing::Point(193, 20);
      this->tabCtrlPuppet->Name = L"tabCtrlPuppet";
      this->tabCtrlPuppet->SelectedIndex = 0;
      this->tabCtrlPuppet->Size = System::Drawing::Size(541, 472);
      this->tabCtrlPuppet->TabIndex = 6;
      // 
      // tabAppearance
      // 
      this->tabAppearance->Location = System::Drawing::Point(4, 22);
      this->tabAppearance->Name = L"tabAppearance";
      this->tabAppearance->Padding = System::Windows::Forms::Padding(3);
      this->tabAppearance->Size = System::Drawing::Size(533, 446);
      this->tabAppearance->TabIndex = 0;
      this->tabAppearance->Text = L"Appearance";
      this->tabAppearance->UseVisualStyleBackColor = true;
      // 
      // tabStats
      // 
      this->tabStats->Location = System::Drawing::Point(4, 22);
      this->tabStats->Name = L"tabStats";
      this->tabStats->Padding = System::Windows::Forms::Padding(3);
      this->tabStats->Size = System::Drawing::Size(533, 446);
      this->tabStats->TabIndex = 1;
      this->tabStats->Text = L"Attributes";
      this->tabStats->UseVisualStyleBackColor = true;
      // 
      // lbPuppets
      // 
      this->lbPuppets->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
        | System::Windows::Forms::AnchorStyles::Left));
      this->lbPuppets->BackColor = System::Drawing::SystemColors::ControlDarkDark;
      this->lbPuppets->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 11, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
        static_cast<System::Byte>(0)));
      this->lbPuppets->ForeColor = System::Drawing::SystemColors::Menu;
      this->lbPuppets->FormattingEnabled = true;
      this->lbPuppets->ItemHeight = 18;
      this->lbPuppets->Location = System::Drawing::Point(7, 146);
      this->lbPuppets->Name = L"lbPuppets";
      this->lbPuppets->Size = System::Drawing::Size(180, 346);
      this->lbPuppets->TabIndex = 5;
      // 
      // PuppetNPCUserControl
      // 
      this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
      this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
      this->AutoSize = true;
      this->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
      this->Controls->Add(this->gbPuppetEdit);
      this->Controls->Add(this->gbPuppetView);
      this->Name = L"PuppetNPCUserControl";
      this->Size = System::Drawing::Size(1024, 510);
      this->gbPuppetView->ResumeLayout(false);
      this->gbPuppetEdit->ResumeLayout(false);
      this->gbPuppetEdit->PerformLayout();
      this->gbEditActions->ResumeLayout(false);
      this->tabCtrlPuppet->ResumeLayout(false);
      this->ResumeLayout(false);

    }
#pragma endregion

  private:

  protected:
    IViewport*                                      m_pViewport;
    
    CPuppet^                                        m_editPuppet;

    System::Collections::Generic::List<CPuppet^>^   m_puppetList;
    System::Collections::Generic::List<CPuppet^>^   m_puppetListFiltered;

  private:
    System::Void tbSearchPuppet_TextChanged(System::Object^  sender, System::EventArgs^  e)
    {
      if (tbSearchPuppet->Text == "" || tbSearchPuppet->Text == "Search")
      {
        lbPuppets->DataSource = m_puppetList;
        m_puppetListFiltered->Clear();
      }
      else
      {
        for each (CPuppet^ p in m_puppetList)
        {
          String^ s = p->ToString();

          if (s->Contains(tbSearchPuppet->Text))
            m_puppetListFiltered->Add(p);
        }

        lbPuppets->DataSource = m_puppetListFiltered;
      }
    }

    System::Void tbSearchPuppet_Click(System::Object^  sender, System::EventArgs^  e)
    {
    }

    System::Void tbSearchPuppet_Leave(System::Object^  sender, System::EventArgs^  e)
    {
      if (tbSearchPuppet->Text == "")
        tbSearchPuppet->Text = "Search";
    }

    System::Void tbSearchPuppet_Enter(System::Object^  sender, System::EventArgs^  e)
    {
      if (tbSearchPuppet->Text == "Search")
        tbSearchPuppet->Text = "";
    }
  };
}
