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

#pragma once

#include <QList>

#include <SelectableElementPtr.hpp>
#include "UIElement.hpp"
#include "TaskbarPanel.hpp"

struct IUIAutomation;

class QFontMetrics;

namespace FastWindowSwitcherLib
{
  class MonitorInfo;
  class WindowsDesktop;
  class SelectableElementRepository;

  namespace PanelFunctions
  {
    void ApppendPanelElements(IUIAutomation& p_ui_automation, IUIAutomationElement& p_parent, const MonitorInfo& p_monitor, const QFontMetrics& p_fontMetrics, const quintptr& p_panelWindow, std::vector<SelectableElementPtr>& p_selectaleElements, int& key_pos);
    void GetPrimaryPanelWindows(quintptr& p_panelWindow, quintptr& p_taskButtonWindow, quintptr& p_quickLaunchWindow);
    void GetSecondaryPanelWindows(quintptr& p_panelWindow, quintptr& p_taskButtonWindow, quintptr& p_quickLaunchWindow);
    void GetDualMonitorTaskbarWindows(quintptr& p_panelWindow, quintptr& p_taskButtonWindow, quintptr& p_quickLaunchWindow);

    TaskbarPanel::PanelEdge GetPrimaryPanelEdge();
    TaskbarPanel::PanelEdge GetSecondaryPanelEdge(const int& p_monitorIndex, const quintptr& p_panelWindow);
  };
};