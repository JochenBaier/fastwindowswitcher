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

#include <QDebug>
#include <QTimer>
#include <QApplication>
#include <QMessageBox>
#include "QFileInfo"

#include "SessionManager.hpp"
#include "PersistentSettingsFunctions.hpp"
#include "UIAutomationFunctions.hpp"
#include "WinApiFunctions.hpp"
#include "HotKeyFunctions.hpp"
#include "WelcomeDialogFunctions.hpp"
#include "SettingsWindow.hpp"


namespace FastWindowSwitcher
{
  //Manages the lifetime of a session
  SessionManager::SessionManager() :
    m_recreateTimer(new QTimer(this)),
    m_persistentSettings(MOD_WIN, 0x59, "Consolas", 14, true),
    m_settingsFile(QApplication::applicationDirPath() + "/" + QCoreApplication::applicationName() + ".xml")
  {
    m_recreateTimer->setSingleShot(true);
    m_recreateTimer->setInterval(1000);
    connect(m_recreateTimer, &QTimer::timeout, this, &SessionManager::ReCreateSession);
  }

  //Start recreation delayed because the old windows are still there after the event is fired
  void SessionManager::ReCreateSessionDelayed()
  {
    m_recreateTimer->stop();
    m_recreateTimer->start();
  }

  //Register hotkey which will show the markers
  bool  RegisterGlobalHotkey(const quintptr& p_window, const int& p_hotKeyVirtualModiferKey, const int& p_hotKeyVirtualKey)
  {
    Q_ASSERT(p_window);
    const bool registerOk = FastWindowSwitcherLib::WinApiFunctions::Win32RegisterHotKey(p_window, 0, p_hotKeyVirtualModiferKey | MOD_NOREPEAT, p_hotKeyVirtualKey);
    return registerOk;
  }

  void UnRegisterGlobalHotkey(const quintptr& p_window)
  {
    FastWindowSwitcherLib::WinApiFunctions::Win32UnregisterHotKey(p_window, 0);
  }

  //Create session on program start. Create settings file if it do not exists
  void SessionManager::CreateFirstSession()
  {
    //Create settings file if it does not exists
    QFile file(m_settingsFile);
    if (!file.exists())
    {
      //Write settings file with default values because its missing
      QString errorText = "";
      bool writeOk = PersistenSettingsFunctions::WritePersistenSettings(m_settingsFile, m_persistentSettings, errorText);
      if (!writeOk)
      {
        QMessageBox::critical(nullptr, QApplication::applicationName(), QString("Could not create default settings file! Reason:\n\n '%1'\n\nPlease make sure that the folder '%2' is writable.").arg(errorText).arg(QApplication::applicationDirPath()));
      }
    }

    ReCreateSession();
  }

