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


#include "SelectablePanelElementFunctions.hpp"

#include <QDebug>

#include "UIAutomationFunctions.hpp"
#include "PanelFunctions.hpp"
#include "SelectablePanelElement.hpp"
#include "SelectableElementRepository.hpp"

namespace FastWindowSwitcherLib
{
  //Functions related to panel elements
  namespace SelectablePanelElementFunctions
  {
    //Returns a list of all selectable panel elements
    void GetSelectablePanelElements(const QFontMetrics& p_fontMectrics, const std::vector<MonitorInfo>& p_monitors, int p_keyLetterPos, std::vector<SelectableElementPtr>& p_selectaleElements, IUIAutomation& p_ui_automation)
    {
      for (const auto& monitor : p_monitors)
      {
        if (!monitor.HasPanel())
        {
          continue;
        }

        const TaskbarPanel panel = monitor.GetPanel().value();

        if (panel.GetListWindowHandle())
        {
          UIElementPtr taskbuttonElement = UIAutomationFunctions::GetElementFromHandle(p_ui_automation, panel.GetListWindowHandle());
          if (taskbuttonElement)
          {
            PanelFunctions::ApppendPanelElements(p_ui_automation, *taskbuttonElement, monitor, p_fontMectrics, panel.GetWindowHandle(), p_selectaleElements, p_keyLetterPos);
          }
        }

        if (panel.GetQuickLaunchWindowHandle())
        {
          //FIXME use smaller markers for quick launch area
          UIElementPtr quickLaunchElement = UIAutomationFunctions::GetElementFromHandle(p_ui_automation, panel.GetQuickLaunchWindowHandle());
          if (quickLaunchElement)
          {
            PanelFunctions::ApppendPanelElements(p_ui_automation, *quickLaunchElement, monitor, p_fontMectrics, panel.GetWindowHandle(), p_selectaleElements, p_keyLetterPos);
          }
        }
      }
    }

    //Invoke a panel element
    void Invoke(SelectablePanelElement& p_selectablePanelElement)
    {
      UIAutomationFunctions::Invoke(p_selectablePanelElement.GetUIElement());
    }

    //Updates a panel element
    void UpdateSelectablePanelElement(SelectablePanelElement& /*p_selectablePanelElement*/, bool& /*p_updateNedded*/)
    {
      //FIXME check if taskbar button is still there
    }
  }
}