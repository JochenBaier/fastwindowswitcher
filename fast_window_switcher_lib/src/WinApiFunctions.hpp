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
#include <QRect>

namespace FastWindowSwitcherLib
{
  namespace WinApiFunctions
  {
    std::size_t GetWindowMonitorIndex(const quintptr& p_window);
    qintptr Win32GetWindowLongPTR(const quintptr& p_window, const int& p_index);
    quintptr Win32GetOwner(const quintptr& p_hwnd);
    quintptr Win32FindWindow(const quintptr& p_parent, const quintptr& p_hWndChildAfter, const char* const p_lpszClass, const char* const p_lpszWindow);
    quintptr Win32FindWindow(const char* const  lpClassName, const char* const lpWindowName);
    quintptr  Win32ChildWindowFromPointEx(const quintptr& p_window, const QPoint& p_point);
    bool Win32IsWindowVisible(const quintptr& p_window);
    bool Win32IsWindowEnabled(const quintptr& p_window);
    bool Win32IsWindowIconic(const quintptr& p_window);
    bool Win32IsHungAppWindow(const quintptr& p_window);
    bool Win32IsZoomed(const quintptr& p_window);
    QRect Win32GetWindowRectangle(const quintptr& p_window);
    QString Win32GetWindowTitle(const quintptr& p_window);
    quintptr Win32GetTopWindow();
    quintptr Win32GetDesktopWindow();
    quintptr Win32GetSibling(const quintptr& p_window);
    void Win32SwitchToThisWindow(const quintptr& p_window);
    quintptr Win32GetForegroundWindow();
    bool Win32SetForegroundWindow(const quintptr& p_window);
    quintptr Win32FindWindowWithProperty(const quintptr& p_parentWindow, const char* const p_class, const char* const p_property);
    bool Win32RegisterHotKey(const quintptr& p_window, const int& p_id, const quint32& p_modifiers, const quint32& p_vk);
    bool Win32UnregisterHotKey(const quintptr& p_window, const int& p_id);
    quint32 Win32RegisterWindowMessage(QString p_message);
    int Win32GetSystemMetrics(const int& p_index);
    const QString GetSystemX86Folder();
    void Win32GetFileVersion(const QString& p_file, int& p_major, int& p_minor, int& p_build, int& p_revision);
    QString Win32GetFileVersionAsString();
  };
};
