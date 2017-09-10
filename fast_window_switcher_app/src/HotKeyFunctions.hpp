#pragma once

#include <QString>
#include <vector>

#include <Windows.h>

namespace FastWindowSwitcher
{
  namespace HotKeyFunctions
  {
    struct Mapping
    {
      int VkKeyCode;
      QString Name;
    };

    std::vector<Mapping> GetVirtualKeyMappings();
    std::vector<Mapping> GetModfierMappings();

    QString GetHotKeyKeyAsString(const int& p_virtualKeyCode);
    int GetHotKeyKeyIndex(const int& p_virtualKeyCode);

    QString GetHotKeyModfierAsString(const int& p_virtualKeyCode);
    int GetHotKeyModifierIndex(const int& p_modifierKeyCode);

  };
}

