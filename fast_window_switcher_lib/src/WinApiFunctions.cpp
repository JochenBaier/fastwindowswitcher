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


#include "WinApiFunctions.hpp"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <QDebug>
#include <QLibrary>
#include <QSysInfo>

namespace FastWindowSwitcherLib
{
  //Wrapper functions for WinAPI function
  namespace WinApiFunctions
  {
    std::size_t GetWindowMonitorIndex(const quintptr& p_window)
    {
      Q_ASSERT(p_window != NULL);

      HMONITOR monitor = ::MonitorFromWindow((HWND)p_window, MONITOR_DEFAULTTONULL);
      if (!monitor)
      {
        return 0;
      }

      MONITORINFO info = { 0 };
      info.cbSize = sizeof(MONITORINFO);
      if (!::GetMonitorInfo(monitor, &info))
      {
        Q_ASSERT(false);
        return 0;
      }

      if (info.dwFlags == MONITORINFOF_PRIMARY)
      {
        return 0;
      }
      else
      {
        return 1;
      }
    }

    qintptr Win32GetWindowLongPTR(const quintptr& p_window, const int& p_index)
    {
      Q_ASSERT(p_window != NULL);

      ::SetLastError(0);
      LONG_PTR res = ::GetWindowLongPtr((HWND)p_window, p_index);
      if (res == 0)
      {
        if (GetLastError() != ERROR_SUCCESS)
        {
          return -1;
        }
      }
      return res;
    }

    quintptr Win32GetOwner(const quintptr&  p_window)
    {
      Q_ASSERT(p_window != NULL);
      return (quintptr) ::GetWindow((HWND)p_window, GW_OWNER);
    }

    quintptr Win32FindWindow(const quintptr& p_parent, const quintptr& hWndChildAfter, const char* const lpszClass, const char* const lpszWindow)
    {
      Q_ASSERT(p_parent != NULL);
      return (quintptr) ::FindWindowExA((HWND)p_parent, (HWND)hWndChildAfter, lpszClass, lpszWindow);
    }

    quintptr Win32FindWindow(const char* const p_lpClassName, const char* const p_lpWindowName)
    {
      HWND hwnd = ::FindWindowA((LPCSTR)p_lpClassName, (LPCSTR)p_lpWindowName);
      return (quintptr)hwnd;
    }

    quintptr  Win32ChildWindowFromPointEx(const quintptr& p_window, const QPoint& p_point)
    {
      POINT p;
      p.x = p_point.x();
      p.y = p_point.y();

      return (quintptr)ChildWindowFromPointEx((HWND)p_window, p, CWP_SKIPINVISIBLE | CWP_SKIPTRANSPARENT);
    }

    bool Win32IsHungAppWindow(const quintptr& p_window)
    {
      Q_ASSERT(p_window != NULL);
      return ::IsHungAppWindow((HWND)p_window);
    }

    bool Win32IsZoomed(const quintptr& p_window)
    {
      return IsZoomed((HWND)p_window);
    }

    using DwmGetWindowAttributeFuncPtr = HRESULT(STDAPICALLTYPE *)(HWND hwnd, DWORD dwAttribute, _Out_writes_bytes_(cbAttribute) PVOID pvAttribute, DWORD cbAttribute);

    //Resolve DwmGetWindowAttribute from Dwmapi.dll because its not available on Windows 7
    DwmGetWindowAttributeFuncPtr ResolveDwmGetWindowAttribute()
    {
      QLibrary dwmapiLib("Dwmapi.dll");
      dwmapiLib.setLoadHints(QLibrary::PreventUnloadHint);

      auto dwmGetWindowAttribute = (DwmGetWindowAttributeFuncPtr)dwmapiLib.resolve("DwmGetWindowAttribute");
      return dwmGetWindowAttribute;
    }

    static DwmGetWindowAttributeFuncPtr dwmGetWindowAttribute = ResolveDwmGetWindowAttribute();

    #define DWMWA_EXTENDED_FRAME_BOUNDS_ 9

