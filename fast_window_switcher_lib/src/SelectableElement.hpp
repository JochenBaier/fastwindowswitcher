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

#include <QString>
#include <QList>
#include <QRect>

#include <SelectableElementPtr.hpp>
#include "SelectableElementType.hpp"

namespace FastWindowSwitcherLib
{
  class SelectableElement
  {
  public:
    SelectableElement(const QString& p_keyCombination, const std::size_t& p_monitor, QList<QRect>& p_markers) :
      m_keyCombination(p_keyCombination),
      m_monitor(p_monitor),
      m_markers(p_markers),
      m_matchingKeyCount(0),
      m_disabled(false)
    {
    }

    virtual ~SelectableElement() = default;

    virtual SelectableElementType GetType() const = 0;

    QString GetKeyCombination() const
    {
      return m_keyCombination;
    }

    QList<QRect> GetMarkers() const
    {
      return m_markers;
    }

    void SetMarkers(const QList<QRect>& p_markers)
    {
      m_markers = p_markers;
    }

    std::size_t GetMonitor() const
    {
      return m_monitor;
    }

    void SetMonitor(const std::size_t& p_monitor)
    {
      m_monitor = p_monitor;
    }

    void SetMatchingKeyCount(int p_count)
    {
      Q_ASSERT(p_count >= 0);
      m_matchingKeyCount = p_count;
    }

    int GetMatchingKeyCount() const
    {
      return m_matchingKeyCount;
    }

    void SetDisable(const bool& p_disable)
    {
      m_disabled = p_disable;
    }

    bool IsDisabled() const
    {
      return m_disabled;
    }

  private:
    QList<QRect> m_markers;
    QString m_keyCombination;
    std::size_t m_monitor;
    int m_matchingKeyCount;
    int m_disabled;
  };
};