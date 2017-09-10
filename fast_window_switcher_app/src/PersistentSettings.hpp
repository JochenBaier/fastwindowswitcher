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

namespace FastWindowSwitcher
{
  class PersistenSettings
  {
  public:

    PersistenSettings(const int& p_hotKeyMotifierKeyCode, const int& p_hotKeyKeyCode, const QString& p_markerFontName, const int& p_markerFontSize) :
      m_hotKeyMotifierKeyCode(p_hotKeyMotifierKeyCode), m_hotKeyKeyCode(p_hotKeyKeyCode), m_markerFontName(p_markerFontName), m_markerFontSize(p_markerFontSize)
    {
    }

    int GetHotKeyMotifierKeyCode() const { return m_hotKeyMotifierKeyCode; }
    int GetHotKeyKeyCode() const { return m_hotKeyKeyCode; }
    QString GetMarkerFontName() const { return m_markerFontName; }
    int GetMarkerFontSize() const { return m_markerFontSize; }

  private:
    int m_hotKeyMotifierKeyCode;
    int m_hotKeyKeyCode;
    QString m_markerFontName;
    int m_markerFontSize;
  };
};