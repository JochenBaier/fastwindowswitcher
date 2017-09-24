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

#include "SelectableWindowFunctions.hpp"

#include <QDebug>
#include <QFontMetrics>

#include "SelectableWindow.hpp"
#include "WindowFunctions.hpp"
#include "WinApiFunctions.hpp"
#include "MarkerFunctions.hpp"
#include "SelectableElementRepository.hpp"

namespace FastWindowSwitcherLib
{
  //Functions related to selectable windows 
  namespace SelectableWindowFunctions
  {
    //Update infos for currently visable window elements
    void UpdateSelectableWindow(SelectableWindow& p_selectableWindow, const QFontMetrics& p_fontMectrics, const std::vector<MonitorInfo>& p_monitors, bool& p_updateNeeded)
    {
      p_updateNeeded = false;

      if (p_selectableWindow.IsOwnerOfModalWindow())
      {
        return;
      }

      quintptr windowHandle = p_selectableWindow.GetNativeWindowHandle();
      Q_ASSERT(windowHandle != NULL);

      bool disable = false;
      p_selectableWindow.SetDisable(false);

      do
      {
        const bool basisChecksOks = WindowFunctions::MakeBasicChecks(windowHandle);
        if (!basisChecksOks)
        {
          disable = true;
          p_updateNeeded = true;
          break;
        }

        bool geoChanged = false;

        QRect window_rect = WinApiFunctions::Win32GetWindowRectangle(windowHandle);
        if (p_selectableWindow.GetRect() != window_rect)
        {
          geoChanged = true;
          p_updateNeeded = true;
          p_selectableWindow.SetRect(window_rect);
        }

        if (window_rect.width() < 10 || window_rect.height() < 10)
        {
          disable = true;
          p_updateNeeded = true;
          break;
        }

        std::size_t monitor = WinApiFunctions::GetWindowMonitorIndex(windowHandle);

        if (p_selectableWindow.GetMonitor() != monitor)
        {
          p_selectableWindow.SetMonitor(monitor);
          break;
        }

        if (!geoChanged)
        {
          return;
        }

      } while (false);

      if (disable)
      {
        p_selectableWindow.SetDisable(true);
        p_updateNeeded = true;
        return;
      }

      QList<QRect> visible_text_positions = MarkerFunctions::FindVisibleMarkerPositions(p_fontMectrics, windowHandle, p_monitors.at(p_selectableWindow.GetMonitor()), p_selectableWindow.GetRect(), p_selectableWindow.GetKeyCombination());
      if (visible_text_positions.empty())
      {
        p_selectableWindow.SetDisable(true);
        p_updateNeeded = true;
        return;
      }

      p_selectableWindow.SetMarkers(visible_text_positions);
      p_updateNeeded = true;
    }

    //Returns a list of all selectable windows
    void GetSelectableWindows(const QFontMetrics& p_fontMectrics, const QList<quintptr>& p_windowBlackList, const std::vector<MonitorInfo>& p_monitors, int& p_key_combination_letter_pos, std::vector<SelectableElementPtr>& p_selectaleElements)
    {
      QList<quintptr> topLevelWindows = WindowFunctions::GetSelectableTopLevelWindow(p_windowBlackList);

      for (const quintptr& window : topLevelWindows)
      {
        const QRect window_rect = WinApiFunctions::Win32GetWindowRectangle(window);
        if (window_rect.width() == 0 || window_rect.height() == 0)
        {
          continue;
        }
                        
        const std::size_t monitorThisWindowBelongsTo = WinApiFunctions::GetWindowMonitorIndex(window);
        Q_ASSERT(monitorThisWindowBelongsTo < p_monitors.size());

        const QString key_combination = MarkerFunctions::GetKeyCombination(++p_key_combination_letter_pos);

        QList<QRect> visible_text_positions = MarkerFunctions::FindVisibleMarkerPositions(p_fontMectrics, window, p_monitors.at(monitorThisWindowBelongsTo), window_rect, key_combination);
        if (visible_text_positions.empty())
        {
          --p_key_combination_letter_pos;
          continue;
        }

        SelectableElementPtr selectableElement = std::make_unique<SelectableWindow>(key_combination, window, window_rect, visible_text_positions, monitorThisWindowBelongsTo, false);
        p_selectaleElements.push_back(std::move(selectableElement));

        //Check for owner windows of modal dialogs
        quintptr ownerOfModalWindow = WindowFunctions::GetOwnerIfModal(window);
        if (ownerOfModalWindow != NULL)
        {
          std::size_t ownerMonitorIndex = WinApiFunctions::GetWindowMonitorIndex(ownerOfModalWindow);
          Q_ASSERT(ownerMonitorIndex < p_monitors.size());

          if (ownerMonitorIndex < p_monitors.size())
          {
            const MonitorInfo& ownerMonitor = p_monitors.at(ownerMonitorIndex);
            QRect ownerWindowRect = WinApiFunctions::Win32GetWindowRectangle(ownerOfModalWindow);

            QList<QRect> visible_text_positions_owner = MarkerFunctions::FindVisibleMarkerPositions(p_fontMectrics, ownerOfModalWindow, ownerMonitor, ownerWindowRect, key_combination);

            SelectableElementPtr selectableOwnerElement = std::make_unique<SelectableWindow>(key_combination, window, ownerWindowRect, visible_text_positions_owner, ownerMonitorIndex, true);
            p_selectaleElements.push_back(std::move(selectableOwnerElement));
          }
        }
      }
    }

    //Switch to this window
    void Invoke(const SelectableWindow& p_selectableWindow)
    {
      WinApiFunctions::Win32SwitchToThisWindow(p_selectableWindow.GetNativeWindowHandle());
    }
  }
}
