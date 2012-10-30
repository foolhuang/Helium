#pragma once

#include "Foundation/Flags.h"

#include "Math/FpuPoint.h"

#include "SceneGraph/API.h"

namespace Helium
{
    class HELIUM_SCENE_GRAPH_API Input
    {
    public:
        Input()
            : m_Skipped( false )
        {

        }

        bool GetSkipped() const
        {
            return m_Skipped;
        }

        void Skip( bool skip = true ) const
        {
            m_Skipped = skip;
        }

    private:
        mutable bool m_Skipped;
    };

    namespace MouseButtonEvents
    {
        enum MouseButtonEvent
        {
            Up          = 0,
            Down        = 1,
            DoubleClick = 2,
        };
    }
    typedef MouseButtonEvents::MouseButtonEvent MouseButtonEvent;

    namespace MouseButtons
    {
        enum MouseButton
        {
            Left        = 1 << 0,
            Middle      = 1 << 1,
            Right       = 1 << 2,
            Forward     = 1 << 3,
            Backward    = 1 << 4,
        };
    }

    namespace AcceleratorButtons
    {
        enum AcceleratorButton
        {
            Alt         = 1 << 16,
            Ctrl        = 1 << 17,
            Shift       = 1 << 18,
        };
    }

    class HELIUM_SCENE_GRAPH_API MouseInput : public Input
    {
    public:
        MouseInput()
            : m_Buttons( 0x0 )
        {

        }

        uint32_t GetButtons() const
        {
            return m_Buttons;
        }

        void SetButtons( uint32_t states )
        {
            m_Buttons = states;
        }

        Point GetPosition() const
        {
            return m_Position;
        }

        void SetPosition( const Point& p )
        {
            m_Position = p;
        }

        // helpers

        bool IsButtonDown( uint32_t button ) const
        {
            return (m_Buttons & button) != 0;
        }

        bool IsButtonUp( uint32_t button ) const
        {
            return !IsButtonDown( button );
        }

        bool IsOnlyButtonDown( uint32_t button ) const
        {
            return (m_Buttons & button) == button;
        }

        bool LeftIsUp() const       { return IsButtonUp( MouseButtons::Left ); }
        bool MiddleIsUp() const     { return IsButtonUp( MouseButtons::Middle ); }
        bool RightIsUp() const      { return IsButtonUp( MouseButtons::Right ); }

        bool LeftIsDown() const     { return IsButtonDown( MouseButtons::Left ); }
        bool MiddleIsDown() const   { return IsButtonDown( MouseButtons::Middle ); }
        bool RightIsDown() const    { return IsButtonDown( MouseButtons::Right ); }

        bool AltIsDown() const      { return IsButtonDown( AcceleratorButtons::Alt ); }
        bool CtrlIsDown() const     { return IsButtonDown( AcceleratorButtons::Ctrl ); }
        bool ShiftIsDown() const    { return IsButtonDown( AcceleratorButtons::Shift ); }

    protected:
        uint32_t        m_Buttons;  // the states of all the buttons (always set)
        Point           m_Position; // the position of the cursor on the screen (always set)
    };

    class HELIUM_SCENE_GRAPH_API MouseMoveInput : public MouseInput
    {
    public:
        bool Dragging() const
        {
            return m_Buttons != 0x0;
        }
    };

    class HELIUM_SCENE_GRAPH_API MouseButtonInput : public MouseInput
    {
    public:
        MouseButtonInput()
            : m_Event( MouseButtonEvents::Up )
            , m_Button( 0x0 )
        {

        }

        MouseButtonEvent GetEvent() const
        {
            return m_Event;
        }

        void SetEvent( MouseButtonEvent e )
        {
            m_Event = e;
        }

        uint32_t GetButton() const
        {
            return m_Button;
        }

        void SetButton( uint32_t button )
        {
            m_Button = button;
        }

        // helpers

        bool LeftUp() const             { return m_Event == MouseButtonEvents::Up && m_Button == MouseButtons::Left; }
        bool MiddleUp() const           { return m_Event == MouseButtonEvents::Up && m_Button == MouseButtons::Middle; }
        bool RightUp() const            { return m_Event == MouseButtonEvents::Up && m_Button == MouseButtons::Right; }

        bool LeftDown() const           { return m_Event == MouseButtonEvents::Down && m_Button == MouseButtons::Left; }
        bool MiddleDown() const         { return m_Event == MouseButtonEvents::Down && m_Button == MouseButtons::Middle; }
        bool RightDown() const          { return m_Event == MouseButtonEvents::Down && m_Button == MouseButtons::Right; }

        bool LeftDoubleClick() const    { return m_Event == MouseButtonEvents::DoubleClick && m_Button == MouseButtons::Left; }
        bool MiddleDoubleClick() const  { return m_Event == MouseButtonEvents::DoubleClick && m_Button == MouseButtons::Middle; }
        bool RightDoubleClick() const   { return m_Event == MouseButtonEvents::DoubleClick && m_Button == MouseButtons::Right; }

