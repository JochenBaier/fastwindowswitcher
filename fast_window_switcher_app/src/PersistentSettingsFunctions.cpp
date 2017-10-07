#include "PersistentSettingsFunctions.hpp"

#include <QFile>
#include <QDebug>
#include <QXmlStreamReader>
#include <QApplication>

#include "XmlFunctions.hpp"
#include "HotKeyFunctions.hpp"
#include "WinApiFunctions.hpp"

namespace FastWindowSwitcher
{
  namespace PersistenSettingsFunctions
  {
    const QString g_defaultMarkerFont = "Consolas";
    const int g_defaultMarkerFontSize = 14;
    const int g_hotKeyMotifierKeyCode = 0x0008; //MOD_WIN
    const int g_hotKeyKeyCode = 0x59; //VK_KEY_Y
    const int g_first_run = 0;
    const int g_settingsVersion = 1;

    //Read settings from FastWindowSwitcher.xml, use fallbacks if file or value does not exitsy
    PersistenSettings ReadPersistenSettingsWithDefaultFallback(const QString& p_filenname)
    {
      Q_ASSERT(!p_filenname.isEmpty());

      //If no settings file exits default values will be used. Settings should be created on first start
      QFile file(p_filenname);
      if (!file.exists())
      {
        return PersistenSettings(g_hotKeyMotifierKeyCode, g_hotKeyKeyCode, g_defaultMarkerFont, g_defaultMarkerFontSize, true);
      }

      if (!file.open(QFile::ReadOnly | QFile::Text))
      {
        Q_ASSERT(false);
        return PersistenSettings(g_hotKeyMotifierKeyCode, g_hotKeyKeyCode, g_defaultMarkerFont, g_defaultMarkerFontSize, true);
      }

      QString fileContent;

      QTextStream in(&file);
      fileContent = in.readAll();
      Q_ASSERT(!fileContent.isEmpty());

      const int hotKeyMotifierKeyCode = XmlFunctions::ReadInt(fileContent, "fastwindowswitcher/general/hotkey/motifier_key_code_hex/text()", 16, g_hotKeyMotifierKeyCode);
      const int hotKeyKeyCode = XmlFunctions::ReadInt(fileContent, "fastwindowswitcher/general/hotkey/key_code_hex/text()", 16, g_hotKeyKeyCode);
      const QString markerFontName = XmlFunctions::ReadString(fileContent, "fastwindowswitcher/general/marker/font/text()", g_defaultMarkerFont);
      const int markerFontSize = XmlFunctions::ReadInt(fileContent, "fastwindowswitcher/general/marker/font_size/text()", 10, g_defaultMarkerFontSize);
      const bool firstRun = XmlFunctions::ReadInt(fileContent, "fastwindowswitcher/general/first_run/text()", 10, g_first_run);

      PersistenSettings persistenSettings(hotKeyMotifierKeyCode, hotKeyKeyCode, markerFontName, markerFontSize, firstRun);
      return persistenSettings;
    }

    //Write all settings to file, old file content will be truncated
    bool WritePersistenSettings(const QString& p_settingsFile, const PersistenSettings& p_persistenSettings, QString& p_errorText)
    {
      QFile file(p_settingsFile);
      if (!file.open(QFile::WriteOnly | QFile::Truncate | QFile::Text))
      {
        p_errorText = file.errorString();
        return false;
      }

      QXmlStreamWriter stream(&file);
      stream.setAutoFormatting(true);
      stream.writeStartDocument();

      stream.writeStartElement("fastwindowswitcher");

      stream.writeAttribute("settings_version", QString::number(g_settingsVersion));

      stream.writeStartElement("general");

      stream.writeTextElement("first_run", QString::number((int)p_persistenSettings.GetFirstRun()));

      stream.writeStartElement("hotkey");
      stream.writeTextElement("motifier_key_code_hex", QString::number(p_persistenSettings.GetHotKeyMotifierKeyCode(), 16));
      stream.writeTextElement("key_code_hex", QString::number(p_persistenSettings.GetHotKeyKeyCode(), 16));
      stream.writeEndElement(); //hotkey


      stream.writeStartElement("marker");
      stream.writeTextElement("font", p_persistenSettings.GetMarkerFontName());
      stream.writeTextElement("font_size", QString::number(p_persistenSettings.GetMarkerFontSize()));
      stream.writeEndElement(); //marker //-V525

      stream.writeEndElement(); //general
      stream.writeEndElement(); //fastwindowswitcher

      stream.writeEndDocument();
      return true;
    }
  }
}
