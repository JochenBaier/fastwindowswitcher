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

#include "Session.hpp"
#include <QObject>
#include <QWindow>
#include <QApplication> 
#include <QScreen>
#include <QDesktopWidget>
#include <QFont>
#include <QFontMetrics>

#include "UIAutomationFunctions.hpp"
#include "PersistentSettings.hpp"
#include "DesktopAbstraction.hpp"
#include "HiddenEventWindow.hpp"

namespace FastWindowSwitcher
{
  //Session builds all necessary objects
  Session::Session(const HiddenEventWindow& p_hiddenEventWindow, const PersistenSettings& p_persistentSettings, IUIAutomation& p_uiAutomation) :
    m_uiAutomation(p_uiAutomation)
  {
    //Layer between FastWindowSwitcher and Windows API functions. Enables independent testing of the FastWindowSwitcher functions
    m_desktopAbstraction = std::make_unique<FastWindowSwitcherLib::DesktopAbstraction>(p_uiAutomation, QFontMetrics(QFont(p_persistentSettings.GetMarkerFontName(), p_persistentSettings.GetMarkerFontSize())));

    //Get infos of available monitors
    std::vector<FastWindowSwitcherLib::MonitorInfo> monitors = m_desktopAbstraction->GetMonitorInfos();
    Q_ASSERT(monitors.size() > 0);

    //List of (system) windows which do not get markers
    QList<quintptr> windowBlackList = m_desktopAbstraction->GetBlackListetWindows(monitors);
   

    //The FastWindowSwitcher class core of the application. It will receive the hotkey event and starts the marker modus
    m_fastWindowSwitcher = std::make_unique<FastWindowSwitcherLib::FastWindowSwitcher>(*m_desktopAbstraction, m_elementRepository,
      monitors, windowBlackList);

    //Create transparent windows which paints the markers
    CreateAndConnectMarkerWindows(p_persistentSettings, monitors, m_elementRepository);

    QObject::connect(&p_hiddenEventWindow, &HiddenEventWindow::HotKeyPressed, m_fastWindowSwitcher.get(), &FastWindowSwitcherLib::FastWindowSwitcher::SwitchSelectionMode, Qt::DirectConnection);
    QObject::connect(&p_hiddenEventWindow, &HiddenEventWindow::Aborted, m_fastWindowSwitcher.get(), &FastWindowSwitcherLib::FastWindowSwitcher::StopSelectionMode, Qt::DirectConnection);

    QObject::connect(m_fastWindowSwitcher.get(), &FastWindowSwitcherLib::FastWindowSwitcher::SelectionModeStarted, &p_hiddenEventWindow, &HiddenEventWindow::BringToTop, Qt::DirectConnection);
    QObject::connect(&p_hiddenEventWindow, &HiddenEventWindow::KeyPressed, m_fastWindowSwitcher.get(), &FastWindowSwitcherLib::FastWindowSwitcher::ProcessKeyPressEvent, Qt::DirectConnection);

  }

  //Create transparent windows which paints the markers
  void Session::CreateAndConnectMarkerWindows(const PersistenSettings& p_persistentSettings, const std::vector<FastWindowSwitcherLib::MonitorInfo>& p_monitors, FastWindowSwitcherLib::SelectableElementRepository& p_selectableElementRepository)
  {
    for (const auto& monitor : p_monitors)
    {
      TransparentMarkerWindowPtr transparentMarkerWindow = std::make_unique<MarkerWindow>(monitor.GetIndex(), p_selectableElementRepository,
        monitor.GetAvailableGemoetry(), QFont(p_persistentSettings.GetMarkerFontName(), p_persistentSettings.GetMarkerFontSize()),
        QFontMetrics(QFont(p_persistentSettings.GetMarkerFontName(), p_persistentSettings.GetMarkerFontSize())));

      transparentMarkerWindow->show();

      transparentMarkerWindow->windowHandle()->setScreen(QApplication::desktop()->screen(monitor.GetIndex())->windowHandle()->screen());

      QObject::connect(m_fastWindowSwitcher.get(), &FastWindowSwitcherLib::FastWindowSwitcher::MarkerChanged, transparentMarkerWindow.get(), &MarkerWindow::Update, Qt::DirectConnection);
      QObject::connect(m_fastWindowSwitcher.get(), &FastWindowSwitcherLib::FastWindowSwitcher::SelectionModeStopped, transparentMarkerWindow.get(), &MarkerWindow::Hide, Qt::DirectConnection);

      m_transparentMarkerWindow.push_back(std::move(transparentMarkerWindow));
    }
  }

  Session::~Session()
  {
    FastWindowSwitcherLib::UIAutomationFunctions::DeInitializeUIAutomation(m_uiAutomation);
  }
}