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
#include <QRect>
#include <qglobal.h>

#include "SelectableElementPtr.hpp"
#include "SelectableElement.hpp"
#include "SelectableElementType.hpp"

namespace FastWindowSwitcherLib
{
  class SelectableWindow : public  SelectableElement
  {
  public:
    SelectableWindow(QString  p_key_combination, const quintptr& p_native_window_handle, const QRect& p_rect, QList<QRect> p_visible_text_positions, const int& p_monitor, const bool&p_isOwnerOfModalWindow);
    
    virtual SelectableElementType GetType() const override;
    quintptr GetNativeWindowHandle() const;
    QRect GetRect() const;
    void SetRect(const QRect& p_rect);
    bool IsOwnerOfModalWindow();

  private:
    QRect m_rect;
    const quintptr m_native_window_handle;
    bool m_isOwnerOfModalWindow;
  };
};