  //Create a  if monitor or panel setup changed
  void SessionManager::ReCreateSession()
  {
    IUIAutomation* uiAutomation = nullptr;

    //Microsoft UI Automation API is used to get the taskbar button positions
    const bool initalUiAutomationOk = FastWindowSwitcherLib::UIAutomationFunctions::InitializeUIAutomation(&uiAutomation);
    if (!initalUiAutomationOk)
    {
      QMessageBox::critical(nullptr, QCoreApplication::applicationName(), "Could not initialize UI Automation API. Please report.\nApplication will be terminated.");
      QTimer::singleShot(0, QApplication::instance(), &QApplication::quit);
      return;
    }

    //try to find a settings file 'FastWindowSwitcher.xml' inside the application folder
    //Read settings from 'FastWindowSwitcher.xml'
    m_persistentSettings = PersistenSettingsFunctions::ReadPersistenSettingsWithDefaultFallback(m_settingsFile);
    if (m_persistentSettings.GetFirstRun())
    {
      m_persistentSettings.SetFirstRun(false);

      WelcomeDialogFunctions::ShowWelcomeDialog();

      QString errorText = "";
      bool writeOk = PersistenSettingsFunctions::WritePersistenSettings(m_settingsFile, m_persistentSettings, errorText);
      if (!writeOk)
      {
        QMessageBox::critical(nullptr, QApplication::applicationName(), QString("Writing settings not possible (first_run to '0')! Reason:\n\n '%1'\n\nPlease make sure that the file '%2' is writable.").arg(errorText).arg(m_settingsFile));
      }
    }

    if (m_hiddenEventWindow)
    {
      UnRegisterGlobalHotkey(m_hiddenEventWindow->winId());
      m_hiddenEventWindow.reset();
    }

    //Create a hidden windows which receives the native events (key press, WM_HOTKEY, TaskbarCreated)
    m_hiddenEventWindow = std::make_unique<HiddenEventWindow>();
    Q_ASSERT(m_hiddenEventWindow->winId());

    RegisterGlobalHotkeyDisplayWarningOnError();

    //Create a new session
    m_session.reset();
    m_session = std::make_unique<Session>(*m_hiddenEventWindow, m_persistentSettings, *uiAutomation);

    //Connect to ExplorerRestarted to recreate the session if explorer.exe process is restarted
    QObject::connect(m_hiddenEventWindow.get(), &FastWindowSwitcher::HiddenEventWindow::ExplorerRestarted, this, &FastWindowSwitcher::SessionManager::ReCreateSessionDelayed, Qt::DirectConnection);


    m_trayIcon.reset();
    m_trayIcon = std::make_unique<TrayIcon>();

    QObject::connect(m_trayIcon.get(), &TrayIcon::QuitMenuClicked, QCoreApplication::instance(), QCoreApplication::quit);
    QObject::connect(m_trayIcon.get(), &TrayIcon::SettingsMenuClicked, this, &SessionManager::ShowSettingsDialog);

    m_trayIcon->Show();
  }

  //Try to register label hotkey
  void SessionManager::RegisterGlobalHotkeyDisplayWarningOnError()
  {
    const bool registerOk = RegisterGlobalHotkey(m_hiddenEventWindow->winId(), m_persistentSettings.GetHotKeyMotifierKeyCode(), m_persistentSettings.GetHotKeyKeyCode());
    if (!registerOk)
    {
      QString hotKeyModifier = HotKeyFunctions::GetHotKeyModfierAsString(m_persistentSettings.GetHotKeyMotifierKeyCode());
      QString hotKeyKey = HotKeyFunctions::GetHotKeyKeyAsString(m_persistentSettings.GetHotKeyKeyCode());

      QMessageBox::warning(nullptr, QCoreApplication::applicationName(), QString("Can not register hotkey '%1-%2'! Please change in settings.").arg(hotKeyModifier, hotKeyKey));
    }
  }

  //Display window with all settings
  void SessionManager::ShowSettingsDialog()
  {
    if (m_settingsWindowIsOpen)
    {
      return;
    }

    //will be auto deleted on dialog close
    SettingsWindow* m_settingsWindow = new SettingsWindow(m_persistentSettings, m_settingsFile);

    QObject::connect(m_settingsWindow, &SettingsWindow::SettingsChanged, [this]()
    {
      QTimer::singleShot(0, this, &SessionManager::ReCreateSession);
    });


    QObject::connect(m_settingsWindow, &SettingsWindow::destroyed, [this]()
    {
      m_settingsWindowIsOpen = false;
    });
    
    m_settingsWindow->show();
    m_settingsWindow->raise();

    m_settingsWindowIsOpen = true;
  }

  //Shutdown session on application exit, Windows logout, Windows shutdown
  void SessionManager::Shutdown()
  {
    if (m_hiddenEventWindow)
    {
      UnRegisterGlobalHotkey(m_hiddenEventWindow->winId());
      m_hiddenEventWindow.reset();
    }

    //For example settings window
    QApplication::closeAllWindows();

    m_session.reset();
  }
}