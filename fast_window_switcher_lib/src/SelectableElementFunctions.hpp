#pragma once

#include "SelectableElementPtr.hpp"

class SelectableElement;

#include <QList>
#include <UIAutomation.h>

#include "MonitorInfo.hpp"
#include <SelectableElementPtr.hpp>

namespace FastWindowSwitcherLib
{
  class WindowsDesktop;
  class SelectableElementRepository;

  namespace SelectableElementFunctions
  {
    void Update(SelectableElement& p_selectableElement, const QFontMetrics& p_fontMectrics, const QList<quintptr>& p_windowBlackList, const std::vector<MonitorInfo>& p_monitors, bool& p_updateNedded);
    std::vector<SelectableElementPtr> GetSelectableElements(const QFontMetrics& p_fontMectrics, const QList<quintptr>& p_windowBlackList,const std::vector<MonitorInfo>& p_monitors, IUIAutomation& p_ui_automation);
    void Invoke(SelectableElement& p_selectableElement);
  };
};

