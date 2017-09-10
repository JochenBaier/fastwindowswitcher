#pragma once

#include <memory>

#include <QObject>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>


namespace FastWindowSwitcher
{
  class TrayIcon : public QObject
  {
    Q_OBJECT

  public:
    TrayIcon();
    void Show();

    Q_SIGNAL void QuitMenuClicked();
    Q_SIGNAL void SettingsMenuClicked();

  private:
    QSystemTrayIcon m_systemTrayIcon;
    QMenu m_systemTrayMenu;
    QAction m_quitAction;
    QAction m_settingsAction;
  };

  using TrayIconPtr = std::unique_ptr<TrayIcon>;
};




