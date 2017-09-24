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

#include "SelectableElementFunctions.hpp"

#include "SelectableElement.hpp"
#include "SelectableWindowFunctions.hpp"
#include "SelectablePanelElementFunctions.hpp"
#include "SelectablePanelElementFunctions.hpp"
#include "SelectableElementRepository.hpp"

namespace FastWindowSwitcherLib
{
  //Functions related to desktop elements (Windows, panel elements) which could be selected
  namespace SelectableElementFunctions
  {
    void Update(SelectableElement& p_selectableElement, const QFontMetrics& p_fontMectrics, const std::vector<MonitorInfo>& p_monitors, bool& p_updateNedded)
    {
      switch (p_selectableElement.GetType())
      {
        case SelectableElementType::SelectableWindow: SelectableWindowFunctions::UpdateSelectableWindow((SelectableWindow&)p_selectableElement, p_fontMectrics, p_monitors, p_updateNedded); break;
        case SelectableElementType::SelectablePanelElement: SelectablePanelElementFunctions::UpdateSelectablePanelElement((SelectablePanelElement&)p_selectableElement, p_updateNedded); break;
      }
    }

    std::vector<SelectableElementPtr>  GetSelectableElements(const QFontMetrics& p_fontMectrics, const QList<quintptr>& p_windowBlackList, const std::vector<MonitorInfo>& p_monitors, IUIAutomation& p_ui_automation)
    {
      int key_combination_letter_pos = -1;

      std::vector<SelectableElementPtr> selectable_elements;

      SelectableWindowFunctions::GetSelectableWindows(p_fontMectrics, p_windowBlackList, p_monitors, key_combination_letter_pos, selectable_elements);
      SelectablePanelElementFunctions::GetSelectablePanelElements(p_fontMectrics, p_monitors, key_combination_letter_pos, selectable_elements, p_ui_automation);

      return selectable_elements;
    }

    void Invoke(SelectableElement& p_selectableElement)
    {
      switch (p_selectableElement.GetType())
      {
        case SelectableElementType::SelectableWindow: SelectableWindowFunctions::Invoke((SelectableWindow&)p_selectableElement); break;
        case SelectableElementType::SelectablePanelElement: SelectablePanelElementFunctions::Invoke((SelectablePanelElement&)p_selectableElement); break;
      }
    }
  }
}