#include "SettingsWindow.hpp"

#include <QDebug>
#include <QFontDialog>
#include <QApplication>
#include <QFile>
#include <QMessageBox>

#include "ui_SettingsDialog.h"
#include "HotKeyFunctions.hpp"
#include "PersistentSettingsFunctions.hpp"

namespace FastWindowSwitcher
{
  QTreeWidgetItem* AddTreeRootItem(QTreeWidget* p_treeWidget, const QString& p_itemText, QWidget* p_linkedPage)
  {
    QTreeWidgetItem* child_item = new QTreeWidgetItem(p_treeWidget, 0);
    child_item->setText(0, p_itemText);
    child_item->setData(0, Qt::UserRole, QVariant::fromValue(p_linkedPage));
    return child_item;
  }

  QTreeWidgetItem* AddTreeChildItem(QTreeWidgetItem* p_parent, const QString& p_itemText, QWidget* p_linkedPage)
  {
    QTreeWidgetItem* child_item = new QTreeWidgetItem(p_parent, 0);
    child_item->setText(0, p_itemText);
    child_item->setData(0, Qt::UserRole, QVariant::fromValue(p_linkedPage));
    return child_item;
  }

  //Window for changing FWS settings
  SettingsWindow::SettingsWindow(const PersistenSettings& p_persitenSettings, const QString& p_settingsFile) :
    QDialog(nullptr), m_ui(new Ui::SettingsDialog), m_persistenSettings(p_persitenSettings), m_settingsFile(p_settingsFile)
  {
    Q_ASSERT(!m_settingsFile.isEmpty());

    m_ui->setupUi(this);
    m_ui->treeWidget->setStyleSheet("background-color:transparent;");
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setFixedSize(size());

    SetWidgetsValuesFromSettings();

    QObject::connect(m_ui->buttonBox, &QDialogButtonBox::accepted, this, &SettingsWindow::OnAccepted);
  }

  //Save settings to file on "Ok"
  void SettingsWindow::OnAccepted()
  {
    const QString fontName = m_ui->lineEditFontName->text();
    Q_ASSERT(!fontName.isEmpty());

    const int fontSize = m_ui->spinBoxFontSize->value();
    Q_ASSERT(fontSize > 0);

    const int motifier_key_code_hex = m_ui->comboBoxHotkeyModifier->currentData().toInt();
    const int key_code_hex = m_ui->comboBoxHotKeyKey->currentData().toInt();

    const PersistenSettings changedSettings(motifier_key_code_hex, key_code_hex, fontName, fontSize, false);

    QString errorText = "";
    bool writeOk = PersistenSettingsFunctions::WritePersistenSettings(m_settingsFile, changedSettings, errorText);
    if (!writeOk)
    {
      QMessageBox::critical(nullptr, QApplication::applicationName(), QString("Writing settings not possible! Reason:\n\n '%1'\n\nPlease make sure that the file '%2' is writable.").arg(errorText).arg(m_settingsFile));
      return;
    }

    emit SettingsChanged();
    close();
  }
   
  //Fill tree
  void SettingsWindow::PopulateSettingsTree()
  {
    auto settingsRootItem = AddTreeRootItem(m_ui->treeWidget, "Settings", m_ui->page_hotkey);
    m_ui->treeWidget->expandToDepth(1);

    QTreeWidgetItem* hotkeyWidgetItem = AddTreeChildItem(settingsRootItem, "Hotkey", m_ui->page_hotkey);
    hotkeyWidgetItem->setSelected(true);

    AddTreeChildItem(settingsRootItem, "Labels", m_ui->page_labels);

    AddTreeRootItem(m_ui->treeWidget, "Help", m_ui->page_help);
    AddTreeRootItem(m_ui->treeWidget, "About", m_ui->page_about);

    ConnectSettingsTreeWithSettingsPages();
  }

  void SettingsWindow::ConnectSettingsTreeWithSettingsPages()
  {
    QObject::connect(m_ui->treeWidget, &QTreeWidget::currentItemChanged, m_ui->stackedWidget, [this]() {

      QTreeWidgetItem* currentitem = m_ui->treeWidget->currentItem();
      Q_ASSERT(currentitem != nullptr);

      QWidget* stackedWidgetToSelect = currentitem->data(0, Qt::UserRole).value<QWidget*>();
      Q_ASSERT(stackedWidgetToSelect != nullptr);

      m_ui->stackedWidget->setCurrentWidget(stackedWidgetToSelect);

    });
  }

  void SettingsWindow::SetFileVersionInAboutPage()
  {
    QString text = m_ui->page_about_label->text();
    text = text.replace("@VERSION@", QApplication::applicationVersion());
    m_ui->page_about_label->setText(text);
  }

  //Fill stacked pages
  void SettingsWindow::SetWidgetsValuesFromSettings()
  {
    SetFileVersionInAboutPage();
    PopulateSettingsTree();

    const std::vector<HotKeyFunctions::Mapping> mappings = HotKeyFunctions::GetModfierMappings();
    for (const HotKeyFunctions::Mapping& mapping : mappings)
    {
      m_ui->comboBoxHotkeyModifier->addItem(mapping.Name, mapping.VkKeyCode);
    }

    int currentModifierIndex = HotKeyFunctions::GetHotKeyModifierIndex(m_persistenSettings.GetHotKeyMotifierKeyCode());
    Q_ASSERT(currentModifierIndex >= 0);
    if (currentModifierIndex != -1)
    {
      m_ui->comboBoxHotkeyModifier->setCurrentIndex(currentModifierIndex);
    }

    m_ui->lineEditFontName->setText(m_persistenSettings.GetMarkerFontName());
    m_ui->spinBoxFontSize->setValue(m_persistenSettings.GetMarkerFontSize());

    const std::vector<HotKeyFunctions::Mapping> hotKeyModifiersMappings = HotKeyFunctions::GetVirtualKeyMappings();
    for (const HotKeyFunctions::Mapping& mapping : hotKeyModifiersMappings)
    {
      m_ui->comboBoxHotKeyKey->addItem(mapping.Name, mapping.VkKeyCode);
    }

    int currentHotKeyIndex = HotKeyFunctions::GetHotKeyKeyIndex(m_persistenSettings.GetHotKeyKeyCode());
    Q_ASSERT(currentHotKeyIndex >= 0);
    if (currentHotKeyIndex != -1)
    {
      m_ui->comboBoxHotKeyKey->setCurrentIndex(currentHotKeyIndex);
    }

    QObject::connect(m_ui->pushButtonFontSelection, &QPushButton::clicked, [this]()
    {
      bool ok = false;

      //FIXME hide not used widget childs
      const QFont font = QFontDialog::getFont(&ok, QFont(m_persistenSettings.GetMarkerFontName(), m_persistenSettings.GetMarkerFontSize()), this, "Select label font");
      if (!ok)
      {
        return;
      }

      m_ui->lineEditFontName->setText(font.family());
      m_ui->spinBoxFontSize->setValue(font.pointSize());

    });
  }
  
  SettingsWindow::~SettingsWindow()
  {
    delete m_ui;
  }
}



