#pragma once

#include <memory>
#include <QDialog>

#include "PersistentSettings.hpp"

namespace Ui
{
  class SettingsDialog;
};

namespace FastWindowSwitcher
{
  class SettingsWindow : public QDialog
  {
    Q_OBJECT
  public:
    SettingsWindow(const PersistenSettings& p_persitenSettings, const QString& p_settingsFile);
    virtual ~SettingsWindow();
    Q_SIGNAL void SettingsChanged();

  private:
    Q_SLOT void OnAccepted();

    void SetWidgetsValuesFromSettings();
    void PopulateSettingsTree();
    void ConnectSettingsTreeWithSettingsPages();
    void SetFileVersionInAboutPage();

    const PersistenSettings m_persistenSettings;
    const QString m_settingsFile;
    Ui::SettingsDialog *m_ui;
  };
};