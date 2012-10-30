#pragma once

#include "Editor/Inspect/wxWidget.h"

namespace Helium
{
    namespace Editor
    {
        class CheckBoxWidget;

        class CheckBoxWindow : public wxCheckBox
        {
        public:
            CheckBoxWindow( wxWindow* parent, CheckBoxWidget* checkBoxWidget );

            void SetOverride( bool override )
            {
                m_Override = override;
            }

            void SetUndetermined()
            {
                Set3StateValue( wxCHK_UNDETERMINED );
            }

            void OnChecked( wxCommandEvent& );

        private:
            CheckBoxWidget* m_CheckBoxWidget;
            bool            m_Override;
        };

        class CheckBoxWidget : public Widget
        {
        public:
            REFLECT_DECLARE_OBJECT( CheckBoxWidget, Widget );

            CheckBoxWidget()
                : m_CheckBoxControl( NULL )
                , m_CheckBoxWindow( NULL )
            {

            }

            CheckBoxWidget( Inspect::CheckBox* control );

            virtual void CreateWindow( wxWindow* parent ) HELIUM_OVERRIDE;
            virtual void DestroyWindow() HELIUM_OVERRIDE;

            // Inspect::Widget
            virtual void Read() HELIUM_OVERRIDE;
            virtual bool Write() HELIUM_OVERRIDE;

            // Editor::Widget
            virtual void IsReadOnlyChanged( const Attribute<bool>::ChangeArgs& args ) HELIUM_OVERRIDE;
            
            // Listeners
            void HighlightChanged( const Attribute< bool >::ChangeArgs& args );

        protected:
            Inspect::CheckBox*  m_CheckBoxControl;
            CheckBoxWindow*     m_CheckBoxWindow;
        };
    }
}