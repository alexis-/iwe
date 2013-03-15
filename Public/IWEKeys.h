#ifndef IWE_KEYS_H
# define IWE_KEYS_H

// http://code.google.com/p/gdk/source/browse/trunk/Source/Gdk/Input/Keyboard.cpp?spec=svn65&r=65
namespace IWEKeys
{
  enum e_IWEKey
  {
    Unknown = 0,

    // Digits / Numbers
    D0,
    D1,
    D2,
    D3,
    D4,
    D5,
    D6,
    D7,
    D8,
    D9,

    // Letters
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

    // Function Keys
    F1,
    F2,
    F3,
    F4,
    F5,
    F6,
    F7,
    F8,
    F9,
    F10,
    F11,
    F12,
    F13,
    F14,
    F15,
    F16,
    F17,
    F18,
    F19,
    F20,
    F21,
    F22,
    F23,
    F24,

    // Number pad
    NumPad0,
    NumPad1,
    NumPad2,
    NumPad3,
    NumPad4,
    NumPad5,
    NumPad6,
    NumPad7,
    NumPad8,
    NumPad9,

    NumPadAdd,
    NumPadMinus,
    NumPadMultiply,
    NumPadDivide,
    NumPadEquals,
    NumPadEnter,
    NumPadDecimal,
    NumPadSeparator,

    // Modifiers
    LeftShift,
    RightShift,
    LeftControl,
    RightControl,
    LeftAlt,
    RightAlt,
    LeftGUI,                // Windows / Apple-Command keys
    RightGUI,
    Function,

    Shift,
    Control,
    Alt,

    // KB State / Lock buttons
    CapsLock,
    NumLock,
    ScrollLock,

    // Editing
    Backspace,
    Tab,
    Enter,
    Pause,
    Escape,
    Space,
    PageUp,
    PageDown,
    End,
    Home,
    Insert,
    Delete,

    // Directional Pad
    Left,
    Right,
    Up,
    Down,

    // Common text keys
    Equals,
    Minus,
    LeftBracket,
    RightBracket,
    ForwardSlash,
    BackSlash,
    Apostrophe,
    Semicolon,
    Comma,
    Period,
    Tilde,

    // Misc Keys
    PrintScreen,
    Application,        // Windows / Menu key 

    // Browser control keys
    BrowserBack,
    BrowserFavorites,
    BrowserForward,
    BrowserHome,
    BrowserRefresh,
    BrowserSearch,
    BrowserStop,

    // Media control keys
    VolumeUp,
    VolumeDown,
    Mute,
    MediaNext,
    MediaPrevious,
    MediaPlay,
    MediaStop,
  };


#if defined(WIN32) && defined(_IWE_GUI)

# include <WinUser.h>

