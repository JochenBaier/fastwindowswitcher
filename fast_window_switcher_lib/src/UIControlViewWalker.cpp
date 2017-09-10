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

#include "UIControlViewWalker.hpp"
#include <uiautomation.h>

namespace FastWindowSwitcherLib
{
  //Helper the Microsoft UI ControlViewWalker: need to get child UI elements
  UIControlViewWalker::UIControlViewWalker(IUIAutomation& p_automation)
  {
    p_automation.get_ControlViewWalker(&m_iui_controller_walker);
    Q_ASSERT(m_iui_controller_walker != nullptr);
  }

  UIElementPtr UIControlViewWalker::GetFirstChildElement(IUIAutomationElement& p_parent_element) const
  {
    IUIAutomationElement* child_element = nullptr;
    const HRESULT res = m_iui_controller_walker->GetFirstChildElement(&p_parent_element, &child_element);
    UIElementPtr child_element_ptr = MakeUniqueUIElement(child_element);
    return child_element_ptr;
  }

  UIElementPtr UIControlViewWalker::GetNextSiblingElement(IUIAutomationElement& p_element) const
  {
    IUIAutomationElement* next_sibling_element = nullptr;
    m_iui_controller_walker->GetNextSiblingElement(&p_element, &next_sibling_element);
    UIElementPtr next_sibling_element_ptr = MakeUniqueUIElement(next_sibling_element);
    return next_sibling_element_ptr;
  }

  UIControlViewWalker::~UIControlViewWalker()
  {
    m_iui_controller_walker->Release();
  }
}