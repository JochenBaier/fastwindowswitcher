#include "MonitorFunctions.hpp"

#include <QApplication>
#include <QDesktopWidget>

#include "PanelFunctions.hpp"
#include "optional.hpp"

namespace FastWindowSwitcherLib
{
  //Functions related to monitor infos
  namespace MonitorFunctions
  {
    //Returns infos about the primary monitor
    MonitorInfo GetPrimaryMonitorInfo()
    {
      quintptr panelWindow = 0;
      quintptr taskButtonWindow = 0;
      quintptr quickLaunchWindow = 0;

      PanelFunctions::GetPrimaryPanelWindows(panelWindow, taskButtonWindow, quickLaunchWindow);

      std::experimental::optional<TaskbarPanel> taskbarPanel;

      if (panelWindow != NULL)
      {
        TaskbarPanel::PanelEdge panelEdge = PanelFunctions::GetPrimaryPanelEdge();
        taskbarPanel.emplace(panelEdge, panelWindow, taskButtonWindow, quickLaunchWindow);
      }

      int screenIndex = QApplication::desktop()->primaryScreen();
      Q_ASSERT(screenIndex >= 0);

      return MonitorInfo(QApplication::desktop()->availableGeometry(screenIndex), QApplication::desktop()->screen(screenIndex), screenIndex, taskbarPanel, true);
    }

    //Returns infos about the primary monitor if available
    MonitorInfo GetSecondaryMonitorInfo()
    {
      quintptr panelWindow = 0;
      quintptr taskButtonWindow = 0;
      quintptr quickLaunchWindow = 0;

      PanelFunctions::GetSecondaryPanelWindows(panelWindow, taskButtonWindow, quickLaunchWindow);

      if (panelWindow == NULL)
      {
        taskButtonWindow = NULL;
        quickLaunchWindow = NULL;
        PanelFunctions::GetDualMonitorTaskbarWindows(panelWindow, taskButtonWindow, quickLaunchWindow);
      }

      std::experimental::optional<TaskbarPanel> taskbarPanel;

      if (panelWindow != NULL)
      {
        TaskbarPanel::PanelEdge panelEdge = PanelFunctions::GetSecondaryPanelEdge(1, panelWindow);
        taskbarPanel.emplace(panelEdge, panelWindow, taskButtonWindow, quickLaunchWindow);
      }

      //FIXME is this always 1 for secondary monitor?
      int screenIndex = 1;

      return MonitorInfo(QApplication::desktop()->availableGeometry(screenIndex), QApplication::desktop()->screen(screenIndex), screenIndex, taskbarPanel, false);
    }

    //Returns list of all used monitors
    std::vector<MonitorInfo> GetMonitorInfos()
    {
      std::vector<MonitorInfo> monitors;

      const int monitorCount = QApplication::desktop()->screenCount();

      if (monitorCount == 0)
      {
        Q_ASSERT(false);
        return monitors;
      }

      MonitorInfo primaryMonitor = GetPrimaryMonitorInfo();
      monitors.push_back(primaryMonitor);

      if (monitorCount == 1)
      {
        return monitors;
      }

      MonitorInfo secondaryMonitor = GetSecondaryMonitorInfo();
      monitors.push_back(secondaryMonitor);
      return monitors;
    }
  }
}
