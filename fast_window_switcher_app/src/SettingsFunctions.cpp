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

#include "SettingsFunctions.hpp"

#include <QFile>
#include <QApplication>
#include <QMessageBox>
#include <QDebug>
#include <QDialog>
#include <QFont>
#include <QFontDialog>
#include <QPushButton>
#include <QXmlStreamReader>

#include <XmlFunctions.hpp>

#include "ui_SettingsDialog.h"
#include "HotKeyFunctions.hpp"

namespace FastWindowSwitcher
{
  //Access to the FastWindowSwitcher.xml settings file
  namespace SettingsFunctions
  {
    const QString g_defaultMarkerFont = "Consolas";
    const int g_defaultMarkerFontSize = 14;
    const int g_hotKeyMotifierKeyCode = 0x0008; //MOD_WIN
    const int g_hotKeyKeyCode = 0x59; //VK_KEY_Y

    const int g_MOD_NONE = 0;
    const int g_MOD_ALT = 0x0001;
    const int g_MOD_CONTROL = 0x0002;
    const int g_MOD_SHIFT = 0x0004;
    const int g_MOD_WIN = 0x0008;

    //Get settings with fallback
    PersistenSettings ReadPersistenSettingsWithDefaultFallback(const QString& p_filenname)
    {
      Q_ASSERT(!p_filenname.isEmpty());

      //If no settings file exits default values will be used. Settings file be be created on first change
      QFile file(p_filenname);
      if (!file.exists())
      {
        return PersistenSettings(g_hotKeyMotifierKeyCode, g_hotKeyKeyCode, g_defaultMarkerFont, g_defaultMarkerFontSize);
      }

      if (!file.open(QFile::ReadOnly | QFile::Text))
      {
        Q_ASSERT(false);
        return PersistenSettings(g_hotKeyMotifierKeyCode, g_hotKeyKeyCode, g_defaultMarkerFont, g_defaultMarkerFontSize);
      }

      QString fileContent;

      QTextStream in(&file);
      fileContent = in.readAll();
      Q_ASSERT(!fileContent.isEmpty());
            
      int hotKeyMotifierKeyCode = XmlFunctions::ReadInt(fileContent, "fastwindowswitcher/general/hotkey/motifier_key_code_hex/text()", 16, g_hotKeyMotifierKeyCode);
      int hotKeyKeyCode = XmlFunctions::ReadInt(fileContent, "fastwindowswitcher/general/hotkey/key_code_hex/text()", 16, g_hotKeyKeyCode);

      QString markerFontName = XmlFunctions::ReadString(fileContent, "fastwindowswitcher/general/marker/font/text()", g_defaultMarkerFont);
      int markerFontSize = XmlFunctions::ReadInt(fileContent, "fastwindowswitcher/general/marker/font_size/text()", 10, g_defaultMarkerFontSize);

      //FIXME check if font name is valid
      PersistenSettings persistenSettings(hotKeyMotifierKeyCode, hotKeyKeyCode, markerFontName, markerFontSize);
      return persistenSettings;
    }

    bool WriteSettingsXml(const QString& p_settingsFile, const int& p_motifier_key_code_hex, const int& p_key_code_hex, const QString& p_fontName, const int& p_fontSize)
    {
      QFile file(p_settingsFile);
      if (!file.open(QFile::WriteOnly | QFile::Truncate | QFile::Text))
      {
        return false;
      }

      QXmlStreamWriter stream(&file);
      stream.setAutoFormatting(true);
      stream.writeStartDocument();

      stream.writeStartElement("fastwindowswitcher");
      stream.writeStartElement("general");
      stream.writeStartElement("hotkey");

      stream.writeTextElement("motifier_key_code_hex", QString::number(p_motifier_key_code_hex, 16));
      stream.writeTextElement("key_code_hex", QString::number(p_key_code_hex, 16));
      stream.writeEndElement(); //hotkey


      stream.writeStartElement("marker");
      stream.writeTextElement("font", p_fontName);
      stream.writeTextElement("font_size", QString::number(p_fontSize));
      stream.writeEndElement(); //marker

      stream.writeEndElement(); //general
      stream.writeEndElement(); //fastwindowswitcher

      stream.writeEndDocument();
      return true;
    }

    bool ShowSettingsDialog(const QString& p_settingsFile, const PersistenSettings& p_persistenSettings)
    {
      QDialog *settingsDialog = new QDialog;
      Ui::SettingsDialog ui;
      ui.setupUi(settingsDialog);

      settingsDialog->setWindowFlags(settingsDialog->windowFlags() & ~Qt::WindowContextHelpButtonHint);

      const std::vector<HotKeyFunctions::Mapping> mappings = HotKeyFunctions::GetModfierMappings();
      for (const HotKeyFunctions::Mapping& mapping : mappings)
      {
        ui.comboBoxHotkeyModifier->addItem(mapping.Name, mapping.VkKeyCode);
      }

      int currentModifierIndex = HotKeyFunctions::GetHotKeyModifierIndex(p_persistenSettings.GetHotKeyMotifierKeyCode());
      Q_ASSERT(currentModifierIndex >= 0);
      if (currentModifierIndex != -1)
      {
        ui.comboBoxHotkeyModifier->setCurrentIndex(currentModifierIndex);
      }

      ui.lineEditFontName->setText(p_persistenSettings.GetMarkerFontName());
      ui.spinBoxFontSize->setValue(p_persistenSettings.GetMarkerFontSize());

      const std::vector<HotKeyFunctions::Mapping> hotKeyModifiersMappings = HotKeyFunctions::GetVirtualKeyMappings();
      for (const HotKeyFunctions::Mapping& mapping : hotKeyModifiersMappings)
      {
        ui.comboBoxHotKeyKey->addItem(mapping.Name, mapping.VkKeyCode);
      }

      int currentHotKeyIndex = HotKeyFunctions::GetHotKeyKeyIndex(p_persistenSettings.GetHotKeyKeyCode());
      Q_ASSERT(currentHotKeyIndex >= 0);
      if (currentHotKeyIndex != -1)
      {
        ui.comboBoxHotKeyKey->setCurrentIndex(currentHotKeyIndex);
      }

      QObject::connect(ui.pushButtonFontSelection, &QPushButton::clicked, [settingsDialog, &ui, p_persistenSettings]()
      {
        bool ok = false;

        //FIXME hide not used widget childs
        QFont font = QFontDialog::getFont(&ok, QFont(p_persistenSettings.GetMarkerFontName(), p_persistenSettings.GetMarkerFontSize()), settingsDialog, "Select marker font");
        if (!ok)
        {
          return;
        }

        ui.lineEditFontName->setText(font.family());
        ui.spinBoxFontSize->setValue(font.pointSize());

      });

      int rs = settingsDialog->exec();
      if (rs == QDialog::Rejected)
      {
        return false;
      }

      QString fontName = ui.lineEditFontName->text();
      Q_ASSERT(!fontName.isEmpty());

      int fontSize = ui.spinBoxFontSize->value();
      Q_ASSERT(fontSize > 0);

      int motifier_key_code_hex = ui.comboBoxHotkeyModifier->currentData().toInt();
      Q_ASSERT(motifier_key_code_hex == g_MOD_NONE || motifier_key_code_hex == g_MOD_ALT || motifier_key_code_hex == g_MOD_CONTROL || motifier_key_code_hex == g_MOD_WIN);

      int key_code_hex = ui.comboBoxHotKeyKey->currentData().toInt();

      return WriteSettingsXml(p_settingsFile, motifier_key_code_hex, key_code_hex, fontName, fontSize);
    }
  }
}