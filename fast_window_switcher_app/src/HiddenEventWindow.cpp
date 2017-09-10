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

#include <Windows.h>
#include <QtWidgets>

#include "HiddenEventWindow.hpp"
#include "WindowFunctions.hpp"
#include "MarkerFunctions.hpp"
#include "PanelFunctions.hpp"
#include "WinApiFunctions.hpp"

namespace FastWindowSwitcher
{
  //Hidden windows which receives the native events (key press, WM_HOTKEY, TaskbarCreated)
  HiddenEventWindow::HiddenEventWindow()
    :QWidget(nullptr)
  {
    setAutoFillBackground(false);
    setGeometry(0, 0, 0, 0);

    //For Debug: set Window background
    /*
    setAttribute(Qt::WA_NoSystemBackground, true);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_ShowWithoutActivating);
    */

    setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

    m_taskbarRestartMessage = FastWindowSwitcherLib::WinApiFunctions::Win32RegisterWindowMessage("TaskbarCreated");
    Q_ASSERT(m_taskbarRestartMessage != 0);
  }

  bool HiddenEventWindow::nativeEvent(const QByteArray &eventType, void *message, long *result)
  {
    Q_UNUSED(eventType);
    Q_UNUSED(result);

    const MSG* const msg = static_cast<MSG*>(message);

    switch (msg->message)
    {
      case  WM_HOTKEY:
      {
        emit HotKeyPressed();
        return true;
      }
      break;

      default:
      {
        if (msg->message == m_taskbarRestartMessage)
        {
          emit ExplorerRestarted();
          return true;
        }
      }
      break;
    }

    return false;
  }

  bool HiddenEventWindow::event(QEvent* p_event)
  {
    if (p_event->type() == QEvent::WindowDeactivate || p_event->type() == QEvent::FocusOut)
    {
      emit Aborted();
    }

    return QWidget::event(p_event);
  }

  void HiddenEventWindow::BringToTop()
  {
    const bool ok = FastWindowSwitcherLib::WinApiFunctions::Win32SetForegroundWindow(winId());
  }

  void HiddenEventWindow::keyPressEvent(QKeyEvent* p_event)
  {
    p_event->ignore();
    emit KeyPressed(*p_event);
  }
}