/***************************************************************************
**
** Copyright (C) 2017 Jochen Baier
** Contact: email@jochen-baier.
**
** This file is part of the FastWindowSwitcher

This program is free software : you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.If not, see < http://www.gnu.org/licenses/>.

Dieses Programm ist Freie Software : Sie können es unter den Bedingungen
der GNU General Public License, wie von der Free Software Foundation,
Version 3 der Lizenz oder(nach Ihrer Wahl) jeder neueren
veröffentlichten Version, weiterverbreiten und / oder modifizieren.

Dieses Programm wird in der Hoffnung, dass es nützlich sein wird, aber
OHNE JEDE GEWÄHRLEISTUNG, bereitgestellt; sogar ohne die implizite
Gewährleistung der MARKTFÄHIGKEIT oder EIGNUNG FÜR EINEN BESTIMMTEN ZWECK.
Siehe die GNU General Public License für weitere Details.

Sie sollten eine Kopie der GNU General Public License zusammen mit diesem
Programm erhalten haben.Wenn nicht, siehe < http://www.gnu.org/licenses/>.
******************************************************************************/


#include "PanelFunctions.hpp"

#include "QDebug"
#include <QRect>
#include <QApplication>
#include <QDesktopWidget>
#include <QList>
#include <QFontMetrics>

#include <uiautomation.h>

#include "UIAutomationFunctions.hpp"
#include "MarkerFunctions.hpp"
#include "SelectablePanelElement.hpp"
#include "TaskbarPanel.hpp"
#include "MonitorInfo.hpp"
#include "SelectableElementRepository.hpp"
#include "WinApiFunctions.hpp"
#include "WindowFunctions.hpp"

namespace FastWindowSwitcherLib
{
  //Functions releated to the desktop panel (Quick Launch, Taskbar List)
  namespace PanelFunctions
  {
    const int taskBarPadding = 2;

    //Returns the marker rect for a panel element
    QRect TaskButtonRecToMarkerRect(const TaskbarPanel::PanelEdge& p_taskBarEdge, const QRect& p_availableScreenRect, const QPoint& p_taskButtonCenter, const int& p_annotionWidth, const int&  p_annotionHeigth)
    {
      Q_ASSERT(p_annotionWidth > 5);
      Q_ASSERT(p_annotionHeigth > 5);

      switch (p_taskBarEdge)
      {
        case TaskbarPanel::PanelEdge::BOTTOM:
        {
          QRect annotionRect(p_taskButtonCenter.x() - p_annotionWidth / 2 + 1, p_availableScreenRect.bottom() - taskBarPadding - p_annotionHeigth, p_annotionWidth, p_annotionHeigth);
          return annotionRect;
        }

        case TaskbarPanel::PanelEdge::TOP:
        {
          QRect annotionRect(p_taskButtonCenter.x() - p_annotionWidth / 2 + 1, p_availableScreenRect.top() + taskBarPadding, p_annotionWidth, p_annotionHeigth);
          return annotionRect;
        }

        case TaskbarPanel::PanelEdge::LEFT:
        {
          QRect annotionRect(p_availableScreenRect.left() + taskBarPadding, p_taskButtonCenter.y() - p_annotionHeigth / 2 + 1, p_annotionWidth, p_annotionHeigth);
          return annotionRect;
        }

        case TaskbarPanel::PanelEdge::RIGHT:
        {
          QRect annotionRect(p_availableScreenRect.right() - taskBarPadding - p_annotionWidth, p_taskButtonCenter.y() - p_annotionHeigth / 2 + 1, p_annotionWidth, p_annotionHeigth);
          return annotionRect;
        }
      }

      Q_ASSERT(false);
      return QRect();
    }

    //returns a list of all selectable elements of a panel
    void ApppendPanelElements(IUIAutomation& p_ui_automation, IUIAutomationElement&  pParent, const MonitorInfo& p_monitor, const QFontMetrics& p_fontMetrics, const quintptr& p_panelWindow, std::vector<SelectableElementPtr>& p_selectaleElements, int& key_pos)
    {
      Q_ASSERT(p_monitor.HasPanel());
      std::vector<UIElementPtr> childElements = UIAutomationFunctions::GetChildElements(p_ui_automation, pParent);
      if (childElements.empty())
      {
        return;
      }

      const TaskbarPanel::PanelEdge taskBarEdge = p_monitor.GetPanel()->GetEdge();
      const QRect availableScreenRect = p_monitor.GetAvailableGemoetry();

      for (UIElementPtr& pNode : childElements)
      {
        const QRect nodeRect = UIAutomationFunctions::GetElementRectangle(*pNode);
        if (nodeRect.isEmpty())
        {
          Q_ASSERT(false);
          continue;
        }

        const bool taskButtonIsVisible = WindowFunctions::IsRectVisibleToUser(p_panelWindow, nodeRect);
        if (!taskButtonIsVisible)
        {
          continue;
        }

        QString key_combination = MarkerFunctions::GetKeyCombination(++key_pos);

        int markerWidth = p_fontMetrics.width(key_combination) + 2;
        int markerHeight = p_fontMetrics.height() + 2;

        if (key_combination.size() == 1)
        {
          const double maxSize = qMax(markerWidth, markerHeight);
          markerWidth = markerHeight = maxSize;
        }

        QRect annotionRect = TaskButtonRecToMarkerRect(taskBarEdge, availableScreenRect, nodeRect.center(), markerWidth, markerHeight);

        QList<QRect> visible_text_positions;
        visible_text_positions.push_back(annotionRect);

        SelectableElementPtr taskbutton = std::make_unique<SelectablePanelElement>(std::move(pNode), key_combination, visible_text_positions, p_monitor.GetIndex());
        p_selectaleElements.push_back(std::move(taskbutton));
      }
    }

