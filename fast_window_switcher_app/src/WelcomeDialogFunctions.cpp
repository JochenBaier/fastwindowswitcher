#include "WelcomeDialogFunctions.hpp"

#include "ui_WelcomeDialog.h"

namespace FastWindowSwitcher
{
  namespace WelcomeDialogFunctions
  {
    //Show welcome dialog on first start
    void FastWindowSwitcher::WelcomeDialogFunctions::ShowWelcomeDialog()
    {
      QDialog welcomeDialog;
      Ui::WelcomeDialog welcomeDialogUi;
      welcomeDialogUi.setupUi(&welcomeDialog);
      welcomeDialog.setFixedSize(welcomeDialog.size());
      welcomeDialog.setWindowFlags(welcomeDialog.windowFlags() & ~Qt::WindowContextHelpButtonHint);
      welcomeDialog.setWindowFlags(welcomeDialog.windowFlags() & ~Qt::WindowCloseButtonHint);
      welcomeDialogUi.label->setStyleSheet("background-color:white;");

      welcomeDialog.exec();
    }
  }
}
