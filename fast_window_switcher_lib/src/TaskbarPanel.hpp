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

#include <qglobal.h>

namespace FastWindowSwitcherLib
{
  class TaskbarPanel
  {
  public:

    enum class PanelEdge
    {
      LEFT,
      TOP,
      RIGHT,
      BOTTOM
    };

    explicit TaskbarPanel(const PanelEdge& p_edge, const quintptr& p_windowHandle, const quintptr& p_listWindowHandle, const quintptr& p_quickLaunchWindowHandle) :
      m_edge(p_edge), m_windowHandle(p_windowHandle), m_listWindowHandle(p_listWindowHandle), m_quickLaunchWindowHandle(p_quickLaunchWindowHandle)
    {
    }

    PanelEdge GetEdge() const
    {
      return m_edge;
    }

    quintptr GetWindowHandle() const
    {
      return m_windowHandle;
    }

    quintptr GetListWindowHandle() const
    {
      return m_listWindowHandle;
    }

    quintptr GetQuickLaunchWindowHandle() const
    {
      return m_quickLaunchWindowHandle;
    }

  private:
    const PanelEdge m_edge;
    const quintptr m_windowHandle;
    const quintptr m_listWindowHandle;
    const quintptr m_quickLaunchWindowHandle;
  };
};