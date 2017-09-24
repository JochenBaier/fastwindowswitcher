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

//#include <vld.h>

#include <QApplication>
#include <QObject>
#include <QDesktopWidget>
#include <QWindow>
#include <QMessageBox>


#include <RunGuard.hpp>
#include "SessionManager.hpp"
#include "SettingsFunctions.hpp"
#include "MiniDumpFunctions.hpp"

#include <QtCore/QtPlugin>
Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin)

int main_internal(int argc, char ** argv)
{
  HRESULT res = CoInitialize(nullptr);
  Q_ASSERT(res == S_OK);

  QApplication app(argc, argv);
  app.setQuitOnLastWindowClosed(false);
  app.setApplicationName("FastWindowSwitcher");
  app.setOrganizationName("Jochen Baier");
  app.setApplicationVersion(QT_VERSION_STR);
  app.setStyleSheet("QMessageBox { messagebox-text-interaction-flags: 5; }");


  //Allow only single instance
  FastWindowSwitcher::RunGuard guard(QCoreApplication::applicationName());
  if (!guard.TryToRun())
  {
    QMessageBox::critical(nullptr, QCoreApplication::applicationName(), "Programm already running.");
    return 1;
  }

  FastWindowSwitcher::SessionManager sessionManager;

  //Recreate session of monitor setup or panel setup changed
  QObject::connect(QApplication::desktop(), &QDesktopWidget::resized, &sessionManager, &FastWindowSwitcher::SessionManager::ReCreateSessionDelayed, Qt::DirectConnection);
  QObject::connect(QApplication::desktop(), &QDesktopWidget::primaryScreenChanged, &sessionManager, &FastWindowSwitcher::SessionManager::ReCreateSessionDelayed, Qt::DirectConnection);
  QObject::connect(QApplication::desktop(), &QDesktopWidget::screenCountChanged, &sessionManager, &FastWindowSwitcher::SessionManager::ReCreateSessionDelayed, Qt::DirectConnection);
  QObject::connect(QApplication::desktop(), &QDesktopWidget::workAreaResized, &sessionManager, &FastWindowSwitcher::SessionManager::ReCreateSessionDelayed, Qt::DirectConnection);

  //Shutdown session before Windows Log out or Windows shutdown to avoid blocking
  QObject::connect(&app, &QApplication::aboutToQuit, &sessionManager, &FastWindowSwitcher::SessionManager::Shutdown, Qt::DirectConnection);

  sessionManager.ReCreateSession();

  int exitCode = app.exec();

  CoUninitialize();

  return exitCode;
}

//Global unhandled exeption filter
LONG WINAPI MyUnhandledExceptionFilter(PEXCEPTION_POINTERS p_exceptionPtrs)
{
   //Create a mini dump on crash
   return FastWindowSwitcher::MiniDumpFunctions::UnhandledExeptionFilterFunction(p_exceptionPtrs);
}

int main(int argc, char *argv[])
{
  //Global unhandled exeption filter
  SetUnhandledExceptionFilter(MyUnhandledExceptionFilter);

#if defined(_DEBUG)
  __try
  {
#endif
    return main_internal(argc, argv);
#if defined(_DEBUG)
  }
  __except (FastWindowSwitcher::MiniDumpFunctions::UnhandledExeptionFilterFunction(GetExceptionInformation()))
  {
      ExitProcess(1);
  }
#endif

}