    //Returns the windows handles of the primary panel
    void GetPrimaryPanelWindows(quintptr& p_panelWindow, quintptr& p_taskButtonWindow, quintptr& p_quickLaunchWindow)
    {
      p_panelWindow = 0;
      p_taskButtonWindow = 0;
      p_quickLaunchWindow = 0;

      p_panelWindow = WinApiFunctions::Win32FindWindow("Shell_TrayWnd", NULL);
      if (!p_panelWindow)
      {
        return;
      }

      quintptr reBarWindow32 = WinApiFunctions::Win32FindWindow(p_panelWindow, NULL, "ReBarWindow32", NULL);
      Q_ASSERT(reBarWindow32 != NULL);

      quintptr msTaskSwClass = WinApiFunctions::Win32FindWindow(reBarWindow32, NULL, "MSTaskSwWClass", NULL);
      Q_ASSERT(msTaskSwClass != NULL);

      p_taskButtonWindow = WinApiFunctions::Win32FindWindow(msTaskSwClass, NULL, "MSTaskListWClass", NULL);
      Q_ASSERT(p_taskButtonWindow != NULL);

      p_quickLaunchWindow = WinApiFunctions::Win32FindWindow(reBarWindow32, NULL, "ToolbarWindow32", "Quick Launch");
    }

    //Returns the windows handles of the secondary panel
    void GetSecondaryPanelWindows(quintptr& p_panelWindow, quintptr& p_taskButtonWindow, quintptr& p_quickLaunchWindow)
    {
      p_panelWindow = NULL;
      p_taskButtonWindow = NULL;
      p_quickLaunchWindow = NULL;

      p_panelWindow = WinApiFunctions::Win32FindWindow("Shell_SecondaryTrayWnd", NULL);
      if (!p_panelWindow)
      {
        return;
      }

      quintptr workerW = WinApiFunctions::Win32FindWindow(p_panelWindow, NULL, "WorkerW", NULL);
      Q_ASSERT(workerW != NULL);

      p_taskButtonWindow = WinApiFunctions::Win32FindWindow(workerW, NULL, "MSTaskListWClass", NULL);
      Q_ASSERT(p_taskButtonWindow != NULL);

      //FIXME quicklaunch?
    }

    //Returns the windows handles the program 'Dual Monitor Taskbar'
    void GetDualMonitorTaskbarWindows(quintptr& p_panelWindow, quintptr& p_taskButtonWindow, quintptr& p_quickLaunchWindow)
    {
      p_panelWindow = NULL;
      p_taskButtonWindow = NULL;
      p_quickLaunchWindow = NULL;

      const char* panelClass = "WindowsForms10.Window.8.app.0.2bf8098_r12_ad1";

      p_panelWindow = WinApiFunctions::Win32FindWindow(panelClass, "SecondaryTaskbar");
      if (!p_panelWindow)
      {
        return;
      }

      quintptr firstFormsWindow = WinApiFunctions::Win32FindWindow(p_panelWindow, NULL, panelClass, NULL);
      Q_ASSERT(firstFormsWindow != NULL);

      quintptr secondFormsWindow = WinApiFunctions::Win32FindWindow(p_panelWindow, firstFormsWindow, panelClass, NULL);
      Q_ASSERT(secondFormsWindow != NULL);

      p_taskButtonWindow = WinApiFunctions::Win32FindWindow(secondFormsWindow, 0, panelClass, NULL);
      Q_ASSERT(p_taskButtonWindow != NULL);
    }

    //returns the position of the primary panel
    TaskbarPanel::PanelEdge GetPrimaryPanelEdge()
    {
      APPBARDATA appBarData = { 0 };
      appBarData.cbSize = sizeof(APPBARDATA);

      //FIXME move to WinApFunctions
      const bool ok = (bool)SHAppBarMessage(ABM_GETTASKBARPOS, &appBarData);
      if (!ok)
      {
        Q_ASSERT(false);
        return TaskbarPanel::PanelEdge::BOTTOM;
      }

      const auto edge = static_cast<TaskbarPanel::PanelEdge>(appBarData.uEdge);
      return edge;
    }

    //returns the position of the secondary panel
    TaskbarPanel::PanelEdge GetSecondaryPanelEdge(const int& p_monitorIndex, const quintptr& p_panelWindow)
    {
      Q_ASSERT(p_monitorIndex > 0);
      Q_ASSERT(p_panelWindow);

      const QRect monitorRect = QApplication::desktop()->screen(p_monitorIndex)->geometry();
      Q_ASSERT(!monitorRect.isNull());

      const QRect panelRect = WinApiFunctions::Win32GetWindowRectangle(p_panelWindow);
      Q_ASSERT(!panelRect.isNull());

      Q_ASSERT(!monitorRect.isNull());
      Q_ASSERT(!panelRect.isNull());

      if (panelRect.width() > panelRect.height())
      {
        //horizontal
        if (panelRect.top() == 0)
        {
          return TaskbarPanel::PanelEdge::TOP;
        }
        else
        {
          return TaskbarPanel::PanelEdge::BOTTOM;
        }
      }
      else
      {
        //vertical
        if (panelRect.left() == monitorRect.left())
        {
          return TaskbarPanel::PanelEdge::LEFT;
        }
        else
        {
          return TaskbarPanel::PanelEdge::RIGHT;
        }
      }
    }
  }
}
