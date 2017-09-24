#pragma once

#include <vector>
#include <QList>
#include <QFontMetrics>

#include "IDesktopAbstraction.hpp"

#include "SelectableElement.hpp"
#include "SelectableElementPtr.hpp"
#include "MonitorInfo.hpp"

namespace FastWindowSwitcherLib
{
  class SelectableElementRepository;

  class DesktopAbstraction : public IDesktopAbstraction
  {
  public:
    DesktopAbstraction(IUIAutomation& p_ui_automation, const QFontMetrics& p_fontMetrics);

    virtual std::vector<MonitorInfo> GetMonitorInfos() override;
    QList<quintptr> GetBlackListetWindows(const std::vector<FastWindowSwitcherLib::MonitorInfo>& p_monitorInfos) override;
    quintptr  GetCurrentForgroundWindow() override;
    void Win32SwitchToThisWindow(const quintptr& p_window) override;
    std::vector<SelectableElementPtr> GetSelectableElements(const QList<quintptr>& p_windowBlackList, const std::vector<MonitorInfo>& p_monitors) override;
    void Invoke(SelectableElement& p_selectableElement) override;
    void UpdateElements(SelectableElementRepository& p_selectableElementRepository, const std::vector<MonitorInfo>& p_monitors, bool& p_updateNedded) override;

  private:
    IUIAutomation& m_ui_automation;
    QFontMetrics m_fontMetrics;
  };
};