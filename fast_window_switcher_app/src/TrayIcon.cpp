#include "TrayIcon.hpp"

namespace FastWindowSwitcher
{
  TrayIcon::TrayIcon() :
    m_quitAction("Quit"),
    m_settingsAction("Settings")
  {
    m_systemTrayMenu.addAction(&m_settingsAction);
    m_systemTrayMenu.addAction(&m_quitAction);

    m_systemTrayIcon.setContextMenu(&m_systemTrayMenu);

    QIcon icon(":/trayicon.png");
    m_systemTrayIcon.setIcon(icon);

    //Signal forwarding
    QObject::connect(&m_quitAction, &QAction::triggered, this, &TrayIcon::QuitMenuClicked);
    QObject::connect(&m_settingsAction, &QAction::triggered, this, &TrayIcon::SettingsMenuClicked);
  }

  void TrayIcon::Show()
  {
    m_systemTrayIcon.show();
  }
}
