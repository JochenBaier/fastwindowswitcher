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


#include "SelectableWindow.hpp"
#include "WinApiFunctions.hpp"

namespace FastWindowSwitcherLib
{
  //Selectable window
  SelectableWindow::SelectableWindow(QString p_key_combination, const quintptr& p_native_window_handle, const QRect& p_rect, QList<QRect> p_visible_text_positions, const int& p_monitor, const bool&p_isOwnerOfModalWindow) :
    SelectableElement(p_key_combination, p_monitor, p_visible_text_positions),
    m_native_window_handle(p_native_window_handle),
    m_rect(p_rect),
    m_isOwnerOfModalWindow(p_isOwnerOfModalWindow)
  {
  }

  FastWindowSwitcherLib::SelectableElementType SelectableWindow::GetType() const
  {
    return SelectableElementType::SelectableWindow;
  }

  quintptr SelectableWindow::GetNativeWindowHandle() const
  {
    return m_native_window_handle;
  }

  QRect SelectableWindow::GetRect() const
  {
    return m_rect;
  }

  void SelectableWindow::SetRect(const QRect& p_rect)
  {
    m_rect = p_rect;
  }

  bool SelectableWindow::IsOwnerOfModalWindow()
  {
    return m_isOwnerOfModalWindow;
  }
}