#include <QDebug>

#include "DesktopAbstraction.hpp"

#include "SelectableElementRepository.hpp"
#include "SelectableElementFunctions.hpp"
#include "WindowFunctions.hpp"
#include "MonitorFunctions.hpp"
#include "WinApiFunctions.hpp"

namespace FastWindowSwitcherLib
{
  //Layer between FastWindowSwitcher and Windows API functions. Enables independent testing of the FastWindowSwitcher functions
  DesktopAbstraction::DesktopAbstraction(IUIAutomation& p_ui_automation, const QFontMetrics& p_fontMetrics) :
    m_ui_automation(p_ui_automation),
    m_fontMetrics(p_fontMetrics)
  {
  }

  //Get infos from used monitors
  std::vector<FastWindowSwitcherLib::MonitorInfo> DesktopAbstraction::GetMonitorInfos()
  {
    return MonitorFunctions::GetMonitorInfos();
  }

  //Returns list of window which should not get markers
  QList<quintptr> DesktopAbstraction::GetBlackListetWindows(const std::vector<FastWindowSwitcherLib::MonitorInfo>& p_monitorInfos)
  {
    QList<quintptr> blackListetWindows;

    quintptr desktopWindow = NULL;
    quintptr progmanWindow = NULL;
    quintptr startButtonWindow = NULL;
    quintptr startMenuWindow = NULL;
    quintptr desktopUserPicture = NULL;

    WindowFunctions::GetSpecialDesktopWindows(desktopWindow, progmanWindow, startButtonWindow, startMenuWindow, desktopUserPicture);

    if (desktopWindow) { blackListetWindows.push_back(desktopWindow); }
    if (progmanWindow) { blackListetWindows.push_back(progmanWindow); }
    if (startButtonWindow) { blackListetWindows.push_back(startButtonWindow); }
    if (startMenuWindow) { blackListetWindows.push_back(startMenuWindow); }
    if (desktopUserPicture) { blackListetWindows.push_back(desktopUserPicture); }
    
    for (const FastWindowSwitcherLib::MonitorInfo& monitorInfo : p_monitorInfos)
    {
      if (!monitorInfo.HasPanel())
      {
        continue;
      }

      quintptr panel_window = monitorInfo.GetPanel()->GetWindowHandle();
      blackListetWindows.push_back(panel_window);

    }
    
    return blackListetWindows;
  }

  //Gets the current ForeGround window
  quintptr DesktopAbstraction::GetCurrentForgroundWindow()
  {
    return WinApiFunctions::Win32GetForegroundWindow();
  }

  //Switches to a window
  void DesktopAbstraction::Win32SwitchToThisWindow(const quintptr& p_window)
  {
    Q_ASSERT(p_window);
    WinApiFunctions::Win32SwitchToThisWindow(p_window);
  }

  //Returns list of elements which should get markers
  std::vector<FastWindowSwitcherLib::SelectableElementPtr> DesktopAbstraction::GetSelectableElements(const QList<quintptr>& p_windowBlackList, const std::vector<MonitorInfo>& p_monitors)
  {
    return SelectableElementFunctions::GetSelectableElements(m_fontMetrics, p_windowBlackList, p_monitors, m_ui_automation);
  }

  //Invokes a element (Switch to or Invoke for panel buttons)
  void DesktopAbstraction::Invoke(SelectableElement& p_selectableElement)
  {
    SelectableElementFunctions::Invoke(p_selectableElement);
  }

  //Updates elements which are currently shown (position, still available)
  void DesktopAbstraction::UpdateElements(SelectableElementRepository& p_selectableElementRepository, const std::vector<MonitorInfo>& p_monitors, bool& p_updateNedded)
  {
    for (std::size_t i = 0; i < p_selectableElementRepository.Count(); i++)
    {
      SelectableElement& selectableElement = p_selectableElementRepository.GetElement(i);
      bool windowsNeedsUpdate = false;
      SelectableElementFunctions::Update(selectableElement, m_fontMetrics, p_monitors, windowsNeedsUpdate);
      p_updateNedded |= windowsNeedsUpdate;
    }
  }
}