#include "TrayIcon.hpp"
#include "QApplication"

namespace FastWindowSwitcher
{
  TrayIcon::TrayIcon() :
    m_quitAction("Quit"),
    m_settingsAction("Settings, Help, About")
  {
    m_systemTrayMenu.addAction(&m_settingsAction);
    m_systemTrayMenu.addAction(&m_quitAction);

    m_systemTrayIcon.setContextMenu(&m_systemTrayMenu);

    m_systemTrayIcon.setToolTip(QApplication::applicationName());

    QIcon icon(":/trayicon.png");
    m_systemTrayIcon.setIcon(icon);

    //Signal forwarding
    QObject::connect(&m_quitAction, &QAction::triggered, this, &TrayIcon::QuitMenuClicked);
    QObject::connect(&m_settingsAction, &QAction::triggered, this, &TrayIcon::SettingsMenuClicked);

    QObject::connect(&m_systemTrayIcon, &QSystemTrayIcon::activated, this, [this](const QSystemTrayIcon::ActivationReason& reason)
    {
      if (reason == QSystemTrayIcon::Trigger || reason == QSystemTrayIcon::DoubleClick)
      {
        emit SettingsMenuClicked();
      }
    });
  }

  void TrayIcon::Show()
  {
    m_systemTrayIcon.show();
  }
}
