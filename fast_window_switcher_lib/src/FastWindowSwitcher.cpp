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

#include "FastWindowSwitcher.hpp"

#include <QDebug>
#include <QTimer>

#include "SelectableElementRepository.hpp"
#include "SelectableElement.hpp"
#include "UIAutomationFunctions.hpp"

namespace FastWindowSwitcherLib
{
  //Core class. Starts the marker modus
  FastWindowSwitcher::FastWindowSwitcher(
    IDesktopAbstraction& p_desktopAbstraction,
    SelectableElementRepository& p_elementRepository,
    const std::vector<MonitorInfo>& p_monitors,
    const QList<quintptr>& p_windowBlackList) :

    m_desktopAbstraction(p_desktopAbstraction),
    m_elementRespository(p_elementRepository),
    m_monitors(p_monitors),
    m_windowBlackList(p_windowBlackList),
    m_refreshTimer(new QTimer(this)),
    m_shaking_timer(new QTimer(this))
  {
    m_refreshTimer->setInterval(200);
    m_refreshTimer->setSingleShot(true);
    connect(m_refreshTimer, &QTimer::timeout, this, &FastWindowSwitcher::RefreshTimerTick, Qt::DirectConnection);

    m_shaking_timer->setInterval(25);
    m_shaking_timer->setSingleShot(true);
    connect(m_shaking_timer, &QTimer::timeout, this, &FastWindowSwitcher::ShakingTimerTick, Qt::DirectConnection);
  }

  //Shake markers if non valid key was pressed
  void FastWindowSwitcher::ShakingTimerTick()
  {
    if (m_shaking_counter > 10)
    {
      m_shaking_timer->stop();
      m_shaking_counter = 0;
      m_shaking_offset_x = 0;
      EmitMarkersChanged();
      return;
    }

    m_shaking_offset_x = m_shaking_offset_x == 5 ? -5 : 5;
    m_shaking_counter++;

    EmitMarkersChanged();
    m_shaking_timer->start();
  }

  //Start shaking of markers
  void FastWindowSwitcher::StartShakingTimer()
  {
    m_shaking_counter = 0;
    m_shaking_offset_x = -5;
    m_shaking_timer->start();
  }

  //Stop shaking of markers
  void FastWindowSwitcher::StopShakingTimer()
  {
    m_shaking_timer->stop();
    m_shaking_offset_x = 0;
  }

  //Changes marker mode
  void FastWindowSwitcher::SwitchSelectionMode()
  {
    if (m_markerMode)
    {
      StopMarkerMode(true);
    }
    else
    {
      StartMarkerMode();
    }
  }

  //Stops marker mode
  void FastWindowSwitcher::StopSelectionMode()
  {
    StopMarkerMode(true);
  }

  //Updates markers during marker mode
  void FastWindowSwitcher::RefreshTimerTick()
  {
    Q_ASSERT(m_elementRespository.Count() > 0);

    bool updateNeeded = false;

    m_desktopAbstraction.UpdateElements(m_elementRespository, m_monitors, updateNeeded);
    if (updateNeeded)
    {
      EmitMarkersChanged();
    }

    m_refreshTimer->start();
  }

  void FastWindowSwitcher::EmitMarkersChanged()
  {
    emit MarkerChanged(m_current_key_combination, m_shaking_offset_x);
  }

  //Starts marker mode
  void FastWindowSwitcher::StartMarkerMode()
  {
    Q_ASSERT(m_elementRespository.Count() == 0);
    Q_ASSERT(m_markerMode == false);

    quintptr lastForegroundWindow = m_desktopAbstraction.GetCurrentForgroundWindow();

    if (m_startMenuWindow != NULL &&  lastForegroundWindow == m_startMenuWindow)
    {
      return;
    }

    m_lastForegroundWindow = lastForegroundWindow;

    std::vector<SelectableElementPtr> selectableElements = m_desktopAbstraction.GetSelectableElements(m_windowBlackList, m_monitors);
    if (selectableElements.empty())
    {
      return;
    }

    m_elementRespository.SetElements(selectableElements);

    m_markerMode = true;
    emit SelectionModeStarted();

    EmitMarkersChanged();

    m_refreshTimer->start();
  }

  //Stops marker mode
  void FastWindowSwitcher::StopMarkerMode(const bool& p_aborted)
  {
    if (!m_markerMode)
    {
      return;
    }

    m_refreshTimer->stop();

    StopShakingTimer();

    emit SelectionModeStopped();

    m_current_key_combination.clear();

    m_elementRespository.ClearAll();

    m_markerMode = false;

    if (p_aborted && m_lastForegroundWindow != NULL)
    {
      m_desktopAbstraction.Win32SwitchToThisWindow(m_lastForegroundWindow);
    }

    m_lastForegroundWindow = NULL;
  }

  //Process key events during marker mode
  void FastWindowSwitcher::ProcessKeyPressEvent(const QKeyEvent& p_event)
  {
    if (!m_markerMode)
    {
      return;
    }

    if (p_event.key() == Qt::Key_Escape)
    {
      StopSelectionMode();
      return;
    }

    StopShakingTimer();

    //delete key on Backspace if the marker contains more the one char
    if (p_event.key() == Qt::Key_Backspace)
    {
      if (m_current_key_combination.size() > 0)
      {
        m_current_key_combination.chop(1);
      }
      else
      {
        StartShakingTimer();
      }

      m_elementRespository.UpdateMatchingKeyCount(m_current_key_combination);
      EmitMarkersChanged();
      return;
    }

    if (p_event.text() == nullptr || p_event.text().isEmpty())
    {
      StartShakingTimer();
      EmitMarkersChanged();
      return;
    }

    QString cur_combination = m_current_key_combination + p_event.text();

    bool one_starts_with = false;

    //Find a marker which matches the pressed keys
    int foundElementIndex = m_elementRespository.GetIndexMatchingText(cur_combination, one_starts_with);
    if (foundElementIndex == -1)
    {
      if (one_starts_with)
      {
        m_current_key_combination = cur_combination;
      }
      else
      {
        StartShakingTimer();
      }

      m_elementRespository.UpdateMatchingKeyCount(m_current_key_combination);
      EmitMarkersChanged();
      return;
    }

    m_current_key_combination = cur_combination;


    SelectableElement& foundElement = m_elementRespository.GetElement(foundElementIndex);

    //Invoke element which matching keys
    m_desktopAbstraction.Invoke(foundElement);

    StopMarkerMode(false);
  }

  FastWindowSwitcher::~FastWindowSwitcher()
  {
    m_refreshTimer->stop();
    StopShakingTimer();
    m_elementRespository.ClearAll();
  }
}