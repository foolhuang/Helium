#pragma once

#include "Editor/Inspect/wxWidget.h"

#include <wx/textctrl.h>
#include <wx/combobox.h>

namespace Helium
{
    namespace Editor
    {
        class ChoiceWidget;

        class ChoiceWindow : public wxComboBox
        {
        public:
            ChoiceWindow(wxWindow* parent, ChoiceWidget* choiceWidget, int flags);

            void SetOverride( bool override )
            {
                m_Override = override;
            }

            void OnConfirm(wxCommandEvent& event);
            void OnSetFocus(wxFocusEvent& event);
            void OnKillFocus(wxFocusEvent& event);

            DECLARE_EVENT_TABLE();

        public:
            ChoiceWidget*   m_ChoiceWidget;
            bool            m_Override;
        };

        class ChoiceWidget : public Widget
        {
        public:
            REFLECT_DECLARE_OBJECT( ChoiceWidget, Widget );

            ChoiceWidget()
                : m_ChoiceControl( NULL )
                , m_ChoiceWindow( NULL )
            {

            }

            ChoiceWidget( Inspect::Choice* choice );

            virtual void CreateWindow( wxWindow* parent ) HELIUM_OVERRIDE;
            virtual void DestroyWindow() HELIUM_OVERRIDE;

            virtual void Read() HELIUM_OVERRIDE;
            virtual bool Write() HELIUM_OVERRIDE;

            void HighlightChanged( const Attribute<bool>::ChangeArgs& args );
            void ItemsChanged( const Attribute< std::vector< Inspect::ChoiceItem > >::ChangeArgs& args );

            tstring GetValue();
            void SetValue( const tstring& value );

        private:
            Inspect::Choice*    m_ChoiceControl;
            ChoiceWindow*       m_ChoiceWindow;
        };
    }
}