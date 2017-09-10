#include "HotKeyFunctions.hpp"

namespace FastWindowSwitcher
{
  namespace HotKeyFunctions
  {
    static  std::vector<Mapping> g_hotKeyKeys = {

      { VK_BACK, "BACK" },

      { VK_TAB             , "TAB" },

      { VK_CLEAR           , "CLEAR" },
      { VK_RETURN          , "RETURN" },

      { VK_SHIFT           , "SHIFT" },
      { VK_CONTROL         , "CONTROL" },
      { VK_MENU            , "MENU" },
      { VK_PAUSE           , "PAUSE" },
      { VK_CAPITAL         , "CAPITAL" },

      { VK_KANA            , "KANA" },
      { VK_HANGEUL         , "HANGEUL" },
      { VK_HANGUL          , "HANGUL" },

      { VK_JUNJA           , "JUNJA" },
      { VK_FINAL           , "FINAL" },
      { VK_HANJA           , "HANJA" },
      { VK_KANJI           , "KANJI" },

      { VK_ESCAPE          , "ESCAPE" },

      { VK_CONVERT         , "CONVERT" },
      { VK_NONCONVERT      , "NONCONVERT" },
      { VK_ACCEPT          , "ACCEPT" },
      { VK_MODECHANGE      , "MODECHANGE" },

      { VK_SPACE           , "SPACE" },
      { VK_PRIOR           , "PRIOR" },
      { VK_NEXT            , "NEXT" },
      { VK_END             , "END" },
      { VK_HOME            , "HOME" },
      { VK_LEFT            , "LEFT" },
      { VK_UP              , "UP" },
      { VK_RIGHT           , "RIGHT" },
      { VK_DOWN            , "DOWN" },
      { VK_SELECT          , "SELECT" },
      { VK_PRINT           , "PRINT" },
      { VK_EXECUTE         , "EXECUTE" },
      { VK_SNAPSHOT        , "SNAPSHOT" },
      { VK_INSERT          , "INSERT" },
      { VK_DELETE          , "DELETE" },
      { VK_HELP            , "HELP" },

      { '0'                , "0" },
      { '1'                , "1" },
      { '2'                , "2" },
      { '3'                , "3" },
      { '4'                , "4" },
      { '5'                , "5" },
      { '6'                , "6" },
      { '7'                , "7" },
      { '8'                , "8" },
      { '9'                , "9" },

      { 'A'                , "A" },
      { 'B'                 , "B" },
      { 'C'                 , "C" },
      { 'D'                 , "D" },
      { 'E'                 , "E" },
      { 'F'                 , "F" },
      { 'G'                 , "G" },
      { 'H'                 , "H" },
      { 'I'                 , "I" },
      { 'J'                 , "J" },
      { 'K'                 , "K" },
      { 'L'                 , "L" },
      { 'M'                 , "M" },
      { 'N'                 , "N" },
      { 'O'                 , "O" },
      { 'P'                 , "P" },
      { 'Q'                 , "Q" },
      { 'R'                 , "R" },
      { 'S'                 , "S" },
      { 'T'                 , "T" },
      { 'U'                 , "U" },
      { 'V'                 , "V" },
      { 'W'                 , "W" },
      { 'X'                 , "X" },
      { 'Y'                 , "Y" },
      { 'Z'                 , "Z" },


      { VK_LWIN            , "LWIN" },
      { VK_RWIN            , "RWIN" },
      { VK_APPS            , "APPS" },

      { VK_SLEEP           , "SLEEP" },

      { VK_NUMPAD0         , "NUMPAD0" },
      { VK_NUMPAD1         , "NUMPAD1" },
      { VK_NUMPAD2         , "NUMPAD2" },
      { VK_NUMPAD3         , "NUMPAD3" },
      { VK_NUMPAD4         , "NUMPAD4" },
      { VK_NUMPAD5         , "NUMPAD5" },
      { VK_NUMPAD6         , "NUMPAD6" },
      { VK_NUMPAD7         , "NUMPAD7" },
      { VK_NUMPAD8         , "NUMPAD8" },
      { VK_NUMPAD9         , "NUMPAD9" },
      { VK_MULTIPLY        , "MULTIPLY" },
      { VK_ADD             , "ADD" },
      { VK_SEPARATOR       , "SEPARATOR" },
      { VK_SUBTRACT        , "SUBTRACT" },
      { VK_DECIMAL         , "DECIMAL" },
      { VK_DIVIDE          , "DIVIDE" },
      { VK_F1              , "F1" },
      { VK_F2              , "F2" },
      { VK_F3              , "F3" },
      { VK_F4              , "F4" },
      { VK_F5              , "F5" },
      { VK_F6              , "F6" },
      { VK_F7              , "F7" },
      { VK_F8              , "F8" },
      { VK_F9              , "F9" },
      { VK_F10             , "F10" },
      { VK_F11             , "F11" },
      { VK_F12             , "F12" },
      { VK_F13             , "F13" },
      { VK_F14             , "F14" },
      { VK_F15             , "F15" },
      { VK_F16             , "F16" },
      { VK_F17             , "F17" },
      { VK_F18             , "F18" },
      { VK_F19             , "F19" },
      { VK_F20             , "F20" },
      { VK_F21             , "F21" },
      { VK_F22             , "F22" },
      { VK_F23             , "F23" },
      { VK_F24             , "F24" },

      //FIXME _WIN32_WINNT >= 0x0604
      /*
      { VK_NAVIGATION_VIEW      , "NAVIGATION_VIEW" },
      { VK_NAVIGATION_MENU      , "NAVIGATION_MENU" },
      { VK_NAVIGATION_UP        , "NAVIGATION_UP" },
      { VK_NAVIGATION_DOWN      , "NAVIGATION_DOWN" },
      { VK_NAVIGATION_LEFT      , "NAVIGATION_LEFT" },
      { VK_NAVIGATION_RIGHT     , "NAVIGATION_RIGHT" },
      { VK_NAVIGATION_ACCEPT    , "NAVIGATION_ACCEPT" },
      { VK_NAVIGATION_CANCEL    , "NAVIGATION_CANCEL" },
      */

      { VK_NUMLOCK         , "NUMLOCK" },
      { VK_SCROLL          , "SCROLL" },


      { VK_OEM_NEC_EQUAL, "'=' key on numpad" },

      { VK_OEM_FJ_JISHO, "Dictionary key" },
      { VK_OEM_FJ_MASSHOU, "Unregister word key" },
      { VK_OEM_FJ_TOUROKU, "Register word" },
      { VK_OEM_FJ_LOYA,    "Left OYAYUBI key" },
      { VK_OEM_FJ_ROYA,    "Right OYAYUB" },

      { VK_BROWSER_BACK         , "BROWSER_BACK" },
      { VK_BROWSER_FORWARD      , "BROWSER_FORWARD" },
      { VK_BROWSER_REFRESH      , "BROWSER_REFRESH" },
      { VK_BROWSER_STOP         , "BROWSER_STOP" },
      { VK_BROWSER_SEARCH       , "BROWSER_SEARCH" },
      { VK_BROWSER_FAVORITES    , "BROWSER_FAVORITES" },
      { VK_BROWSER_HOME         , "BROWSER_HOME" },

      { VK_VOLUME_MUTE          , "VOLUME_MUTE" },
      { VK_VOLUME_DOWN          , "VOLUME_DOWN" },
      { VK_VOLUME_UP            , "VOLUME_UP" },
      { VK_MEDIA_NEXT_TRACK     , "MEDIA_NEXT_TRACK" },
      { VK_MEDIA_PREV_TRACK     , "MEDIA_PREV_TRACK" },
      { VK_MEDIA_STOP           , "MEDIA_STOP" },
      { VK_MEDIA_PLAY_PAUSE     , "MEDIA_PLAY_PAUSE" },
      { VK_LAUNCH_MAIL          , "LAUNCH_MAIL" },
      { VK_LAUNCH_MEDIA_SELECT  , "LAUNCH_MEDIA_SELECT" },
      { VK_LAUNCH_APP1          , "LAUNCH_APP1" },
      { VK_LAUNCH_APP2          , "LAUNCH_APP2" },

      { VK_OEM_1          ,"OEM_1 ';:' for US" },
      { VK_OEM_PLUS       ,"OEM_PLUS '+'" },
      { VK_OEM_COMMA      ,"OEM_COMMA ','" },
      { VK_OEM_MINUS      ,"OEM_MINUS '-'" },
      { VK_OEM_PERIOD     ,"OEM_PERIOD '.'" },
      { VK_OEM_2          ,"OEM_2 '/?' for US" },
      { VK_OEM_3          ,"'`~' for US" },
      { VK_OEM_3          ,"'`~' for US " },


      { VK_OEM_4          ,"OEM_4 '[{' for US" },
      { VK_OEM_5          ,"OEM_5 '\\|' for US" },
      { VK_OEM_6          ,"OEM_6']}' for US" },
      { VK_OEM_7          ,"OEM_7 ''\"' for US" },
      { VK_OEM_8          ,"OEM_8" },


      { VK_OEM_AX         ,"'AX' key on Japanese AX kbd" },
      { VK_OEM_102        ,"'<>' or '\\|' on RT 102-key kbd." },
      { VK_ICO_HELP       ,"Help key on ICO" },
      { VK_ICO_00         ,"00 key on ICO" },

      { VK_PROCESSKEY      , "PROCESSKEY" },
      { VK_ICO_CLEAR       , "ICO_CLEAR" },
      { VK_PACKET          , "PACKET" }

    };

