#pragma once

#include "Editor/ArtProvider.h"
#include "Editor/Inspect/wxWidget.h"

namespace Helium
{
    namespace Editor
    {
        class ButtonWidget;

        class ButtonWindow : public wxPanel
        {
        public:
            ButtonWindow( wxWindow* parent, ButtonWidget* buttonWidget );

            virtual void OnClicked( wxCommandEvent& );

            void SetIcon( const tstring& icon );
            void SetLabel( const tstring& label );

        protected:
            wxSizer* m_Sizer;
            wxButton* m_Button;
            ButtonWidget* m_ButtonWidget;
        };

        class ButtonWidget : public Widget
        {
        public:
            REFLECT_DECLARE_OBJECT( ButtonWidget, Widget );

            ButtonWidget()
                : m_ButtonControl( NULL )
                , m_ButtonWindow( NULL )
            {

            }

            ButtonWidget( Inspect::Button* button );

            virtual void CreateWindow( wxWindow* parent ) HELIUM_OVERRIDE;
            virtual void DestroyWindow() HELIUM_OVERRIDE;

            virtual void Read() HELIUM_OVERRIDE {}
            virtual bool Write() HELIUM_OVERRIDE { return true; }

            void SetLabel( const tstring& label );
            void SetIcon( const tstring& icon );

        protected:
            void OnIconChanged( const Attribute< tstring >::ChangeArgs& args );
            void OnLabelChanged( const Attribute< tstring >::ChangeArgs& args );

            Inspect::Button*  m_ButtonControl;
            ButtonWindow*     m_ButtonWindow;
        };
    }
}