  inline e_IWEKey getKey(uchar OSKey)
  {
    switch (OSKey)
    {
      // Digits / Numbers
    case 0x30: return IWEKeys::D0;
    case 0x31: return IWEKeys::D1;
    case 0x32: return IWEKeys::D2;
    case 0x33: return IWEKeys::D3;
    case 0x34: return IWEKeys::D4;
    case 0x35: return IWEKeys::D5;
    case 0x36: return IWEKeys::D6;
    case 0x37: return IWEKeys::D7;
    case 0x38: return IWEKeys::D8;
    case 0x39: return IWEKeys::D9;

      // Letters
    case 0x41: return IWEKeys::A;
    case 0x42: return IWEKeys::B;
    case 0x43: return IWEKeys::C;
    case 0x44: return IWEKeys::D;
    case 0x45: return IWEKeys::E;
    case 0x46: return IWEKeys::F;
    case 0x47: return IWEKeys::G;
    case 0x48: return IWEKeys::H;
    case 0x49: return IWEKeys::I;
    case 0x4A: return IWEKeys::J;
    case 0x4B: return IWEKeys::K;
    case 0x4C: return IWEKeys::L;
    case 0x4D: return IWEKeys::M;
    case 0x4E: return IWEKeys::N;
    case 0x4F: return IWEKeys::O;
    case 0x50: return IWEKeys::P;
    case 0x51: return IWEKeys::Q;
    case 0x52: return IWEKeys::R;
    case 0x53: return IWEKeys::S;
    case 0x54: return IWEKeys::T;
    case 0x55: return IWEKeys::U;
    case 0x56: return IWEKeys::V;
    case 0x57: return IWEKeys::W;
    case 0x58: return IWEKeys::X;
    case 0x59: return IWEKeys::Y;
    case 0x5A: return IWEKeys::Z;

      // Function Keys
    case VK_F1: return IWEKeys::F1;
    case VK_F2: return IWEKeys::F2;
    case VK_F3: return IWEKeys::F3;
    case VK_F4: return IWEKeys::F4;
    case VK_F5: return IWEKeys::F5;
    case VK_F6: return IWEKeys::F6;
    case VK_F7: return IWEKeys::F7;
    case VK_F8: return IWEKeys::F8;
    case VK_F9: return IWEKeys::F9;
    case VK_F10: return IWEKeys::F10;
    case VK_F11: return IWEKeys::F11;
    case VK_F12: return IWEKeys::F12;
    case VK_F13: return IWEKeys::F13;
    case VK_F14: return IWEKeys::F14;
    case VK_F15: return IWEKeys::F15;
    case VK_F16: return IWEKeys::F16;
    case VK_F17: return IWEKeys::F17;
    case VK_F18: return IWEKeys::F18;
    case VK_F19: return IWEKeys::F19;
    case VK_F20: return IWEKeys::F20;
    case VK_F21: return IWEKeys::F21;
    case VK_F22: return IWEKeys::F22;
    case VK_F23: return IWEKeys::F23;
    case VK_F24: return IWEKeys::F24;

      // Number pad
    case VK_NUMPAD0: return IWEKeys::NumPad0;
    case VK_NUMPAD1: return IWEKeys::NumPad1;
    case VK_NUMPAD2: return IWEKeys::NumPad2;
    case VK_NUMPAD3: return IWEKeys::NumPad3;
    case VK_NUMPAD4: return IWEKeys::NumPad4;
    case VK_NUMPAD5: return IWEKeys::NumPad5;
    case VK_NUMPAD6: return IWEKeys::NumPad6;
    case VK_NUMPAD7: return IWEKeys::NumPad7;
    case VK_NUMPAD8: return IWEKeys::NumPad8;
    case VK_NUMPAD9: return IWEKeys::NumPad9;

    case VK_ADD: return IWEKeys::NumPadAdd;
    case VK_SUBTRACT: return IWEKeys::NumPadMinus;
    case VK_MULTIPLY: return IWEKeys::NumPadMultiply;
    case VK_DIVIDE: return IWEKeys::NumPadDivide;
    case 0x92: return IWEKeys::NumPadEquals;
      //case ?: return IWEKeys::NumPadEnter;
    case VK_DECIMAL: return IWEKeys::NumPadDecimal;
    case VK_SEPARATOR: return IWEKeys::NumPadSeparator;

      // Modifiers
    case VK_LSHIFT: return IWEKeys::LeftShift;
    case VK_RSHIFT: return IWEKeys::RightShift;
    case VK_LCONTROL: return IWEKeys::LeftControl;
    case VK_RCONTROL: return IWEKeys::RightControl;
    case VK_LMENU: return IWEKeys::LeftAlt;
    case VK_RMENU: return IWEKeys::RightAlt;
    case VK_LWIN: return IWEKeys::LeftGUI;
    case VK_RWIN: return IWEKeys::RightGUI;
      
    case VK_SHIFT: return IWEKeys::Shift;
    case VK_CONTROL: return IWEKeys::Control;
    case VK_MENU: return IWEKeys::Alt;
      //case ?: return IWEKeys::Function;

      // KB State / Lock buttons
    case VK_CAPITAL: return IWEKeys::CapsLock;
    case VK_NUMLOCK: return IWEKeys::NumLock;
    case VK_SCROLL: return IWEKeys::ScrollLock;

      // Editing
    case VK_BACK: return IWEKeys::Backspace;
    case VK_TAB: return IWEKeys::Tab;
    case VK_RETURN: return IWEKeys::Enter;
    case VK_PAUSE: return IWEKeys::Pause;
    case VK_ESCAPE: return IWEKeys::Escape;
    case VK_SPACE: return IWEKeys::Space;
    case VK_PRIOR: return IWEKeys::PageUp;
    case VK_NEXT: return IWEKeys::PageDown;
    case VK_END: return IWEKeys::End;
    case VK_HOME: return IWEKeys::Home;
    case VK_INSERT: return IWEKeys::Insert;
    case VK_DELETE: return IWEKeys::Delete;

      // Directional Pad
    case VK_LEFT: return IWEKeys::Left;
    case VK_RIGHT: return IWEKeys::Right;
    case VK_UP: return IWEKeys::Up;
    case VK_DOWN: return IWEKeys::Down;

      // Common text keys
    case VK_OEM_PLUS: return IWEKeys::Equals;
    case VK_OEM_MINUS: return IWEKeys::Minus;
    case VK_OEM_4: return IWEKeys::LeftBracket;
    case VK_OEM_6: return IWEKeys::RightBracket;
    case VK_OEM_2: return IWEKeys::ForwardSlash;
    case VK_OEM_5: return IWEKeys::BackSlash;
    case VK_OEM_7: return IWEKeys::Apostrophe;
    case VK_OEM_1: return IWEKeys::Semicolon;
    case VK_OEM_COMMA: return IWEKeys::Comma;
    case VK_OEM_PERIOD: return IWEKeys::Period;
    case VK_OEM_3: return IWEKeys::Tilde;

      // Misc Keys
    case VK_SNAPSHOT: return IWEKeys::PrintScreen;
    case VK_APPS: return IWEKeys::Application;

      // Browser control keys
    case VK_BROWSER_BACK: return IWEKeys::BrowserBack;
    case VK_BROWSER_FAVORITES: return IWEKeys::BrowserFavorites;
    case VK_BROWSER_FORWARD: return IWEKeys::BrowserForward;
    case VK_BROWSER_HOME: return IWEKeys::BrowserHome;
    case VK_BROWSER_REFRESH: return IWEKeys::BrowserRefresh;
    case VK_BROWSER_SEARCH: return IWEKeys::BrowserSearch;
    case VK_BROWSER_STOP: return IWEKeys::BrowserStop;

      // Media control keys
    case VK_VOLUME_UP: return IWEKeys::VolumeUp;
    case VK_VOLUME_DOWN: return IWEKeys::VolumeDown;
    case VK_VOLUME_MUTE: return IWEKeys::Mute;
    case VK_MEDIA_NEXT_TRACK: return IWEKeys::MediaNext;
    case VK_MEDIA_PREV_TRACK: return IWEKeys::MediaPrevious;
    case VK_MEDIA_PLAY_PAUSE: return IWEKeys::MediaPlay;
    case VK_MEDIA_STOP: return IWEKeys::MediaStop;

    default:
      break;
    }

    return IWEKeys::Unknown;
  }

#elif defined(_MACOSX)

#endif

}



#endif // !IWE_KEYS_H