    std::vector<Mapping> g_hotKeyModifiers =
    {
      {0, "None"},
      { MOD_ALT, "Alt" },
      { MOD_CONTROL, "Control" },
      { MOD_WIN, "Win" }
    };

    std::vector<Mapping> GetVirtualKeyMappings()
    {
      return g_hotKeyKeys;
    }

    std::vector<Mapping> GetModfierMappings()
    {
      return g_hotKeyModifiers;
    }

    QString GetHotKeyKeyAsString(const int& p_virtualKeyCode)
    {
      for (const Mapping& mapping : g_hotKeyKeys)
      {
        if (p_virtualKeyCode == mapping.VkKeyCode)
        {
          return mapping.Name;
        }
      }

      Q_ASSERT(false);
      return "Unknown hotkey: " + QString::number(p_virtualKeyCode);
    }

    QString GetHotKeyModfierAsString(const int& p_virtualKeyCode)
    {
      for (const Mapping& mapping : g_hotKeyModifiers)
      {
        if (p_virtualKeyCode == mapping.VkKeyCode)
        {
          return mapping.Name;
        }

      }

      Q_ASSERT(false);
      return "Unknown modifier: " + QString::number(p_virtualKeyCode);
    }

    int GetHotKeyKeyIndex(const int& p_virtualKeyCode)
    {
      for (int i = 0; i < g_hotKeyKeys.size(); i++)
      {
        if (p_virtualKeyCode == g_hotKeyKeys[i].VkKeyCode)
        {
          return i;
        }
      }

      return -1;
    }

    int GetHotKeyModifierIndex(const int& p_modifierKeyCode)
    {
      for (int i = 0; i < g_hotKeyModifiers.size(); i++)
      {
        if (p_modifierKeyCode == g_hotKeyModifiers[i].VkKeyCode)
        {
          return i;
        }
      }

      return -1;
    }

  };
}
