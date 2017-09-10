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

#include <QDebug>

#include <Windows.h>

#include "WindowFunctions.hpp"
#include "MarkerFunctions.hpp"
#include "SelectableWindow.hpp"
#include "WinApiFunctions.hpp"

namespace FastWindowSwitcherLib
{
  //Functions related to windows
  namespace WindowFunctions
  {
    quintptr GetOwnerIfModal(const quintptr& p_window);

    //Checks if the window is selectable
    bool MakeBasicChecks(const quintptr& p_window)
    {
      if (!WinApiFunctions::Win32IsWindowVisible(p_window))
      {
        return false;
      }

      if (!WinApiFunctions::Win32IsWindowEnabled(p_window))
      {
        return false;
      }

      if (WinApiFunctions::Win32IsWindowIconic(p_window))
      {
        return false;
      }

      if (WinApiFunctions::Win32IsHungAppWindow(p_window))
      {
        return false;
      }

      return true;
    }

    bool IsWindowSelectable(const quintptr& p_window)
    {
      const bool basisChecksOks = MakeBasicChecks(p_window);
      if (!basisChecksOks)
      {
        return false;
      }

      const QRect window_rect = WinApiFunctions::Win32GetWindowRectangle(p_window);
      if (window_rect.width() < 10 || window_rect.height() < 10)
      {
        return false;
      }

      return true;
    }

    //Returns a list of selectable windows
    QList<quintptr> GetSelectableTopLevelWindow(const QList<quintptr>& p_windowBlackList)
    {
      QList<quintptr> topLevelWindows;

      quintptr window = WinApiFunctions::Win32GetTopWindow();
      if (window == NULL)
      {
        Q_ASSERT(false);
        return topLevelWindows;
      }

      while (window != NULL)
      {
        if (WindowFunctions::IsWindowSelectable(window) && !p_windowBlackList.contains(window))
        {
          topLevelWindows.append(window);
        }

        window = WinApiFunctions::Win32GetSibling(window);
      }

      return topLevelWindows;
    }

    //Checks a point of a window if visible (not obscured by other window)
    bool IsPointVisibleToUser(const quintptr& p_window, const QPoint& p_point)
    {
      //no CWP_SKIPDISABLED flag because the parent of a module window has the WS_DISABLED flag set
      const auto win_from_point = WinApiFunctions::Win32ChildWindowFromPointEx(WinApiFunctions::Win32GetDesktopWindow(), p_point);
      return win_from_point == p_window;
    }

    //Checks to corners to a rect if visible (not obscured by other window)
    bool IsRectVisibleToUser(const quintptr& p_window, const QRect& p_rect)
    {
      const bool top_left_visible = IsPointVisibleToUser(p_window, p_rect.topLeft());
      if (!top_left_visible)
      {
        return false;
      }

      const bool bottom_right_visible = IsPointVisibleToUser(p_window, p_rect.bottomRight());
      return bottom_right_visible;
    }

    //Find owner of a modal window. From: https://blogs.msdn.microsoft.com/oldnewthing/20111212-00/?p=8923
    quintptr GetOwnerIfModal(const quintptr& p_window)
    {
      const qintptr style = WinApiFunctions::Win32GetWindowLongPTR(p_window, GWL_STYLE);
      if (style == -1)
      {
        Q_ASSERT(false);
        return NULL;
      }

      // child windows cannot have owners
      if ((style & WS_CHILD) == WS_CHILD)
      {
        return NULL;
      }

      quintptr hwndOwner = WinApiFunctions::Win32GetOwner(p_window);
      if (hwndOwner == NULL)
      {
        return NULL; // not an owned window
      }

      //Modal owners are disabled
      if (WinApiFunctions::Win32IsWindowEnabled(hwndOwner))
      {
        return NULL;
      }

      return hwndOwner; // an owned window whose owner is disabled
    }

    //Returns a list of special windows
    void GetSpecialDesktopWindows(quintptr& p_desktopWindow, quintptr& p_progmanWindow, quintptr& p_startButtonWindow, quintptr& p_startMenuWindow, quintptr& p_desktopUserPicture)
    {
      p_desktopWindow = WinApiFunctions::Win32GetDesktopWindow();
      Q_ASSERT(p_desktopWindow != NULL);

      p_progmanWindow = WinApiFunctions::Win32FindWindow("Progman", 0);
      p_startMenuWindow = WinApiFunctions::Win32FindWindowWithProperty(0, "DV2ControlHost", "StartMenuTag");
      p_startButtonWindow = WinApiFunctions::Win32FindWindowWithProperty(0, "Button", "StartButtonTag");

      p_desktopUserPicture = WinApiFunctions::Win32FindWindow("Desktop User Picture", 0);
    }
  }
}
