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

#pragma  once

#include <QList>
#include <QFontMetrics>

#include "SelectableElementPtr.hpp"
#include "MonitorInfo.hpp"
#include <UIAutomation.h>
#include "SelectableElementRepository.hpp"
#include "IDesktopAbstraction.hpp"

class QString;
class QTimer;
struct IUIAutomation;

#include <QKeyEvent>

namespace FastWindowSwitcherLib
{
  class FastWindowSwitcher :public QObject
  {
    Q_OBJECT

  public:
    FastWindowSwitcher(IDesktopAbstraction& p_desktopAbstraction, SelectableElementRepository& p_elementRepository, const std::vector<MonitorInfo>& p_monitors,
      const QList<quintptr>& p_windowBlackList);
    virtual ~FastWindowSwitcher() override;

    void StartMarkerMode();
    void ProcessKeyPressEvent(const QKeyEvent& p_event);

    Q_SIGNAL void MarkerChanged(QString p_letterSequence, int p_shakingOffset);
    Q_SIGNAL void SelectionModeStarted();
    Q_SIGNAL void SelectionModeStopped();

    Q_SLOT void SwitchSelectionMode();
    Q_SLOT void StopSelectionMode();

  private:
    void EmitMarkersChanged();

    void StartShakingTimer();
    void StopShakingTimer();
    void StopMarkerMode(const bool& p_aborted);

    Q_SLOT void ShakingTimerTick();
    Q_SLOT void RefreshTimerTick();

    IDesktopAbstraction& m_desktopAbstraction;
    SelectableElementRepository& m_elementRespository;
    std::vector<MonitorInfo> m_monitors;
    quintptr m_lastForegroundWindow = 0;
    bool m_markerMode = false;
    QString m_current_key_combination = "";
    QTimer *m_refreshTimer = nullptr;
    QTimer* m_shaking_timer = nullptr;
    int m_shaking_counter = 0;
    int m_shaking_offset_x = 0;
    QList<quintptr> m_windowBlackList;
    quintptr m_startMenuWindow = NULL;
  };

  using FastWindowSwitcherPtr = std::unique_ptr<FastWindowSwitcher>;
}