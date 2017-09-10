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

#include <memory>
#include "optional.hpp"

#include <QRect>

#include "TaskbarPanel.hpp"

class QWidget;

namespace FastWindowSwitcherLib
{
  class MonitorInfo
  {
  public:

    MonitorInfo(const QRect&  p_availableGeometry, const QWidget* p_screenWidget, const int& p_index, const std::experimental::optional<TaskbarPanel>& p_panel, const bool& p_isPrimary) :
      m_availableGeometry(p_availableGeometry), m_screenWidget(p_screenWidget), m_index(p_index), m_panel(p_panel), m_isPrimary(p_isPrimary)
    {
    }

    const QRect GetAvailableGemoetry() const
    {
      return m_availableGeometry;
    }

    const QWidget* GetScreenWidget() const
    {
      return m_screenWidget;
    }

    const int GetIndex() const
    {
      return m_index;
    }

    bool HasPanel() const
    {
      return m_panel.has_value();
    }

    const std::experimental::optional<TaskbarPanel> GetPanel() const
    {
      return m_panel;
    }

    const bool IsPrimary() const
    {
      return m_isPrimary;
    }

  private:
    const QRect m_availableGeometry;
    const QWidget* m_screenWidget;
    const int m_index;
    const bool m_isPrimary;
    const std::experimental::optional<TaskbarPanel> m_panel;
  };
}