    //Returns the bounding rectangle for a window handle
    //For Windows 10  DwmGetWindowAttribute() is used
    QRect Win32GetWindowRectangle(const quintptr& p_window)
    {
      Q_ASSERT(p_window != NULL);

      RECT windowRect = { 0 };

      //Windows 10 windows has a invisible border/shadow around each windows. GetWindowRect() would return a rect including this boarder/shadow ->it would be too large
      if (dwmGetWindowAttribute != nullptr)
      {
        HRESULT hr = dwmGetWindowAttribute((HWND)p_window, DWMWA_EXTENDED_FRAME_BOUNDS_, &windowRect, sizeof(RECT));
        if (hr != S_OK)
        {
          return QRect();
        }
      }
      else
      {
        if (!GetWindowRect((HWND)p_window, &windowRect))
        {
          Q_ASSERT(false);
          return QRect();
        }
      }

      QRect qrect(windowRect.left, windowRect.top, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top);
      return qrect;
    }

    QString Win32GetWindowTitle(const quintptr& p_window)
    {
      Q_ASSERT(p_window != NULL);

      wchar_t title_buffer[1024] = { 0 };

      ::GetWindowTextW((HWND)p_window, &title_buffer[0], sizeof(title_buffer) / sizeof(wchar_t));

      QString asQString = QString::fromWCharArray(title_buffer);
      return asQString;
    }

    quintptr Win32GetTopWindow()
    {
      return (quintptr) ::GetTopWindow(0);
    }

    quintptr Win32GetDesktopWindow()
    {
      return (quintptr) ::GetDesktopWindow();
    }

    quintptr Win32GetSibling(const quintptr& p_window)
    {
      Q_ASSERT(p_window != 0);
      return (quintptr) ::GetNextWindow((HWND)p_window, GW_HWNDNEXT);
    }

    bool Win32IsWindowVisible(const quintptr& p_window)
    {
      Q_ASSERT(p_window != 0);
      return IsWindowVisible((HWND)p_window);
    }

    bool Win32IsWindowIconic(const quintptr& p_window)
    {
      Q_ASSERT(p_window != NULL);
      return ::IsIconic((HWND)p_window);
    }

    bool Win32IsWindowEnabled(const quintptr& p_window)
    {
      Q_ASSERT(p_window != NULL);
      return ::IsWindowEnabled((HWND)p_window);
    }

    void Win32SwitchToThisWindow(const quintptr& p_window)
    {
      Q_ASSERT(p_window != NULL);
      ::SwitchToThisWindow((HWND)p_window, true);
    }

    quintptr Win32GetForegroundWindow()
    {
      HWND lastForegroundWindow = GetForegroundWindow();
      return (quintptr)lastForegroundWindow;
    }

    bool Win32SetForegroundWindow(const quintptr& p_window)
    {
      Q_ASSERT(p_window);
      return SetForegroundWindow((HWND)p_window);
    }

    quintptr Win32FindWindowWithProperty(const quintptr& p_parentWindow, const char* const p_class, const char* const p_property)
    {
      Q_ASSERT(p_class);
      Q_ASSERT(p_property);

      HWND foundWindow = FindWindowExA((HWND)p_parentWindow, NULL, p_class, NULL);

      while (foundWindow != NULL)
      {
        const HANDLE foundProb = GetPropA(foundWindow, p_property);
        if (foundProb)
        {
          return (quintptr)foundWindow;
        }

        foundWindow = FindWindowExA((HWND)p_parentWindow, foundWindow, p_class, NULL);
      }

      return NULL;
    }

    bool Win32RegisterHotKey(const quintptr& p_window, const int& p_id, const quint32& p_modifiers, const quint32& p_vk)
    {
      Q_ASSERT(p_window);
      return RegisterHotKey((HWND)p_window, p_id, p_modifiers, p_vk);
    }

    bool Win32UnregisterHotKey(const quintptr& p_window, const int& p_id)
    {
      Q_ASSERT(p_window);
      return UnregisterHotKey((HWND)p_window, p_id);
    }

    quint32 Win32RegisterWindowMessage(QString p_message)
    {
      Q_ASSERT(!p_message.isEmpty());
      return RegisterWindowMessageW(p_message.toStdWString().c_str());
    }

    int Win32GetSystemMetrics(const int& p_index)
    {
      return GetSystemMetrics(p_index);
    }
  }
}
