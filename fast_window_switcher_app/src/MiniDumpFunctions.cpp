/*
Copyright:  Andy Pennell, Modifications: Jochen Baier
http://www.codeproject.com/KB/debug/postmortemdebug_standalone1.aspx
*/

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



#include "MiniDumpFunctions.hpp"
#include <DbgHelp.h>

#include <io.h>
#include <QString>
#include <QApplication>
#include <QLibrary>
#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QDateTime>

#include "WinApiFunctions.hpp"

using MiniDumpWriteDumpPtr = BOOL(WINAPI*)(HANDLE hProcess, DWORD dwPid, HANDLE hFile, MINIDUMP_TYPE DumpType, CONST PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam,
  CONST PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam, CONST PMINIDUMP_CALLBACK_INFORMATION CallbackParam);

namespace FastWindowSwitcher
{
  namespace MiniDumpFunctions
  {
    enum class DumpResult
    {
      FAILED_TO_LOAD_DEBUG_HELP_DLL,
      FAILED_TO_FIND_DEBUG_HELP_DLL,
      FAILED_TO_CREATE_EMPTY_CRASH_REPORT_FILE,
      FAILED_TO_CREATE_CRASH_FOLDER,
      FAILED_TO_CREATE_CRASH_REPORT_FILE_HANLDE,
      FAILED_TO_CREATE_CRASH_REPORT,
      CRASH_REPORT_CREATED
    };

    const DumpResult CreateCrashReportFile(_EXCEPTION_POINTERS* p_exceptionInfo, QString& p_crashReportFile)
    {
      const QDateTime now = QDateTime::currentDateTime();

      const QString dumpFileFolder = QApplication::applicationDirPath() + "/CrashReports";
      p_crashReportFile = QString("%1/CrashReport_%2.dmp").arg(QApplication::applicationDirPath() + "/CrashReports").arg(now.toString("yyyy.MM.dd.hh_mm_ss.zzz"));

      QDir dir(dumpFileFolder);
      if (!dir.exists())
      {
        bool folder_created = dir.mkdir(dumpFileFolder);
        if (!folder_created)
        {
          return DumpResult::FAILED_TO_CREATE_CRASH_FOLDER;
        }
      }

      const QString dbghelpDllPath = QString("%1/dbghelp.dll").arg(FastWindowSwitcherLib::WinApiFunctions::GetSystemX86Folder());
      QFileInfo check_file(dbghelpDllPath);
      if (!check_file.exists())
      {
        return DumpResult::FAILED_TO_FIND_DEBUG_HELP_DLL;
      }

      QLibrary dwmapiLib(dbghelpDllPath);
      dwmapiLib.setLoadHints(QLibrary::PreventUnloadHint);
      auto pMiniDumpWriteDump = (MiniDumpWriteDumpPtr)dwmapiLib.resolve("MiniDumpWriteDump");
      if (!pMiniDumpWriteDump)
      {
        return DumpResult::FAILED_TO_LOAD_DEBUG_HELP_DLL;
      }

      QFile file(p_crashReportFile);
      if (!file.open(QFile::WriteOnly | QFile::Text))
      {
        return DumpResult::FAILED_TO_CREATE_EMPTY_CRASH_REPORT_FILE;
      }

      auto hFile = (HANDLE)_get_osfhandle(file.handle());
      if (hFile == INVALID_HANDLE_VALUE)
      {
        return DumpResult::FAILED_TO_CREATE_CRASH_REPORT_FILE_HANLDE;
      }

      _MINIDUMP_EXCEPTION_INFORMATION ExInfo;
      ExInfo.ThreadId = ::GetCurrentThreadId();
      ExInfo.ExceptionPointers = p_exceptionInfo;
      ExInfo.ClientPointers = NULL;

      // write the dump
      auto mdt = (MINIDUMP_TYPE)(MiniDumpNormal | MiniDumpWithThreadInfo);
      bool bOK = pMiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, mdt, (p_exceptionInfo != nullptr) ? &ExInfo : nullptr, nullptr, nullptr);
      if (!bOK)
      {
        return DumpResult::FAILED_TO_CREATE_CRASH_REPORT;
      }

      return DumpResult::CRASH_REPORT_CREATED;
    }

    LONG UnhandledExeptionFilterFunction(_EXCEPTION_POINTERS* p_exceptionInfo)
    {
      QString crashReportFile = "";

      DumpResult miniDumpResult = CreateCrashReportFile(p_exceptionInfo, crashReportFile);

      QString crashMessage = QString("%1 Crash-Reporting has trapped an application crash. If you want to help improve the stability of %1 please send the following infos to email@jochen-baier.de:\n\n").arg(QApplication::applicationName()) +

        QString("*List of all visible and minimized windows before the crash occurred\n") +
        QString("*Description of the last 10 seconds before the crash occurred: did the crash occurred while the labels were visible or after a label was selected?\n") +
        QString("*Which window was selected?\n*Was the selected label on a window frame or taskbar button?");

      if (miniDumpResult == DumpResult::CRASH_REPORT_CREATED)
      {
        crashMessage += QString("\n\nCrash-Report was able the generate a crash dump (memory dump). Please send the file\n\n'%1'\n\nin addition to the infos above to email@jochen-baier.de.").arg(crashReportFile);
      }
      else
      {
        QString miniDumpResultTxt = "";

        switch (miniDumpResult)
        {
        case DumpResult::FAILED_TO_FIND_DEBUG_HELP_DLL: miniDumpResultTxt = QString("Failed to find dbghelp.dll in the '%1' folder").arg(FastWindowSwitcherLib::WinApiFunctions::GetSystemX86Folder());  break;
        case DumpResult::FAILED_TO_LOAD_DEBUG_HELP_DLL: miniDumpResultTxt = "Failed to load dbghelp.dll into memory";  break;
        case DumpResult::FAILED_TO_CREATE_EMPTY_CRASH_REPORT_FILE: miniDumpResultTxt = "Failed to create empty crash reports file";  break;
        case DumpResult::FAILED_TO_CREATE_CRASH_FOLDER: miniDumpResultTxt = "Failed to create 'CrashReports' folder inside the FastWindowSwitcher folder";   break;
        case DumpResult::FAILED_TO_CREATE_CRASH_REPORT_FILE_HANLDE: miniDumpResultTxt = "Failed to create crash report file handle";    break;
        case DumpResult::FAILED_TO_CREATE_CRASH_REPORT: miniDumpResultTxt = "Failed to create crash report (MiniDumpWriteDump() failed)";  break;
        }
        crashMessage += QString("\n\nCrash-Report was NOT able the generate a crash dump (memory dump). Please report. Reason: '%1'.").arg(miniDumpResultTxt);
      }

      crashMessage += "\n\nThank you.";

      QMessageBox dlg(QMessageBox::Critical, QCoreApplication::applicationName() + " Crash-Reporting", crashMessage);
      dlg.setWindowFlags(dlg.windowFlags() | Qt::WindowStaysOnTopHint);
      dlg.exec();

      return miniDumpResult == DumpResult::CRASH_REPORT_CREATED ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH;
    }
  }
}