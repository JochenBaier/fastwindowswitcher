#pragma once

#include <vector>
#include <memory>
#include <QList>

#include "SelectableElement.hpp"
#include "SelectableElementPtr.hpp"
#include "MonitorInfo.hpp"

class QFontMetrics;
struct IUIAutomation;

namespace FastWindowSwitcherLib
{
  class SelectableElementRepository;

  class IDesktopAbstraction
  {
  public:
    virtual ~IDesktopAbstraction() = default;

    virtual std::vector<MonitorInfo> GetMonitorInfos() = 0;
    virtual QList<quintptr> GetBlackListetWindows() = 0;
    virtual quintptr  GetCurrentForgroundWindow() = 0;
    virtual void Win32SwitchToThisWindow(const quintptr& p_window) = 0;
    virtual std::vector<SelectableElementPtr>  GetSelectableElements(const QList<quintptr>& p_windowBlackList, const std::vector<MonitorInfo>& p_monitors) = 0;
    virtual void UpdateElements(SelectableElementRepository& p_selectableElementRepository, const QList<quintptr>& p_windowBlackList, const std::vector<MonitorInfo>& p_monitors, bool& p_updateNedded) = 0;
    virtual void Invoke(SelectableElement& p_selectableElement) = 0;
  };

  using IDesktopAbstractionPtr = std::unique_ptr<IDesktopAbstraction>;
};