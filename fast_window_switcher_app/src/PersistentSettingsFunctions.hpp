#pragma once

#include <QString>

#include "PersistentSettings.hpp"

namespace FastWindowSwitcher
{
  namespace PersistenSettingsFunctions
  {
    PersistenSettings ReadPersistenSettingsWithDefaultFallback(const QString& p_filenname);
    bool WritePersistenSettings(const QString& p_settingsFile, const PersistenSettings& p_persistenSettings, QString& p_errorText);
  };
};
