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


#include "MarkerFunctions.hpp"

#include <cmath>
#include <QFont>
#include <QFontMetrics>
#include <QApplication>
#include <QDesktopWidget>

#include "WinApiFunctions.hpp"
#include "WindowFunctions.hpp"
#include "MonitorInfo.hpp"
#include <Windows.h>

namespace FastWindowSwitcherLib
{
  //Helper functions related to markers
  namespace MarkerFunctions
  {
    const QList<QString> g_keyList({ "f", "j", "d", "k", "l", "g", "h", "r", "u", "e", "i", "o", "v", "w", "n", "b", "z", "p", "q", "c", "m", "x", "y" });
    const double space = 100;

    //Get next key for a marker, first from g_keyList, second "a"+ g_keyList, third "s" + g_keyList
    //if still not enough "#" + number
    QString GetKeyCombination(const int& p_key_combination_letter_pos)
    {
      Q_ASSERT(p_key_combination_letter_pos >= 0);

      if (p_key_combination_letter_pos < g_keyList.size())
      {
        return g_keyList[p_key_combination_letter_pos];
      }

      const int letterPosAModifier = p_key_combination_letter_pos - g_keyList.size();

      if (letterPosAModifier < g_keyList.size())
      {
        return "a" + g_keyList[letterPosAModifier];
      }

      const int letterPosSModifier = p_key_combination_letter_pos - g_keyList.size() * 2;

      if (letterPosSModifier < g_keyList.size())
      {
        return "s" + g_keyList[letterPosSModifier];
      }

      Q_ASSERT(false);

      return "#" + QString::number(p_key_combination_letter_pos);
    }

    //Returns a list of markers positions for an element (window)
    QList<QRect> FindVisibleMarkerPositions(const QFontMetrics& p_fontMectrics, const quintptr& window_handle, const MonitorInfo& p_monitor, const QRect& p_window_rect_raw, const QString& p_text)
    {
      //FIXME move to DesktopAbstraction
      static int windowBorderWidth = WinApiFunctions::Win32GetSystemMetrics(SM_CXSIZEFRAME) + 2; //FIXME +2
      Q_ASSERT(windowBorderWidth > 0);

      double textRectWidth = p_fontMectrics.width(p_text) + 2;
      double textRectHeight = p_fontMectrics.height() + 2;

      if (p_text.size() == 1)
      {
        const double maxSize = qMax(textRectWidth, textRectHeight);
        textRectHeight = textRectWidth = maxSize;
      }

      QRect window_rect = p_window_rect_raw;

      const bool isMaximized = WinApiFunctions::Win32IsZoomed(window_handle);
      if (isMaximized)
      {
        const QRect availableScreenGeometry = p_monitor.GetAvailableGemoetry();

        window_rect.setTop(availableScreenGeometry.top());
        window_rect.setLeft(availableScreenGeometry.left());

        window_rect.setWidth(availableScreenGeometry.width());
      }

      QList<QRect> key_positions;

      const double window_width = window_rect.width();
      Q_ASSERT(window_width > 0);

      const double useable_width = window_width - 2 * ((textRectWidth / 2) + windowBorderWidth - 1);

      if (useable_width <= (int)space)
      {
        QRect text_rect(window_rect.left() + ((window_rect.right() - window_rect.left()) / 2) - ((textRectWidth / 2) - 1), window_rect.top() + windowBorderWidth - 1, textRectWidth, textRectHeight);

        const bool visible = WindowFunctions::IsRectVisibleToUser(window_handle, text_rect);
        if (visible)
        {
          key_positions.push_back(text_rect);
        }

        return key_positions;
      }

      const int spaceCount = (useable_width) / space;
      Q_ASSERT(spaceCount > 0);

      const double realSpace = (useable_width / spaceCount);
      const double left_x = (double)window_rect.left() + (textRectWidth / 2) + windowBorderWidth - 1;
      const int posCount = spaceCount + 1;

      for (int i = 0; i < posCount; i++)
      {
        const double key_rect_center_x = left_x + i*(realSpace - 1);
        int textRectLeft = std::round(key_rect_center_x - ((textRectWidth / 2) - 1));

        if (i == posCount - 1)
        {
          textRectLeft = window_rect.right() - (textRectWidth - 1) - (windowBorderWidth - 1);
        }

        //FIXME +2
        QRect text_rect(textRectLeft, (window_rect.top() + windowBorderWidth), textRectWidth, textRectHeight);

        const bool visible = WindowFunctions::IsRectVisibleToUser(window_handle, text_rect);
        if (visible) //FIXME
        {
          key_positions.push_back(text_rect);
        }
      }

      return key_positions;
    }
  }
}