    private:
        MouseButtonEvent    m_Event;        // what happened?
        uint32_t                 m_Button;       // to which button?
    };

    class HELIUM_SCENE_GRAPH_API MouseScrollInput : public MouseInput
    {
    public:
        MouseScrollInput()
            : m_WheelRotation( 0.f )
            , m_WheelDelta( 0.f )
        {
        }

        float32_t GetWheelRotation() const
        {
            return m_WheelRotation;
        }

        void SetWheelRotation( float32_t wheelRotation )
        {
            m_WheelRotation = wheelRotation;
        }

        float32_t GetWheelDelta() const
        {
            return m_WheelDelta;
        }

        void SetWheelDelta( float32_t wheelDelta )
        {
            m_WheelDelta = wheelDelta;
        }

    private:
        float32_t m_WheelRotation;    // wheel rotation amount
        float32_t m_WheelDelta;       // wheel rotation threshold of a notch
    };

    namespace KeyCodes
    {
        //
        // 33 -> 126 ASCII characters,  128 -> 255 ASCII extended characters
        //
        enum KeyCode
        {
            // Keyboard keys
            Escape      =   0,
            Tab         =   1,
            Backspace   =   2,
            Return      =   3,
            Insert      =   4,
            Delete      =   5,
            Home        =   6,
            End         =   7,
            PageUp      =   8,
            PageDown    =   9,
            Left        =  10,
            Right       =  11,
            Up          =  12,
            Down        =  13,
            Alt         =  14,
            Ctrl        =  15,
            Shift       =  16,
            System      =  17,
            Menu        =  18,
            F1          =  19,
            F2          =  20,
            F3          =  21,
            F4          =  22,
            F5          =  23,
            F6          =  24,
            F7          =  25,
            F8          =  26,
            F9          =  27,
            F10         =  28,
            F11         =  29,
            F12         =  30,

            // ASCII
            Space = 32,
            Exclamation,
            DoubleQuote,
            Number,
            Dollar,
            Percent,
            Ampersand,
            Quote,
            LeftParenthesis,
            RightParenthesis,
            Asterisk,
            Plus,
            Comma,
            Minus,
            Period,
            Slash,
            Zero,
            One,
            Two,
            Three,
            Four,
            Five,
            Six,
            Seven,
            Eight,
            Nine,
            Colon,
            Semicolon,
            LessThan,
            Equals,
            GreaterThan,
            Question,
            At,
            A,
            B,
            C,
            D,
            E,
            F,
            G,
            H,
            I,
            J,
            K,
            L,
            M,
            N,
            O,
            P,
            Q,
            R,
            S,
            T,
            U,
            V,
            W,
            X,
            Y,
            Z,
            LeftBracket,
            Backslash,
            RightBracket,
            Caret,
            Underscore,
            BackTick,
            a,
            b,
            c,
            d,
            e,
            f,
            g,
            h,
            i,
            j,
            k,
            l,
            m,
            n,
            o,
            p,
            q,
            r,
            s,
            t,
            u,
            v,
            w,
            x,
            y,
            z,
            LeftBrace,
            Pipe,
            RightBrace,
            Tilde,
            Box
        };

        HELIUM_COMPILE_ASSERT( (int)Box == 127 );
    }
    typedef KeyCodes::KeyCode KeyCode;

    namespace Modifiers
    {
        enum Modifier
        {
            Ctrl  = 1 << 0,
            Alt   = 1 << 1,
            Shift = 1 << 2,
            Meta  = 1 << 3,
        };
    }

    class HELIUM_SCENE_GRAPH_API KeyboardInput : public Input
    {
    public:
        KeyboardInput( KeyCode code = (KeyCode)0x0, short modifier = 0x0 )
            : m_KeyCode( code )
            , m_Modifiers( modifier )
        {

        }

        KeyCode GetKeyCode() const
        {
            return m_KeyCode;
        }

        void SetKeyCode( KeyCode code )
        {
            m_KeyCode = code;
        }

        void SetCtrlDown( bool value )
        {
            SetFlag< short >( m_Modifiers, Modifiers::Ctrl, value );
        }
        bool IsCtrlDown() const
        {
            return HasFlags< short >( m_Modifiers, Modifiers::Ctrl );
        }

        void SetAltDown( bool value )
        {
            SetFlag< short >( m_Modifiers, Modifiers::Alt, value );
        }
        bool IsAltDown() const
        {
            return HasFlags< short >( m_Modifiers, Modifiers::Alt );
        }

        void SetShiftDown( bool value )
        {
            SetFlag< short >( m_Modifiers, Modifiers::Shift, value );
        }
        bool IsShiftDown() const
        {
            return HasFlags< short >( m_Modifiers, Modifiers::Shift );
        }

        void SetMetaDown( bool value )
        {
            SetFlag< short >( m_Modifiers, Modifiers::Meta, value );
        }
        bool IsMetaDown() const
        {
            return HasFlags< short >( m_Modifiers, Modifiers::Meta );
        }

    private:
        KeyCode m_KeyCode;
        short   m_Modifiers;
    };
}